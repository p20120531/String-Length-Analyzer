#include "kaluzaMgr.h"
#include "autMgr.h"
KaluzaMgr* kmgr   = new KaluzaMgr();
AutMgr*  autmgr = new AutMgr();

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
    //kmgr->read(argv[1],argv[2]);
    //kmgr->buildAndWriteDG();
    /*
    string opt(argv[1]);
    if (opt == "0")
        autmgr->blif2vmt(argv[2],argv[3]);
    else if(opt == "1")
        autmgr->readCmdFile(argv[2]);
    */
    /*
    Aut* a1 = new Aut("easy/a.vmt");
    Aut* a2 = new Aut("1001.corecstrs.readable/1/T2_2.vmt");
    Aut* a3 = new Aut();
    a3->concate(a1,a2);
    a3->write("test.vmt");
    */

    /*
    autmgr->readDefFile(argv[1]);
    cout << endl;
    autmgr->readPredFile(argv[2]);
    cout << endl;
    autmgr->print();
    */

    //kmgr->closeLogFile();
    //kmgr->analyzePT();
    
    string input(argv[1]);
    //AutOp
    /*
    if (input == "0") {
        const char* c1 = "blif2vmt/literal_45.blif";
        const char* c2 = "blif2vmt/literal_45.vmt.1";
        autmgr->blif2vmt(c1,c2);
    }
    if (input == "1") {
        Aut* a1 = new Aut("easy/a.vmt");
        a1->write("easy/a.vmt.1");
        Aut* a2 = new Aut("easy/a.vmt.1");
        a2->write("easy/a.vmt.2");
    }
    else if (input == "2") {
        Aut* a1 = new Aut("easy/b.vmt");
        a1->write("easy/b.vmt.1");
        Aut* a2 = new Aut("easy/b.vmt.1");
        a2->write("easy/b.vmt.2");
    }
    else if (input == "3") {
        Aut* a1 = new Aut("easy/a_lc.vmt");
        a1->write("easy/a_lc.vmt.1");
        Aut* a2 = new Aut("easy/a_lc.vmt.1");
        a2->write("easy/a_lc.vmt.2");
    }
    else if (input == "4") {
        Aut* a1 = new Aut("easy/a.vmt.1");
        a1->addlen("0");
        a1->write("easy/a_lc.vmt.3");
    }
    else if (input == "5") {
        Aut* a1 = new Aut("easy/a.vmt");
        Aut* a2 = new Aut("easy/b.vmt");
        Aut* a3 = new Aut();
        a3->intersect(a1,a2);
        a3->write("easy/ab_inter.vmt.1");
    }
    else if (input == "6") {
        Aut* a1 = new Aut("easy/a.vmt.1");
        Aut* a2 = new Aut("easy/b.vmt.1");
        Aut* a3 = new Aut();
        a3->intersect(a1,a2);
        a3->write("easy/ab_inter.vmt.2");
    }
    else if (input == "7") {
        Aut* a1 = new Aut("easy/a.vmt");
        Aut* a2 = new Aut("easy/b.vmt");
        Aut* a3 = new Aut();
        a3->concate(a1,a2);
        a3->write("easy/ab_concate.vmt.1");
    }
    else if (input == "8") {
        Aut* a1 = new Aut("easy/ab_concate.vmt.1");
        Aut* a2 = new Aut("easy/a.vmt");
        Aut* a3 = new Aut();
        a3->concate(a1,a2);
        a3->write("easy/aba_concate.vmt.1");
    }
    else if (input == "9") {
        Aut* a1 = new Aut("easy/ab_concate.vmt.1");
        a1->write("easy/ab_concate.vmt.2");
    }
    else if (input == "10")
        autmgr->blif2vmt(argv[2],argv[3]);
    else if (input == "11")
        autmgr->readCmdFile(argv[2]);
    else if (input == "12") {
        kmgr->read(argv[2],argv[3]);
        kmgr->analyzePTASCII();
    }
    */
    if (input == "--intersect") {
        Aut* a1 = new Aut(argv[2]);
        Aut* a2 = new Aut(argv[3]);
        Aut* a3 = new Aut();
        a3->intersect(a1,a2);
        a3->write(argv[3]);
    }
    else if (input == "--buildDG") {
        kmgr->read(argv[2],argv[3]);
        kmgr->buildAndWriteDG();
    }
    else if (input == "--blif2vmt") {
        autmgr->blif2vmt(argv[2],argv[3]);
    }
    else if (input == "--readCmd") {
        autmgr->readCmdFile(argv[2]);
    }
    else if (input == "--analyze") {
        kmgr->read(argv[2],argv[3],1);
        kmgr->analyzePT();
    }
    else if (input == "0") {
        cout << "addlen " << argv[2] << endl << "write " << argv[3] << endl;
        Aut* a1 = new Aut(argv[2]);
        a1->addlen("10");
        a1->write(argv[3]);
    }
    else if (input == "1") {
        cout << "read " << argv[2] << endl << "write "  << argv[3] << endl;
        Aut* a1 = new Aut(argv[2]);
        a1->write(argv[3]);
    }
    else if (input == "2") {
        cout << "intersect " << argv[2] << " " << argv[3] << endl << "write " << argv[4] << endl;
        Aut* a1 = new Aut(argv[2]);
        Aut* a2 = new Aut(argv[3]);
        Aut* a3 = new Aut();
        a3->intersect(a1,a2);
        a3->write(argv[4]);
    }
    else if (input == "3") {
        cout << "concate " << argv[2] << " " << argv[3] << endl << "write " << argv[4] << endl;
        Aut* a1 = new Aut(argv[2]);
        Aut* a2 = new Aut(argv[3]);
        Aut* a3 = new Aut();
        a3->concate(a1,a2);
        a3->write(argv[4]);
    }
    else
        cout << "invalid option=" << input << endl;
    

    //cout << "at main"<< endl;
    //autmgr->_epsilon->print(0);
    //Aut* a2 = new Aut(argv[2]);
    //Aut* a3 = new Aut();
    //a3->intersect(a1,a2);
    //a1->addlen("1");
    //for (int j=0;j<4;++j)
    //for (int i=0;i<8;++i)
    //    cout << Aut::_xsList[j][i]->getName() << endl;
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
