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
    _path = path + ss.str() + "/";
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
    //for (size_t i=0, size=lcList.size(); i<size; ++i)
        //cmdFile << "\n" << lcList.at(i);
    /*
    for (Str2TypeMap::const_iterator it=intVarMap.begin();it!=intVarMap.end();++it)
        defFile << "(declare-fun " << it->first << " () Int)\n";
    size_t cnt = 0;
    _sink->lcTraversal(_intVarMap,cnt);
    size_t intermediateCnt = 0;
    for (PTNodeList::const_iterator it=lcptList.begin();it!=lcptList.end();++it) {
        lcFile  << "(define-fun LC" << intermediateCnt++ << " () Bool";
        (*it)->lcTraversal(lcFile,_intVarMap);
        lcFile  << ")\n";
    }
    _sink->writeCmdFile(_intVarMap,cmdFile,autFile);
    */
    cmdFile.close();
    autFile.close();
}

void DG::writeCVC4File(const IMP& curimp)
{
    #ifndef _NLOG_
        splitLine(logFile,"DG::writeCVC4File");
    #endif
    string cvc4str = _path + "sink.smt2";
    string defstr  = _path + "def";
    string predstr = _path + "pred";
    ofstream cvc4File(cvc4str.c_str());
    ofstream defFile(defstr.c_str());
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
        defFile << "(declare-fun " << *it << " () Bool)" << endl;
    }
    for (vector<string>::iterator it=intList.begin(); it!=intList.end(); ++it) {
        Str2UintMap::iterator jt=_lengthVarCntMap.find(*it);
        cvc4File << "(declare-fun " << *it << " () Int)";
        defFile << "(declare-fun " << *it << " () Int)";
        if (jt != _lengthVarCntMap.end()) {
            cvc4File << " ; " << jt->second << endl;
            defFile << " ; " << jt->second << endl;
        }
        else { 
            cvc4File << endl;
            defFile << endl;
        }
    }
    for (vector<string>::iterator it=strList.begin(); it!=strList.end(); ++it)
        cvc4File << "(declare-fun " << *it << " () String)" << endl;
    for (vector<string>::iterator it=_cvc4StrList.begin(); it!=_cvc4StrList.end(); ++it)
        cvc4File << *it << endl;
    for (vector<string>::iterator it=_cvc4PredList.begin(); it!=_cvc4PredList.end(); ++it) {
        cvc4File << *it << endl;
        predFile << *it << endl;
    }
    if (curimp.first != 0) {
        if (curimp.second) {
            cvc4File << "(assert " << curimp.first->getName() << ")" << endl;
            predFile << "(assert " << curimp.first->getName() << ")" << endl;
        }
        else {
            cvc4File << "(assert (not " << curimp.first->getName() << "))" << endl;
            predFile << "(assert (not " << curimp.first->getName() << "))" << endl;
        }
    }
    cvc4File << "(check-sat)";
    cvc4File.close();
}

