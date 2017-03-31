#include "autopMgr.h"
#include <cstdio>
void AutOpMgr::blif2vmt(const char* inFileName, const char* outFileName)
{
    string line;
    ifstream inFile(inFileName);
    
    // Read Input
    if (!inFile) {
        cout << "fail open file=" << inFileName << endl;
        return;
    }
    Aut* a = new Aut();
    VmtNode* iNode = new VmtNode("i");
    VmtNode* oNode = new VmtNode("o");
    VmtNode* tNode = new VmtNode("t");
    a->_vmap.insert(Str2VmtNode("i",iNode));
    a->_vmap.insert(Str2VmtNode("o",oNode));
    a->_vmap.insert(Str2VmtNode("t",tNode));
    
    int sCnt = -1, dCnt = -1;
    size_t stateCnt = 0;
    VmtNodeList iot;
    iot.push_back(iNode);
    iot.push_back(oNode);
    iot.push_back(tNode);
    VmtNodeList cubeList;
    vector<string> tokenList;
    while (getline(inFile,line)) {
        if (line.empty() || line[0] == '#') continue;
        tokenList.clear();
        str2tokens(line,tokenList);
        if (tokenList[0] == ".inputs") {
            // in blif : i=input s=current state n=next state
            for (size_t i = 9,size = tokenList.size(); i < size; ++i) {
                if (tokenList[i][0] == 's')
                    ++stateCnt;
            }
            assert((stateCnt <= _stateBitNum));
            a->_stateVarCnt = stateCnt;
            for (size_t i = 0; i < stateCnt; ++i) {
                a->_snList[0].push_back(_xsList[1][i]);
                a->_snList[2].push_back(_xsList[3][i]);
            }
            for (size_t i = 0; i < 8; ++i) {
                string nstr = itos(i);
                string s = "(! x" + nstr + " :next x" + nstr + ".next)";
                VmtNode* newNode = new VmtNode("d"+itos(++dCnt));
                newNode->addChild(Aut::buildVmtNode(s,0,s.size(),a->_vmap));
                a->_nxtList.push_back(newNode);
            }
            for (size_t i = 0; i < stateCnt; ++i) {
                string nstr = itos(i);
                string s = "(! s" + nstr + " :next s" + nstr + ".next)";
                VmtNode* newNode = new VmtNode("d"+itos(++dCnt));
                newNode->addChild(Aut::buildVmtNode(s,0,s.size(),a->_vmap));
                a->_nxtList.push_back(newNode);
            }
        }
        else if (tokenList[0] == ".names" || tokenList[0] == ".end") {
            if (sCnt != -1) {
                if (cubeList.size() > 1) {
                    VmtNode* newNode = new VmtNode("or");
                    newNode->_children = cubeList;
                    iot[sCnt]->addChild(newNode);
                    for (size_t i=0,size=cubeList.size();i<size;++i)
                        a->_imdList.push_back(cubeList[i]);
                }
                else {
                    iot[sCnt]->_children = cubeList[0]->_children;
                }
                cubeList.clear();
            }
            ++sCnt;
        }
        else if (line[0] == '0' || line[0] == '1' || line[0] == '-') {
            string s = "(and";
            if (sCnt < 2) {
                for (size_t i = 0, size = tokenList[0].size(); i < size; ++i) {
                    string nstr = itos(i);
                    if (tokenList[0][i] == '1')
                        s += " s" + nstr;
                    else if (tokenList[0][i] =='0')
                        s += " (not s" + nstr + ")";
                }
            }
            else {
                for (size_t i = 0, size = tokenList[0].size(); i < size; ++i) {
                    if (tokenList[0][i] == '1') {
                        if (i < 8) s += " x" + itos(i);
                        else if(i < 8 + stateCnt) s += " s" + itos(i-8);
                        else s += " s" + itos(i-(8+stateCnt)) + ".next";
                    }
                    else if (tokenList[0][i] == '0') {
                        s += " (not ";
                        if (i < 8) s += "x" + itos(i);
                        else if(i < 8 + stateCnt) s += "s" + itos(i-8);
                        else s += "s" + itos(i-(8+stateCnt)) + ".next";
                        s += ")";
                    }
                }
            }
            s+= ")";
            VmtNode* newNode = new VmtNode("d"+itos(++dCnt));
            newNode->addChild(Aut::buildVmtNode(s,0,s.size(),a->_vmap));
            cubeList.push_back(newNode);
        }
        else {
            cout << "[WARNING] ignored input=\"" << line << "\"" << endl;
        }
    }
    inFile.close();
    a->_imdList.push_back(iNode);
    a->_imdList.push_back(oNode);
    a->_imdList.push_back(tNode);
    VmtNode* INode = new VmtNode("I");
    VmtNode* TNode = new VmtNode("T");
    VmtNode* ONode = new VmtNode("O");
    string si = "(! i :init true)";
    string st = "(! t :trans true)";
    string so = "(! (not o) :invar-property 0)";
    INode->addChild(Aut::buildVmtNode(si,0,si.size(),a->_vmap));
    TNode->addChild(Aut::buildVmtNode(st,0,st.size(),a->_vmap));
    ONode->addChild(Aut::buildVmtNode(so,0,so.size(),a->_vmap));
    a->_itoList.push_back(INode);
    a->_itoList.push_back(TNode);
    a->_itoList.push_back(ONode);
    //Write Output
    a->write(outFileName);
}

void AutOpMgr::initXSListAndEpsilon()
{
    _xsList.assign(4,VmtNodeList());
    for (size_t i = 0; i < 8; ++i) {
        _xsList[0].push_back(new VmtNode("x"+itos(i),INPUT,i));
        _xsList[2].push_back(new VmtNode("x"+itos(i)+".next",INPUT_N,i));
    }
    for (size_t i = 0; i < _stateBitNum; ++i) {
        _xsList[1].push_back(new VmtNode("s"+itos(i),STATE,i));
        _xsList[3].push_back(new VmtNode("s"+itos(i)+".next",STATE_N,i));
    }
    _epsilon = new VmtNode("epsilon");
    _epsilon->addChild(new VmtNode("and"));
    for (size_t i = 0; i < 8; ++i) {
        VmtNode* notNode = new VmtNode("not");
        notNode->addChild(_xsList[0][i]);
        _epsilon->_children[0]->addChild(notNode);
    }
}

void AutOpMgr::readCmdFile(const char* fileName)
{
    string line;
    ifstream file(fileName);
    
    // Read Input
    if (!file) {
        cout << "fail open file=" << fileName << endl;
        return;
    }

    string path(fileName);
    path = path.substr(0,path.find_last_of("/")) + "/";

    Aut* cur = 0;
    Aut* a1 = 0;
    Aut* a2 = 0;
    vector<string> tokenList;
    while(getline(file,line)) {
        tokenList.clear();
        str2tokens(line,tokenList);
        if (tokenList[0] == "intersect") {
            cout << "intersect " << tokenList[1] << " " << tokenList[2] << endl;
            a1  = new Aut(path+tokenList[1]);
            a2  = new Aut(path+tokenList[2]);
            cur = new Aut();
            cur->intersect(a1,a2);
        }
        else if (tokenList[0] == "concate") {
            cout << "concate " << tokenList[1] << " " << tokenList[2] << endl;
            a1  = new Aut(path+tokenList[1]);
            a2  = new Aut(path+tokenList[2]);
            cur = new Aut();
            cur->concate(a1,a2);
        }
        else if (tokenList[0] == "addlen") {
            cout << "addlen " << tokenList[1] << " " << tokenList[2] << endl;
            cur = new Aut(path+tokenList[1]);
            cur->addlen(tokenList[2]);
        }
        else if (tokenList[0] == "write") {
            cout << "write " << tokenList[1] << endl;
            cur->write(path+tokenList[1]);
        }
    }
    string defstr = path + "def";
    string predstr = path + "pred";
    readDefFile(defstr);
    readPredFile(predstr);
    mergeStringAndPred(cur);
    cur->write(path+"arg_"+tokenList[1]);
}

void AutOpMgr::readDefFile(const string& fileName)
{
    string line;
    ifstream file(fileName.c_str());
    
    if (!file) {
        cout << "fail open file=" << fileName << endl;
        return;
    }
    while(getline(file,line)) {
        cout << "line=" << line << endl;
        size_t i = 0;
        while (line[i] != ' ') ++i;
        size_t j = ++i;
        while (line[i] != ' ') ++i;
        string name = line.substr(j,i-j);
        i += 4;
        j = i;
        while (line[i] != ')') ++i;
        string typestr = line.substr(j,i-j);
        VmtNode* newNode = 0;
        cout << "name=" << name << " type=" << typestr << endl;
        if (typestr == "Bool") {
            newNode = new VmtNode(name);
            _defBVList.push_back(newNode);
            _vmap.insert(Str2VmtNode(name,newNode));
        }
        else if(typestr == "Int") {
            newNode = new VmtNode(name,LEN);
            if (*(line.rbegin()) == ')')
                _defIVList.push_back(newNode);
            _vmap.insert(Str2VmtNode(name,newNode));
        }
        else cout << "readDefFile::[WARNING] invalid type=" << typestr << endl;
    }
    file.close();
}

void AutOpMgr::readPredFile(const string& fileName)
{
    string line;
    ifstream file(fileName.c_str());
    if (!file) {
        cout << "fail open file=" << fileName << endl;
        return;
    }
    int pCnt = -1;
    VmtNodeList plist;
    vector<string> slist;
    while(getline(file,line)) {
        cout << "line=" << line << endl;
        VmtNode* newNode = new VmtNode("p"+itos(++pCnt),NOPARAM);
        if (line[8] == '(') {
            if (*(line.rbegin()) == ')') {
                string s = line.substr(8,line.size()-1-8);
                cout << "regular=" << s << endl;
                newNode->addChild(Aut::buildVmtNode(s,0,s.size(),_vmap));
                _predList.push_back(newNode);
            }
            else {
                size_t i = line.size() - 1;
                while (line[i] != ' ') --i;
                string lcvar = line.substr(i+1);
                size_t j = i--;
                while (line[i] != ' ') --i;
                string type = line.substr(i+1,j-(i+1));
                
                cout << "type=" << type << " lcvar=" << lcvar << endl;

                i = 8;
                size_t dCnt = 1;
                while (dCnt != 0) {
                    ++i;
                    if (line[i] == '(') ++dCnt;
                    else if(line[i] == ')') --dCnt;
                }
                ++i;
                string s = line.substr(8,i-8);
                cout << "strlen=" << s << endl;
                VmtNode* tmpnode = Aut::buildVmtNode(s,0,s.size(),_vmap);
                
                if (type == "cstrlen") {
                    string s1 = "(" + tmpnode->_name + " " + tmpnode->_children[0]->_name + " n" + lcvar + ")";
                    newNode->addChild(Aut::buildVmtNode(s1,0,s1.size(),_vmap));
                    _predList.push_back(newNode);
                }
                else if (type == "vstrlen") {
                    slist.push_back("n"+lcvar);
                    plist.push_back(tmpnode->_children[0]);
                }
                else {
                    cout << "readPredFile::[WARNING] invalid type=" << type << endl;
                }
            }
        }
        else {
            size_t i = 8;
            while (line[i] != ')') ++i;
            string name = line.substr(8,i-8);
            cout << "var=" << name << endl;
            Str2VmtNodeMap::iterator it=_vmap.find(name);
            assert((it!=_vmap.end()));
            newNode->addChild(it->second);
            _predList.push_back(newNode);
        }
    }
    for (size_t i=0,size=plist.size(); i<size; ++i) {
        plist[i]->_name = slist[i];
    }
    file.close();
}

void AutOpMgr::print()
{        
    for (VmtNodeList::iterator it=_defBVList.begin(); it!=_defBVList.end(); ++it)
        (*it)->print(3);
    cout << endl;
    for (VmtNodeList::iterator it=_defIVList.begin(); it!=_defIVList.end(); ++it)
        (*it)->print(3);
    cout << endl;
    for (VmtNodeList::iterator it=_predList.begin(); it!=_predList.end(); ++it)
        (*it)->print(3);
}

void AutOpMgr::mergeStringAndPred(Aut* a)
{
    for (size_t i=0,size=_defBVList.size(); i<size; ++i)
        a->_snList[3].push_back(_defBVList[i]);
    for (size_t i=0,size=_defIVList.size(); i<size; ++i)
        a->_snList[3].push_back(_defIVList[i]);
    
    VmtNode* oNode = a->getO();
    VmtNode* newNode1 = new VmtNode("tmp1");
    VmtNode* and1 = new VmtNode("and");
    and1->addChild(oNode);
    for (size_t i=0,size=_predList.size(); i<size; ++i) {
        and1->addChild(_predList[i]);
        a->_imdList.push_back(_predList[i]);
    }
    newNode1->addChild(and1);
    a->_imdList.push_back(newNode1);
    a->setO(newNode1);
    
    VmtNode* iNode = a->getI();
    VmtNode* newNode2 = new VmtNode("tmp2");
    VmtNode* and2 = new VmtNode("and");
    and2->addChild(iNode);
    int eCnt = -1;
    for (size_t i=0,size=a->_snList[1].size(); i<size; ++i) {
        VmtNode* newNode3 = new VmtNode("e"+itos(++eCnt),NOPARAM);
        VmtNode* newNode4 = new VmtNode("=");
        newNode4->addChild(a->_snList[1][i]);
        newNode4->addChild(new VmtNode("0"));
        newNode3->addChild(newNode4);
        a->_imdList.push_back(newNode3);
        and2->addChild(newNode3);
    }
    newNode2->addChild(and2);
    a->_imdList.push_back(newNode2);
    a->setI(newNode2);
}
