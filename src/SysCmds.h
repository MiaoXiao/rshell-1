#ifndef __SYSCMDS__
#define __SYSCMDS__
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>

class SysCmds{
public:
    SysCmds();
    ~SysCmds();
    int setCmd(char** cmdStrm);
    int contin();
    int getSucc();
    
private:
    
    int next;
    char** cmd;
    int dispatchCmds();
    int checkPath(char* path);
    int callBuiltIn(const int code);
    int ls();
    int cd(char* path);
    int succ;
    int callCmds();
    int isBuiltIn();
    void sysCall();
    void sysCall(char** sysCmd);
    int waitProc(int pid);
};

#endif

