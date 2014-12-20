#include "SysCmds.h"

SysCmds::SysCmds() : next(1), succ(1){}
SysCmds::~SysCmds(){}


int SysCmds::setCmd(char** cmdStrm){
    cmd = cmdStrm;
    if (cmdStrm!=NULL){
        std:: cout << cmdStrm[0] << std::endl;
    }
    dispatchCmds();
    return succ;
}

int SysCmds::contin(){
    return  next;
}

int SysCmds::getSucc(){
    return succ;
}

int SysCmds::dispatchCmds(){
    int succ = 0;
    next = 1;
    if(cmd == NULL){
        succ = 0;
        next = 0;
    }
    else if((succ = isBuiltIn()) > 0){
        succ = callBuiltIn(succ);
    }
    else{
        succ = callCmds();
    }
    cmd = NULL;
    return succ;
}

int SysCmds::checkPath(char* path){
    int good = 1;
    if(path == NULL){
        good = 0;
    }
    return good;
}

int SysCmds::cd(char* path){
    int success = 1;
    int a = chdir(path);
    if (path == NULL || path[0] == '\0'){
        std::cout << "null path blah " << std::endl;
    }
    if (a == -1){
        perror("chdir");
    }
    else{
        char* c = getenv("PATH");
        std:: cout << c << std::endl;
    }
    return success;
}

int SysCmds::isBuiltIn(){
    int builtin = 0;
    if(!strcmp(*cmd,"exit")){
        builtin = 1;
    }
    else if(!strcmp(*cmd,"cd")){
        builtin = 2;
    }
    return builtin;
}

int SysCmds::callBuiltIn(const int code){
    int success = 1;
    switch (code) {
        case 1:
            success = 0;
            next = -1;
            break;
        case 2:
            success = cd(cmd[1]);
            
        default:
            break;
    }
    return success;
}

int SysCmds::ls(){
    return 0;
}

int SysCmds::callCmds(){
    int success = 1;
    int pid = fork();
    switch (pid) {
        case (-1):
            perror("fork");
            success = 0;
            break;
        case 0:
            sysCall();
            break;
        default:
            success = waitProc(pid);
    }
    return success;
}

void SysCmds::sysCall(){
    int err = execvp(*cmd, cmd);
    if (err){
        perror("execvp");
        exit(1);
    }
}

void SysCmds::sysCall(char** sysCmd){
    int err = execvp(*sysCmd, sysCmd);
    if (err){
        perror("execvp");
        exit(1);
    }
}

int SysCmds::waitProc(int pid){
    int success = wait(&pid);
    while (success == -1 && errno == EINTR){
        success = wait(&pid);
    }
    if (success == -1){
        perror("wait");
        success = 0;
    }
    else{
        success = 1;
    }
    return success;
}
