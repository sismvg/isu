#ifndef ISU_PIXLE_HPP
#define ISU_PIXLE_HPP

#include <windows.h>

#include <isu/isudef.hpp>

namespace isu
{
	template<size_t BPP>
	class pixle_impl
	{};

	//暂时不管1和4，太麻烦了
	template<>
	class pixle_impl<1>
	{};

	template<>
	class pixle_impl<4>
		:public pixle_impl<1>
	{};

	template<>
	class pixle_impl<16>
	{//2字节以及一下的都要依赖颜色表，所以返回个word就好了
	public:
		typedef unsigned short int word_type;
		//返回byte的话可能有点效率问题
		byte& at(size_t);
		byte at(size_t) const;
		word_type word() const;
		word_type& word();
	private:
		word_type _word;
	};

	template<>
	class pixle_impl<24>
	{
	public:
		pixle_impl(byte r, byte g, byte b)
			:_red(r), _green(g), _bule(b)
		{}
		byte red() const
		{
			return _red;
		}
		byte blue() const
		{
			return _bule;
		}
		byte green() const
		{
			return _green;
		}
		byte& red()
		{
			return _red;
		}
		byte& blue()
		{
			return _bule;
		}
		byte& green()
		{
			return _green;
		}
	private://bitmap的像素排列
		//byte _bule, _red, _green;
		byte _bule,_green,_red;
	//	byte _red, _green, _bule;
		//byte _red, _bule, _green;
		//byte _green, _red, _bule;
		//byte _green, _bule, _red;
	};

	template<>
	class pixle_impl<32>
		:public pixle_impl<24>
	{
	public:
		pixle_impl(byte r, byte g, byte b, byte a = 0)
			:pixle_impl<24>(r, g, b), _alpha(a)
		{}
		byte alpha() const
		{
			return _alpha;
		}
		byte& alpha()
		{
			return _alpha;
		}
	private:
		byte _alpha;
	};

	template<size_t BPP>
	class pixle
		:public pixle_impl<BPP>
	{
	public:
		pixle(byte r, byte g, byte b, byte a)
			:pixle_impl<BPP>(r, g, b, a)
		{}
		static const size_t bpp = BPP;
	private:
	};

	typedef pixle<32> colour_pixle;
	template<class Pixle>
	COLORREF struct_cast(const Pixle& pixle)
	{
		return RGB(pixle.red(), pixle.green(), pixle.blue());
	}
}
#endif