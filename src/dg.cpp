#include "dg.h"
#include "kaluzaMgr.h"

extern KaluzaMgr* kmgr;
static PT*&       pt      = kmgr->getPT();
static ofstream&  logFile = kmgr->getLogFile();

DG::DG (DGNode* sink,size_t& indent,size_t& gflag,const size_t& bflag,const string& path,const size_t& idx): _sink(sink),_indent(indent),_gflag(gflag),_bflag(bflag)
{
    _idx = idx;
    stringstream ss;
    ss << _idx;
    _path = path + "/" + ss.str() + "/";
    system(("mkdir -p "+_path).c_str());
}

void DG::print() const 
{
    #ifndef _NLOG_
    splitLine(logFile,"DG::print");
    _sink->findLeader()->print(_indent,0);
    #endif
}

void DG::printLengthVarList() const
{
    #ifndef _NLOG_
    splitLine(logFile,"DG::printLengthVarList");
    _sink->findLeader()->printLengthVarList();
    #endif
}

void DG::printAssertionList() const
{
    #ifndef _NLOG_
    splitLine(logFile,"DG::printAssertionList");
    _sink->findLeader()->printAssertionList();
    #endif
}


void DG::merge() 
{
    #ifndef _NLOG_
    splitLine(logFile,"DG::merge");
    #endif
    _sink->findLeader()->merge();
}

void DG::renameLengthVar()
{
    #ifndef _NLOG_
    splitLine(logFile,"DG::renameLengthVar");
    #endif
    size_t lengthVarCnt = 0;
    _sink->findLeader()->renameLengthVar(lengthVarCnt);
}

void DG::writeCmdFile ()
{
    #ifndef _NLOG_
        splitLine(logFile,"DG::writeCmdFile");
    #endif
    string cmdstr = _path + "cmd";
    string autstr = _path + "aut";
    ofstream cmdFile(cmdstr.c_str());
    ofstream autFile(autstr.c_str());
    _sink->findLeader()->writeCmdFile(cmdFile,autFile);
    cmdFile << "write beforeaddpred\n"
            << "addpred\n"
            << "write sink\n"
            << "isempty beforeaddpred";
    cmdFile.close();
    autFile.close();
}

void DG::writeCVC4File(const IMP& curimp)
{
    #ifndef _NLOG_
        splitLine(logFile,"DG::writeCVC4File");
    #endif
    string cvc4str = _path + "sink.smt2";
    string s3str   = _path + "sink.s3";
    string predstr = _path + "pred";
    ofstream cvc4File(cvc4str.c_str());
    ofstream s3File(s3str.c_str());
    //ofstream defFile(defstr.c_str());
    ofstream predFile(predstr.c_str());

    if (curimp.first != 0) {
        assert((curimp.first->getType() == VAR_BOOL));
        curimp.first->writeCVC4PredVar();
    }

    cvc4File << "(set-logic QF_S)" << endl;
    _sink->findLeader()->writeCVC4File();
    vector<string> boolList,intList,strList;
    for (Str2TypeMap::iterator it=_typeMap.begin(); it!=_typeMap.end(); ++it) {
        switch (it->second) {
            case VAR_BOOL :
                boolList.push_back(it->first);
                break;
            case VAR_INT:
                intList.push_back(it->first);
                break;
            case VAR_STRING:
                strList.push_back(it->first);
                break;
            default:
                break;
        }
    }
    for (vector<string>::iterator it=boolList.begin(); it!=boolList.end(); ++it) {
        cvc4File << "(declare-fun " << *it << " () Bool)" << endl;
        s3File   << "(declare-variable " << *it << " Bool)" << endl;
        //defFile << "(declare-fun " << *it << " () Bool)" << endl;
        predFile << "(declare-fun " << *it << " () Bool)" << endl;
    }
    for (vector<string>::iterator it=intList.begin(); it!=intList.end(); ++it) {
        Str2UintMap::iterator jt=_lengthVarCntMap.find(*it);
        cvc4File << "(declare-fun " << *it << " () Int)" << endl;
        s3File   << "(declare-variable " << *it << " Int)" << endl;
        //defFile << "(declare-fun " << *it << " () Int)";
        predFile << "(declare-fun " << *it << " () Int)" << endl;
        // append extra length variable information
        /*if (jt != _lengthVarCntMap.end()) {
            cvc4File << " ; " << jt->second << endl;
            //defFile << " ; " << jt->second << endl;
            predFile << " ; " << jt->second << endl;
        }
        else { 
            cvc4File << endl;
            //defFile << endl;
            predFile << endl;
        }*/
    }
    predFile << ";" << endl;
    for (vector<string>::iterator it=strList.begin(); it!=strList.end(); ++it) {
        cvc4File << "(declare-fun " << *it << " () String)" << endl;
        s3File   << "(declare-variable " << *it << " String)" << endl;
    }
    for (vector<string>::iterator it=_cvc4StrList.begin(); it!=_cvc4StrList.end(); ++it)
        cvc4File << *it << endl;
    for (vector<string>::iterator it=_s3StrList.begin(); it!=_s3StrList.end(); ++it)
        s3File << *it << endl;
    
    // Handle Assertion Repetition
    if (curimp.first != 0) {
        string s;
        if (curimp.second) {
            s = "(assert " + curimp.first->getName() + ")";
        }
        else {
            s = "(assert (not " + curimp.first->getName() + "))";
        }
        bool alreadyExist = 0;
        for (vector<string>::iterator it=_cvc4PredList.begin(); it!=_cvc4PredList.end(); ++it) {
            if (s == *it) {
                alreadyExist = 1;
                break;
            }
        }
        if (!alreadyExist) {
            _cvc4PredList.push_back(s);
        }
    }
    if (curimp.first != 0) {
        string s;
        if (curimp.second) {
            s = "(assert " + curimp.first->getName() + ")";
        }
        else {
            s = "(assert (not " + curimp.first->getName() + "))";
        }
        bool alreadyExist = 0;
        for (vector<string>::iterator it=_s3PredList.begin(); it!=_s3PredList.end(); ++it) {
            if (s == *it) {
                alreadyExist = 1;
                break;
            }
        }
        if (!alreadyExist) {
            _s3PredList.push_back(s);
        }
    }
    
    /*
    for (vector<string>::iterator it=_cvc4PredList.begin(); it!=_cvc4PredList.end(); ++it) {
        cvc4File << *it << endl;
        predFile << *it << endl;
    }
    */
    
    set<string> ibvSet;
    for (vector<string>::iterator it=_cvc4PredList.begin(); it!=_cvc4PredList.end(); ++it) {
        string name;
        if (isAssertion(*it,name)) {
            if ((ibvSet.find(name) != ibvSet.end())) {
                _cvc4PredList.erase(it);
                --it;
            }
            else {
                ibvSet.insert(name);
                cvc4File << *it << endl;
                predFile << *it << endl;
            }
        }
        else {
            cvc4File << *it << endl;
            predFile << *it << endl;
        }
    }
    
    set<string> ibvSet2;
    for (vector<string>::iterator it=_s3PredList.begin(); it!=_s3PredList.end(); ++it) {
        string name;
        if (isAssertion(*it,name)) {
            if ((ibvSet2.find(name) != ibvSet2.end())) {
                _s3PredList.erase(it);
                --it;
            }
            else {
                ibvSet2.insert(name);
                s3File << *it << endl;
            }
        }
        else {
            s3File << *it << endl;
        }
    }
    
    cvc4File << "(check-sat)";
    cvc4File.close();
    s3File << "(check-sat)";
    s3File.close();
}
bool DG::isAssertion(const string& pred, string& name) {
    size_t size = pred.size();
    for (size_t i = 8; i < size; ++i)
        if (pred[i] == '(') return 0;
    name = pred.substr(8,size-1-8);
    return 1;
}
