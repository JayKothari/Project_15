#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{	
	int fd ;
	char kbuff[100], ubuff[]="hello world";
	
	fd = open ("/dev/MySerialDevice", O_RDWR , 0777 ) ;

	printf("%d\n",fd);
	
	if (fd == 0 )
	{	
		perror ("open:%s") ;
		exit (-1 ) ;
	}
	int err=read( fd, kbuff, sizeof (kbuff ));
	 if(err<0)
	{printf("read error\n");
	}
	printf ("%s\n", kbuff ) ;
	if(write ( fd, ubuff, sizeof ( ubuff ) ) < 0)
	{
		perror("write:") ;
		exit(-1);
	}

	err=read( fd, kbuff, sizeof (kbuff ));
        if(err<0)
        {printf("read error\n");
        }
	  printf ("%s\n", kbuff ) ;





	return 0 ;
}


