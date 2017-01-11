#ifndef _PT_H_
#define _PT_H_
#include "ptnode.h"

class PT {
    friend class Mgr;
    public :
        PT              () {};
        PT              (const string& name): _root(new PTAndNode(name)) {}
        PTNode*         getRootNode() const {return _root;}
        void            print(const size_t&) const ;
        void            addAssertion(PTNode*);
    private :
        PTNode*         _root;
        PTNode*         _cur;
};
#endif
