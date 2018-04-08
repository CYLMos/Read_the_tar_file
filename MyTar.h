#ifndef MYTAR_H
#define MYTAR_H

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <cstdlib>

using namespace std;

struct TarHeader{
    /// Tar Header Section
    char filename[100];
    char filemode[8];
    char userid[8];
    char groupid[8];
    char filesize[12];
    char mtime[12];
    char checksum[8];
    char type;
    char lname[100];

    /// USTAR Section
    char USTAR_id[6];
    char USTAR_ver[2];
    char username[32];
    char groupname[32];
    char devmajor[8];
    char devminor[8];
    char prefix[155];
    char pad[12];
};

class MyTar{

public:
    static const int USTAR = 1;  /// Represent the tar file is USTAR format.
    static const int NO_USTAR = 0;   /// Resprent the tar file is not USTAR format.

    MyTar(const char *file);
    ~MyTar();

    int isUStar();  /// Check the tar file is USTART format.
    int startRead();  /// Start to read the tar file.
    void showContent();  /// Show the content of the tar file.
    int getFileNum();  /// Get the number of the files in the tar file.

private:
    ifstream inputStream;
    vector<TarHeader> tarVector;  /// Store the tar header in this vector.
    int usBlockSize;  /// Represent the block size of USTAR format.
    int fileNum;   /// Store the number of the files in the tar file.

    string getFileMode(int);  /// Get the file mode of the file in the tar file.
    int hex2Dec(const char*, int);  /// Change the value from hex to dec.
};

#endif // MYTAR_H
