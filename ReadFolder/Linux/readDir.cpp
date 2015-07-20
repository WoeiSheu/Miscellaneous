#include <unistd.h>
#include <iostream>
#include <dirent.h>
#include <sys/stat.h>
#include <vector>
#include <string.h>
#include <algorithm> 
#include <stdlib.h>
#define MAX_COUNT 100 //如果查找的文件很多,超过100个,每按一次显示100个
#define FILE 0 //查找的是文件
#define DIRECTORY 1 //查找的是目录
using namespace std;

//返回目录dir下的,filetype类型文件或目录,每次返回一条,没有返回""
string getContent(const char* dir, string filetype="",int fileOrDir=FILE);
//得到目录dir下的filetype类型文件或dir下的目录的列表
vector<string> getall(const char* dir, string filetype="", int fileOrDir=FILE);
//对得到得到的返回列表进行排序的函数,实则为比较两个字符串大小
bool fileCmp(string file1, string file2);
//把一个string字符串中的小写字符转换成大写
string toUpper(string& str);

//目录数
int dirCount = 0;
//文件数
int fileCount = 0;

int main(int argc, char*argv[]) {
    //至少两个参数,即命令+目录,还可以在加一个文件类型
    if(argc<2) {
        cout << "Usage:executable [directory] (+[file type])!" << endl
            << " Like:./a.out /home/localhost/" << endl
            << "      ./a.out /home/localhost/src/ .cpp" << endl
            << "      ./a.out /home/localhost/src/ cpp" << endl;
            return -1;
    }
    string stropt = "";
    //判断目录是否存在.如果不存在,提示是否创建,输入y,创建,否则什么也不做!
    if(access(argv[1],F_OK)!=0) {
        cout << "Directory " << argv[1] << " is not exist!Create it?(Y/N):";
        string str = "";
        cin >> str;
        if(str.at(0)=='y'||str.at(0)=='Y') {
            mkdir(argv[1],777);
        }
    } else {
        string fullPathfFilename;
        string filetype = "";
        if(argc>2) {
            filetype = argv[2];
            if(filetype.at(0)=='.') {
                filetype = filetype.substr(1,filetype.length()-1);
            }
        }
        int i=0;
        //先显示所有目录
        while(1) {
            i++;
            //每次获取dir下的一个目录
            fullPathfFilename=getContent(argv[1],filetype,DIRECTORY);
            if(fullPathfFilename.length()==0) break;
            if(i==1) {
                cout << "Directory [" << argv[1] << "]:has [" << dirCount << "] directorys!" << endl;
            }
            cout << fullPathfFilename << endl;
        }
        cout << endl << endl;
        i=0;
        //显示该类型的或全部文件
        while(1) {
            i++;
            //每次获取dir下的一个filetype类型文件,如果filetype为"",则获取所有文件
            fullPathfFilename=getContent(argv[1],filetype,FILE);
            if(fullPathfFilename.length()==0) break;
            if(i==1) {
                cout << "And has["<< fileCount << ((filetype.size()==0)?"]":("] "+toUpper(filetype))) <<" files!" << endl;
            }
            cout << fullPathfFilename << endl;
            if(i>199&&i%100==0) {
                cout << "Press Any key to continue show other 100 items!";
                cin >> stropt;
                if(stropt.at(0)=='e'||stropt.at(0)=='q') break;
                cout << "                                                \r";
            }
        }
    }

    return 0;
}

string getContent(const char* dir,string filetype, int fileOrDir)
{
	static vector<string> vs;
	static vector<string> vdir;
	static int index = 0;
	static int idir = 0;
	
	if(index==0)
	{
		vs = getall(dir,filetype,fileOrDir);
		fileCount = vs.size();
		sort(vs.begin(),vs.end(),fileCmp);
	}
	
	if(idir==0)
	{
		vdir = getall(dir,filetype,fileOrDir);
		dirCount = vdir.size();
		sort(vdir.begin(),vdir.end(),fileCmp);
	}
	
	if(fileOrDir==FILE)
	{
		if(index<vs.size())
		{
			return vs.at(index++);
		}
		else
		{
			return "";
		}
	}
	else
	{
		if(idir<vdir.size())
			return vdir.at(idir++);
		else
			return "";
	}
}

vector<string> getall(const char* dir, string filetype, int fileOrDir)
{
	vector<string> dirlists,filelists;
	dirlists.clear();
	filelists.clear();
	
	string strCurDir = "";
	string strSubDir = "";
	string strFile = "";
	
	strCurDir = dir;
	if(strCurDir.length()==0)
	{
		return filelists;
	}
	
	if(strCurDir.at(strCurDir.length()-1)!='/')
	{
		strCurDir += "/";
	}
	
	dirlists.push_back(strCurDir);
	dirCount++;
	DIR* dp;
	struct dirent* direntp;
	
	int k = filetype.size();
	int n = 0;
	string strTemp = "";
	
	while(!dirlists.empty())
	{
		strCurDir = dirlists.back();
		dirlists.pop_back();
		dp = NULL;
		
		if((dp=opendir(strCurDir.c_str()))==NULL)
		{
			cout << "ERROR:Open " << strCurDir << " failed!" << endl;
			//该目录无法打开，查找下一个目录
			continue;
		}
		while((direntp=readdir(dp))!=NULL)
		{
			strFile.clear();
			strFile = strCurDir;
			strFile += direntp->d_name;
			
			//每个文件下都有“.”代表当前目录， “..”代表上一层目录，都要过滤掉
			if(strcmp(direntp->d_name,".")==0||strcmp(direntp->d_name,"..")==0)
			{
				continue;
			}
			
			if(fileOrDir==FILE)
			{
				if(direntp->d_type==DT_DIR)
				{
					strSubDir.clear();
					strSubDir = strCurDir+direntp->d_name;
					strSubDir += "/";
					dirlists.push_back(strSubDir);
					continue;
				}
				if(filetype.size()!=0)
				{
					n = strFile.size();
					if(strFile.at(n-k-1)!='.') continue;
					strTemp.clear();
					strTemp = strFile.substr(n-k,k);
					if(!(filetype==strTemp)) continue;
				}
				filelists.push_back(strFile);
			}
			else
			{
				if(direntp->d_type!=DT_DIR)
				{
					continue;
				}
				else
				{
					strSubDir.clear();
					strSubDir = strCurDir+direntp->d_name;
					strSubDir += "/";
					filelists.push_back(strSubDir);
					dirlists.push_back(strSubDir);
				}
			}
		}
		closedir(dp);
	}
	return filelists;
}

bool fileCmp(string file1, string file2)
{
	return file1<file2;
}

string toUpper(string& str)
{
	int n = str.size();
	char c = 'a';
	for(int i=0; i<n; i++)
	{
		c = str.at(i);
		if(c>='a'&&c<='z')
		{
			str.at(i) = c + 'A' - 'a';
		}
	}
	return str;
}
