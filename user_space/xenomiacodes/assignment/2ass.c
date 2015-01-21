#include<native/task.h>
#include<native/timer.h>
#include <native/sem.h>
#define NO_TASK		1
#define TASK_STKSZ	4096
#define TASK_MODE	(T_FPU|T_CPU(0))

#define u_NSEC		1000
#define m_NSEC		1000000
#define s_NSEC		1000000000

#define SETUP_TICK_NSEC		1000000

#define PERIOD_1	(m_NSEC*1)
#define PERIOD_2	(m_NSEC*2)
#define PERIOD_3	(m_NSEC*3)
#define PERIOD_4	(m_NSEC*4)
#define PERIOD_5        (m_NSEC*5)

#define WAKE_TIME	(m_NSEC*200)

#define NO_ITERATION	5		

#define ONE_SHOT_MODE	1//if it is define 1 then then select oneshot mode otherwise select 
                            //periodic mode

#define 	SEM_INIT 	1 /* Initial semaphore count */
#define 	SEM_MODE 	S_FIFO /* Wait by FIFO order */
RT_TASK task_desc1,task_desc2,task_desc3,task_desc4,task_desc5;

RT_SEM sem_desc;
int ret;

RTIME tmp=0;
RTIME systime1=0,systime2=0;


//*************************************************************task1*************************************************************************************************//
void rt_task_1(void *cookie)
{	rt_sem_p(&sem_desc,TM_INFINITE);
	RT_TASK_INFO rttinfo;
	RTIME reltime;			//to store wokenup tik_desc[i]e
	RTIME task_pstart_time=0, expected_time=0,cur_sys_time=0,jitter=0;
	int i;
	int tmp_ret;
	
	systime2 = rt_timer_read();			//read the current system time
        xnprintf("initial task system time ....value  %llu \n", systime2);
        
        task_pstart_time = systime2+WAKE_TIME;
        xnprintf("expected initial task wake up time ..value  %llu \n",task_pstart_time);
        

	ret = rt_task_set_periodic(NULL, task_pstart_time, PERIOD_1);//makes real time task periodic// int rt_task_set_periodic (RT_TASK ∗ task, RTIME idate, RTIME period)
	if(ret<0) { xnprintf("error in rt_task_set_periodic()  %d ...\n",ret); return ;}


	systime2 = rt_timer_read();			//read the current system time
        xnprintf("actual system time after first task wake-up....value %llu \n",systime2);
        
        xnprintf("initial jitter after first task wake-up....value %lld \n", systime2-task_pstart_time);
	for(i=1; i<=NO_ITERATION; ++i)
	{
	        ret = rt_task_inquire(NULL, &rttinfo);
	        if(ret<0) { xnprintf("error in rt_task_inquire()  %d ...\n",ret); return ;}
                reltime = rttinfo.relpoint;				//extract info
		
             xnprintf("iteration %d ..relpoint value....value %llu \n",i,reltime);
             expected_time = task_pstart_time + i*PERIOD_1;
             xnprintf("iteration %d ..expected task wake-up....value %llu \n",i,expected_time);
             //real -time code typically should go here 

             rt_task_wait_period(NULL);				//wait for next period release point

             cur_sys_time = rt_timer_read();			//read the sys_time
             xnprintf("iteration %d ..actual task wake-up....value %llu \n", i, cur_sys_time);
             jitter = cur_sys_time - expected_time;
          //   xnprintf("iteration %d ..actual task wake-up....value %llu \n", i, cur_sys_time);
             xnprintf("%d th jitter initial jitter after first task wake-up....value %lld \n", i, jitter );
	
	}//end of while
	
	xnprintf("exiting rt_task_1()...\n");
	rt_sem_v(&sem_desc);
}//end of rt_task_1 function
//*******************************************************************************************************************************************************************//
//*************************************************************task2************************************************************************************************//

void rt_task_2(void *cookie)
{	rt_sem_p(&sem_desc,TM_INFINITE);
        RT_TASK_INFO rttinfo;
        RTIME reltime;                  //to store wokenup tik_desc[i]e
        RTIME task_pstart_time=0, expected_time=0,cur_sys_time=0,jitter=0;
        int i;
        int tmp_ret;

        systime2 = rt_timer_read();                     //read the current system time
        xnprintf("initial task system time ....value  %llu \n", systime2);

        task_pstart_time = systime2+WAKE_TIME;
        xnprintf("expected initial task wake up time ..value  %llu \n",task_pstart_time);


        ret = rt_task_set_periodic(NULL, task_pstart_time, PERIOD_2);//makes real time task periodic// int rt_task_set_periodic (RT_TASK ∗ task, RTIME idate, RTIME period)
        if(ret<0) { xnprintf("error in rt_task_set_periodic()  %d ...\n",ret); return ;}


        systime2 = rt_timer_read();                     //read the current system time
        xnprintf("actual system time after first task wake-up....value %llu \n",systime2);

        xnprintf("initial jitter after first task wake-up....value %lld \n", systime2-task_pstart_time);
        for(i=1; i<=NO_ITERATION; ++i)
        {     
                ret = rt_task_inquire(NULL, &rttinfo);
                if(ret<0) { xnprintf("error in rt_task_inquire()  %d ...\n",ret); return ;}
                reltime = rttinfo.relpoint;                             //extract info

             xnprintf("iteration %d ..relpoint value....value %llu \n",i,reltime);
             expected_time = task_pstart_time + i*PERIOD_2;
             xnprintf("iteration %d ..expected task wake-up....value %llu \n",i,expected_time);
             //real -time code typically should go here 

             rt_task_wait_period(NULL);                         //wait for next period release point

             cur_sys_time = rt_timer_read();                    //read the sys_time
             xnprintf("iteration %d ..actual task wake-up....value %llu \n", i, cur_sys_time);
             jitter = cur_sys_time - expected_time;
          //   xnprintf("iteration %d ..actual task wake-up....value %llu \n", i, cur_sys_time);
             xnprintf("%d th jitter initial jitter after first task wake-up....value %lld \n", i, jitter );

        }//end of while

        xnprintf("exiting rt_task_2()...\n");
	rt_sem_v(&sem_desc);
}//end of rt_task_2 function
//********************************************************************************************************************************************************//

//**************************************************************task3*************************************************************************************//
void rt_task_3(void *cookie)
{	rt_sem_p(&sem_desc,TM_INFINITE);
        RT_TASK_INFO rttinfo;
        RTIME reltime;                  //to store wokenup tik_desc[i]e
        RTIME task_pstart_time=0, expected_time=0,cur_sys_time=0,jitter=0;
        int i;
        int tmp_ret;

        systime2 = rt_timer_read();                     //read the current system time
        xnprintf("initial task system time ....value  %llu \n", systime2);

        task_pstart_time = systime2+WAKE_TIME;
        xnprintf("expected initial task wake up time ..value  %llu \n",task_pstart_time);


        ret = rt_task_set_periodic(NULL, task_pstart_time, PERIOD_3);//makes real time task periodic// int rt_task_set_periodic (RT_TASK ∗ task, RTIME idate, RTIME period)
        if(ret<0) { xnprintf("error in rt_task_set_periodic()  %d ...\n",ret); return ;}


        systime2 = rt_timer_read();                     //read the current system time
        xnprintf("actual system time after first task wake-up....value %llu \n",systime2);

        xnprintf("initial jitter after first task wake-up....value %lld \n", systime2-task_pstart_time);
        for(i=1; i<=NO_ITERATION; ++i)
        {     
                ret = rt_task_inquire(NULL, &rttinfo);
                if(ret<0) { xnprintf("error in rt_task_inquire()  %d ...\n",ret); return ;}
                reltime = rttinfo.relpoint;                             //extract info

             xnprintf("iteration %d ..relpoint value....value %llu \n",i,reltime);
             expected_time = task_pstart_time + i*PERIOD_3;
             xnprintf("iteration %d ..expected task wake-up....value %llu \n",i,expected_time);
             //real -time code typically should go here 

             rt_task_wait_period(NULL);                         //wait for next period release point

             cur_sys_time = rt_timer_read();                    //read the sys_time
             xnprintf("iteration %d ..actual task wake-up....value %llu \n", i, cur_sys_time);
             jitter = cur_sys_time - expected_time;
          //   xnprintf("iteration %d ..actual task wake-up....value %llu \n", i, cur_sys_time);
             xnprintf("%d th jitter initial jitter after first task wake-up....value %lld \n", i, jitter );

        }//end of while

        xnprintf("exiting rt_task_3()...\n");
	rt_sem_v(&sem_desc);
}//end of rt_task_3 function



void rt_task_4(void *cookie)
{	rt_sem_p(&sem_desc,TM_INFINITE);//decrements semaphore(no resource available)
        RT_TASK_INFO rttinfo;
        RTIME reltime;                  //to store wokenup tik_desc[i]e
        RTIME task_pstart_time=0, expected_time=0,cur_sys_time=0,jitter=0;
        int i;
        int tmp_ret;

        systime2 = rt_timer_read();                     //read the current system time
        xnprintf("initial task system time ....value  %llu \n", systime2);

        task_pstart_time = systime2+WAKE_TIME;
        xnprintf("expected initial task wake up time ..value  %llu \n",task_pstart_time);


        ret = rt_task_set_periodic(NULL, task_pstart_time, PERIOD_4);//makes real time task periodic// int rt_task_set_periodic (RT_TASK ∗ task, RTIME idate, RTIME period)
        if(ret<0) { xnprintf("error in rt_task_set_periodic()  %d ...\n",ret); return ;}


        systime2 = rt_timer_read();                     //read the current system time
        xnprintf("actual system time after first task wake-up....value %llu \n",systime2);

        xnprintf("initial jitter after first task wake-up....value %lld \n", systime2-task_pstart_time);
        for(i=1; i<=NO_ITERATION; ++i)
        {     
                ret = rt_task_inquire(NULL, &rttinfo);
                if(ret<0) { xnprintf("error in rt_task_inquire()  %d ...\n",ret); return ;}
                reltime = rttinfo.relpoint;                             //extract info

             xnprintf("iteration %d ..relpoint value....value %llu \n",i,reltime);
             expected_time = task_pstart_time + i*PERIOD_4;
             xnprintf("iteration %d ..expected task wake-up....value %llu \n",i,expected_time);
             //real -time code typically should go here 

             rt_task_wait_period(NULL);                         //wait for next period release point

             cur_sys_time = rt_timer_read();                    //read the sys_time
             xnprintf("iteration %d ..actual task wake-up....value %llu \n", i, cur_sys_time);
             jitter = cur_sys_time - expected_time;
          //   xnprintf("iteration %d ..actual task wake-up....value %llu \n", i, cur_sys_time);
             xnprintf("%d th jitter initial jitter after first task wake-up....value %lld \n", i, jitter );

        }//end of while

        xnprintf("exiting rt_task_4()...\n");
	rt_sem_v(&sem_desc);//increments semaphore(resource available)
}//end of rt_task_4 function
//****************************************************************************************************************************************************************//

//***************************************************************task5*********************************************************************************************//
void rt_task_5(void *cookie)
{
	rt_sem_p(&sem_desc,TM_INFINITE);
        RT_TASK_INFO rttinfo;
        RTIME reltime;                  //to store wokenup tik_desc[i]e
        RTIME task_pstart_time=0, expected_time=0,cur_sys_time=0,jitter=0;
        int i;
        int tmp_ret;

        systime2 = rt_timer_read();                     //read the current system time
        xnprintf("initial task system time ....value  %llu \n", systime2);

        task_pstart_time = systime2+WAKE_TIME;
        xnprintf("expected initial task wake up time ..value  %llu \n",task_pstart_time);


        ret = rt_task_set_periodic(NULL, task_pstart_time, PERIOD_5);//makes real time task periodic// int rt_task_set_periodic (RT_TASK ∗ task, RTIME idate, RTIME period)
        if(ret<0) { xnprintf("error in rt_task_set_periodic()  %d ...\n",ret); return ;}


        systime2 = rt_timer_read();                     //read the current system time
        xnprintf("actual system time after first task wake-up....value %llu \n",systime2);

        xnprintf("initial jitter after first task wake-up....value %lld \n", systime2-task_pstart_time);
        for(i=1; i<=NO_ITERATION; ++i)
        {     
                ret = rt_task_inquire(NULL, &rttinfo);
                if(ret<0) { xnprintf("error in rt_task_inquire()  %d ...\n",ret); return ;}
                reltime = rttinfo.relpoint;                             //extract info

             xnprintf("iteration %d ..relpoint value....value %llu \n",i,reltime);
             expected_time = task_pstart_time + i*PERIOD_5;
             xnprintf("iteration %d ..expected task wake-up....value %llu \n",i,expected_time);
             //real -time code typically should go here 

             rt_task_wait_period(NULL);                         //wait for next period release point

             cur_sys_time = rt_timer_read();                    //read the sys_time
             xnprintf("iteration %d ..actual task wake-up....value %llu \n", i, cur_sys_time);
             jitter = cur_sys_time - expected_time;
          //   xnprintf("iteration %d ..actual task wake-up....value %llu \n", i, cur_sys_time);
             xnprintf("%d th jitter initial jitter after first task wake-up....value %lld \n", i, jitter );

        }//end of while

        xnprintf("exiting rt_task_5()...\n");
	rt_sem_v(&sem_desc);
}//end of rt_task_5 function
//*************************************************************************************************************************//

//	const char *rt_task_name[NO_TASK]={"rt_task_1","rt_task_2","rt_task_3","rt_task_4","rt_task_5"};
//	int rt_task_prio[NO_TASK] = {5,5,5,5,5};	//set the priority for each task
	void(*rt_task_body[NO_TASK])(void *cookie) = {rt_task_1,rt_task_2,rt_task_3,rt_task_4,rt_task_5};

//	char *rt_task_name[NO_TASK]={"rt_task_1"};
//	int rt_task_prio[NO_TASK] = {5};	//set the priority for each task
//	void(*rt_task_body[NO_TASK])(void *cookie) = {rt_task_1};//task entry point

//***********************************************initialized************************************************************//

int init_module()//called when modules is loaded
{
	static int i=0;
	
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
	
	int err;
       err = rt_sem_create(&sem_desc,"MySemaphore",SEM_INIT,SEM_MODE);



        xnprintf("init...system time value  %llu \n", systime1);

//	for(i=1; i<=NO_TASK; ++i)
	 {
		//create task
                
		           ret = rt_task_create(&task_desc1,  "rt_task_1", TASK_STKSZ,5,TASK_MODE);
			   ret = rt_task_create(&task_desc2,  "rt_task_2", TASK_STKSZ,5,TASK_MODE);
			   ret = rt_task_create(&task_desc3,  "rt_task_3", TASK_STKSZ,5,TASK_MODE);
			   ret = rt_task_create(&task_desc4,  "rt_task_4", TASK_STKSZ,5,TASK_MODE);
			   ret = rt_task_create(&task_desc5,  "rt_task_5", TASK_STKSZ,5,TASK_MODE);

			if(ret<0) 
			{xnprintf("error in rt_task_create()_1...\n"); return ret;
			}
                
		//start task
                
		rt_task_start(&task_desc1,&rt_task_1,NULL);
		rt_task_start(&task_desc2,&rt_task_2,NULL);
		rt_task_start(&task_desc3,&rt_task_3,NULL);
		rt_task_start(&task_desc4,&rt_task_4,NULL);
		rt_task_start(&task_desc5,&rt_task_5,NULL);
		

		// ret = rt_task_start(rt_task_1, rt_task_body[i], NULL);
		// ret = rt_task_start(rt_task_2, rt_task_body[i], NULL);
		// ret = rt_task_start(rt_task_3, rt_task_body[i], NULL);
		// ret = rt_task_start(rt_task_4, rt_task_body[i], NULL);
		// ret = rt_task_start(rt_task_5, rt_task_body[i], NULL);
	
		printk("task started %d\n",i+1);	
			if(ret<0) 
			{	
                            xnprintf("error in rt_task_start()_1...\n"); 
                            rt_task_delete(&task_desc1); 
                 	}

	}//end of for_loop

	xnprintf("end of init_module()...\n");
	return 0;
}//end of init_module

//***************************************************************************************************************//
//*********************************************exit*****************************************************************//
void cleanup_module()//called when module is remmoved
{
//	int i;
//	for(i=0; i<NO_TASK; ++i)
	{       
               
	         ret = rt_task_delete(&task_desc1);
		 ret = rt_task_delete(&task_desc2);
		 ret = rt_task_delete(&task_desc3);
		 ret = rt_task_delete(&task_desc4);
		 ret = rt_task_delete(&task_desc5);
		 ret = rt_sem_delete(&sem_desc);
		if(ret<0 && ret!= -EINVAL) 
			xnprintf("error in rt_task_delete()_1...\n");
	}


}//end of cleanup_module

//***************************************************************************************************************//

MODULE_LICENSE("GPL"); //To access Xenomai symbols

