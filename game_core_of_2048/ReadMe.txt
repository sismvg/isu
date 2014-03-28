board是主类,在isu命名空间下
构造参数为: 
board_setting:关于表的长，宽设定，以及表中最大的数字为多少的时候就算胜利
staus_callback:为了界面留下的东西，是一堆字函数对象，你可以穿lambda,普通函数，函数对象，都可以
具体的自己看成员名字，怎么看也懂了吧

一直纠结用boost的线程池还是用windows的线程池..所以也就没有写
下个版本会写的，到那个时候staus_callback都是异步的

成员函数:
dead_board();检查是否已经游戏无法再继续下去了，其实没什么用，一旦无法继续下去了以后就直接调用 dead_board回调函数了。。

change ,参数operation是要移动的方向
start 参数是初始的时候有多少个块，如果多余表的大小就异常
shap(std::size_t) size_t其实是enum operation_axis的成员，lateral是横轴，另外一个是纵轴
返回一个该轴在此表中的长度
data_of(multi_index) 返回指定二维坐标的数据

core_of_2048_help只有一个帮助你在控制台下现实表的函数
