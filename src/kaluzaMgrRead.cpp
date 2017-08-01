#include "kaluzaMgr.h"

void SmtMgr::initEscapeSet()
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

string SmtMgr::escape(string regex)
{
    //cout << "[SmtMgr::escape] origin regex=" << regex << endl;
    if (regex == ".*" || regex == "") return "\"" + regex + "\"";
    else if (regex == "~(.*)") return "~\\(\".*\"\\)";
    else if (regex == "~()") return "~\\(\"\"\\)";
    bool isComple = 0;
    if (regex[0] == '~' && regex[1] == '(' && *(regex.rbegin()) == ')' ) {
        isComple = 1;
        regex = regex.substr(2,regex.size()-3);
    }
    //cout << "[SmtMgr::escape] trimed regex=" << regex << endl;
    string escaped;
    for (size_t i = 0, size = regex.size(); i < size; ++i) {
        assert( (regex[i] != '"') );
        if (_escapeSet.find(regex[i]) != _escapeSet.end())
            escaped += '\\';
        escaped += regex[i];
    }
    //cout << "[SmtMgr::escape] escaped regex=" << escaped << endl;
    if (isComple) return "~\\(\"" + escaped + "\"\\)";
    else          return "\"" + escaped + "\"";
}

void SmtMgr::read(const char* fileName)
{
    cout << ">> SmtMgr::read::file =" << fileName << endl;
    
    ifstream file(fileName);
    if (!file) {
        cout << "fail open file=" << fileName << endl;
        return;
    }
    
    _path = string(fileName)
    _path = _dir.substr(0,_dir.find_last_of('.')) + "/";
    size_t lineCnt = 0,lCnt = 0,rCnt = 0;
    string line;
    vector<string> tokList;
    vector<vector<string> > contentList;
    while(getline(file,line)) {
        #ifndef _SMTMGR_NDEBUG_
            _logFile << "lineCnt=" << ++lineCnt << " \"" << line << "\"" << endl;
        #endif
        for (size_t i = 0, j = 0, size = line.size(); j < size; ++j) {
            if (line[j] == '(') {
                ++lCnt;
                tokList.push_back("(");
                i = j + 1;
            }
            else if (line[j] == ' ' || line[j] == ')') {
                if (i != j) 
                    tokList.push_back(line.substr(i,j-i));
                if (line[j] == ')') {
                    ++rCnt;
                    tokList.push_back(")");
                }
                i = j + 1;
            }
        }
        if (lCnt == rCnt && !tokList.empty()) {
            #ifndef _SMTMGR_NDEBUG_
                for (size_t i = 0,size = tokList.size(); i < size; ++i)
                    cout << tokList[i];
                cout << endl;
            #endif
            contentList.push_back(tokList);
            // reset counter and token list
            lCnt = 0;
            rCnt = 0;
            tokList.clear();
        }
    }
    file.close();

    // build leaves first, then build assertions
    for (size_t i = 0, size = contentList.size(); i < size; ++i) 
        if      (contentList[i][1] == "declare-fun"  ) declaration(contentList[i],contentList[i][5]);
        else if (contentList[i][1] == "declare-const") declaration(contentList[i],contentList[i][3]);
    for (size_t i = 0, size = contentList.size(); i < size; ++i) 
        if      (contentList[i][1] == "assert"       ) _root->addChild(assertion(contentList[i],2,contentList[i].size()-2));
    
    _root->mergeNotEquivalence();
    buildSmtNodeListMap();
    /*
    _pt->print();
    */
    /*
    _pt->setLevel();
    _pt->buildSmtNodeListMap();
    printTypeMap();
    _pt->printStr2SmtNodeListMap();
    _pt->print();
    */
}

void SmtMgr::declaration(const vector<string>& tokList, const string& type)
{
    const string& name = tokList[2];
    if (type == "Bool") {
        SmtNode* n = new SmtVarBoolNode(name);
        _smtLeafMap.insert(Str2Smt(name,n));
        _piList[0].push_back(n);
    }
    else if (type == "Int") {
        SmtNode* n = new SmtVarIntNode(name);
        _smtLeafMap.insert(Str2Smt(name,n));
        _piList[1].push_back(n);
    }
    else {
        assert(type == "String");
        SmtNode* n = new SmtVarStrNode(name);
        _smtLeafMap.insert(Str2Smt(name,n));
        _piList[2].push_back(n);
    }
}

SmtNode* SmtMgr::assertion(const vector<string>& tokList,size_t bpos, size_t epos)
{
    SmtNode* newNode = 0;
    
    if (bpos != epos) newNode = buildSmtNode(tokList[bpos+1]);
    else              newNode = buildSmtNode(tokList[bpos]);
    
    size_t l_lCnt = 0, l_rCnt = 0, l_bpos = bpos + 2;
    for (size_t i = bpos+2; i < epos; ++i) {
        
        const string& cstr = tokList[i];
        
        if      (cstr == "(") ++l_lCnt;
        else if (cstr == ")") ++l_rCnt;
        
        if (l_lCnt == l_rCnt) {
            newNode->addChild(assertion(tokList,l_bpos,i));
            l_bpos = i + 1;
            l_lCnt = 0;
            l_rCnt = 0;
        }
    }
    return newNode;
}

SmtNode* SmtMgr::buildSmtNode(const string& name)
{
    if      (name == "not"        ) return new SmtNotNode();
    else if (name == "="          ) return new SmtEqNode();
    else if (name == "and"        ) return new SmtAndNode();
    else if (name == "or"         ) return new SmtOrNode();
    else if (name == "ite"        ) return new SmtIteNode();
    else if (name == "<"          ) return new SmtLTNode();
    else if (name == "<="         ) return new SmtLTOEQNode();
    else if (name == ">"          ) return new SmtMTNode();
    else if (name == ">="         ) return new SmtMTOEQNode();
    else if (name == "+"          ) return new SmtPlusNode();
    else if (name == "-"          ) return new SmtMinusNode(); // default MINUS
    else if (name == "div"        ) return new SmtDivNode();
    else if (name == "str.++"     ) return new SmtStrConcateNode();
    else if (name == "str.len"    ) return new SmtStrLenNode();
    else if (name == "str.in.re"  ) return new SmtStrInReNode();
    else if (name == "str.replace") return new SmtStrReplaceNode();
    else if (name == "str.to.re"  ) return new SmtStrToReNode();
    else if (name == "re.++"      ) return new SmtReConcateNode();
    else if (name == "re.union"   ) return new SmtReUnionNode();
    else if (name == "re.inter"   ) return new SmtReInterNode();
    else {
        Str2SmtNodeMap::iterator it = _smtLeafMap.find(name);
        if (it != _smtLeafTypeMap.end())            return it->second;
        else if (name[0] == '\"')                   return new SmtConstStrNode(name);
        else if (name == "true" || name == "false") return new SmtConstBoolNode(name);    
        else if (isNumber(name))                    return new SmtConstIntNode(name);
        else
            cout << "[WARNING:SmtMgr::buildSmtNode] invalid string=" << name << " at path=" << _path << endl;
    }
}

void SmtMgr::buildSmtNodeListMap()
{
    for (size_t i = 0; i < 2; ++i)
        for (size_t j = 0, size = _piList[i].size(); j < size; ++j)
            _smtNodeListMap.insert(Str2SmtNodeList(_piList[i][j],SmtNodeList()));
    
    SmtNodeQueue q;
    q.push(_root);
    while (!q.empty()) {
        SmtNode* cur = q.front();
        q.pop();
        for (SmtNodeList::iterator it = cur->_chikldren.begin(); it != cur->_children.end(); ++it)
            if ( (*it)->_name == "ite" ) {
                for (size_t i = 1; i < 3; ++i)
                    q.push((*it)->_children[i]);
            }
            else if ( (*it)->_name == "or" ){
                for (size_t i = 0, size = _children.size(); i < size; ++i)
                    q.push((*it)->_children[1]);
            }
            else 
                (*it)->buildSmtNodeListMap(*it);
    }
}

void SmtMgr::buildAndWriteDT()
{
    #ifndef _SMTMGR_NDEBUG_
        splitLine(_logFile,"mtMgr::buildAndWriteDG");
    #endif

    SmtNodeList branchList;
    _root->buildBranchList(branchList);
    branching(branchList, 0, branchList.size());
}
/*
def rec(v,i,n,bound) :
    if i == n : 
        print v
        del v[-1]
        return
    for j in range(bound[i]) :
        v.append(j)
        rec(v,i+1,n,bound)
    if i > 0 :
        del v[-1]

rec([],0,3,[1,2,3])
*/
void SmtMgr::branching(const SmtNodeList& branchList, size_t level, const size_t& totLevel)
{
    if (level == totLevel) {
        // buildDG
        return;
    }
    for (size_t i = branchList[level]->getBeginBranchIdx(), size = branchList[level]->getEndBranchIdx(); i < size; ++i) {
        branchList[level]->setBranchIdx(i);
        branching(branchList,level+1,totLevel);
    }
}

void SmtMgr::xxx()
{
    size_t dgIdx = 0;
    PTNodeQueue&   ptq   = _pt->getPTQ();
    IMPQueue&      impq  = _pt->getIMPQ();
    Str2DGNodeMap& dgMap = _pt->getDGMap();
    ptq.push(_pt->_root);
    impq.push(IMP(0,0));
    while (!ptq.empty()) {
        dgMap.clear();
        PTNode* cur = ptq.front();
        IMP curimp = impq.front();
        ptq.pop();
        impq.pop();
        #ifndef _SMTMGR_NDEBUG_
            cout << "[handle " << cur->getName() << "]" << endl;
        #endif
        ++_gflag;
        cur->buildDG();
        size_t bflag = _gflag;
        
        ++_gflag;
        DGNodeList sinkList;
        for (Str2DGNodeMap::iterator it=dgMap.begin(); it!=dgMap.end(); ++it) {
            DGNode* leader = it->second->findLeader();
            if (leader->isSink() && leader->getFlag() != _gflag) {
                leader->setFlag(_gflag);
                sinkList.push_back(leader);
            }
        }

        for (DGNodeList::iterator it=sinkList.begin(); it!=sinkList.end(); ++it) {
            _dg = new DG(*it,_indent,_gflag,bflag,_path,++dgIdx);
            #ifndef _NLOG_
                _logFile << "found sink node = " << (*it)->getName() << endl;
                _logFile << "\n>> processing DG " << dgIdx << endl;
            #endif
            //_dg->print();
            ++_gflag;
            _dg->merge();
            #ifndef _NLOG_
                _dg->print();
                _dg->printLengthVarList();
                _dg->printAssertionList();
            #endif
            _dg->renameLengthVar();
            //_dg->print();
            ++_gflag;
            _dg->writeCVC4File(curimp);
            ++_gflag;
            _dg->writeCmdFile();
        }
        //cout << cur->getName() << ": level=" << cur->getLevel() << " sinkCnt=" << sinkList.size() << endl;
    }

}
