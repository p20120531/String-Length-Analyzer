#include "ptmgr.h"

PTNode::PTNode(const string& name) : _name(name)
{
    setOpType(name);
}

void PTNode::setOpType(const string& optype)
{
    if      (optype == "not"        ) _optype = OP_NOT;
    else if (optype == "-"          ) _optype = OP_NEG;
    else if (optype == "="          ) _optype = OP_EQ;
    else if (optype == "and"        ) _optype = OP_AND;
    else if (optype == "or"         ) _optype = OP_OR;
    else if (optype == "ite"        ) _optype = OP_ITE;
    else if (optype == "str.++"     ) _optype = OP_STRCONCATE;
    else if (optype == "str.len"    ) _optype = OP_STRLEN;
    else if (optype == "str.in.re"  ) _optype = OP_STRINRE;
    else if (optype == "str.replace") _optype = OP_STRREPLACE;
    else if (optype == "str.to.re"  ) _optype = OP_STRTORE;
    else if (optype == "re.++"      ) _optype = OP_RECONCATE;
    else if (optype == "re.union"   ) _optype = OP_REUNION;
    else if (optype == "re.inter"   ) _optype = OP_REINTER;
    else                              _optype = OPERAND;
}

void PTMgr::print()
{
    for (size_t i = 0, size = _ptList.size(); i < size; ++i) {
        cout << "PT " << i << endl;
        _ptList[i]->print();
    }
}

void PT::print()
{
    _root->print(_indent,0);
}

void PTNode::print(const size_t& indent,size_t level)
{
    cout << string( indent * level ,' ')
         << _name << endl;
    for (size_t i = 0, size = _children.size(); i < size; ++i)
        _children[i]->print(indent,level+1);
}
