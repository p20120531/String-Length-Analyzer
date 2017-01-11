#ifndef _DG_H_
#define _DG_H_
#include "typedef.h"

class DGNode {
    friend class DG;
    friend class Mgr;
    public :
        DGNode () {}
        DGNode (const string& name,const Type& type): _name(name),_type(type){
            _isSink = 1;
            _length = 0;
        }
        DGNode (const string& name,const Type& type,const bool& length): _name(name),_type(type),_length(length){
            _isSink = 1;
        }
        DGNode (const string& name,const string& regex): _name(name),_regex(regex){
            _type   = CONST_STRING;
            _length = 0;
            _isSink = 1;
        }
        const Type& getType() {return _type;}
        const string& getName() {return _name;}
        const bool& isSink() {return _isSink;}
        
        void setName (const string& name) {_name = name;}
        void setRegex(const string& regex) {_regex = regex;}
        void addChild(DGNode*& n) {_children.push_back(n);}
        void setNotSink() {_isSink = 0;}
        void setLength()  {_length = 1;}
        void mergeVSOS(DGNode*);
        void mergeVSCS(DGNode*);
        
        void print () const;
    private:
        Type          _type;
        string        _name;    // NEW_STR_n / NEW_RE_n for extra nodes
        string        _regex;   // "..." for _type=CONST_STRING , "" otherwise
        bool          _length;
        bool          _isSink;
        DGNodeList    _children;
};
class DG {
    friend class Mgr;
    public :
        DG  (DGNode* sink): _sink(sink) {}
        DGNode* successor();
        DGNode* getSinkNode() {return _sink;}
        void setSinkNode(DGNode* sink) {_sink = sink;}
        void print ()const;
    private :
        DGNode*         _sink;
        DGNode*         _cur;
};
#endif
