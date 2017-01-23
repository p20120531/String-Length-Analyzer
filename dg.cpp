#include "dg.h"
#include "mgr.h"

extern Mgr* mgr;
static PT* pt = mgr->getPT();
static ofstream&    logFile   = mgr->getLogFile();
static PTNodeList&  lcptList  = pt->getLCPTList();
static Str2TypeMap& intVarMap = pt->getIntVarMap();

DG::DG (DGNode* sink,size_t& indent,size_t& gflag,const string& path,const size_t& idx): _sink(sink),_indent(indent),_gflag(gflag) 
{
    _idx = idx;
    stringstream ss;
    ss << _idx + 1;
    _path = path + ss.str() + "/";
    system(("mkdir -p "+_path).c_str());
}

void DG::writeDBG() const 
{
    splitLine(logFile,"DG::writeDBG()");
    _sink->writeDBG(_indent,0);
}

void DG::merge() 
{
    splitLine(logFile,"DG::merge()");
    _sink->merge(++_gflag);
}

void DG::writeCmdFile ()
{
    splitLine(logFile,"DG::writeCmdFile()");
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
    cmdFile.close();
    autFile.close();
    defFile.close();
    lcFile.close();
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
    _sink->lcTraversal(smt2File,true,_strVarMap);
    smt2File << endl;
    for (PTNodeList::iterator it=lcptList.begin();it!=lcptList.end();++it) {
        smt2File << "(assert";
        (*it)->lcTraversal(smt2File);
        smt2File << ")\n";
    }    
    smt2File << "\n(check-sat)";
    smt2File.close();
    */
}
