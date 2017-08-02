Features :-
	semi colon seprated commnads work;
	& for background;
	remove extra spaces in echo;
	builtin of cd , echo , pwd;
	show relative path in promt and user name and host name;
	Show message when a background process stop;
	pinfo to get a particular process information;
	exit on exit command

Steps for compilation:-
	- make
	- ./main

decision made:-
	- functions stored in an array and run according to the command type only in case of pwd,echo,cd
	- check for pinfo and exit 

files relation:-
	- shell.c :- main loop;
	- builtin.c , builtin.h :- for defining builtin function (cd,pwd,echo)
	- execute.c , execute.h :- for execution of command and checking whether its builtin or not
	- parse.c , parse.h :- for parsing the command
	- pinfo.c ,pinfo.h :- for execution of pinfo command
	- readCommand.c , readCommand.h :- for taking input command 
	- rootDirectory.c , rootDirectory.h :- for findng initial root directory and then for finding relative path
	- signal.c , signal.h :- for getting the signal when a background process ends and print a message

For Assginment 3:

Input-Output Redirection:-
	-can take input from a file, use "< fileName" for input
	-can save output in a file, use "> fileName" for output or use ">> fileName" to append output in file

Piping along with Input-Output Redirection:-
	- support multiple pipes use "|" for piping
	- ASSUMPTION use input redirction only in 1st command in pipe and use output redirection in last command of pipe

Commands like:- 
	- jobs 
	- killall
	- kjob <jobid> <signalNumber>
	- fg <jobid>
	- quit

Signals like:- 
	- Ctrl + D
	- Ctrl + C
	- Ctrl + Z

decision made:-
	- use input redirction only in 1st command in pipe and use output redirection in last command of pipe

files relation:-
	- shell.c :- main loop
	- execute.c , execute.h :- contain code for I/O redirection
	- pipe.c, pipe.h :- contains code for piping
	- signal.c,signal.h :- code for signal Handling