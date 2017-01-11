#include "mgr.h"
#include <bitset>
Mgr* mgr = new Mgr();
int main(int argc, char* argv[]) {
    //for (size_t i =0;i<50;++i)
    //    cout << mgr->uint2str(i) << endl;
    
    mgr->parse(argv[1]);
    mgr->printPT();
    mgr->buildDG();
    mgr->printDG();
    
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
