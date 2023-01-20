#include "str2l-error.h"
#include <iostream>
#include <sstream>

str2l_error:: str2l_error(string str){
    ostringstream os;
    os<<"(str2l_error: unable to convert the string "<<"\""<<str<<"\" to an long) ";
   this->str=os.str();
}

const char *str2l_error::what() const throw (){
    return this->str.c_str();
}

str2l_error::~str2l_error() throw (){} 
