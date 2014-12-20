#ifndef __USERIN__
#define __USERIN__
#include <iostream>
#include <stdio.h>
#include <vector>
#include <stdlib.h>
#include <unistd.h>

class userIn{
public:
    userIn();
    ~userIn();
    void reset();
    char** getCmd(const int succ);
    std::vector<char*> a(){
        return strAry;
    }

private:
    
    // It should receive a success measure. That way it can determine the next that should be executed.
    // For example if successful and curDelim == ||, do not return that section. If the next is terminated by ; return the section after that. ; is basically a default for the last, but at that point there isn't anything that would have to call it. It also ensures that NULL isn't inappropriately returned.
    
    std::string buf;
    std::vector<char*> strAry;
    std::string curDelim;
    std::string user;
    std::string host;
    bool hostInfo;
    char** cmd;
    int pos;
    void getIn();
    void clpCmts();
    int valid(const int succ);
    void prse();
    char* getStr(const int start,const int len);
    void setDelim(const char* delim);
    void bldary();    
    int inRange(const std::vector<char*> b, const unsigned int index);
    int inRange(const std::string str,const unsigned int index);
    int isCmd(const std::string chck,const int index);
    int isFlagStrt(const std::string chck,const unsigned int index);
    int isPipe(const std::string chck, const unsigned int index);
    int isDelim(const char* delm);
    int isDelim(const std::string chck,const unsigned int index);
    int isSpce(const std::string chck,const unsigned int index);
};

#endif
