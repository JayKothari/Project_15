#include<stdio.h>
#include<signal.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/mman.h>
#include<native/task.h>
#include<native/task.h>
#include<rtdk.h>

RT_TASK demo_task;

void demo(void *arg)
{
	RT_TASK *curtask;
	RT_TASK_INFO curtaskinfo;
	//hello world
	rt_printf("hello world\n");
	//inquire current task
	curtask=rt_task_self();
	rt_task_inquire(curtask,&curtaskinfo);
	//printf task name
	rt_printf("task name:%s\n",curtaskinfo.name);
}
int main(int argc,char *argv[])
{
	char str[10];
	//perform auto-init of rt_printf buffer if the task doesnt do so
	rt_printf_auto_init(1);
	//lock memory : avoid memmory swapping for this program
	mlockall(MCL_CURRENT | MCL_FUTURE);
	sprintf(str,"hello");
	rt_task_create(&demo_task,str,0,50,0);
	rt_task_start(&demo_task,&demo,0);
}


