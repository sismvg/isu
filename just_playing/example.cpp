#include <iostream>
#include "defer.hpp"

class teset1
{
public:
test1(int val)
:_val(val)
{}

void print()
{
std::cout<<val<<std::endl;
}

~test1()
{
std::cout<<_val<<std::endl;
}

private:
int _val;
};

void test_fn(int val)
{
std::cout<<val<<std::endl;
}

int main()
{
test1 my1(1000);
test2 my2(2000);
defer
{
my1.print();
};
defer_fn(test_fn,10045);
throw std::runtime_error("");
return 0;
}
