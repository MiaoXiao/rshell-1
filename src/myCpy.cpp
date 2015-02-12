#include <iostream>
#include <string>
#include <fstream>
#include <fcntl.h>
#include <cstdio>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "Timer.h"
#include <iomanip>
#include <cstring>

using namespace std;

//stream method
void oneChar(const char* src, const char* dest){
    char b = '\0';
    string placeholder = "";
    ifstream input(src, fstream::in);
    ofstream output(dest, fstream::out);
    while(getline(input, placeholder)){
        b = input.get();
        output.put(b);
    }
}

//read write with bufsize 1
void bufOneChar(const char* src, const char* dest){
    int srcdsc = open(src,O_RDONLY);
    if(srcdsc == -1){
        perror("open");
        exit(EXIT_FAILURE);
    }
    int destdsc = open(dest,O_RDWR | O_CREAT,00600);
    if(destdsc == -1){
        perror("open");
        exit(EXIT_FAILURE);
    }
    char buf[1];
    int readsuc = 0;
    int wrtsuc = 0;
    while((readsuc = read(srcdsc,&buf,1)) && (wrtsuc = write(destdsc,&buf,readsuc)));
    close(srcdsc);
    close(destdsc);
}

//read write with BUFSIZ
void bufOut(const char* src, const char* dest){
    int srcdsc = open(src,O_RDONLY);
    if(srcdsc == -1){
        perror("error with open");
        exit(EXIT_FAILURE);
    }
    int destdsc = open(dest,O_RDWR | O_CREAT,00600);
    if(destdsc == -1){
        perror("error with open");
        exit(EXIT_FAILURE);
    }
    char buf[BUFSIZ];
    int readsuc = 0;
    int wrtsuc = 0;
    while((readsuc = read(srcdsc,&buf,BUFSIZ)) && (wrtsuc = write(destdsc,&buf,readsuc)));
    close(srcdsc);
    close(destdsc);
}

int main(int argc, char* argv[]){
    int i = 1;
    while(i < argc && argv[i][0] == '-'){
        ++i;
    }
    if(i != argc-2){
        cout << "Invalid args" << endl;
        exit(EXIT_FAILURE);
    }
    struct stat srcst;
    struct stat destst;
    if(stat(argv[argc-2],&srcst) == -1){
        perror("Source does not exist");
        exit(EXIT_FAILURE);
    }
    if (S_ISDIR(srcst.st_mode)){
        cout << "Error: source is a directory" << endl;
        exit(1);
    }
    if(stat(argv[argc-1],&destst) == 0){
        perror("File already exists");
        exit(EXIT_FAILURE);
    }

    if (i < 2){ //best method
        bufOut(argv[argc-2], argv[argc-1]);
    }
    else{ //all 3 methods
        Timer t;
        double wall = 0.0;
        double user = 0.0;
        double syst = 0.0;
	
        t.start();
        oneChar(argv[argc-2],argv[argc-1]);
        t.elapsedTime(wall,user,syst);
        cout << "1:" << endl << "wall: " << wall << endl << "user: " << user << endl << "syst: " << syst << endl;
	
        t.start();
        bufOneChar(argv[argc-2], argv[argc-1]);
        t.elapsedTime(wall,user,syst);
        cout << "2:" << endl << "wall: " << wall << endl << "user: " << user << endl << "syst: " << syst << endl;

        t.start();
        bufOut(argv[argc-2], argv[argc-1]);
        t.elapsedTime(wall,user,syst);
        cout << "3:" << endl << "wall: " << wall << endl << "user: " << user << endl << "syst: " << syst << endl;
    }
    return 0;
}

