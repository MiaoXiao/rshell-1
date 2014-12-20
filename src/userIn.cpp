#include "userIn.h"

userIn::userIn() :  curDelim(";"), hostInfo(1), cmd(NULL), pos(0){
    host.resize(64);
    gethostname(&host[0],64);
    user = getlogin();
    if (user.empty() || host.empty()){
	perror("host");
	hostInfo = 0;
    }
    buf.clear();
    strAry.clear();
    getIn();
    clpCmts();
    prse();
}


userIn::~userIn(){
    delete[]cmd;
    if(strAry.size() > 0){
        unsigned int index = 0;
        while (index < strAry.size()) {
            delete[]strAry[index];
            ++index;
        }
    }
    strAry.clear();
    buf.clear();
}

void userIn::getIn(){
    if (hostInfo){
        std::cout << user << "@" << host;
    }
    std::cout << "$"; 
    getline(std::cin, buf,'\n');
}


char** userIn::getCmd(int succ){
    delete[] cmd;
    cmd = NULL;
    int index = 0;
    int val = 0;
    if(inRange(strAry, pos)){
        val = valid(succ);
    }
    while (inRange(strAry, pos+index)&& !val){
        if(isDelim(strAry[pos + index])){
            setDelim(strAry[pos + index]);
            val = valid(succ);
            while(inRange(strAry, pos+index+1) && isDelim(strAry[pos+index+1]) && val){
                ++index;
                setDelim(strAry[pos + index]);
                val = valid(1);
            }
        }
        else{
            ++index;
        }
    }
    pos += index;
    if(val){
        bldary();
    }
    if(inRange(strAry, pos)){
        setDelim(strAry[pos]);
    }
    else{
        curDelim = ";";
    }
    return cmd;
}


void userIn::clpCmts(){
    int len = (int)buf.find_first_of("#");
    if (len != std::string::npos){
        buf = buf.substr(0,len);
    }
}

int userIn::valid(const int succ){
    int cont = 0;
    switch (curDelim[0]){
        case '&':
            cont = (succ > 0);
            break;
        case '|':
            cont = (succ == 0);
            break;
        case ';':
            cont = 1;
        default:;
    }
    return cont;
}


void userIn::bldary(){
    int index = 0;
    while(inRange(strAry, pos+index) && !isDelim(strAry[pos + index])){
        ++index;
    }
    if(index > 0){
        try {
            cmd = new char*[index];
        } catch (std::bad_alloc) {
            perror("new");
            exit(1);
        }
        int i = 0;
        while (i < index){
            cmd[i] = strAry[pos+i];
            ++i;
        }
        cmd[i] = NULL;
        pos += index;
    }
    else{
        cmd = NULL;
    }
}

void userIn::setDelim(const char* delim){
    int is = 0;
    is = isDelim(delim);
    if (is){
        curDelim = delim;
        ++pos;
    }
    else {
        curDelim = ";";
    }
}

void userIn::prse(){
    int index = 0;
    int stubLen = 0;
    while(inRange(buf,index)){
        if((stubLen = isCmd(buf, index)) > 0){
            strAry.push_back(getStr(index, stubLen));
            index += stubLen;
        }
        else if((stubLen = isDelim(buf, index)) > 0){
            strAry.push_back(getStr(index, stubLen));
            index += stubLen;
        }
        else if((stubLen = isPipe(buf, index))){
            strAry.push_back(getStr(index, stubLen));
            index += stubLen;
        }
        else if((stubLen = isSpce(buf, index)) > 0){
            index += stubLen;
        }
        else{
            perror("invalid input");
            index = (int)buf.length();
        }
    }
}

char* userIn::getStr(const int start,const int len){
    char* str = NULL;
    try {
        str = new char[len+1];
    }catch (std::bad_alloc) {
        perror("new");
        exit(1);
    }
    int index = 0;
    while (index < len) {
        str[index] = buf[start+index];
        ++index;
    }
    str[index] = '\0';
    return str;
}

int userIn::inRange(std::vector<char*> strVec, unsigned int index){
    return index < strVec.size();
}

int userIn::inRange(const std::string srch, const unsigned int index){
    if (index > srch.length()){
        perror("out of range");
    }
    return (index < srch.length());
}


int userIn::isCmd(const std::string chck,const int index){
    int len = 0;
    if(isFlagStrt(chck, index)){
        ++len;
    }
    while(inRange(buf,index+len)
          && !isFlagStrt(chck, index+len)
          && !isDelim(chck, index+len)
          && !isSpce(chck, index+len)
          && !isPipe(chck, index+len)){
        ++len;
    }
    return len;
}

int userIn::isFlagStrt(const std::string chck,const unsigned int index){
    return chck[index] == '-';
}

int userIn::isPipe(const std::string chck, const unsigned int index){
    return (!isDelim(chck, index) && chck[index] == '|');
}

int userIn::isDelim(const char* delm){
    int is = 0;
    if (delm != NULL) {
        std::string tmp = delm;
        is = isDelim(tmp, 0);
    }
    return is;
}


int userIn::isDelim(const std::string chck, const unsigned int index){
    int is = 0;
    switch (chck[index]) {
        case '&':
        case '|':
            if(inRange(chck,index+1) && (chck[index+1] == chck[index])){
                is = 2;
            }
            break;
        case ';':
            is = 1;
        default:;
    }
    return is;
}

int userIn::isSpce(const std::string chck,const unsigned int index){
    int len = 0;
    while (inRange(chck, index+len) && (chck[index+len] < 0x21 || chck[index+len] > 0x7E)){
        ++len;
    }
    return len;
}

