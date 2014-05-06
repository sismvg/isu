#include <iostream>
#include "mati.hpp"

int test(int v1,int v2)
{
std::cout<<v1<<std::endl;
return v2;
}
int main()
{
//注意，如果test返回一个不能copy的对象的话，那么在vs下面会报错
//因为vs不支持is_copy_constructible boost对vs似乎也没有办法
//gcc却可以
auto pack=mati(test)(10,30)(39,50);
pack(1,4);
for(int& val:pack)
{
std::cout<<val<<std::endl;
}
return 0;
}
