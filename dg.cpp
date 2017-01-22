#include "dg.h"
#include "mgr.h"

extern Mgr* mgr;
static ofstream& logFile = mgr->getLogFile();

DG::DG (DGNode* sink,size_t& indent,size_t& gflag,const string& path,const size_t& idx): _sink(sink),_indent(indent),_gflag(gflag) 
{
    _idx = idx;
    stringstream ss;
    ss << _idx + 1;
    _path = path + ss.str() + "/";
    system(("mkdir -p "+_path).c_str());
}

void DG::writeDBG() const {
    splitLine(logFile,"DG::writeDBG()");
    _sink->writeDBG(_indent,0);
}

void DG::merge() {
    splitLine(logFile,"DG::merge()");
    _sink->merge(++_gflag);
}
void DG::writeCmdFile () const {
    splitLine(logFile,"DG::writeCmdFile()");
    const LCList& lcList = mgr->getLCList(_idx);
    string cmdstr = _path + "cmd";
    string autstr = _path + "aut";
    ofstream cmdFile(cmdstr.c_str());
    ofstream autFile(autstr.c_str());
    _sink->writeCmdFile(cmdFile,autFile);
    for (size_t i=0, size=lcList.size(); i<size; ++i)
        cmdFile << "\n" << lcList.at(i);
    cmdFile.close();
    autFile.close();
}
