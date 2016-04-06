/*
 * Author: Cody Malick
 * Program: Smallsh
 * General help implementing this shell from:
 * gnu.org/software/libc/manual/html_node/Implementing-a-Shell.html
 */

#include<stdlib.h>	 //Standard Library
#include<stdio.h>	//input output
#include<unistd.h> //Unix Standard Library
#include<fcntl.h> //clone on exec library
#include<signal.h> //Signal header
#include<sys/types.h>
#include<termios.h>
#include<string.h>

#define IN_BUFF 1024

typedef struct process
{
	struct process *next;
	char **argv;
	pid_t pid;
	char completed;
	char stopped;
	int status;
} process;

typedef struct job
{
	struct job *next;
	char *command;
	process *first_process;
	pid_t pgid;
	char notified;
	struct termios tmodes;
	int stdin, stdout, stderr;
} job;

job *first_job = NULL;
pid_t shell_pgid;
struct termios shell_tmodes;
int shell_terminal;
int shell_is_interactive;

//function prototypes
void prompt();
void handle_input(char input[IN_BUFF]);
void launch_process(process *p, pid_t pgid, int infile, int outfile, int errfile, int foreground);
job* find_job(pid_t pgid);
int job_is_stopped(job *j);
int job_is_completed(job *j);
void initialize_shell();
char* trim(char* s);
//shell commands
void sh_exit();
void sh_status();
void sh_cd();
void bin_exec(char command[IN_BUFF]);

int main(int argc, char** argv, char** envp) {

	initialize_shell();

	char input[IN_BUFF];

	while(1) {
		memset(input, 0, IN_BUFF);
		prompt();
		fgets(input, IN_BUFF, stdin);
		//strcpy(input, trim(input));


		handle_input(input);	
	}

	return 0;
}

void launch_process(process *p, pid_t pgid, int infile, int outfile, int errfile, int foreground) {	
	return;	
}

/*
 * gnu.org/software/libc/manual/html_node/Implementing-a-shell.html
 * The above was heavily referenced for the below code. I've commented the code so that
 * it is clear what is happening.
*/
void initialize_shell() {
	//check if interactive shell
	shell_terminal = STDIN_FILENO; //STDIN_FILENO, macro defined in <unistd.h>, should be 0
	
	//isatty() determines if file descriptor is associated with a terminal, returns 0 if it's not
	shell_is_interactive = isatty(shell_terminal);

	if(shell_is_interactive) {

		/*-------------------------------------
		 * loop until we are in the foreground
		 * 
		 * pid_t tcgetpgrp(int fd) - returns the process group ID of the foreground process group associated to
		 * fd, wihhc must be the controlling terminal of the calling process
		 *
		 * pid_t getpgrp(int pid) - returns PGID of calling process of pid, if(pid == 0), uses callling process
		 * pid
		 *-------------------------------------*/

		while(tcgetpgrp(shell_terminal) != (shell_pgid = getpgrp()))
			kill(- shell_pgid, SIGTTIN);

		// Ignore interactive and job-control signals
		//signal (SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		signal(SIGTSTP, SIG_IGN);
		signal(SIGTTIN, SIG_IGN);
		signal(SIGTTOU, SIG_IGN);
		signal(SIGCHLD, SIG_IGN);

		//put ourselves in our own process group
		shell_pgid = getpid();
		if(setpgid(shell_pgid, shell_pgid) < 0) {
			perror("couldn't put the shell in it's own process group");
		}

		//grab control of the terminal
		tcsetpgrp(shell_terminal, shell_pgid);
		//save default terminal attributes for shell
		tcgetattr(shell_terminal, &shell_tmodes);
	}


}

job* find_job (pid_t pgid)
{
	  job *j;

	    for (j = first_job; j; j = j->next)
			    if (j->pgid == pgid)
					      return j;
		  return NULL;
}

/* Return true if all processes in the job have stopped or completed.  */
int job_is_stopped (job *j)
{
	  process *p;

	    for (p = j->first_process; p; p = p->next)
			    if (!p->completed && !p->stopped)
					      return 0;
		  return 1;
}

/* Return true if all processes in the job have completed.  */
int job_is_completed (job *j)
{
	  process *p;

	    for (p = j->first_process; p; p = p->next)
			    if (!p->completed)
					      return 0;
		  return 1;
}

void prompt() {
	printf(":");
}

void handle_input(char input[IN_BUFF]) {
	if(!strcmp(input, "exit\n")) {	//'exit\n' because fgets adds a newline to the input
		sh_exit();
	}
	else if(!strcmp(input, "cd\n")) {
		sh_cd();
	}
	else if(!strcmp(input, "status\n")) {
		printf("Not implemented");
	}
	else {
		bin_exec(input);
	}	
	
	return;
}

void sh_exit() {
	exit(0);
}

void sh_cd() {
	char* home;
    home =  getenv("HOME");
	printf("%s\n", home);
}

char* get_home() {
	char* home;
    home =  getenv("HOME");
	return home;
}

char* get_path() {
	char* path = getenv("PATH");
	char* path = "/bin/";
	return path;
}

char* trim(char *s) {
	int len = strlen(s)-1;
	if((len>0) && (s[len] == '\n')) {
		s[len] = '\0';
	}
	return s;
}

void bin_exec(char command[IN_BUFF]) {
	char path[100];
	strcpy(command, trim(command));
	strcpy(path, get_path());
	strcat(path, command);

	printf("%s, %s\n", path, command);

	int ret;
	ret = fork();
	switch(ret) {
		case 0:
			printf("I am the child\n");
			ret = execl(path, command, (char *)NULL);
			perror("exec");
			exit(0);	
			break;
		case -1:
			perror("fork");
			break;
		default:
			printf("%i\n", ret);
			printf("I am the parent\n");
			wait();
			break;
	}

	
}
