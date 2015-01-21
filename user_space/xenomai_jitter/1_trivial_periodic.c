#include<stdio.h>
#include<signal.h>
#include<unistd.h>
#include<sys/mman.h>
#include<rtdk.h>

#include<native/task.h>
#include<native/timer.h>
#include<native/pipe.h>

#define TASK_PRIO 99
#define TASK_MODE T_FPU|T_CPU(0)
#define TASK_STKSZ 4096

RT_TASK demo_task;

RT_PIPE p1;
int ret=0;

void demo(void *arg)
{
	RTIME now, previous;
	ret =rt_pipe_create(&p1,"test_pipe",0,0);
	if(ret<0)
	{
	printf("error in rt_pipe and value is %d\n",ret);
	}
	rt_task_set_periodic(NULL,TM_NOW,1000000000);
	previous = rt_timer_read();

	while(1)
	{
		rt_task_wait_period(NULL);
		now=rt_timer_read();

		printf("timer since last turn:%ld.061d ms\n",
				(long)(now-previous)/1000000,
				(long)(now-previous) % 1000000);
				previous = now;
	}
}
void catch_signal(int sig)
{
printf("real time task is terminated");
}

int main(int argc,char * argv[])
{
	signal(SIGTERM,catch_signal);
	signal(SIGINT,catch_signal);//CRTL+C

	mlockall(MCL_CURRENT|MCL_FUTURE);
	

	rt_task_create(&demo_task,"trivial",0,99,0);


	rt_task_start(&demo_task,&demo,NULL);

	pause();

	rt_task_delete(&demo_task);
}



