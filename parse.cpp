#include "dgmgr.h"
void DGMgr::parse(const char* fileName)
{
    string line;

    ifstream file(fileName);
    if (file) {
        size_t lineCnt = 0,lCnt = 0,rCnt = 0;
        vector<string> tokenList;
        while(getline(file,line)) {
            cout << "lineCnt"<<++lineCnt <<" \""<< line <<"\""<<endl;
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
                handleConstraint(tokenList);
                for (size_t i = 0,size = tokenList.size(); i < size; ++i)
                    cout << tokenList[i] << endl;
                tokenList.clear();
            }
        }
        file.close();
    }
    else
        cout << "fail open" <<endl;
    for (Str2VarMap::iterator it=_typeMap.begin(); it!=_typeMap.end(); ++it)
        cout << it->first << " => " << it->second << '\n';
}

void DGMgr::handleConstraint(const vector<string>& tokenList)
{   
    if (tokenList[1] == "assert") {
        DG* ndg = new DG();
        ndg->setSinkNode(handleAssertion(tokenList,2,tokenList.size()-1));
        _dgList.push_back(ndg);
    }
    else if (tokenList[1] == "declare-fun"){
        handleDeclare(tokenList);
    }
}

Node* DGMgr::handleAssertion(const vector<string>& tokenList,size_t bpos, size_t epos)
{
    Node* newNode = new Node();
    newNode->setOpType(tokenList[1]);
    
}

void DGMgr::handleDeclare(const vector<string>& tokenList)
{
    if (tokenList[5] == "Int")
        _typeMap.insert(Str2Var(tokenList[2],VAR_INT));
    else if (tokenList[5] == "Bool")
        _typeMap.insert(Str2Var(tokenList[2],VAR_BOOL));
    else if (tokenList[5] == "String")
        _typeMap.insert(Str2Var(tokenList[2],VAR_STRING));

}
