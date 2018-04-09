#include "MyTar.h"

MyTar::MyTar(const char *file){
    this->usBlockSize = 512;  /// Every block in USTAR format is 512 bytes.
    this->fileNum = -1;
    this->inputStream.open(file, ifstream::in | ifstream::binary);

    /// If failed, close the program.
    if(!this->inputStream.is_open()){
        cout << "Open failed!" << endl;
        exit(0);
    }
}

MyTar::~MyTar(){
    this->inputStream.close();
    this->tarVector.clear();
}

/// Check the tar file is USTART format.
int MyTar::isUStar(){
    this->inputStream.seekg(0, ios::end);
    int fileSize = this->inputStream.tellg();

    /// If the file is not the mutiple of 512, the file is not USTAR format.
    if(fileSize % this->usBlockSize == 0){
        this->inputStream.seekg(257, ios::beg);
        char buffer[6];

        /// If the USTAR_id != "ustar ", the file is not USTAR format.
        if(this->inputStream.read(buffer, 6)){
            string str(buffer);
            if(str == "ustar "){
                return MyTar::USTAR;
            }
        }
    }

    return MyTar::NO_USTAR;
}

/// Start to read the tar file.
int MyTar::startRead(){

    /// If the file is not the mutiple of 512, the file is not USTAR format.
    if(MyTar::USTAR != this->isUStar()){
        return MyTar::NO_USTAR;
    }

    /// Make pointer to the first of the tar file.
    this->inputStream.seekg(0, ios::beg);

    while(this->inputStream){
        struct TarHeader buffer;

        if(this->inputStream.read((char*)&buffer, this->usBlockSize)){
            this->tarVector.push_back(buffer);

            int fileSize = this->hex2Dec(buffer.filesize, sizeof(buffer.filesize));

            /* After the header data is the content of the file in tar file.
             * So we need to ignore the block of the content.
             * jumpBock represents how many blocks we need to ignore.
             */
            int jumpBlock = ceil((double)fileSize / (double)this->usBlockSize);
            this->inputStream.seekg(jumpBlock * this->usBlockSize, ios::cur);
        }
    }

    /// If the size of tarVector > 0, the operation of reading successed.
    return this->tarVector.size() > 0 ? MyTar::USTAR : MyTar::NO_USTAR;
}

/// Show the content of the tar file.
void MyTar::showContent(){
    cout << "Total " << this->getFileNum() << " " << (this->getFileNum() > 1 ? "files." : "file.") << endl;
    cout << left << setw(12) << "filemode" << setw(32) << "username" << setw(32) << "groupname" << setw(6) << "size" << setw(100) << "filename" << endl;

    for(int i = 0; i < this->getFileNum(); i++){
        struct TarHeader buffer = this->tarVector.at(i);

        /// Check the file is directory and output the file mode.
        cout << left << setw(12) << (buffer.type == '5' ? 'd' : '-') + this->getFileMode(atoi(buffer.filemode));

        /// Output the user name.
        cout << left << setw(32) << buffer.username;

        /// Output the group name.
        cout << left << setw(32) << buffer.groupname;

        /// Output the file size.
        cout << left << setw(6) << this->hex2Dec(buffer.filesize, sizeof(buffer.filesize));
        //cout << left << setw(6) << this->hex2Dec(atoi(buffer.filesize));

        /// Output the file name.
        cout << left << setw(100) << buffer.filename << endl;
    }
}

/// Get the number of the files in the tar file.
int MyTar::getFileNum(){

    /// If fileNum > -1, return it directly.
    if(this->fileNum >= 0){
        return this->fileNum;
    }

    /* If the checksum is NULL, the block is NULL.
     * Check until the checksum is not NULL.
     * And return the number of files.
    */
    for(int i = this->tarVector.size() - 1; i > -1; i--){
        bool zeroFlag = true;
        struct TarHeader buffer = this->tarVector.at(i);

        for(int j = 0; j < (int)sizeof(buffer.checksum); j++){
            if(buffer.checksum[j] != 0x00){
                zeroFlag = false;

                break;
            }
        }

        if(!zeroFlag){
            /// Assign the value to the fileNum.
            this->fileNum = i + 1;
            break;
        }
    }

    return this->fileNum;
}

/// Get the file mode of the file in the tar file.
string MyTar::getFileMode(int modeNum){
    char r = 'r', w = 'w', x = 'x', no = '-';
    string fileMode[3];

    int index = 2;
    while(modeNum > 0){
        int subMode = modeNum % 10;
        modeNum /= 10;

        /// Check the read priviledge section.
        if(subMode >= 4){
            fileMode[index] += r;
            subMode -= 4;
        }
        else{
            fileMode[index] += no;
        }

        /// Check the write priviledge section.
        if(subMode >= 2){
            fileMode[index] += w;
            subMode -= 2;
        }
        else{
            fileMode[index] += no;
        }

        /// Check the execution priviledge section.
        if(subMode >= 1){
            fileMode[index] += x;
            subMode -= 1;
        }
        else{
            fileMode[index] += no;
        }

        --index;
    }

    return fileMode[0] + fileMode[1] + fileMode[2];
}

/// Change the value from hex to dec.
int MyTar::hex2Dec(const char* sizeArray, int length){
    int n = 0;
    int ans = 0;

    /* i = length - 2 beacuse the last char in sizeArray is null.
     * So we need to ignore it.
     */
    for(int i = length - 2; i > -1; i--){
        int num = sizeArray[i] >= 'a' && sizeArray[i] <= 'f' ? sizeArray[i] - 'a' + 10 : sizeArray[i] - '0';

        ans += num * pow(16, n);
        ++n;
    }

    /// return bytes, not bits.
    return ans /= 8;
}
