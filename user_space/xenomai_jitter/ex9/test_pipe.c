#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
//#include <native/pipe.h>   //not needed ???

#define PIPE_MINOR 0


int pipe_fd;

int main(int argc, char *argv[])
{
	char devname[32], buf[16];


//	sprintf(devname, "/dev/rtp%d", PIPE_MINOR);
	pipe_fd = open("/dev/rtp0", O_RDWR);

	if (pipe_fd < 0)
	printf("fail to open\n");

	write(pipe_fd, "World", sizeof("World"));

/* Wait for the prompt string "Hello"... */
	read(pipe_fd, buf, sizeof(buf));

        printf("value read from rt task is %s\n", buf);

	/* Then send the reply string "World": */
//	write(pipe_fd, "World", sizeof("World"));

	/* continue and complete non-trivial work - eventually, close pipe ... */
        pause();


}

/*
#include<stdio.h>
 
int main()
{
  int a = 1;
  float b = 1.23;
  char c = 'u', string[100] = "My initial character array.";
 
//  printf("%s\n", string);
 
  sprintf(string, "Integer = %d, Float = %f, Character = %c.", a, b, c);
 
  printf("%s\n", string);
 
  return 0;
}
*/
