#ifndef _DG_H_
#define _DG_H_
#include "typedef.h"

class DGNode {
    friend class DG;
    friend class KaluzaMgr;
    public :
        DGNode () {}
        DGNode (const string& name) {
            _name        = name;
            _type        = VAR_STRING;
            _isSink      = 1;
            _isStrVar    = 1;
            _leader      = this;
        }
        DGNode (const string& name,const Type& type) {
            _name        = name;
            _type        = type;
            _isSink      = 0;
            _isStrVar    = 0;
            _leader      = this;
        }
        DGNode (const string& name,const string& regex) {
            _name        = name;
            _type        = CONST_STRING;
            _regex       = regex;
            _isSink      = 0;
            _isStrVar    = 0;
            _leader      = this;
        }
        DGNode (const string& name,DGNode* n) {
            _name        = name;
            _type        = n->_type;
            _regex       = n->_regex;
            _isSink      = 0;
            _isStrVar    = 0;
            _leader      = this;
            _lengthVarList = n->_lengthVarList;
            _impList     = n->_impList;
            _children    = n->_children;
            //FIXME
            _flag        = n->_flag;
        }
        // Print Function
        void print (const size_t&,size_t)const;
        // Access Function
        const Type&    getType       ()                    {return _type;}
        const string&  getName       ()                    {return _name;}
        const bool&    isSink        ()                    {return _isSink;}
        const bool&    isStrVar      () {return _isStrVar;}
        const size_t&  getFlag       ()                    {return _flag;}
        string         getRegex      ();
        DGNode*        findLeader    ();
        const char* getTypeString() const;
        // Modify Function
        void           setName       (const string& name)  {_name = name;}
        void           setRegex      (const string& regex) {_regex = regex;}
        void           addChild      (DGNode*& n)          {_children.push_back(n);}
        void           addIMP(const IMP& n) {_impList.push_back(n);}
        void           addLengthVar(PTNode* n) {_lengthVarList.push_back(n);}
        void           clearChildren ()                    {_children.clear();}
        void           setType       (const Type& type)    {_type = type;}
        void           setNotSink    ()                    {_isSink = 0;}
        void           setLeader     (DGNode* n);
        void           setFlag       (const size_t& flag)  {_flag=flag;}
        
        void           merge         ();
        void renameLengthVar (size_t&);
        void writeCmdFile (const Str2UintMap&,ofstream&,ofstream&) const;
        void writeCVC4File(Str2TypeMap&,vector<string>&,vector<string>&,const size_t&);
        void lcTraversal (Str2UintMap&,size_t&)const;
    private:
        string        _name;    // NEW_STR_n / NEW_RE_n for extra nodes
        Type          _type;
        string        _regex;   // "..." for _type=CONST_STRING , "" otherwise
        bool          _isSink;  // default 1
        bool          _isStrVar;
        DGNode*       _leader;
        DGNodeList    _children;
        PTNodeList    _lengthVarList;
        IMPList       _impList;
        size_t        _flag;
};
class DG {
    friend class KaluzaMgr;
    public :
        DG  (DGNode*,size_t&,size_t&,const size_t&,const string&,const size_t&);

        DGNode* getSinkNode() {return _sink;}
        void setSinkNode(DGNode* sink) {_sink = sink;}
        void writeCmdFile();
        void writeIC3LengthFile();
        void writeCVC4File (const size_t&) const;
        void print () const;
        void merge();
        void renameLengthVar();
    private :
        DGNode*        _sink;
        size_t&        _indent;
        size_t&        _gflag;
        size_t         _bflag; // Branch Flag
        string         _path;
        size_t         _idx;
        Str2TypeMap    _typeMap;
        vector<string> _cvc4StrList;
        vector<string> _cvc4PredList
        Str2UintMap    _strVarMap;
        Str2UintMap    _intVarMap;
};
#endif
