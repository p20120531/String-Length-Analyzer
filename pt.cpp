#include "pt.h"

void PT::addAssertion(PTNode* n)
{
    _root->addChild(n);
}

void PT::print(const size_t& indent) const
{
    for (size_t i = 0, size = _root->_children.size(); i < size; ++i) {
        cout << _root->_name << " " << i + 1 << endl;
        _root->_children[i]->print(indent,0);
    }
}

