#include <iostream>
#include <string>
#include <vector>
#include <io.h>
#include <string.h>

using namespace std;

void getFiles(string foler, vector<string>& files);

int main() {
    string folder;
    cin >> folder;

    vector<string> files;
    getFiles(folder, files );

    for( int i = 0; i < files.size(); i++ ) {
        //To do here
        //cout << files[i] << endl;
    }
    return 0;
}

void getFiles( string path, vector<string>& files ) {
    //文件句柄
    long hFile = 0;
    //文件信息
    struct _finddata_t fileinfo;
    string p;
    if((hFile = _findfirst(p.assign(path).append("\\*").c_str(),&fileinfo)) !=  -1) {
        do {
            //如果是目录,迭代之
            //如果不是,加入列表
            if((fileinfo.attrib &  _A_SUBDIR)) {
                if(strcmp(fileinfo.name,".") != 0  &&  strcmp(fileinfo.name,"..") != 0)
                    getFiles( p.assign(path).append("\\").append(fileinfo.name), files );
            } else {
                files.push_back(p.assign(path).append("\\").append(fileinfo.name) );
            }
        } while(_findnext(hFile, &fileinfo)  == 0);
        _findclose(hFile);
    }
}
