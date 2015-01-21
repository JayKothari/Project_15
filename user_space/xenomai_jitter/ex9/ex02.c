#include <native/task.h>
#include <sys/mman.h>
#include <stdio.h>
#define TASK_PRIO 20 
#define TASK_MODE 0
#define TASK_STKSZ 0
RT_TASK task_desc;
void task_body (void *cookie) 
{
printf ("hello world\n");
}
int main (int argc, char *argv[]) 
{

int err;

mlockall(MCL_CURRENT|MCL_FUTURE);
err = rt_task_spawn(&task_desc, "simpleTask",TASK_STKSZ, TASK_PRIO, TASK_MODE,&task_body, NULL); 
if (!err) rt_task_delete(&task_desc);
printf ("Good Bye\n");
return 0;
}

