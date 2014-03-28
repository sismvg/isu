/*
Copyright : 四元飒 2014
email : sismvg@hotmail.com
这一程序是自由软件，你可以遵照自由软件基金会出版的GNU通用公共许可证条款来修改和重新发布这一程序。或者用许可证的第二版，或者（根据你的选择）用任何更新的版本。
发布这一程序的目的是希望它有用，但没有任何担保。甚至没有适合特定目的的隐含的担保。更详细的情况请参阅GNU通用公共许可证。
*/
#include "core.hpp"
#include "core_of_2048_help.hpp"

namespace isu
{
	void show_board(const board& core, std::ostream& out)
	{
		board::length_type lateral =
			core.shape(operation_axis::lateral), longitudinal = core.shape(operation_axis::longitudinal);
		for (std::size_t longitudinal_index = 0; longitudinal_index != longitudinal; ++longitudinal_index)
		{
			for (std::size_t lateral_index = 0; lateral_index != lateral; ++lateral_index)
			{
				std::cout << core.data_of(isu::multi_index(longitudinal_index, lateral_index)) << " ";
			}
			std::cout << std::endl;
		}
	}
}
