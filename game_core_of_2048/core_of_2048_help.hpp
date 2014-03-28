/*
Copyright : 四元飒 2014
email : sismvg@hotmail.com
这一程序是自由软件，你可以遵照自由软件基金会出版的GNU通用公共许可证条款来修改和重新发布这一程序。或者用许可证的第二版，或者（根据你的选择）用任何更新的版本。
发布这一程序的目的是希望它有用，但没有任何担保。甚至没有适合特定目的的隐含的担保。更详细的情况请参阅GNU通用公共许可证。
*/
#ifndef CORE_OF_2048_HELP_HPP
#define CORE_OF_2048_HELP_HPP

#include <iostream>

namespace isu
{
	class board;
	void show_board(const board&,std::ostream& =std::cout);
}
#endif