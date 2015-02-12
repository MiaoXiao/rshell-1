all:  rshell mycpy ls cp dir

rshell:
	g++ -ansi -pedantic -Wall -Werror ./src/main.cpp ./src/SysCmds.cpp ./src/userIn.cpp -o rshell
ls:
	g++ -ansi -pedantic -Wall -Werror ./src/lsnew.cpp -o ls
cp:
	g++ -ansi -pedantic -Wall -Werror ./src/cp.cpp -o cp
mycpy:
	g++ -ansi -pedantic -Wall -Werror ./src/myCpy.cpp -o mycpy
dir:
	mkdir ./bin;
	mv RSHELL ./bin;
	mv mycpy ./bin;
	mv ls ./bin;

