#include "dgmgr.h"
DGMgr* dgmgr = new DGMgr();
int main(int argc, char* argv[]) {
    dgmgr->parse(argv[1]);
    return 0;
    /*
    string delimiter = " ",token,line;
    size_t pos,cnt;

    ifstream file(argv[1]);
    if (file) {
        size_t lineCnt = 0;
        while(getline(file,line)) {
            cout << "lineCnt"<<++lineCnt <<" \""<< line <<"\""<<endl;
            queue<string> q;
            for (size_t i=0,j=0,size=line.length(); j < size; ++j) {
                if (line[j] == ' ' || line[j] == ')') {
                    if (i != j)
                        q.push(line.substr(i,j-i));
                    if (line[j] == ')')
                        q.push(")");
                    i = j + 1;
                }
                else if (line[j] == '(') {
                    q.push("(");
                    i = j + 1;
                }
                else {
                }   
            }
            while (!q.empty()) {
                cout << q.front() << endl;
                q.pop();
            }
        }
        file.close();
    }
    else
        cout << "fail open" <<endl;
    return 0;
    */
    /*
    vector<string> v;
    for(int i=0; i<argc; ++i) {
        cout << argv[i] << " ";
        v.push_back(argv[i]);
    }
    cout << endl;
    for(int i=0; i<argc; ++i)
        cout<<v[i]<<" ";
    cout<< endl;
    //dgmgr->parse(argv)
    cout << "123\b " << endl;

    const char* c1 = "1234aa";
    char c2[] = "12355dd";
    cout << c1 <<endl;
    cout << c2 <<endl;
    cout << c1[2] <<endl;
    c2[2] = 'a';
    cout<<c2<<endl;
    char* c3 = NULL;
    cout << c3 <<endl;
    cout <<"G"<<endl;
    *c3 = 'a';
    cout << c3 <<endl;
    cout << *c3 <<endl;
    *c3 = 'g';
    cout << c3 <<endl;
    */
}
