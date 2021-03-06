#include "kaluzaMgr.h"

void KaluzaMgr::initEscapeSet()
{
    _escapeSet.insert('!');
    //_escapeSet.insert('"');
    _escapeSet.insert('#');
    _escapeSet.insert('$');
    _escapeSet.insert('%');
    _escapeSet.insert('&');
    _escapeSet.insert('\'');
    _escapeSet.insert('(');
    _escapeSet.insert(')');
    _escapeSet.insert('*');
    _escapeSet.insert('+');
    _escapeSet.insert(',');
    _escapeSet.insert('-');
    _escapeSet.insert('.');
    _escapeSet.insert('/');
    _escapeSet.insert(':');
    _escapeSet.insert(';');
    _escapeSet.insert('<');
    _escapeSet.insert('=');
    _escapeSet.insert('>');
    _escapeSet.insert('?');
    _escapeSet.insert('@');
    _escapeSet.insert('[');
    _escapeSet.insert('\\');
    _escapeSet.insert(']');
    _escapeSet.insert('^');
    _escapeSet.insert('_');
    _escapeSet.insert('`');
    _escapeSet.insert('{');
    _escapeSet.insert('|');
    _escapeSet.insert('}');
    _escapeSet.insert('~');
}

string KaluzaMgr::escape(string regex)
{
    //cout << "[KaluzaMgr::escape] origin regex=" << regex << endl;
    if (regex == ".*" || regex == "") return "\"" + regex + "\"";
    else if (regex == "~(.*)") return "~\\(\".*\"\\)";
    else if (regex == "~()") return "~\\(\"\"\\)";
    bool isComple = 0;
    if (regex[0] == '~' && regex[1] == '(' && *(regex.rbegin()) == ')' ) {
        isComple = 1;
        regex = regex.substr(2,regex.size()-3);
    }
    //cout << "[KaluzaMgr::escape] trimed regex=" << regex << endl;
    string escaped;
    for (size_t i = 0, size = regex.size(); i < size; ++i) {
        assert( (regex[i] != '"') );
        if (_escapeSet.find(regex[i]) != _escapeSet.end())
            escaped += '\\';
        escaped += regex[i];
    }
    //cout << "[KaluzaMgr::escape] escaped regex=" << escaped << endl;
    if (isComple) return "~\\(\"" + escaped + "\"\\)";
    else          return "\"" + escaped + "\"";
}

void KaluzaMgr::printTypeMap()
{
    #ifndef _NLOG_
    splitLine(_logFile,"KaluzaMgr::printTypeMap");
    for (Str2TypeMap::iterator it=_typeMap.begin(); it!=_typeMap.end(); ++it) {
        _logFile << it->first << " => ";
        switch (it->second) {
            case VAR_INT :
                _logFile << "Int\n";
                break;
            case VAR_BOOL :
                _logFile << "Bool\n";
                break;
            case VAR_STRING :
                _logFile << "String\n";
                break;
            default :
                break;
        }
    }
    #endif
}

void KaluzaMgr::printPT()
{
    #ifndef _NLOG_
        splitLine(_logFile,"KaluzaMgr::printPT");
    #endif
    _pt->print();
}

void KaluzaMgr::analyzePTASCII()
{
    _pt->analyzeASCII();
}

void KaluzaMgr::analyzePT()
{
    _pt->analyze();
}

void KaluzaMgr::read(const char* fileName,const char* dgDirName = "",const bool isAnalyze)
{
    string line;
    
    cout << ">> read::file =" << fileName << endl
         << "         dgDir=" << dgDirName << endl;
    _file = string(fileName);
    _path = string(dgDirName);
    if (!isAnalyze) {
        #ifndef _NDIR_
            system(("mkdir -p "+_path).c_str());
        #endif
        #ifndef _NLOG_
            _logFile.open((_path+"/log").c_str());
            splitLine(_logFile,"KaluzaMgr::read");
        #endif
    }
    ifstream file(fileName);
    if (file) {
        size_t lineCnt = 0,lCnt = 0,rCnt = 0;
        vector<string> tokenList;
        while(getline(file,line)) {
            #ifndef _NLOG_
                _logFile << "lineCnt=" << ++lineCnt << " \"" << line << "\"" << endl;
            #endif
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
                #ifndef _NLOG_
                    for (size_t i = 0,size = tokenList.size(); i < size; ++i)
                        _logFile << tokenList[i];
                    _logFile << endl;
                #endif
                handleConstraint(tokenList);
                tokenList.clear();
            }
        }
        file.close();
    }
    else
        cout << "fail open" << endl;
    //printTypeMap();
    _pt->mergeNotEquivalence();
    #ifndef _NLOG_
        _pt->print();
    #endif
    _pt->buildPTNodeListMap();
    //_pt->printPTNodeListMap();
    _pt->setLevel();
    #ifndef _NLOG_
        _pt->print();
        _pt->printPTNodeListMap();
    #endif
    /*
    _pt->print();
    */
    /*
    _pt->setLevel();
    _pt->buildPTNodeListMap();
    printTypeMap();
    _pt->printStr2PTNodeListMap();
    _pt->print();
    */
}

void KaluzaMgr::handleConstraint(const vector<string>& tokenList)
{   
    if (tokenList[1] == "assert") {
        addAssertion(handleAssertion(tokenList,2,tokenList.size()-2));
    }
    else if (tokenList[1] == "declare-fun"){
        handleDeclare(tokenList);
    }
}

PTNode* KaluzaMgr::handleAssertion(const vector<string>& tokenList,size_t bpos, size_t epos)
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

void KaluzaMgr::handleDeclare(const vector<string>& tokenList)
{
    if (tokenList[5] == "Bool") {
        _bvList.push_back(tokenList[2]);
        _typeMap.insert(Str2Type(tokenList[2],VAR_BOOL));
    }
    else if (tokenList[5] == "Int") {
        _ivList.push_back(tokenList[2]);
        _typeMap.insert(Str2Type(tokenList[2],VAR_INT));
    }
    else if (tokenList[5] == "String") {
        _svList.push_back(tokenList[2]);
        _typeMap.insert(Str2Type(tokenList[2],VAR_STRING));
    }
}

void KaluzaMgr::addAssertion(PTNode* n)
{
    _pt->addAssertion(n);
}

PTNode* KaluzaMgr::buildPTNode(const string& name)
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
        else {
            #ifndef _NLOG_
                _logFile << "[WARNING:KaluzaMgr::buildPTNode] invalid string=" << name << " at file=" << _file << endl;
            #endif
            cout << "[WARNING:KaluzaMgr::buildPTNode] invalid string=" << name << " at file=" << _file << endl;
        }
    }
}

