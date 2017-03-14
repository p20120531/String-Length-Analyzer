#include "autopMgr.h"
#include <cstdio>
void AutOpMgr::blif2vmt(const char* inFileName, const char* outFileName)
{
    string line;
    ifstream inFile(inFileName);
    ofstream outFile(outFileName);
    
    // Read Input
    if (!inFile) {
        cout << "fail open file=" << inFileName << endl;
        return;
    }
    size_t stateCnt = 0;
    char outputName[3] = {'i','o','t'};
    vector<size_t> dCntRange;
    vector<vector<string> > outputs;
    vector<string> tokenList;
    while (getline(inFile,line)) {
        if (line.empty() || line[0] == '#') continue;
        cout << line << endl;
        tokenList.clear();
        str2tokens(line,tokenList);
        if (line[0] == '0' || line[0] == '1' || line[0] == '-') {
            outputs.back().push_back(tokenList[0]);
        }
        else if (tokenList[0] == ".names") {
            outputs.push_back(vector<string>());
        }
        else if (tokenList[0] == ".inputs") {
            for (size_t i = 9,size = tokenList.size(); i < size; ++i) {
                if (tokenList[i][0] == 's')
                    ++stateCnt;
            }
        }
        else {
            
        }
    }
    inFile.close();
    
    //Write Output
    size_t dCnt = 0;
    for (size_t i = 0; i < 8; ++i)
        outFile << "(declare-fun x" << i << " () Bool)\n";
    for (size_t i = 0; i < stateCnt; ++i)
        outFile << "(declare-fun s" << i << " () Bool)\n";
    for (size_t i = 0; i < 8; ++i)
        outFile << "(declare-fun x" << i << ".next () Bool)\n";
    for (size_t i = 0; i < stateCnt; ++i)
        outFile << "(declare-fun s" << i << ".next () Bool)\n";
    
    outFile << ";\n";
    
    for (size_t i = 0; i < 8; ++i)
        outFile << "(define-fun d" << dCnt++ << " () Bool (! x" << i << " :next x" << i << ".next))\n";
    for (size_t i = 0; i < stateCnt; ++i)
        outFile << "(define-fun d" << dCnt++ << " () Bool (! s" << i << " :next s" << i << ".next))\n";
    
    outFile << ";\n";
    
    for (size_t i = 0; i < 3; ++i) {
        vector<string>& cubeList = outputs[i];
        size_t size = cubeList.size();
        if (size > 1) {
            dCntRange.push_back(dCnt);
            for (size_t j = 0; j < size; ++j) {
                outFile << "(define-fun d" << dCnt++ << " () Bool (and";
                if (i < 2)
                    writeSingleCubeIO(cubeList[j],outFile);
                else
                    writeSingleCubeT(cubeList[j],stateCnt,outFile);
                outFile << "))\n";
            }
        }
    }
    dCntRange.push_back(dCnt);

    outFile << ";\n";
    
    size_t rCnt = 0;
    for (size_t i = 0; i < 3; ++i) {
        vector<string>& cubeList = outputs[i];
        size_t size = cubeList.size();
        outFile << "(define-fun " << outputName[i] << " () Bool (";
        if (size == 1) {
            outFile << "and";
            if (i < 2)
                writeSingleCubeIO(cubeList[0],outFile);
            else
                writeSingleCubeT(cubeList[0],stateCnt,outFile);
        }
        else {
            outFile << "or";
            for (size_t j = dCntRange[rCnt]; j < dCntRange[rCnt+1]; ++j)
                outFile << " d" << j;
            ++rCnt;
        }
        outFile << "))\n";
    }
    
    outFile << ";\n";
    outFile << "(define-fun I () Bool (! i :init true))\n"
            << "(define-fun T () Bool (! t :trans true))\n"
            << "(define-fun O () Bool (! (not o) :invar-property 0))";
    outFile.close();
}

void AutOpMgr::writeSingleCubeIO(const string& cube,ofstream& outFile)
{
    for (size_t i = 0, size = cube.size(); i < size; ++i) {
        if (cube[i] == '0') {
            outFile << " (not s" << i << ")";
        }
        else if (cube[i] == '1') {
            outFile << " s" << i;
        }
    }
}

void AutOpMgr::writeSingleCubeT(const string& cube,const size_t& stateCnt,ofstream& outFile)
{
    for (size_t i = 0,size = cube.size(); i < size; ++i) {
        string var;
        if (i < 8) {
            var += "x" + itos(i);
        }
        else if (i < 8 + stateCnt) {
            var += "s" + itos(i-8);
        }
        else {
            var += "s" + itos(i-8-stateCnt) + ".next";
        }
        if (cube[i] == '0') {
            outFile << " (not " << var << ")";
        }
        else if (cube[i] == '1') {
            outFile << " " << var;
        }
    }
}

void AutOpMgr::readCmdFile(const char* fileName)
{
    size_t lcvarCnt = 0;
    string line;
    ifstream file(fileName);
    
    // Read Input
    if (!file) {
        cout << "fail open file=" << fileName << endl;
        return;
    }
    
    vector<string> tokenList;
    while(getline(file,line)) {
        tokenList.clear();
        str2tokens(line,tokenList);
        if (tokenList[0] == "inter") {
            Aut* a1 = new Aut(tokenList[1]);
            Aut* a2 = new Aut(tokenList[2]);
            Aut* a3 = new Aut();
            a1->addlen(lcvarCnt++);
            a2->addlen(lcvarCnt++);
            a3->inter(a1,a2);
            a3->write(tokenList[3].c_str());
        }
        else if (tokenList[0] == "concate") {
            Aut* a1 = new Aut(tokenList[1]);
            Aut* a2 = new Aut(tokenList[2]);
            Aut* a3 = new Aut();
            a1->addlen(lcvarCnt++);
            a2->addlen(lcvarCnt++);
            a3->concate(a1,a2);
            a3->write(tokenList[3].c_str());
        }
        else if (tokenList[0] == "addlen") {
            Aut* a = new Aut(tokenList[1]);
            a->addlen(lcvarCnt++);
            a->write(tokenList[1].c_str());
        }
    }
}

