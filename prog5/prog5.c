#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <signal.h>

int i = 0;
char fname[1024] = "image.dat";
struct pair arr;

struct pair
{
	char val;
	short int cnt;
};

int myErr(int val, const char *msg)
{
	if (val == -1)
	{
		perror(msg);
		exit(EXIT_FAILURE);
	}
	return val;
}

void printPair(char val, int val2){
	char out[256];

	sprintf(out, "(%c, %d)", val, val2);
	write(STDOUT_FILENO, out, strlen(out));
}

void printStr(const char *msg)
{
	write(STDOUT_FILENO, msg, strlen(msg));
}

void sigHand(int sig)
{
	if(sig == SIGALRM)
	{
		printf("(%c, %2d) :: ", arr.val, arr.cnt);
		for (i = 0; i < arr.cnt; i++)
		{
			printf("%c", arr.val);
		}
		printf("\n");
	}
}

int main(int argc, char *argv[])
{
	int fd;
	int numRead;sa_handler
	struct sigaction sa;
	struct itimerval it;

	it.it_interval.tv_sec = 1;
	it.it_interval.tv_usec = 0;
	it.it_value.tv_sec = 1;
	it.it_value.tv_usec = 0;

	sa.sa_handler = sigHand;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);

	myErr(sigaction(SIGALRM, &sa, NULL), "sigaction");
	myErr(setitimer(ITIMER_REAL, &it, NULL), "setittimer");

	if (argc > 1) strcpy(fname, argv[1]);

	fd = myErr(open(fname, O_RDONLY), "open");

	while ((numRead = read(fd, &arr, sizeof(struct pair))) > 0)
	{
		pause();
	}
	close(fd);
	exit(EXIT_SUCCESS);
}