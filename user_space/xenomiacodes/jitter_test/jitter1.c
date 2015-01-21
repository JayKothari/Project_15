#include <native/task.h>
#include <native/timer.h>


#define NO_TASK		1	//number of task descripter
//#define TASK_PRIO	0
#define TASK_STKSZ	4096
#define TASK_MODE	(T_FPU | T_CPU(0))


#define u_NSEC 			1000		//one microsecond in nanoseconds
#define m_NSEC 			1000000		//one millisecond in nanoseconds 
#define s_NSEC			1000000000	//one second in nanoseconds

#define SETUP_TICK_NSEC               1000000         //one millisecond timer tick - can be reasonably 
                                                //modified (increased or decreased)


#define PERIOD_1		(m_NSEC*1)	
#define PERIOD_2		(m_NSEC*2)
#define PERIOD_3		(m_NSEC*3)
#define PERIOD_4		(m_NSEC*4)


#define WAKE_TIME		(m_NSEC*200)
#define NO_ITERATION	5		

#define ONE_SHOT_MODE	1//if it is define 1 then then select oneshot mode otherwise select 
                            //periodic mode


RT_TASK task_desc[NO_TASK];

unsigned long  overrun_r;

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
	
	systime2 = rt_timer_read();			//read the current system time
        xnprintf("initial task system time ....value  %llu \n", systime2);
        
        task_pstart_time = systime2+WAKE_TIME;
        xnprintf("expected initial task wake up time ..value  %llu \n",task_pstart_time);
        
	ret = rt_task_set_periodic(NULL, task_pstart_time, PERIOD_1);
	if(ret<0) { xnprintf("error in rt_task_set_periodic()  %d ...\n",ret); return ;}

	systime2 = rt_timer_read();			//read the current system time
        xnprintf("actual system time after first task wake-up....value %llu \n",systime2);
        
        xnprintf("initial jitter after first task wake-up....value %lld \n",systime2-task_pstart_time);
	


	for(i=1; i<=NO_ITERATION; ++i){	
	
	       // ret = rt_task_inquire(NULL, &rttinfo);
	       // if(ret<0) { xnprintf("error in rt_task_inquire()  %d ...\n",ret); return ;}
               // reltime = rttinfo.relpoint;				//extract info
		
             xnprintf("iteration %d ..relpoint value....value %llu \n",i,reltime);
             expected_time = task_pstart_time + i*PERIOD_1;
             xnprintf("iteration %d ..expected task wake-up....value %llu \n",i,expected_time);
             //real -time code typically should go here 

             rt_task_wait_period(&overrun_r);
	     xnprintf("overrun=%lu\n",overrun_r);

             cur_sys_time = rt_timer_read();			//read the sys_time
             xnprintf("iteration %d ..actual task wake-up....value %llu \n", i, cur_sys_time);
             jitter = cur_sys_time - expected_time;
             xnprintf("iteration %d ..actual task wake-up....value %llu \n", i, cur_sys_time);
             xnprintf("%d th jitter initial jitter after first task wake-up....value %lld \n", i, jitter );
	
	}//end of while
	
END:ret = 0;
	xnprintf("exiting rt_task_1()...\n");
}//end of rt_task_1 function


//	char *rt_task_name[NO_TASK]={"rt_task_1","rt_task_2","rt_task_3","rt_task_4"};
//	int rt_task_prio[NO_TASK] = {11,2,3,4};	//set the priority for each task
//	void(*rt_task_body[NO_TASK])(void *cookie) = {rt_task_1,rt_task_2,rt_task_3,rt_task_4};

	char *rt_task_name[NO_TASK]={"rt_task_1"};
	int rt_task_prio[NO_TASK] = {11};	//set the priority for each task
	void(*rt_task_body[NO_TASK])(void *cookie) = {rt_task_1};

int init_module()
{
	int i;
	
	ret = 0;
#if ONE_SHOT_MODE == 1
	ret = rt_timer_set_mode(TM_ONESHOT);
	xnprintf("timer base in one-shot mode  ***********");
#else
	ret = rt_timer_set_mode(SETUP_TICK_NSEC);
	xnprintf("timer base in periodic mode  *********");
#endif
	if(ret<0) {
		xnprintf("error in rt_timer_set_mode()...\n");
		return ret;
	}
                
        systime1 = rt_timer_read();  //get system time 
	
        xnprintf("init...system time value  %llu \n", systime1);

	for(i=0; i<NO_TASK; ++i) 
	{
		//create task
                
		ret = rt_task_create(&task_desc[i], 
                         rt_task_name[i], TASK_STKSZ, 
                         rt_task_prio[i], TASK_MODE);
			if(ret<0) {	xnprintf("error in rt_task_create()_1...\n"); return ret;}
		
                //start task
                
		ret = rt_task_start(&task_desc[i], rt_task_body[i], NULL);
			if(ret ==  -EINVAL) 
			{	

                            xnprintf("error in rt_task_start()_1...\n"); 
                            rt_task_delete(&task_desc[i]); 
                            return ret;
                        }
	}//end of for_loop

	xnprintf("end of init_module()...\n");
	return 0;
}//end of init_module

void cleanup_module()
{
	int i;
	for(i=0; i<NO_TASK; ++i)
	{
		ret = rt_task_delete(&task_desc[i]);
		if(ret<0 && ret!= -EINVAL) 
			xnprintf("error in rt_task_delete()_1...\n");
	}


}//end of cleanup_module

MODULE_LICENSE("GPL");

