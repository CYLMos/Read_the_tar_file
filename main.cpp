#include <iostream>
#include <fstream>
#include <cstdlib>
#include "MyTar.h"

using namespace std;

int main(int argc, char *argv[])
{
    /// If the number of the params is wrong
    if(argc >= 3){
        cout << "To many params" << endl;
        exit(0);
    }
    else if(argc <= 1){
        cout << "To less params" << endl;
        exit(0);
    }

    MyTar myTar(argv[1]);

    myTar.startRead();
    myTar.showContent();

    return 0;
}
