#include "autMgr.h"
#include <cstdio>
//#define AUTMGR_NDEBUG

void AutMgr::dot2blif(const char* inFileName, const char* outFileName)
{
    cout << "dot2blif\n";
    TGraph* tgraph = new TGraph(inFileName);
    tgraph->print();
    tgraph->write(outFileName);
}

void AutMgr::writeCompleTransition(ofstream& file, const vector<string>& alphabetList, const string& s1, const string& s2, const size_t idx)
{
    for (size_t j = 0; j < 256; ++j) {
        if (j != epsilonEncode && j != leftEncode && j != rightEncode && j != idx)
            file << "\n" << alphabetList[j] << s1 << s2 << " 1";
    }
}

void AutMgr::regex2blif_r(const vector<string>& alphabetList, const string& target, const string& fileName)
{
    // TODO : Only support string literal or complement of string literal
    string regex(target);
    #ifndef AUTMGR_NDEBUG
        cout << "[regex2blif] regex=" << regex << endl
             << "             file=" << fileName << endl;
    #endif
    assert((regex[0] == '\"') && (*(regex.rbegin()) == '\"'));
    regex = regex.substr(1,regex.size()-2);
    if (regex == ".*" || regex == "~(.*)" || regex == "" || regex == "~()")
        return;
    ofstream file(fileName.c_str());
    /*
    if (regex == ".*") {
        file << ".model sigma_star"
             << "\n.inputs";
        for (size_t i = 1; i <= 8; ++i) file << " i" << i;
        file << " s1 n1";
        file << "\n.outputs i o t";
        
        // initial state
        file << "\n.names s1 i"
             << "\n0 1";
        
        // accepting state
        file << "\n\n.names s1 o"
             << "\n0 1";
        
        // transition function
        file << "\n\n.names";
        for (size_t i = 1; i <= 8; ++i) file << " i" << i;
        file << " s1 n1 t";
        writeCompleTransition(file,alphabetList,"0","0");
    }
    else if (regex == "~(.*)") {
        file << ".model empty_set"
             << "\n.inputs";
        for (size_t i = 1; i <= 8; ++i) file << " i" << i;
        file << " s1 n1";
        file << "\n.outputs i o t";
        
        // initial state
        file << "\n.names s1 i"
             << "\n0 1";
        
        // accepting state
        file << "\n\n.names s1 o";
        
        // transition function
        file << "\n\n.names";
        for (size_t i = 1; i <= 8; ++i) file << " i" << i;
        file << " s1 n1 t";
    }
    else if (regex == "") {
        file << ".model epsilon"
             << "\n.inputs";
        for (size_t i = 1; i <= 8; ++i) file << " i" << i;
        file << " s1 n1";
        file << "\n.outputs i o t";
        
        // initial state
        file << "\n.names s1 i"
             << "\n0 1";
        
        // accepting state
        file << "\n\n.names s1 o"
             << "\n0 1";
        
        // transition function
        file << "\n\n.names";
        for (size_t i = 1; i <= 8; ++i) file << " i" << i;
        file << " s1 n1 t";
    }
    else if (regex == "~()") {
        file << ".model sigma_plus"
             << "\n.inputs";
        for (size_t i = 1; i <= 8; ++i) file << " i" << i;
        file << " s1 n1";
        file << "\n.outputs i o t";
        
        // initial state
        file << "\n.names s1 i"
             << "\n0 1";
        
        // accepting state
        file << "\n\n.names s1 o"
             << "\n1 1";
        
        // transition function
        file << "\n\n.names";
        for (size_t i = 1; i <= 8; ++i) file << " i" << i;
        file << " s1 n1 t";
        writeCompleTransition(file,alphabetList,"0","1");
        writeCompleTransition(file,alphabetList,"1","1");
    }
    */
    if (regex[0] == '~' && regex[1] == '(' && (*regex.rbegin()) == ')') {
        regex = regex.substr(2,regex.size()-3);
        size_t stateBitNum = 0, maxState = regex.size() + 1;
        while (maxState % 2 != 0 || maxState / 2 != 0) {
            ++stateBitNum;
            maxState /= 2;
        }
        vector<string> stateList;
        for (size_t i = 0, maxState = regex.size() + 1; i <= maxState; ++i)
            stateList.push_back(Uint2BitString(i,stateBitNum));
        
        #ifndef AUTMGR_NDEBUG
            cout << "stateBitNum=" << stateBitNum << endl
                 << "stateList\n";
            for (size_t i = 0, size = stateList.size(); i < size; ++i)
                cout << stateList[i] << endl;
            cout << "regex=" << regex << endl;
            for (size_t i = 0, size = regex.size(); i < size; ++i)
                cout << regex[i] << "->" << int(regex[i]) << endl;
        #endif

        file << ".model ~(" << regex << ")"
             << "\n.inputs";
        for (size_t i = 1; i <= 8; ++i)           file << " i" << i;
        for (size_t i = 1; i <= stateBitNum; ++i) file << " s" << i;
        for (size_t i = 1; i <= stateBitNum; ++i) file << " n" << i;
        file << "\n.outputs i o t";
        
        // initial state
        file << "\n.names";
        for (size_t i = 1; i <= stateBitNum; ++i) file << " s" << i;
        file << " i\n" << stateList[0] << " 1\n";
        
        // accepting state
        file << "\n.names";
        for (size_t i = 1; i <= stateBitNum; ++i) file << " s" << i;
        file << " o";
        for (size_t i = 0, size = stateList.size(); i < size; ++i)
            if (i != size-2) {
                file << "\n" << stateList[i] << " 1";
            }

        // transition function
        const string& live = *(stateList.rbegin());
        file << "\n\n.names";
        for (size_t i = 1; i <= 8; ++i)           file << " i" << i;
        for (size_t i = 1; i <= stateBitNum; ++i) file << " s" << i;
        for (size_t i = 1; i <= stateBitNum; ++i) file << " n" << i;
        file << " t";
        for (size_t i = 0,size = regex.size(); i < size; ++i) {
            size_t idx = int(regex[i]);
            file << "\n" << alphabetList[idx] << stateList[i] << stateList[i+1] << " 1";
            writeCompleTransition(file,alphabetList,stateList[i],live,idx);
            file << "\n";
        }
        size_t pos = stateList.size() - 2;
        writeCompleTransition(file,alphabetList,stateList[pos],live);
        writeCompleTransition(file,alphabetList,live,live);
    }
    else {
        size_t stateBitNum = 0, maxState = regex.size();
        while (maxState % 2 != 0 || maxState / 2 != 0) {
            ++stateBitNum;
            maxState /= 2;
        }
        vector<string> stateList;
        for (size_t i = 0, maxState = regex.size(); i <= maxState; ++i)
            stateList.push_back(Uint2BitString(i,stateBitNum));
        
        #ifndef AUTMGR_NDEBUG
            cout << "stateBitNum=" << stateBitNum << endl
                 << "stateList\n";
            for (size_t i = 0, size = stateList.size(); i < size; ++i)
                cout << stateList[i] << endl;
            cout << "regex=" << regex << endl;
            for (size_t i = 0, size = regex.size(); i < size; ++i)
                cout << regex[i] << "->" << int(regex[i]) << endl;
        #endif

        file << ".model " << regex
             << "\n.inputs";
        for (size_t i = 1; i <= 8; ++i)           file << " i" << i;
        for (size_t i = 1; i <= stateBitNum; ++i) file << " s" << i;
        for (size_t i = 1; i <= stateBitNum; ++i) file << " n" << i;
        file << "\n.outputs i o t";
        
        // initial state
        file << "\n.names";
        for (size_t i = 1; i <= stateBitNum; ++i) file << " s" << i;
        file << " i\n" << stateList[0] << " 1\n";
        
        // accepting state
        file << "\n.names";
        for (size_t i = 1; i <= stateBitNum; ++i) file << " s" << i;
        file << " o\n" << *(stateList.rbegin()) << " 1\n";
        
        // transition function
        file << "\n.names";
        for (size_t i = 1; i <= 8; ++i)           file << " i" << i;
        for (size_t i = 1; i <= stateBitNum; ++i) file << " s" << i;
        for (size_t i = 1; i <= stateBitNum; ++i) file << " n" << i;
        file << " t";
        for (size_t i = 0,size = regex.size(); i < size; ++i) {
            size_t idx = int(regex[i]);
            file << "\n" << alphabetList[idx] << stateList[i] << stateList[i+1] << " 1";
        }
    }
    file << "\n\n.end";
    file.close();
}

void AutMgr::regex2blif(const char* autFileName)
{
    string line;
    ifstream autFile(autFileName);

    if (!autFile) {
        cout << "fail open file=" << autFileName << endl;
        return;
    }
    
    string path(autFileName);
    path = path.substr(0,path.find_last_of("/")) + "/";
    cout << "[regex2blif] DG=" << path << endl;
    
    // initialize alphabet list
    vector<string> alphabetList;
    for (size_t i = 0; i < 256; ++i)
        alphabetList.push_back(bitset<8>(i).to_string());
        
    vector<string> tokenList;
    while(getline(autFile,line)) {
        tokenList.clear();
        str2tokens(line,tokenList);
        #ifndef AUTMGR_NDEBUG
        cout << "----------" << endl;
        for (size_t i = 0, size = tokenList.size(); i < size; ++i)
            cout << tokenList[i] << endl;
        #endif
        regex2blif_r(alphabetList,tokenList[1],path+tokenList[0]+".blif");
    }
}

void AutMgr::blif2vmt(const char* inFileName, const char* outFileName)
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
    
    size_t stateCnt = 0, sCnt = 0, dCnt = 0;
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
            a->_stateVarNum = 0;
            for (size_t i = 0, size = tokenList.size(); i < size; ++i) {
                if (tokenList[i][0] == 's')
                    ++a->_stateVarNum;
            }
            if ( a->_stateVarNum >= Aut::stateBitNum )
                Aut::expandVarList(STATE);
            for (size_t i = 0; i < a->_stateVarNum; ++i) {
                a->_state[0].push_back(Aut::state[0][i]);
                a->_state[1].push_back(Aut::state[1][i]);
            }
            for (size_t i = 0; i < 8; ++i)
                a->defineFun( "d" + itos(dCnt++), "(! x" + itos(i) + " :next x" + itos(i) + ".next", a->_nxtList);
            for (size_t i = 0; i < a->_stateVarNum; ++i)
                a->defineFun( "d" + itos(dCnt++), "(! s" + itos(i) + " :next s" + itos(i) + ".next)", a->_nxtList);
        }
        else if (tokenList[0] == ".names" || tokenList[0] == ".end") {
            if (sCnt != 0) {
                if (cubeList.size() > 1) {
                    VmtNode* newNode = new VmtNode("or");
                    newNode->_children = cubeList;
                    iot[sCnt-1]->addChild(newNode);
                    for (size_t i = 0, size = cubeList.size(); i < size; ++i)
                        a->_imdList.push_back(cubeList[i]);
                }
                else {
                    iot[sCnt-1]->_children = cubeList[0]->_children;
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
            s += ")";
            /*VmtNode* newNode = new VmtNode("d"+itos(++dCnt));
            newNode->addChild(Aut::buildVmtNode(s,0,s.size(),a->_vmap));
            cubeList.push_back(newNode);*/
            a->defineFun( "d" + itos(dCnt++), s, cubeList);
        }
        else {
            cout << "[WARNING] ignored input=\"" << line << "\"" << endl;
        }
    }
    inFile.close();
    a->_imdList.push_back(iNode);
    a->_imdList.push_back(oNode);
    a->_imdList.push_back(tNode);
    /*VmtNode* INode = new VmtNode("I");
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
    a->_itoList.push_back(ONode);*/
    a->defineFun( "I", "(! i :init true)"             , a->_itoList);
    a->defineFun( "T", "(! t :trans true)"            , a->_itoList);
    a->defineFun( "O", "(! (not o) :invar-property 0)", a->_itoList);
    //Write Output
    a->write(outFileName);
}

void AutMgr::readCmdFile(const char* fileName)
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
    cout << ">> dgDir = " << path << endl;
    Aut* cur = 0;
    Aut* a1 = 0;
    Aut* a2 = 0;
    vector<string> tokenList;
    while(getline(file,line)) {
        tokenList.clear();
        str2tokens(line,tokenList);
        if (tokenList[0] == "intersect") {
            cout << "intersect " << tokenList[1] << " " << tokenList[2] << endl;
            a1  = new Aut(path+tokenList[1]+".vmt");
            a2  = new Aut(path+tokenList[2]+".vmt");
            cur = new Aut();
            cur->intersect(a1,a2);
        }
        else if (tokenList[0] == "concate") {
            cout << "concate " << tokenList[1] << " " << tokenList[2] << endl;
            a1  = new Aut(path+tokenList[1]+".vmt");
            a2  = new Aut(path+tokenList[2]+".vmt");
            cur = new Aut();
            cur->concate(a1,a2);
        }
        else if (tokenList[0] == "addlen") {
            cout << "addlen " << tokenList[1] << " " << tokenList[2] << endl;
            cur = new Aut(path+tokenList[1]+".vmt");
            cur->addlen(tokenList[2]);
        }
        else if (tokenList[0] == "read") {
            cout << "read " << tokenList[1] << endl;
            cur = new Aut(path+tokenList[1]+".vmt");
        }
        else if (tokenList[0] == "addpred") {
            //string defstr  = path + "def";
            string predstr = path + "pred";
            //cout << "addpred def=" << defstr << " pred=" << predstr << endl;
            //readDefFile(defstr);
            //readPredFile(predstr);
            //mergeStringAndPred(cur);
            cur->addpred(predstr);
        }
        else if (tokenList[0] == "write") {
            cout << "write " << tokenList[1] << endl;
            cur->write(path+tokenList[1]+".vmt");
        }
        else
            cout << "[ERROR] invalid command=" << tokenList[0] << endl;
    }
    /*
    string defstr = path + "def";
    string predstr = path + "pred";
    cout << "defFile = " << defstr << endl << "predFile = " << predstr << endl;
    readDefFile(defstr);
    readPredFile(predstr);
    mergeStringAndPred(cur);
    cout << path+"sink.vmt" << endl;
    cur->write(path+"sink.vmt");
    */
}
