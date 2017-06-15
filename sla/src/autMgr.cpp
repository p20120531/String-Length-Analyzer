#include "autMgr.h"

void AutMgr::dot2blif(const char* inFileName, const char* outFileName)
{
    #ifndef AUTMGR_NDEBUG
        cout << ">> dot2blif::inFile=" << inFileName << " outFile=" << outFileName << endl;
    #endif
    TGraph* tgraph = new TGraph(inFileName);
    //tgraph->print();
    tgraph->write(outFileName);
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
    #ifndef AUTMGR_NDEBUG
        cout << ">> blif2vmt:: inFile=" << inFileName << " outFile=" << outFileName << endl;
    #endif
    
    int sCnt = -1, stateVarNum = 0;
    vector<vector<string> > iotCubeList (3,vector<string>());
    vector<string> tokenList;
    while (getline(inFile,line)) {
        if (line.empty() || line[0] == '#') continue;
        tokenList.clear();
        str2tokens(line,tokenList);
        if (tokenList[0] == ".inputs" || tokenList[0] == ".names") {
            // in blif : x=input s=current state n=next state
            if ( *(tokenList.rbegin()) == "\\") {
                tokenList.pop_back();
                getline(inFile,line);
                vector<string> extraTokens;
                str2tokens(line,extraTokens);
                for (size_t i = 0, size = extraTokens.size(); i < size; ++i)
                    tokenList.push_back(extraTokens[i]);
            }
            #ifndef AUTMGR_NDEBUG
                cout << "tokenList:";
                for (size_t i = 0, size = tokenList.size(); i < size; ++i)
                    cout << " " << tokenList[i];
                cout << endl;
            #endif
            if (tokenList[0] == ".inputs") {
                for (size_t i = 1, size = tokenList.size(); i < size; ++i)
                    if (tokenList[i][0] == 's') ++stateVarNum;
            }
            else {
                ++sCnt;
            }
        }
        else if (line[0] == '0' || line[0] == '1' || line[0] == '-') {
            #ifndef AUTMGR_DEBUG
                cout << "tokenList: " << tokenList[0] << " " << tokenList[1] << endl;
            #endif
            iotCubeList[sCnt].push_back(tokenList[0]);
        }
        else {
            cout << "[WARNING] ignored input=" << line << endl;
        }
    }
    inFile.close();
    
    int dCnt = -1;
    Aut* a = new Aut();
    a->addStateVar(stateVarNum);

    VmtNodeList pList0;
    VmtNodeList pList1;
    for (size_t i = 0; i < stateVarNum; ++i) pList0.push_back(Aut::state[0][i]);
    for (size_t i = 0; i < Aut::inputBitNum; ++i) pList1.push_back(Aut::input[0][i]);
    for (size_t i = 0; i < stateVarNum; ++i) pList1.push_back(Aut::state[0][i]);
    for (size_t i = 0; i < stateVarNum; ++i) pList1.push_back(Aut::state[1][i]);

    // Initial State
    assert( (iotCubeList[0].size() == 1) );
    parseCubeList(a,"i",iotCubeList[0],pList0,dCnt);
    // Transition Relation
    parseCubeList(a,"t",iotCubeList[2],pList1,dCnt);
    // Accepting States
    parseCubeList(a,"o",iotCubeList[1],pList0,dCnt);
    a->defineFun( "I", "(! i :init true)"             , a->_itoList);
    a->defineFun( "T", "(! t :trans true)"            , a->_itoList);
    a->defineFun( "O", "(! (not o) :invar-property 0)", a->_itoList);
    //Write Output
    a->write(outFileName);
}

void AutMgr::parseCubeList(Aut* a, const string& name, const vector<string>& cubeList, const VmtNodeList& pList, int& dCnt)
{
    /*
    cout << pList.size() << endl;
    for (size_t i = 0, size = pList.size(); i < size; ++i) cout << pList[i]->_name << " ";
    cout << "-----" << endl;
    for (size_t i = 0, size = cubeList.size(); i < size; ++i) cout << cubeList[i] << endl;
    cout << "-----" << endl;*/
    assert( (pList.size() == cubeList[0].size()) );
    if (cubeList.size() == 1) {
        string s = "(and";
        for (size_t i = 0, size = cubeList[0].size(); i < size; ++i) {
            if (cubeList[0][i] == '1') 
                s += " " + pList[i]->_name;
            else if (cubeList[0][i] == '0') 
                s += " (not " + pList[i]->_name + ")";
        }
        s += ")";
        a->defineFun( name, s, a->_imdList);
    }
    else {
        string ors = "(or";
        for (size_t i = 0, size = cubeList.size(); i < size; ++i) {
            string s = "(and";
            for (size_t j = 0, size1 = cubeList[i].size(); j < size1; ++j) {
                if (cubeList[i][j] == '1') 
                    s += " " + pList[j]->_name;
                else if (cubeList[i][j] == '0') 
                    s += " (not " + pList[j]->_name + ")";
            }
            a->defineFun( "d" + itos(++dCnt), s + ")", a->_imdList);
            ors += " d" + itos(dCnt);
        }
        a->defineFun( name, ors + ")", a->_imdList);
    }
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
    string sigma_star_dir = "experiment/special_regex/sigma_star.vmt";
    string alphabet_dir   = "experiment/special_regex/alphabet.vmt";
    string path(fileName);
    path = path.substr(0,path.find_last_of("/")) + "/";
    cout << ">> readCmd::dgDir = " << path << endl;
    Aut* cur = 0;
    vector<string> tokenList;
    while(getline(file,line)) {
        tokenList.clear();
        str2tokens(line,tokenList);
        for (size_t i = 0, size = tokenList.size(); i < size; ++i)
            cout << tokenList[i] << " ";
        cout << endl;
        if (tokenList[0] == "intersect" || tokenList[0] == "contains" || tokenList[0] == "notcontains" ||
            tokenList[0] == "notprefixof_smt" || tokenList[0] == "notsuffixof_smt") {
            Aut* a1  = new Aut( path + tokenList[1] + ".vmt" );
            Aut* a2  = new Aut( path + tokenList[2] + ".vmt" );
            cur = new Aut();
            cur->intersect(a1,a2);
        }
        else if (tokenList[0] == "prefixof_smt") {
            Aut* a1 = new Aut( path + tokenList[1] + ".vmt" );
            Aut* a2 = new Aut( path + tokenList[2] + ".vmt" );
            Aut* a3 = new Aut( sigma_star_dir );
            Aut* a4 = new Aut();
            a4->concate(a1,a3);
            cur = new Aut();
            cur->intersect(a4,a2);
        }
        else if (tokenList[0] == "suffixof_smt"){
            Aut* a1 = new Aut( path + tokenList[1] + ".vmt" );
            Aut* a2 = new Aut( path + tokenList[2] + ".vmt" );
            Aut* a3 = new Aut( sigma_star_dir );
            Aut* a4 = new Aut();
            a4->concate(a3,a1);
            cur = new Aut();
            cur->intersect(a4,a2);
        }
        else if (tokenList[0] == "concate") {
            Aut* a1  = new Aut( path + tokenList[1] + ".vmt" );
            Aut* a2  = new Aut( path + tokenList[2] + ".vmt" );
            cur = new Aut();
            cur->concate(a1,a2);
        }
        else if (tokenList[0] == "addlen") {
            cur = new Aut( path + tokenList[1] + ".vmt" );
            cur->addlen(tokenList[2]);
        }
        else if (tokenList[0] == "replace") {
            Aut* a1 = new Aut( path + tokenList[1] + ".vmt" );
            Aut* a2 = new Aut( path + tokenList[2] + ".vmt" );
            Aut* a3 = new Aut( path + tokenList[3] + ".vmt" );
            Aut* ah = new Aut( path + tokenList[4] + ".vmt" );
            size_t alpha = a1->mark();
            Aut* a4 = new Aut();
            a4->replace_A4(a2,ah);
            Aut* a5 = new Aut();
            a5->intersect(a1,a4);
            cur = new Aut();
            cur->replace(a5,a3,alpha);
        }
        else if (tokenList[0] == "substr") {
            cur = new Aut( path + tokenList[1] + ".vmt" );
            cur->substr(tokenList[2],tokenList[3]);
        }
        else if (tokenList[0] == "indexof") {
            Aut* a1 = new Aut( path + tokenList[3] + ".vmt" );
            Aut* a2 = new Aut( alphabet_dir );
            Aut* a3 = new Aut();
            a3->concate(a1,a2);
            Aut* a4 = new Aut( sigma_star_dir );
            a4->addlen(tokenList[4]);
            Aut* a5 = new Aut( path + tokenList[2] + ".vmt" );
            Aut* a6 = new Aut();
            a6->concate(a4,a5);
            Aut* a7 = new Aut();
            a7->intersect(a6,a3);
            Aut* a8 = new Aut( sigma_star_dir );
            a8->addlen(tokenList[5]);
            Aut* a9 = new Aut();
            a9->concate(a8,a7);
            Aut* a10 = new Aut( path + tokenList[1] + ".vmt" );
            cur->intersect(a10,a9);
        }
        else if (tokenList[0] == "read") {
            cur = new Aut( path + tokenList[1] + ".vmt" );
        }
        else if (tokenList[0] == "addpred") {
            cur->addpred( path + "pred" );
        }
        else if (tokenList[0] == "write") {
            cur->write( path + tokenList[1] + ".vmt" );
        }
        else
            cout << "[ERROR] invalid command=" << tokenList[0] << endl;
    }
}
