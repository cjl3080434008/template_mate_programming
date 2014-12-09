template_mate_programming
=========================

Some useful code programed with template technology under C++11 standard.

Require: C++11 Support

adapter.h: 
  Adapter. It can include the reference of variety different type into one class, and provide neccessory static case.
  
detect.h:
  Some macro and temnplate class to detect the exist of some memory function.
  
tupleinitclass.h
  Some template class that help to contruct a class with a tuple.
  Needed: "tuple"
  
inheriance.h
  Connect differnet class into single class, and provide some helpful class and function for the polling of each type.
  Needed: "tupleinitclass.h"

typepair.h
  Similer to std::pair, but it stores type informaton and init parameter.
  Needed: "tuple"
  
Chinese:
  
adapter.h: 
  用模板制成的适配器类，能够将多个或者一个类型的引用集合到一个类中，能达到节省参数的目的。
  
detect.h:
  利用模板和宏来创建的一系列判断某个类是否存在某个函数或者某个typdefef。
  
tupleinitclass.h
  一个包装，通过该包装能使类能通过对应的tuple来进行初始化
  Needed: "tuple"
  
inheriance.h
  不平衡树的继承结构，将多个不同的类型集合到一个类中。
  Needed: "tupleinitclass.h"

typepair.h
  利用模板和tuple制成的一个能同时储存多个类类型信息和初始化参数的神奇的东西，有点类似于std::pair。
  Needed: "tuple"
