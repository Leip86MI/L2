#include "str2i-error.h"
#include <iostream>
#include <sstream>

str2i_error::str2i_error(string str){
  ostringstream os;
  os<<"str2i_error: unable to convert the string " <<"\""<<str<<"\" to an int) ";
   this->str=os.str();
}
const char *str2i_error::what() const throw (){
    return this->str.c_str();
}
str2i_error::~str2i_error() throw (){} 