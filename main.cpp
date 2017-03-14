#include "kaluzaMgr.h"
#include "autopMgr.h"
#include <bitset>
KaluzaMgr* kmgr   = new KaluzaMgr();
AutOpMgr*  autkmgr = new AutOpMgr();

int main(int argc, char* argv[]) {
    //for (size_t i =0;i<50;++i)
    //    cout << kmgr->uint2str(i) << endl;
    /*
    vector<string> v;
    str2tokens(string("abc   dd asdas  "),v);
    for (size_t i=0, size = v.size(); i < size; ++i)
        cout << v[i] << endl;
    
    str2tokens(string("abc   ddd  asdas~"),v);
    for (size_t i=0, size = v.size(); i < size; ++i)
        cout << v[i] << endl;
    */
    
    //Kaluza
    kmgr->read(argv[1]);
    kmgr->buildAndWriteDG();
    //kmgr->closeLogFile();
    //kmgr->analyzePT();
    
    //AutOp
    //autkmgr->blif2vmt(argv[1],argv[2]);
    //Aut* aut = new Aut(argv[1]);
    //aut->addlen(0);
    //aut->write(argv[2]);

    /*
    for (int i = 0; i< 4;++i) {
        string s(new char(i));
        cout << s << " | " << bitset<8>(s[0]);
        if (s[0] == i) cout << " y" << endl;
        else                 cout << " n" << endl;
    }
    string s(new char(2));
    cout << s << endl;
    if (s[0] == 3) cout << "gg" << endl;
    else cout << "ng" << endl;
    */
    return 0;
}
