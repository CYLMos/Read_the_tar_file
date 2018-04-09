#include <iostream>
#include <cstdlib>
#include "MyTar.h"

using namespace std;

int main(int argc, char *argv[])
{
    /// If the number of the params is wrong
    if(argc >= 3){
        cout << "Too many params" << endl;
        exit(0);
    }
    else if(argc <= 1){
        cout << "Too less params" << endl;
        exit(0);
    }

    MyTar myTar(argv[1]);

    if(!myTar.startRead()){
	cout << "Not USTART file!" << endl;
	exit(0);
    }
    myTar.showContent();

    return 0;
}
