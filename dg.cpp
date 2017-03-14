#include "dg.h"
#include "kaluzaMgr.h"

extern       KaluzaMgr* kmgr;
static       PT* pt = kmgr->getPT();
static const Str2PTNodeListMap& str2PTNodeListMap = pt->getStr2PTNodeListMap();
static ofstream&    logFile   = kmgr->getLogFile();

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
    _sink->print(_indent,0);
    #endif
}

void DG::merge() 
{
    #ifndef _NLOG_
    splitLine(logFile,"DG::merge");
    #endif
    _sink->merge();
}

void DG::renameLengthVar()
{
    #ifndef _NLOG_
    splitLine(logFile,"DG::renameLengthVar");
    #endif
    size_t lengthVarCnt = 0;
    _sink->renameLengthVar(lengthVarCnt);
}

void DG::writeCmdFile ()
{
    #ifndef _NLOG_
        splitLine(logFile,"DG::writeCmdFile");
    #endif
    //const LCList& lcList = mgr->getLCList(_idx);
    string cmdstr = _path + "cmd";
    string autstr = _path + "aut";
    string defstr = _path + "def";
    string lcstr  = _path + "lc";
    ofstream cmdFile(cmdstr.c_str());
    ofstream autFile(autstr.c_str());
    ofstream defFile(defstr.c_str());
    ofstream lcFile(lcstr.c_str());
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
    defFile.close();
    lcFile.close();
}

void DG::writeCVC4File()
{
    #ifndef _NLOG_
        splitLine(logFile,"DG::writeCVC4File");
    #endif
    string cvc4str = _path + "cvc4";
    ofstream cvc4File(cvc4str.c_str());
    
    cvc4File << "(set-logic QF_S)" << endl;
    ++_gflag;
    _sink->writeCVC4File(_typeMap,_cvc4StrList,_cvc4PredList,_bflag);
    for (Str2TypeMap::iterator it=_typeMap.begin(); it!=_typeMap.end(); ++it) {
        cvc4File << "(declare-fun " << it->first << " () ";
        switch (it->second) {
            case VAR_INT :
                cvc4File << "Int)" << endl;
                break;
            case VAR_BOOL:
                cvc4File << "Bool)" << endl;
                break;
            case VAR_STRING:
                cvc4File << "String)" << endl;
                break;
            default:
                break;
        }
    }
    for (vector<string>::iterator it=_cvc4StrList.begin(); it!=_cvc4StrList.end(); ++it)
        cvc4File << *it << endl;
    for (vector<string>::iterator it=_cvc4PredList.begin(); it!=_cvc4PredList.end(); ++it)
        cvc4File << *it << endl;
    cvc4File << "(check-sat)";
    cvc4File.close();
}

void DG::writeSmt2File() const
{
    /*
    splitLine(logFile,"DG::writeSmt2File()");
    string smt2str = _path + "smt2";
    ofstream smt2File(smt2str.c_str());
    smt2File << "(set-logic QF_S)\n\n";
    _sink->lcTraversal(smt2File,false,_strVarMap);
    for (Str2TypeMap::iterator it=intVarMap.begin();it!=intVarMap.end();++it)
        smt2File << "(declare-fun " << it->first << " () Int)\n";
    for (Str2TypeMap::iterator it=_strVarMap.begin();it!=_strVarMap.end();++it)
        smt2File << "(declare-fun " << it->first << " () String)\n";
    smt2File << endl;
    _sink->writeSmt2File(smt2File);
    smt2File << endl;
    for (PTNodeList::iterator it=lcptList.begin();it!=lcptList.end();++it) {
        smt2File << "(assert";
        (*it)->lcTraversal(smt2File);
        smt2File << ")\n";
    }    
    smt2File << endl << "(check-sat)";
    smt2File.close();
    */
}
