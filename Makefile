all:  rshell ls dir

rshell:
	g++ -ansi -pedantic -Wall -Werror ./src/main.cpp ./src/SysCmds.cpp ./src/userIn.cpp -o rshell
	
ls:
	g++ -ansi -pedantic -Wall -Werror ./src/lsnew.cpp -o ls


dir:
	mkdir ./bin;
	mv RSHELL ./bin;
	mv ls ./bin;