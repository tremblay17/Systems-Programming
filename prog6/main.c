/*
For this exercise we will be writing a program that creates a child process;
as to be expected the parent and the child perform different tasks.

Here is the parent's behavior: 
	signals to handle : 
	*SIGCHLD-- 
		the parent should use wait or waitpid to clean up after the children that have just terminated. 
		If no children remain, the parent should print a message indicating this and terminate.
	*SIGINT-- 
		the parent should handle SIGINT just like you did on the test, 
		print the message "Exit: Are you sure (Y/n)?" and handle accordingly.
	*SIGUSR1-- 
		the parent should print the message "The child has generated X values less than 25" and then 
		return to its pause state.
	*SIGUSR2-- 
		the parent should print the message "The child has generated Y values greater than 75" and then 
		return to its pause state.

Behavior in function main: 
	*The program should setup all signal handlers, spawn the child, and enter a pause loop; 
	*feel free to print messages along the way indicating everything you are doing.

    	*You might want to set up an exit handler that sends a SIGTERM to any children that are still running;
	*by default, the SIGINT from ctrl - c should be sent to the parent and it's children.

Here is the child's behavior: signals to handle :
    	*SIGINT-- 
		if your intention is to pause the child while handling SIGINT in the parent,
 		you will likely need to ignore SIGINT in the child.
	*SIGTERM-- 
		informs the child that its parent has died and needs to terminate itself

	*NONE or SIGALRM(depends on the timer you choose below)--

BEHAVIOR IN MAIN:	
	*The child should generate a random number between 0 and 100 every 15 seconds. 
		*If that number is less than 25: the child should generate SIGUSR1 and send it to the parent;
		
		*If that number is greater than 75: the child should generate SIGUSR2 and send it to the parent;
		
		*If that number is between 48 and 51: the child should exit.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <signal.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>

int cno = 0;
int stage = 0;
pid_t pid[3];

void exitP(void)
{
	char *out = "Parent has normally terminated\n";
	write(STDOUT_FILENO, out, strlen(out));
}
void exitC(void)
{
	char *out = "Child has normally terminated\n";
	write(STDOUT_FILENO, out, strlen(out));
}
void signalHandlerP(int sig)
{
	char other[1024] = {0};
	pid_t p;
	int status, i;

	if (sig == SIGCHLD)
	{
		while ((p = waitpid(-1, &status, WNOHANG | WUNTRACED | WCONTINUED)) > 0)
		{
			
		}
		if (p == -1)
		{
			if (errno == ECHILD)
			{
				exit(EXIT_SUCCESS);
			}
			perror("waitpid");
			exit(EXIT_FAILURE);
		}
	}
	if (sig == SIGINT) {
		char exitKey;
		printf("Exit: Are you sure? (y/n)");
		scanf("%c", &exitKey);
		if(exitKey == "y") {
			exit(EXIT_SUCCESS);
		}
		if(exitKey == "n") {
			exit(EXIT_SUCCESS);
		} else {
			perror("exitKey");
			exit(EXIT_FAILURE);
		}
	}
	if (sig == SIGUSR1){
		
	}
	if (sig == SIGUSR2) {

	}
}
void signalHandlerC(int sig)
{
	if (sig == SIGTERM){

	}
	if (sig == SIGINT) {

	}
	if (sig == SIGALRM) {
		
	}
}
int randNum(int high, int low) {
	int rng = high - low + 1;
	static int init;
	double mult;

	if (!init)
	{
		srand(time(NULL));
		init = 1;
	}
	mult = rand() / (double)RAND_MAX;
	return low + (rng * mult);
}

int main(int argc, char *argv[]) {
	int i = 0; /* loop counter */
	struct sigaction saP;
	struct sigaction saC;
	struct itimerval tv;
	saP.sa_handler = signalHandlerP;
	sigemptyset(&saP.sa_mask);
	saP.sa_flags = 0;
	saC.sa_handler = signalHandlerC;
	sigemptyset(&saC.sa_mask);
	saC.sa_flags = 0;

	tv.it_interval.tv_sec = 3;
	tv.it_interval.tv_usec = 0;
	tv.it_value.tv_sec = 3;
	tv.it_value.tv_usec = 0;

	/* first, add the signal hanlder for SIGCHLD */
	if (sigaction(SIGCHLD, &saP, NULL) == -1)
	{
		perror("sigaction");
		exit(EXIT_FAILURE);
	}

	/* ok .. let's fork some children */
	for (i = 0; i < 3; i++)
	{
		pid[i] = fork();
		switch (pid[i])
		{
		case -1:
			perror("fork");
			exit(EXIT_FAILURE);
		case 0:
			atexit(exitC);
			if (sigaction(SIGHUP, &saC, NULL) == -1)
			{
				perror("sigaction");
				exit(EXIT_FAILURE);
			}
			printf("Starting child %d\n", i + 1);
			while (1)
			{
				pause();
			}
			exit(EXIT_SUCCESS);
			break;
		}
	}
}

