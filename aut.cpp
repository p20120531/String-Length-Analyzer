#include "autopMgr.h"
#include <map>

void VmtNode::addChild(VmtNode* n)
{
    _children.push_back(n);
}

void VmtNode::write(ofstream& outFile,const size_t& gflag)
{
    if (_flag == gflag) {
        outFile << _name;
        return;
    }
    _flag = gflag;
    size_t size = _children.size();
    if (size != 0) {
        outFile << "(" << _name;
        for (size_t i = 0; i < size; ++i) {
            outFile << " ";
            _children[i]->write(outFile,gflag);
        }
        outFile << ")";
    }
    else {
        outFile << _name;
    }
}

void Aut::parse(const char* fileName)
{
    string line;
    ifstream file(fileName);

    if (!file) {
        cout << "fail open file=" << fileName << endl;
        return;
    }
    
    vector<string> tokenList;
    size_t sCnt = 0;
    while (getline(file,line)) {
        if (line[0] == ';') {
            ++sCnt;
            continue;
        }
        tokenList.clear();
        //cout << line << endl;
        vmtTokenize(line,tokenList);
        //cout << tokenList[0] << "|" << tokenList[1] << "|" <<endl;
        if (tokenList[0] == "epsilon") continue;
        if (sCnt == 0) {
            VmtNode* newNode = new VmtNode(tokenList[0]);
            _vmap.insert(Str2VmtNode(tokenList[0],newNode));
            _varList.push_back(newNode);
            if (tokenList[0][0] == 's') ++_stateVarCnt;
        }
        else {
            cout << tokenList[0] << "|" << tokenList[1] << "|" <<endl;
            VmtNode* newNode1 = new VmtNode(tokenList[0]);
            VmtNode* newNode2 = buildVmtNode(tokenList[1],0,tokenList[1].size(),_vmap);
            newNode1->addChild(newNode2);
            cout << newNode1->_name << " " << newNode2->_name << endl;
            _vmap.insert(Str2VmtNode(tokenList[0],newNode1));
            if      (sCnt == 1) _nxtList.push_back(newNode1);
            else if (sCnt == 2) _imdList.push_back(newNode1);
            else if (sCnt == 3) _itoList.push_back(newNode1);
        }
    }
    _stateVarCnt /= 2;
    file.close();
}

void Aut::write(const char* fileName)
{
    ofstream file(fileName);
    rename();
    ++_gflag;
    
    for (size_t i = 0,size = _varList.size(); i < size; ++i) {
        file << "(declare-fun " << _varList[i]->_name;
        if (_varList[i]->_name[0] == 'n')
            file << " () Int)\n";
        else
            file << " () Bool)\n";
        _varList[i]->_flag = _gflag;
    }
    file << ";\n";
    
    for (size_t i = 0,size = _nxtList.size(); i < size; ++i) {
        file << "(define-fun " 
             << _nxtList[i]->_name
             << " () Bool ";
        _nxtList[i]->_flag = _gflag;
        _nxtList[i]->_children[0]->write(file,_gflag);
        file << ")\n";
    }
    file << ";\n";

    file << "(define-fun epsilon () Bool (and (not x0) (not x1) (not x2) (not x3) (not x4) (not x5) (not x6) (not x7)))\n";
    for (size_t i = 0,size = _imdList.size(); i < size; ++i) {
        file << "(define-fun " 
             << _imdList[i]->_name
             << " () Bool ";
        _imdList[i]->_flag = _gflag;
        _imdList[i]->_children[0]->write(file,_gflag);
        file << ")\n";
    }
    file << ";\n";

    for (size_t i = 0,size = _itoList.size(); i < size; ++i) {
        file << "(define-fun " 
             << _itoList[i]->_name
             << " () Bool ";
        _itoList[i]->_flag = _gflag;
        _itoList[i]->_children[0]->write(file,_gflag);
        file << ")\n";
    }
}

void Aut::addepsilon()
{
    vector<VmtNode*> newimdList;
    
    VmtNode* newNode = new VmtNode("tmp1");
    VmtNode* tNode   = _itoList[1]->_children[0]->_children[0];
    // (or tnode (and epsilon (= s0.next s0) ... (= sk.next sk)))
}

void Aut::addlen(const size_t& lcvarCnt)
{
    vector<VmtNode*> newimdList;

    // Construct Length Variable
    string lcvar( "n" + itos(lcvarCnt) );
    string lcvarn = lcvar + ".next";
    
    // Add Length Variable
    VmtNode* newNode1 = new VmtNode(lcvar);
    VmtNode* newNode2 = new VmtNode(lcvarn);
    _vmap.insert(Str2VmtNode(lcvar,newNode1));
    _vmap.insert(Str2VmtNode(lcvarn,newNode2));
    _varList.push_back(newNode1);
    _varList.push_back(newNode2);

    // Add Next State
    string s1( "(! " + lcvar + " :next " + lcvarn + ")" );
    newNode1 = new VmtNode("tmp1");
    newNode1->addChild(buildVmtNode(s1,0,s1.size(),_vmap));
    _nxtList.push_back(newNode1);

    // Add Length Constraint
    string s2( "(= " + lcvarn + " (+ " + lcvar + " 1))" );
    string s3( "(= " + lcvarn + " " + lcvar + ")" );
    newNode1 = new VmtNode("tmp2");
    newNode2 = new VmtNode("tmp3");
    _vmap.insert(Str2VmtNode("tmp2",newNode1));
    _vmap.insert(Str2VmtNode("tmp3",newNode2));
    newNode1->addChild(buildVmtNode(s2,0,s2.size(),_vmap));
    newNode2->addChild(buildVmtNode(s3,0,s3.size(),_vmap));
    
    VmtNode* tNode = _itoList[1]->_children[0]->_children[0];
    VmtNode* newNode3 = new VmtNode("tmp4");
    string s4( "(and " + tNode->_name + " (or (and (not epsilon) tmp2) (and epsilon tmp3)))" );
    newNode3->addChild(buildVmtNode(s4,0,s4.size(),_vmap));

    newimdList.push_back(newNode1);
    newimdList.push_back(newNode2);
    newimdList.push_back(tNode);
    for (size_t i = 0,size = _imdList.size(); i < size; ++i) {
        if (_imdList[i] != tNode)
            newimdList.push_back(_imdList[i]);
    }
    newimdList.push_back(newNode3);
    _imdList = newimdList;
    _itoList[1]->_children[0]->_children[0] = newNode3;
}

void Aut::inter(Aut* a1,Aut* a2)
{
    vector<VmtNode*> newimdList;
    
    // Add Epsilon Loop
    VmtNode* newNode = new VmtNode("tmp1");
    
}

void Aut::concate(Aut* a1,Aut* a2)
{
}

void Aut::rename()
{
    int dCnt = -1;
    for (size_t i = 0,size = _nxtList.size(); i < size; ++i) {
        _nxtList[i]->_name = string("d"+itos(++dCnt));
    }
    for (size_t i = 0,size = _imdList.size(); i < size; ++i) {
        _imdList[i]->_name = string("d"+itos(++dCnt));
    }
}

void Aut::vmtTokenize(const string& s,vector<string>& tokenList)
{
    size_t size = s.size();
    // Handle 
    if (s[size-1] == 13) --size;
    size_t state = 0;
    for (size_t i = 0; i < size; ++i) {
        if (state == 0) {
            if (s[i] != ' ') continue;
            else {
                state = 1;
            }
        }
        else if (state == 1) {
            size_t j = i;
            while (s[j] != ' ') ++j;
            tokenList.push_back(s.substr(i,j-i));
            if (j+9 < size) {
                tokenList.push_back(s.substr(j+9,size-1-(j+9)));
            }
            else {
                tokenList.push_back(string(""));
            }
            cout << tokenList[0] << "|" << tokenList[1] << "|" << endl;
            return;
        }
    }
}

VmtNode* Aut::buildVmtNode(const string& s,size_t bpos,size_t epos,Str2VmtNodeMap& vmap)
{
    if (s[bpos] != '(') {
        string name = s.substr(bpos,epos-bpos);
        Str2VmtNodeMap::iterator it = vmap.find(name);
        if (it != vmap.end()) return it->second;
        else {
            VmtNode* newNode = new VmtNode(name);
            //if (name != "!" && name != ":next" && name != "and" && name != "or" && name != "not" && name != ":init" && name != ":trans" && name != "invar-property")
                //vmap.insert(Str2VmtNode(name,newNode));
            return newNode;
        }
    }
    else {
        size_t i = bpos + 1;
        while (s[i] != ' ') ++i;
        VmtNode* newNode = new VmtNode(s.substr(bpos+1,i-(bpos+1)));
        for (++i; i < epos; ++i) {
            size_t j = i;
            if (s[i] == '(') {
                int diff = 0;
                while (diff != 1) {
                    ++j;
                    if (s[j] == '(') --diff;
                    else if (s[j] == ')') ++diff;
                }
                newNode->addChild(buildVmtNode(s,i,j+1,vmap));
                i = j+1;
            }
            else {
                while (s[j] != ' ' && j != epos-1) ++j;
                newNode->addChild(buildVmtNode(s,i,j,vmap));
                i = j;
            }
        }
        return newNode;
    }
}
