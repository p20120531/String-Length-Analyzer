#ifndef _UTIL_H_
#define _UTIL_H_
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

bool isNumber(const string&);
string itos(const size_t&);
void splitLine(ofstream& , const char* );
void str2tokens(const string& , vector<string>&);

#endif
