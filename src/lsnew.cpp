
#include <iostream>
#include <vector>
#include <sys/stat.h>
#include <sys/types.h>
#include <grp.h>
#include <pwd.h>
#include <dirent.h>
#include <cstdio>
#include <errno.h>
#include <iomanip>
#include <time.h>

#define green "\x1b[32;1m"
#define blue "\x1b[34;1m"
#define greybg "\x1b[;47m"
#define reset "\x1b[0m"

using namespace std;

int curDir(std::string chck){
    int is = 0;
    if(chck.length() == 1 && chck[0] == '.') {
        is = 1;
    }
    else if(!chck.compare("..")){
        is = 1;
    }
    return is;
}


int getStat(std::string name, struct stat& st){
    int succ = stat(name.c_str(),&st);
    if (succ == -1) {
        std::cout << name << endl;
        perror("stat");
        succ = 0;
    }
    else{
        succ = 1;
    }
    return succ;
}

int flags(const char* srch, int& key){
    int i  = 0;
    if (srch[0] == '-') {
        while (srch[i] != '\0') {
            switch (srch[i]) {
                case 'a':
                    key |= 1;
                    break;
                case 'l':
                    key |= 2;
                    break;
                case 'R':
                    key |= 4;
                default:;
            }
            ++i;
        }
    }
    return key;
}

char printChck(char succ, const int key, const int match){
    char perm = '-';
    if (key & match) {
        perm = succ;
    }
    return perm;
}


void txtColorPrnt(const std::string prnt, const struct stat& st){
    
    if (S_ISDIR(st.st_mode)) {
        std::cout << blue;
    }
    else if ((S_IXUSR|S_IXGRP|S_IXOTH)&(st.st_mode)) {
        std::cout << green;
    }
    if (prnt[0] == '.') {
        std::cout << greybg;
    }
    std::cout << prnt;
    std::cout << reset;
}


void printFile(string buf, struct stat& file, int flg){
    
    if (flg&2){
        int val = 1;
        struct group* grpid = getgrgid(file.st_gid);
        if (grpid == NULL) {
            perror("getgrgid");
            val = 0;
        }
        struct passwd* p = getpwuid(file.st_uid);
        if (p == NULL) {
            perror("getpwuid");
            val = 0;
        }
        struct tm* timeinfo = localtime (&file.st_mtime);
        string tim = asctime(timeinfo);
        tim = tim.substr(0,tim.length()-1);
        if (val) {
            string permissions;
            permissions +=(printChck('r', file.st_mode, S_IRWXU));
            permissions +=(printChck('w', file.st_mode, S_IWUSR));
            permissions +=(printChck('x', file.st_mode, S_IXUSR));
            
            permissions +=(printChck('r', file.st_mode, S_IRGRP));
            permissions +=(printChck('w', file.st_mode, S_IWGRP));
            permissions +=(printChck('x', file.st_mode, S_IXGRP));
            
            permissions +=(printChck('x', file.st_mode, S_IROTH));
            permissions +=(printChck('x', file.st_mode, S_IWOTH));
            permissions +=(printChck('x', file.st_mode, S_IXOTH));
            cout << permissions;
            cout << left << setw(4) << file.st_nlink; // number of hard links
            cout << left << setw(12) << p->pw_name; //user id of owner
            cout << left << setw(12) << grpid->gr_name;
            cout << left << setw(8) << file.st_size; // file size
            cout << left << setw(25) << tim;
            
        }
        txtColorPrnt(buf, file);
        if (val && flg&2) {
            cout << endl;
        }
    }
}
    
    int nextEntry(DIR*& dir, dirent*& direntp, struct stat& st){
        int succ = 0;
        if (dir != NULL) {
            direntp = readdir(dir);
            errno = 0;
            if ((direntp != NULL) && getStat(direntp->d_name, st)) {
                succ = 1;
            }
            else if (direntp == NULL && errno == EBADF){
                perror("readdir");
            }
        }
        return succ;
    }
    
    
    int getDir(string& dir, DIR*& dirp,struct stat & st){
        int succ = 1;
        
        dirp = opendir(dir.c_str());
        if (dirp == NULL) {
            perror("opendir");
            succ = 0;
        }
        
        if(getStat(dir,st) == 0){
            succ = 0;
        }
        return succ;
    }

    
    void traverse(string dir, int flg){
        unsigned int i = 0;
        DIR* dirp = NULL;
        dirent* direntp = NULL;
        vector<string> subdirs;
        struct stat st;
        if(getDir(dir, dirp, st)){
            printFile(dir+":", st, flg);
            while (nextEntry(dirp, direntp, st)) {
                string file = direntp->d_name;
                if (file[0] != '.' || (flg&1)){
                    printFile(file, st, flg);
                }
                if ((flg&4) && S_ISDIR(st.st_mode) && !curDir(file)){
                    string sub = dir;
                    sub += '/';
                    sub += file;
                    subdirs.push_back(sub);
                    cout << subdirs[subdirs.size()-1] << endl;
                }
            }
            closedir(dirp);
        }
        i = 0;
        while (i < subdirs.size()){
            traverse(subdirs[i], flg);
            ++i;
        }
    }
    
    
    vector<string>queue(vector<string>&buf, vector<string>&dirs, int flg){
        unsigned int i = 0;
        while (i < buf.size()) {
            struct stat st;
            int succ = getStat(buf[i], st);
            if(succ ){
                if (S_ISREG(st.st_mode)) {
                    printFile(buf[i], st, flg);
                }
                if (S_ISDIR(st.st_mode)) {
                    dirs.push_back(buf[i]);
                    if (buf.size() > 1) {
                        printFile(buf[i], st, flg);
                    }
                }
            }
            ++i;
        }
        return dirs;
    }
    
    int main(int argc, char * argv[]) {
        int key = 0;
        vector<string> buf;
        unsigned int i = 1;
        while (i < argc && argv[i][0] == '-') {
            key = flags(argv[i],key);
            ++i;
        }
        //http://unix.stackexchange.com/questions/103114/what-do-the-fields-in-ls-al-output-mean
        // just flag note for me delete
        while (i < argc){
            buf.push_back(argv[i]);
            ++i;
        }
        if (buf.empty()){
            buf.push_back(string("."));
        }
        vector<string> dirs;
        queue(buf,dirs, key);
        i = 0;
        while (i < dirs.size()) {
            traverse(dirs[i], key);
            ++i;
        }
        buf.clear(); // quells valgrind's paranoia
        cout<<endl;
        return 0;
    }
