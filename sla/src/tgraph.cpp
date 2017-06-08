#include "autMgr.h"

void TGraph::init()
{
    // initialize alphabet list
    for (size_t i = 0; i < 256; ++i)
        _alphabetList.push_back(bitset<8>(i).to_string());
    
}

string TGraph::Uint2bitString(const size_t& n,const size_t& stateBitNum)
{
    string bitstr (Aut::stateBitNum,'0');
    size_t tmp = n,j = Aut::stateBitNum;
    while (tmp % 2 != 0 || tmp / 2 != 0) {
        --j;
        if (tmp % 2 != 0) bitstr[j] = '1';
        tmp /= 2;
    }
    return bitstr;
}

void TGraph::writeCompleTransition(ofstream& file, const vector<string>& alphabetList, const string& s1, const string& s2, const size_t idx)
{
    for (size_t j = 0; j < 256; ++j) {
        if (j != epsilonEncode && j != leftEncode && j != rightEncode && j != idx)
            file << "\n" << alphabetList[j] << s1 << s2 << " 1";
    }
}

void TGraph::parse(const char* fileName)
{
    string line;
    ifstream file(fileName);

    if (!file) {
        cout << "fail open file=" << fileName << endl;
        return;
    }
    size_t maxStateIdx = 0;
    set<size_t> oSet;
    vector<string> tokenList;
    while (getline(file,line)) {
        tokenList.clear();
        str2tokens(line,tokenList);
        if (line[0] == "initial" && line[1] == "->") {
            _init = new TGNode(itos(line[2]),1,0);
        }
        else if (_nSet.find(line[0][0]) != _nSet.end()) {
            if (line[1] == "->") {
                size_t sIdx = stoi(line[0]);
                size_t eIdx = stoi(line[2]);
                size_t label0, label1;
                parseLabels(line[3], label0, label1);
                _tList.push_back(new TGEdge(sIdx,eIdx,label0,label1));
            }
            else {
                assert( (line[1][0] == "[" && *(line[1].rbegin()) == "]") );
                size_t idx = stoi(line[0]);
                if (idx > maxStateIdx) maxStateIdx = idx;
                if (isAccepting(line[1])) oSet.insert(idx);
            }
        }
        
    }
    file.close();
    size_t tmp = maxStateIdx;
    while (tmp % 2 != 0 || tmp / 2 != 0) {
        ++_stateBitNum;
        tmp /= 2;
    }
    for (size_t i = 0; i <= maxState; ++i)
        stateList.push_back(Uint2bitString(i,stateBitNum));

    for (set<size_t>::iterator it = oSet.begin(); it != oSet.end(); ++it)
        _oList.push_back(*it);
    
}

bool TGraph::isAccepting(const string& line)
{
    size_t i = 0;
    while(line[i] != '=') ++i;
    size_t j = ++i;
    while(line[i] != ',') ++i;
    string type = line.substr(j,i-j);
    assert( (type == "circle" || type == "doublecircle") );
    if (type == "circle") return 0;
    else                  return 1;
}

void TGraph::parseLabels(const string& line, size_t& label0, size_t& label1)
{
    size_t i = 0;
    while (line[i] != '\"') ++i;
    size_t j = ++i;
    bool isRange = 1;
    while (line[i] != '-') {
        if (line[i] == '\"') {
            isRange = 0;
            break;
        }
        ++i;
    }
    if (isRange) {
        string2Index(line.substr(j,i-j),label0);
        j = ++i;
        while (line[i] != '\"') ++i;
        string2Index(line.substr(j,i-j),label1);
    }
    else {
        string s = line.substr(j,i-j);
        string2Index(s,label0);
        label1 = 256;
    }
}

//TODO
void TGraph::string2Index(const string& s, size_t& label)
{
    if (s[0] == '\\') 
        label = _h2bMap[s.substr(4,2)];
    else {
        assert( (s.size() == 1) );
        label = size_t(s[0]);
    }
}
void TGraph::write(const char* fileName)
{
    ofstream file(fileNmae.c_str());

    file << "model " << fileName
         << "\n.inputs";
    for (size_t i = 0; i < _inputBitNum; ++i) file << " i" << i;
    for (size_t i = 0; i < _stateBitNum; ++i) file << " s" << i;
    for (size_t i = 0; i < _stateBitNum; ++i) file << " n" << i;
    file << "\n.outputs i o t";

    // initial state
    file << "\n.names";
    for (size_t i = 0; i < _stateBitNum; ++i) file << " s" << i;
    file << " i\n" << 

    // accpeting states

    // transition function

    file.close();
}

void TGraph::print() const
{
}
