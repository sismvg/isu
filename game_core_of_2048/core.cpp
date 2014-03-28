/*
Copyright : 四元飒 2014
email : sismvg@hotmail.com
这一程序是自由软件，你可以遵照自由软件基金会出版的GNU通用公共许可证条款来修改和重新发布这一程序。或者用许可证的第二版，或者（根据你的选择）用任何更新的版本。
发布这一程序的目的是希望它有用，但没有任何担保。甚至没有适合特定目的的隐含的担保。更详细的情况请参阅GNU通用公共许可证。
*/
#include "core.hpp"
#include <random>

namespace isu
{
	//控制片的转换
#define detail operation_detail
	const detail board::_mains_axis[4]=
	{//主轴的起始坐标是动态的，所以这里设定坐标没有意义
		detail(multi_index(0,0),longitudinal,-1,1),//up
		detail(multi_index(0,0),longitudinal,0,-1),//down
		detail(multi_index(0,0),lateral,-1,1),//left
		detail(multi_index(0,0),lateral,0,-1),//right	
	};
	//控制轴的转换
	const detail board::_vice_axis[4]=
	{
		detail(multi_index(0,0),lateral,-1,1),//up
		detail(multi_index(-1,0),lateral,-1,1),//down
		detail(multi_index(0, 0), longitudinal, -1, 1),//left
		detail(multi_index(0,-1), longitudinal, -1, 1)//right
	};
#undef detail

	board_setting::board_setting(
		const length_type longitudinal,const length_type lateral, const data_type count) :
		lateral(lateral), longitudinal(longitudinal), maxinum_count(count)
	{}

	board::board(const board_setting& setting, const staus_callback& callback):
		_callback(callback),_bits_count(0),
		_board(boost::extents[setting.longitudinal][setting.lateral]), 
		_maxinum_count(setting.maxinum_count)
	{

	}

	const board::data_type& board::data_of(const multi_index& index) const
	{
		return const_cast<board*>(this)->_data_of(index);
	}

	board::length_type board::shape(std::size_t dim) const
	{
		return _board.shape()[dim];
	}

	board::data_type& board::_data_of(const multi_index& index)
	{
		return _board[index.axis(longitudinal)][index.axis(lateral)];
	}

	void board::_mapping_multi_index(multi_index& index)
	{	
		auto _mapping=[&](operation_axis axis)
		{
			if (index.axis(axis) == none_pos) index.axis(axis) = _board.shape()[axis] - 1;
		};
		_mapping(lateral); _mapping(longitudinal);
	}

	void board::_mapping_operation_detail(operation_detail& detail)
	{
		_mapping_multi_index(detail.start);
		//这里的负数的边界是用来反向扫描的
		detail.boundary = 
			(detail.boundary == 0) ? --detail.boundary : _board.shape()[detail.axis];
	}

	void board::_random_piece()
	{//慢的很..
		if (_full_board()) return;

		typedef std::uniform_int_distribution<> machine;

		std::random_device device;
		std::mt19937 seed(device() );		
		machine rand(0, _board.num_elements() - 1);

		multi_index index(0, 0);
		length_type 
			lateral_length = _board.shape()[lateral], 
			longitudinal_length = _board.shape()[longitudinal];
#pragma  warning(push)
#pragma  warning(disable:4018)
		do 
		{
			length_type pos=rand(seed);
			index.axis(longitudinal) = floor(pos / lateral_length);
			index.axis(lateral) = pos - lateral_length*index.axis(longitudinal);
		} while (_data_of(index)!=0);
		assert(index != multi_index(_board.shape()[longitudinal],_board.shape()[lateral]) );
#pragma warning(pop)
		machine rand_data(1, 2);//注意，index绝不会是无效的,以及是不是要给个选项来设定随机数值范围呢。。
		data_type data = rand_data(seed);
		_set_board(index,data);
		_callback.random_piece(index,data);
		_check_dead_board();
	}

	bool board::_full_board() const
	{
		return _bits_count == _board.num_elements();
	}
	bool board::_check_dead_board()	const
	{

#pragma  warning(push)
#pragma  warning(disable:4018)
		auto trans_data =
		[&](multi_index index, operation_axis axis)->data_type
		{
			length_type& main_axis = index.axis(axis);
			return (++main_axis >= 0 && main_axis < _board.shape()[axis]) ?
					data_of(index) : none_pos;
		};
#pragma warning(pop)

		if (!_full_board()) return false;
		bool is_dead = false;
		for (std::size_t lat=0; lat != _board.shape()[longitudinal];++lat)
		for (std::size_t lon = 0; lon != _board.shape()[lateral];++lon)
		{
			assert(_board[lat][lon] != 0);
			const data_type& cur = _board[lat][lon];

#define  check_ret(val_name,axis)\
			data_type val_name= trans_data(multi_index(lat, lon),axis);\
			if (val_name != -1 && !(is_dead = (cur != val_name))) return false;

			check_ret(cur_s_left, lateral);
			check_ret(cur_s_down, longitudinal);

#undef check_ret
		}
		return is_dead ? _callback.dead_board(), true : false;
	}
	void board::_move(const multi_index& src, const multi_index& dest)
	{
		data_type& src_data = _data_of(src);
		_callback.move(src, dest);
		_set_board(dest, src_data);	
		_set_board(src, 0);
	}

	void board::_merge(const multi_index& src, const multi_index& dest)
	{
		data_type& dest_data = _data_of(dest);
		bool is_same = (_data_of(src) == dest_data);
		_move(src,dest);
		if (is_same)
		{
			_set_board(dest, ++dest_data);
			if (dest_data == _maxinum_count) _callback.win();
		}
	}
	void board::_set_board(const multi_index& index, const data_type& data)
	{
		data_type& temp = _data_of(index);
		
		bool temp_zero = (temp == 0), data_zero = (data == 0);
		temp_zero ?
			(data_zero ? _bits_count : ++_bits_count) : (data_zero ? --_bits_count : _bits_count);
		temp = data;
		_callback.change(index, data);
	}

	void board::start(std::size_t intial_piece)
	{
		if (intial_piece > _board.num_elements())
			throw std::runtime_error("intial_piece can not greate than board.num_elements");
		while (intial_piece-- != 0) _random_piece();
	}

	void board::change(operation direction)
	{//应该可以消除代码重复
		auto mains=_mains_axis[direction];
		_mapping_operation_detail(mains);
		auto vices=_vice_axis[direction];
		_mapping_operation_detail(vices);
		_change_board(mains, vices);
	}

	void board:: _change_board(const operation_detail& mains, const operation_detail& vices)
	{
		_each_axis(vices, [&](multi_index index)
		{
			operation_detail mains_temp = mains;
			while (true)
			{
				mains_temp.start= index;
				_axis_next(mains_temp);
				mains_temp.start = mains_temp.start;//如果index就是非0的怎么办的情况下
				piece_info next_piece = _next_piece(mains_temp);
				if (next_piece.second == none_pos) return false;
				data_type& cur_data = _data_of(index);
				multi_index& next_index = next_piece.first;
				if (cur_data!=0)
				{
					if (cur_data == next_piece.second)
					{
						_merge(next_index, index);
						index = mains_temp.start;
					}
					else
					{
						index = mains_temp.start;
						if (index != next_index) _merge(next_index, index);
					}
					continue;
				}
				_move(next_index, index);
			}
			return false;
		});
		_random_piece();
	}

	void board::_axis_next(multi_index& index, operation_axis axis, length_type advance)
	{
		index.axis(axis) += advance;
	}
	void board::_axis_next(operation_detail& detail)
	{
		detail.start.axis(detail.axis) += detail.main_axis_setp;
	}
	board::piece_info board::_next_piece(const operation_detail& detail)
	{
		//找到从index开始的detail.axis轴的第一个非空方块，如果没有则返回-1
		piece_info result(multi_index(none_pos, none_pos), none_pos);
		_each_axis(detail, [&](multi_index index)
		{
			data_type& data = _data_of(index);
			if (data != 0)
			{
				result = piece_info(index, data);
				return true;
			}
			return false;
		});
		return result;
	}

	void board::_each_axis(const operation_detail& detail, multi_fn fn)
	{
		multi_index start = detail.start;
		length_type& axis = start.axis(detail.axis);
		for (; axis != detail.boundary;axis+=detail.main_axis_setp)
			if (fn(start)) break;
	}

	multi_index::multi_index(const length_type longitudinal, const length_type lateral)
		:_lateral(lateral), _longitudinal(longitudinal)
	{

	}

	multi_index::length_type& multi_index::axis(const operation_axis axis)
	{
		return axis == lateral ? _lateral : _longitudinal;
	}

	const multi_index::length_type& multi_index::axis(const operation_axis axis) const
	{
		return const_cast<multi_index*>(this)->axis(axis);
	}
	const bool multi_index::operator == (const multi_index& mtx)
	{
		return _longitudinal == mtx._longitudinal&&_lateral == mtx._lateral;
	}

	const bool multi_index::operator != (const multi_index& mtx)
	{
		return !((*this) == mtx);
	}

	operation_detail::operation_detail(
		const multi_index index, const operation_axis axis, const length_type boundary, const setp_type setp):
		start(index), axis(axis), boundary(boundary), main_axis_setp(setp)
	{}
}