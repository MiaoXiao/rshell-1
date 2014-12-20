all:  rshell ls cp dir

rshell:
	g++ -ansi -pedantic -Wall -Werror ./src/main.cpp ./src/SysCmds.cpp ./src/userIn.cpp -o rshell
	
ls:
	g++ -ansi -pedantic -Wall -Werror ./src/lsnew.cpp -o ls
cp:
	g++ -ansi -pedantic -Wall -Werror ./src/cp.cpp -o cp

dir:
	mkdir ./bin;
	mv RSHELL ./bin;
	mv ls ./bin;
