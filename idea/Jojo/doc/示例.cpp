#include <go/go.h>
#include <go/channel/channel.h>
#include <go/gohelper.h>
#include <iostream>
#include <windows.h>

//具体参见
//http://blog.csdn.net//fq_hayate

int main()
{
//启动一个jo以异步执行任务
jo
{
//参数默认从外部引用,jo可以替换成joimpl宏
//然后传入参数以控制传参，具体语法与C++lambda一样 joimpl(=) 从外部复制参数
//只要是函数内可以包含的代码jo代码块间都可以写
std::cout<<"start jo"<<endl;
};
Sleep(100);//防止程序退出以至于无法数据
//jo与主线程间的通信,jo间也可以用channel通信#define
isu::channel<int> c;(不用担心析构问题,channel与shared_ptr有一样的特性)  
jo  
{  
//如果使用isu::go::yield函数可以释放自己的时间片，让给别的jo执行
int val=1;  
c<<val;  
};  
int val=0;  
c>>val;(阻塞的，直到有数据被放入C)  
std::cout<<val<<std::endl;  

return 0;
}