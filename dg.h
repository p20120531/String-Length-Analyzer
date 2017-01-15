#ifndef _DG_H_
#define _DG_H_
#include "typedef.h"

class DGNode {
    friend class DG;
    friend class Mgr;
    public :
        DGNode () {}
        DGNode (const string& name,const Type& type,const bool& isStringVar) {
            _type = type;
            _name = name;
            _length = 0;
            _isSink = 1;
            _isStringVar = isStringVar;
            _leader = this;
        }
        DGNode (const string& name,const string& regex) {
            _type   = CONST_STRING;
            _name   = name;
            _regex  = regex;
            _length = 0;
            _isSink = 1;
            _isStringVar = 0;
            _leader = this;
        }
        DGNode (const string& name,DGNode* n) {
            _type     = n->_type;
            _name     = name;
            _regex    = n->_regex;
            _length   = n->_length;
            _isSink   = 0;
            _isStringVar = n->_isStringVar;
            _leader   = this;
            _children = n->_children;
            _flag     = n->_flag;
        }
        const Type&    getType() {return _type;}
        const string&  getName() {return _name;}
        const bool&    isSink()  {return _isSink;}
        const bool&    isStringVar() {return _isStringVar;}
        const size_t&  getFlag() {return _flag;}
        string         getRegex();
        DGNode*        findLeader();
        
        void setName (const string& name) {_name = name;}
        void setRegex(const string& regex) {_regex = regex;}
        void addChild(DGNode*& n) {_children.push_back(n);}
        void clearChildren() {_children.clear();}
        void setType(const Type& type) {_type = type;}
        void setNotSink() {_isSink = 0;}
        void setLength()  {_length = 1;}
        void setLeader(DGNode* n) {_leader = n;}
        void mergeVSOS(DGNode*);
        void mergeVSCS(DGNode*);
        void merge(const size_t&);
        void setFlag(const size_t& flag) {_flag=flag;}
        
        void printType() const;
        void print () const;
        void printDBG (const size_t&,size_t)const;
    private:
        Type          _type;
        string        _name;    // NEW_STR_n / NEW_RE_n for extra nodes
        string        _regex;   // "..." for _type=CONST_STRING , "" otherwise
        bool          _length;  // default 0
        bool          _isSink;  // default 1
        bool          _isStringVar;
        DGNode*       _leader;
        DGNodeList    _children;
        size_t        _flag;
};
class DG {
    friend class Mgr;
    public :
        DG  (DGNode* sink) {
            _sink = sink;
            _indent = 3;
            _flag = 0;
        }
        DGNode* successor();
        DGNode* getSinkNode() {return _sink;}
        void setSinkNode(DGNode* sink) {_sink = sink;}
        void print ()const;
        void printDBG ()const;
        void merge();
    private :
        DGNode*        _sink;
        size_t         _indent;
        size_t         _flag;
};
#endif
