#ifndef _DG_H_
#define _DG_H_
#include "typedef.h"

class DGNode {
    friend class DG;
    friend class KaluzaMgr;
    public :
        DGNode (const string& name) {
            _name        = name;
            _type        = VAR_STRING;
            _lengthVarCnt= -1;
            _isSink      = 1;
            _isStrVar    = 1;
            _leader      = this;
            _flag        = 0;
        }
        DGNode (const string& name,const Type& type) {
            _name        = name;
            _type        = type;
            _lengthVarCnt= -1;
            _isSink      = 0;
            _isStrVar    = 0;
            _leader      = this;
            _flag        = 0;
        }
        DGNode (const string& name,const string& regex) {
            _name        = name;
            _type        = CONST_STRING;
            _regex       = regex;
            _lengthVarCnt= -1;
            _isSink      = 0;
            _isStrVar    = 0;
            _leader      = this;
            _flag        = 0;
        }
        DGNode (const string& name,DGNode* n) {
            _name        = name;
            _type        = n->_type;
            _regex       = n->_regex;
            _lengthVarCnt= -1;
            _isSink      = 0;
            _isStrVar    = 0;
            _leader      = this;
            //_lengthVarList = n->_lengthVarList;
            //_assertionList = n->_assertionList;
            _children    = n->_children;
            //FIXME
            _flag        = n->_flag;
        }
        // Print Function
        void print (const size_t&,size_t)const;
        void printLengthVarList() const;
        void printAssertionList() const;
        // Access Function
        const Type&    getType       ()                    {return _type;}
        const string&  getName       ()                    {return _name;}
        const bool&    isSink        ()                    {return _isSink;}
        const bool&    isStrVar      () {return _isStrVar;}
        const size_t&  getFlag       ()                    {return _flag;}
        string         getRegex      ()const;
        const int&     getLengthVarCnt() {return _lengthVarCnt;}
        DGNode*        findLeader    ();
        const char* getTypeString() const;
        // Modify Function
        void           setName       (const string& name)  {_name = name;}
        void           setRegex      (const string& regex) {_regex = regex;}
        void           addChild      (DGNode*& n)          {_children.push_back(n);}
        void           addAssertion(const PTNodePair& n) {_assertionList.push_back(n);}
        void           addLengthVar(const PTNodePair& n) {_lengthVarList.push_back(n);}
        void           clearChildren ()                    {_children.clear();}
        void           setType       (const Type& type)    {_type = type;}
        void           setNotSink    ()                    {_isSink = 0;}
        void           setLeader     (DGNode* n);
        void           setFlag       (const size_t& flag)  {_flag=flag;}
        
        void           merge         ();
        void renameLengthVar (size_t&);
        void writeCmdFile (ofstream&,ofstream&);
        void writeCVC4File();
        void writeCVC4LeafNode(string&,string&);
        void lcTraversal (Str2UintMap&,size_t&)const;
    private:
        string        _name;    // NEW_STR_n / NEW_RE_n for extra nodes
        Type          _type;
        string        _regex;   // "..." for _type=CONST_STRING , "" otherwise
        int           _lengthVarCnt;
        bool          _isSink;  // default 1
        bool          _isStrVar;
        DGNode*       _leader;
        DGNodeList    _children;
        PTNodePairList _lengthVarList;
        PTNodePairList _assertionList;
        size_t        _flag;
};
class DG {
    friend class KaluzaMgr;
    public :
        DG  (DGNode*,size_t&,size_t&,const size_t&,const string&,const size_t&);

        // Print Function
        void print () const;
        void printLengthVarList() const;
        void printAssertionList() const;
        // Access Function
        bool isAssertion(const string&, string&);
        DGNode* getSinkNode() {return _sink->findLeader();}
        const size_t& getBFlag() {return _bflag;}
        Str2TypeMap& getTypeMap() {return _typeMap;}
        Str2UintMap& getLengthVarCntMap() {return _lengthVarCntMap;}
        vector<string>& getCVC4StrList() {return _cvc4StrList;}
        vector<string>& getCVC4PredList() {return _cvc4PredList;}
        vector<string>& getS3StrList() {return _s3StrList;}
        vector<string>& getS3PredList() {return _s3PredList;}
        set<string>& getBVStrSet() {return _bvStrSet;}
        set<string>& getIVStrSet() {return _ivStrSet;}
        set<string>& getBVPredSet() {return _bvPredSet;}
        set<string>& getIVPredSet() {return _ivPredSet;}
        // Modify Function
        //void setSinkNode(DGNode* sink) {_sink = sink;}
        void merge();
        void renameLengthVar();
        void writeCmdFile();
        void writeIC3LengthFile();
        void writeCVC4File(const IMP&);
    private :
        DGNode*        _sink;
        size_t&        _indent;
        size_t&        _gflag;
        size_t         _bflag; // Branch Flag
        string         _path;
        size_t         _idx;
        Str2TypeMap    _typeMap;
        Str2UintMap    _lengthVarCntMap;
        vector<string> _cvc4StrList;
        vector<string> _cvc4PredList;
        vector<string> _s3StrList;
        vector<string> _s3PredList;
        set<string>    _bvStrSet;
        set<string>    _ivStrSet;
        set<string>    _bvPredSet;
        set<string>    _ivPredSet;
};
#endif
