#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>

struct pair 
{
	char val;
	short int cnt;
};

int myErr(int val, const char *msg)
{
	if(val == -1)
	{
		perror(msg);
		exit(EXIT_FAILURE);
	}
	return val;
}

int main(int argc, char *argv[]){
	int i = 0;
	char fname[1024] = "image.dat";
	int fd;
	int numRead;
	struct pair arr;
	struct timespec ts;

	if (argc > 1)
		strcpy(fname, argv[1]);
	ts.tv_sec = 1;
	ts.tv_nsec = 0;

	fd = myErr(open(fname, O_RDONLY), "open");

	while ((numRead = read(fd,&arr, sizeof(struct pair))) > 0)
	{
		printf("(%c, %2d) :: ", arr.val, arr.cnt);
		for (i = 0; i < arr.cnt;i++)
		{
			printf("%c", arr.val);
		}
		printf("\n");
		nanosleep(&ts, NULL);
	}
	close(fd);
}