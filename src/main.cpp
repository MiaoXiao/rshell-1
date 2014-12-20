 
#include "userIn.h"
#include "SysCmds.h"
using namespace std;

//https://github.com/mikeizbicki/ucr-cs100/blob/cs100-2014fall/lecture/signals.cpp#L2
// getenv "PATH"
// tokenize by colon
//execv first is full path for example /bin/ls

int main(int argc, const char * argv[]) {
   // int a = NAME_MAX;
  
    int cont = 1;
    while(cont != -1){
        cont = 1;
        SysCmds b;
        userIn a;
        while (cont > 0){
            b.setCmd(a.getCmd(b.getSucc()));
            cont = b.contin();
        }
    }
    return 0;
}
