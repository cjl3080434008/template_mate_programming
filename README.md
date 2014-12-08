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
  Needed: <tuple>
  
inheriance.h
  Connect differnet class into single class, and provide some helpful class and function for the polling of each type.
  Needed: "tupleinitclass.h"

typepair.h
  Similer to std::pair, but it stores type informaton and init parameter.
  Needed: <tuple>
