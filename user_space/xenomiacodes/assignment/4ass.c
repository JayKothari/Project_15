#include<native/task.h>
#include<native/timer.h>
#include <native/pipe.h>

#define NO_TASK		1
#define TASK_STKSZ	4096
#define TASK_MODE	(T_FPU|T_CPU(1))

#define u_NSEC		1000
#define m_NSEC		1000000
#define s_NSEC		1000000000

#define SETUP_TICK_NSEC		1000000
#define PIPE_MINOR 	0
#define PERIOD_1	(u_NSEC*1)
#define PERIOD_2	(m_NSEC*2)
#define PERIOD_3	(m_NSEC*3)
#define PERIOD_4	(u_NSEC*4)

#define WAKE_TIME	(m_NSEC*200)

#define NO_ITERATION	5		

#define ONE_SHOT_MODE	1//if it is define 1 then then select oneshot mode otherwise select 
                            //periodic mode


RT_TASK task_desc[NO_TASK];
RT_PIPE pipe_desc;

int ret;

RTIME tmp=0;
RTIME systime1=0,systime2=0;

void rt_task_1(void *cookie)
{
	RT_TASK_INFO rttinfo;
	RTIME reltime;			//to store wokenup time
	RTIME task_pstart_time=0, expected_time=0,cur_sys_time=0,jitter=0;
	int i;
	int tmp_ret;
	RT_PIPE_MSG *msgout;	


	systime2 = rt_timer_read();			//read the current system time
        xnprintf("initial task system time ....value  %llu \n", systime2);
        
        task_pstart_time = systime2+WAKE_TIME;
        xnprintf("expected initial task wake up time ..value  %llu \n",task_pstart_time);
        

	ret = rt_task_set_periodic(NULL, task_pstart_time, PERIOD_1);//makes real time task periodic// int rt_task_set_periodic (RT_TASK ∗ task, RTIME idate, RTIME period)
	if(ret<0) { xnprintf("error in rt_task_set_periodic()  %d ...\n",ret); return ;}


	systime2 = rt_timer_read();			//read the current system time
        xnprintf("actual system time after first task wake-up....value %llu \n",systime2);
        
        xnprintf("initial jitter after first task wake-up....value %lld \n", systime2-task_pstart_time);
	


	for(i=1; i<=(NO_ITERATION); ++i)
	{	
	
	       // ret = rt_task_inquire(NULL, &rttinfo);
	       // if(ret<0) { xnprintf("error in rt_task_inquire()  %d ...\n",ret); return ;}
               // reltime = rttinfo.relpoint;				//extract info
		
             xnprintf("iteration %d ..relpoint value....value %llu \n",i,reltime);
             expected_time = task_pstart_time + PERIOD_4 + PERIOD_1;
             xnprintf("iteration %d ..expected task wake-up....value %llu \n",i,expected_time);
             //real -time code typically should go here 
             
             rt_task_wait_period(NULL);				//wait
              
             cur_sys_time = rt_timer_read();			//read the sys_time
             xnprintf("iteration %d ..actual task wake-up....value %llu \n", i, cur_sys_time);
             jitter = cur_sys_time - expected_time;
             xnprintf("iteration %d ..actual task wake-up....value %llu \n", i, cur_sys_time);
             xnprintf("%d th jitter initial jitter after first task wake-up....value %lld \n", i, jitter );
	
	}//end of while
	
	int len = sizeof("Hello");
	strcpy(P_MSGPTR(msgout), "Hello");
	rt_pipe_send(&pipe_desc, msgout, len, P_NORMAL);
	

	xnprintf("exiting rt_task_1()...\n");
}

void rt_task_2(void *cookie)
{
RT_PIPE_MSG *msgin;
int n = rt_pipe_receive(&pipe_desc, &msgin, TM_INFINITE);
if(n<0)
{printk("error");
}

xnprintf("%s\n",msgin);
rt_pipe_free(&pipe_desc, msgin);

}




//	char *rt_task_name[NO_TASK]={"rt_task_1","rt_task_2","rt_task_3","rt_task_4"};
//	int rt_task_prio[NO_TASK] = {11,2,3,4};	//set the priority for each task
//	void(*rt_task_body[NO_TASK])(void *cookie) = {rt_task_1,rt_task_2,rt_task_3,rt_task_4};

	char *rt_task_name[NO_TASK]={"rt_task_1","rt_task_2","rt_task_3","rt_task_4","rt_task_5"};
	int rt_task_prio[NO_TASK] = {5,5,5,5,5};	//set the priority for each task
	void(*rt_task_body[NO_TASK])(void *cookie) = {rt_task_1,rt_task_2};//task entry point

//***********************************************initialized************************************************************//

int init_module()//called when modules is loaded
{
	int i;
	
	int err = rt_pipe_create(&pipe_desc, "rt_pipe_jit", PIPE_MINOR,1024);
	if(err<0)
	{printk("pipe creation error");	
	}
	ret = 0;
#if ONE_SHOT_MODE == 1
	ret = rt_timer_set_mode(TM_ONESHOT);//sets timer MODE
	xnprintf("timer base in one-shot mode  ***********");
#else
	ret = rt_timer_set_mode(SETUP_TICK_NSEC);
	xnprintf("timer base in periodic mode  *********");
#endif
	if(ret<0) 
	{
		xnprintf("error in rt_timer_set_mode()...\n");
		return ret;
	}
	if(ret==-ENODEV)
	{
	printk("underlying architecture does not support the requested periodic timing");
	}
                
        systime1 = rt_timer_read();  //get system time 
	
        xnprintf("init...system time value  %llu \n", systime1);

	for(i=0; i<NO_TASK; ++i)
	 {
		//create task
                
			ret = rt_task_create(&task_desc[i],  rt_task_name[i], TASK_STKSZ,rt_task_prio[i],TASK_MODE);
		
			if(ret<0) 
			{xnprintf("error in rt_task_create()_1...\n"); return ret;
			}
			if(ret==-ENOMEM)
			{printk("e system fails to get enough dynamic memory from the global real-time heap in order to create or register the task.");
			}
			if(ret== -EEXIST)
			{printk("e name is already in use by some registered object.");
			}
			if(ret== -EPERM)
			{printk(" service was called from an asynchronous context.");
			}
                
		//start task
                
		ret = rt_task_start(&task_desc[i], rt_task_body[i], NULL);
			
			if(ret<0) 
			{	
                            xnprintf("error in rt_task_start()_1...\n"); 
                            rt_task_delete(&task_desc[i]); 
                            return ret;
                        }
			if(ret==-EINVAL)
			{printk(" task is not a task descriptor.");
			}
			if(ret==-EIDRM)
			{printk("task is a deleted task descriptor.");
			}
			if(ret==-EBUSY)
			{printk("f task is already started.");
			}
			if(ret== -EPERM)
			{printk("service was called from an asynchronous context.");
			}
	}//end of for_loop

	xnprintf("end of init_module()...\n");
	return 0;
}//end of init_module

//***************************************************************************************************************//
//*********************************************exit*****************************************************************//
void cleanup_module()//called when module is remmoved
{
	int i;
	for(i=0; i<NO_TASK; ++i)
	{
		ret = rt_task_delete(&task_desc[i]);
		if(ret<0 && ret!= -EINVAL) 
			xnprintf("error in rt_task_delete()_1...\n");
	}
	rt_pipe_delete(&pipe_desc);

}//end of cleanup_module

//***************************************************************************************************************//

MODULE_LICENSE("GPL"); //To access Xenomai symbols

