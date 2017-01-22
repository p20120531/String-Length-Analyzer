#ifndef _UTIL_H_
#define _UTIL_H_
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

bool isNumber(const string&);
string itos(const size_t&);
void splitLine(ofstream& , const char* );

#endif
