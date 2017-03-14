#include "utility.h"

bool isNumber(const string& s)
{
    size_t size = s.size();
    if (size == 0) return false;
    else if (size == 1) {
        if (s[0] < 48 || s[0] > 57)
            return false;
        else
            return true;
    }
    else {
        if (s[0] == 48) return false;
        bool b = true;
        for (size_t i=0; i<size; ++i)
            if (s[0] < 48 || s[i] > 57){
                b = false;
                break;
            }
        return b;
    }
}

string itos(const size_t& n)
{
    stringstream ss;
    ss << n;
    return ss.str();
}

void splitLine(ofstream& outFile, const char* s)
{
    outFile << endl;
    outFile << "--------------------"
            << s
            << "--------------------";
    outFile << endl;
}

void str2tokens(const string& s, vector<string>& tokenList)
{
    for (size_t i = 0, j = 0, size = s.size(); i < size; ++i) {
        if (s[i] == ' ') continue;
        j = i;
        while (s[j] != ' ' && j < size) ++j;
        tokenList.push_back(s.substr(i,j-i));
        i = j;
    }
}
