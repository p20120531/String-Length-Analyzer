#include "mgr.h"
void Mgr::printPT()
{
    _pt->mergeNotAndStrInRe();
    splitLine(_logFile,"Mgr::printPT()");
    _pt->writeDBG();
}

void Mgr::parse(const char* fileName)
{
    string line;

    string fstr = string(fileName);
    _path = fstr.substr(0,fstr.find_last_of(".")) + "/";
    cout << ">> parse::path=" << _path << endl;
    system(("mkdir -p "+_path).c_str());
    _logFile.open((_path+"log").c_str());
    
    splitLine(_logFile,"Mgr::parse()");
    ifstream file(fileName);
    if (file) {
        size_t lineCnt = 0,lCnt = 0,rCnt = 0;
        vector<string> tokenList;
        while(getline(file,line)) {
            //cout << "lineCnt"<<++lineCnt <<" \""<< line <<"\""<<endl;
            _logFile << "lineCnt"<<++lineCnt <<" \""<< line <<"\""<<endl;
            for (size_t i=0,j=0,size=line.length(); j < size; ++j) {
                if (line[j] == ' ' || line[j] == ')') {
                    if (i != j ) {
                        tokenList.push_back(line.substr(i,j-i));
                    }
                    if (line[j] == ')') {
                        ++rCnt;
                        tokenList.push_back(")");
                    }
                    i = j + 1;
                }
                else if (line[j] == '(') {
                    ++lCnt;
                    tokenList.push_back("(");
                    i = j + 1;
                }
                else {
                }   
            }
            if (lCnt == rCnt && !tokenList.empty()) {
                lCnt = 0;
                rCnt = 0;
                for (size_t i = 0,size = tokenList.size(); i < size; ++i)
                    _logFile << tokenList[i];
                _logFile << endl;
                handleConstraint(tokenList);
                tokenList.clear();
            }
        }
        file.close();
    }
    else
        _logFile << "fail open" <<endl;
    for (Str2TypeMap::iterator it=_typeMap.begin(); it!=_typeMap.end(); ++it)
        _logFile << it->first << " => " << it->second << '\n';
}

void Mgr::handleConstraint(const vector<string>& tokenList)
{   
    if (tokenList[1] == "assert") {
        addAssertion(handleAssertion(tokenList,2,tokenList.size()-2));
    }
    else if (tokenList[1] == "declare-fun"){
        handleDeclare(tokenList);
    }
}

PTNode* Mgr::handleAssertion(const vector<string>& tokenList,size_t bpos, size_t epos)
{
    PTNode* newNode = NULL;
    
    if (bpos != epos) newNode = buildPTNode(tokenList[bpos+1]);
    else              newNode = buildPTNode(tokenList[bpos]);
    
    size_t l_lCnt = 0, l_rCnt = 0, l_bpos = bpos + 2;
    for (size_t i = bpos+2; i < epos; ++i) {
        
        const string& cstr = tokenList[i];
        
        if      (cstr == "(") ++l_lCnt;
        else if (cstr == ")") ++l_rCnt;
        
        if (l_lCnt == l_rCnt) {
            newNode->addChild(handleAssertion(tokenList,l_bpos,i));
            l_bpos = i + 1;
            l_lCnt = 0;
            l_rCnt = 0;
        }
    }
    return newNode;
}

void Mgr::handleDeclare(const vector<string>& tokenList)
{
    if (tokenList[5] == "Int")
        _typeMap.insert(Str2Type(tokenList[2],VAR_INT));
    else if (tokenList[5] == "Bool")
        _typeMap.insert(Str2Type(tokenList[2],VAR_BOOL));
    else if (tokenList[5] == "String")
        _typeMap.insert(Str2Type(tokenList[2],VAR_STRING));

}

void Mgr::addAssertion(PTNode* n)
{
    _pt->addAssertion(n);
}

PTNode* Mgr::buildPTNode(const string& name)
{
    if      (name == "not"        ) return new PTNotNode(name);
    else if (name == "="          ) return new PTEqNode(name);
    else if (name == "and"        ) return new PTAndNode(name);
    else if (name == "or"         ) return new PTOrNode(name);
    else if (name == "ite"        ) return new PTIteNode(name);
    else if (name == "<"          ) return new PTLTNode(name);
    else if (name == "<="         ) return new PTLTOEQNode(name);
    else if (name == ">"          ) return new PTMTNode(name);
    else if (name == ">="         ) return new PTMTOEQNode(name);
    else if (name == "+"          ) return new PTPlusNode(name);
    else if (name == "-"          ) return new PTMinusNode(name);
    else if (name == "div"        ) return new PTDivNode(name);
    else if (name == "str.++"     ) return new PTStrConcateNode(name);
    else if (name == "str.len"    ) return new PTStrLenNode(name);
    else if (name == "str.in.re"  ) return new PTStrInReNode(name);
    else if (name == "str.replace") return new PTStrReplaceNode(name);
    else if (name == "str.to.re"  ) return new PTStrToReNode(name);
    else if (name == "re.++"      ) return new PTReConcateNode(name);
    else if (name == "re.union"   ) return new PTReUnionNode(name);
    else if (name == "re.inter"   ) return new PTReInterNode(name);
    else {
        Str2TypeMap::iterator it = _typeMap.find(name);
        if (it != _typeMap.end()) {
            const Type& type = it->second;
            switch (type) {
                case VAR_INT :
                    return new PTVarIntNode(name);
                    break;
                case VAR_BOOL :
                    return new PTVarBoolNode(name);
                    break;
                case VAR_STRING:
                    return new PTVarStringNode(name);
                    break;
                default :
                    break;
            }
        }
        else if (name[0] == '\"')                   return new PTConstStringNode(name);
        else if (name == "true" || name == "false") return new PTConstBoolNode(name);    
        else if (isNumber(name))                    return new PTConstIntNode(name);
        else
            _logFile << "invalid string=" << name << " at file=" << _path << endl;
    }
}

