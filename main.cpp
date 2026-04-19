#include <iostream>
using namespace std;
int main(int argc, char* argv[]){
    if (argc <= 1){cout << "no file" << endl; return 0;}
    for (int i = 1; i < argc; i++){
        string filename = argv[i];
        pass1(filename);
        pass2(filename);
    }
    return 0;
}