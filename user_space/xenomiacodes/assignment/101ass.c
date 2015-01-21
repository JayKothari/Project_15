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

#define 	SEM_INIT 	0 /* Initial semaphore count */
#define 	SEM_MODE 	S_PRIO /* Wait by FIFO order */
RT_TASK task_desc1,task_desc2,task_desc3,task_desc4,task_desc5;

RT_SEM sem_desc;
int ret;
int i=0;
RTIME tmp=0;
RTIME systime1=0,systime2=0;


//*************************************************************task1*************************************************************************************************//
void rt_task_1(void *cookie)
{	 rt_sem_p(&sem_desc,TM_INFINITE);
//	i=0;
//	while(i==10)
	{xnprintf("task 1 is processing.............\n");
//	i++;
	}
	xnprintf("exiting rt_task_1()...\n");
	rt_sem_v(&sem_desc);
}//end of rt_task_1 function
//*******************************************************************************************************************************************************************//
//*************************************************************task2************************************************************************************************//

void rt_task_2(void *cookie)
{	rt_sem_p(&sem_desc,TM_INFINITE);
//	i=0;   
//	while(i==10)
	{xnprintf("task 2 is processing................\n");
//	i++;
	}
        xnprintf("exiting rt_task_2()...\n");
	rt_sem_v(&sem_desc);
}//end of rt_task_2 function
//********************************************************************************************************************************************************//

//**************************************************************task3*************************************************************************************//
void rt_task_3(void *cookie)
{	rt_sem_p(&sem_desc,TM_INFINITE);
//	i=0;
  //     while(i==10)
	{xnprintf("task 3 is processing...................\n");
//	i++;
	}     
        xnprintf("exiting rt_task_3()...\n");
	rt_sem_v(&sem_desc);
}//end of rt_task_3 function



void rt_task_4(void *cookie)
{	rt_sem_p(&sem_desc,TM_INFINITE);
//	i=0;
//	while(i==10)
	{printk("task 4 is processing..................\n");
//	i++;
	}

       xnprintf("exiting rt_task_4()...\n");
	rt_sem_v(&sem_desc);
}//end of rt_task_4 function
//****************************************************************************************************************************************************************//

//***************************************************************task5*********************************************************************************************//
void rt_task_5(void *cookie)
{	int k=0;
	rt_sem_v(&sem_desc);
	
	rt_sem_p(&sem_desc,TM_INFINITE);
  //    	i=0;
//	 while(i==10)
	{printk("task 5 is processing.....................\n");
//	i++;
	}
	  xnprintf("exiting rt_task_5()...\n");
   
        k=rt_sem_broadcast(&sem_desc);                                    //	 int rt_sem_broadcast (RT_SEM ∗ sem)
	if(k<0)
	{xnprintf("error is broadcast");
	}
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
	 
		//create task
                
		           ret = rt_task_create(&task_desc1,  "rt_task_1", TASK_STKSZ,5,TASK_MODE);
			   ret = rt_task_create(&task_desc2,  "rt_task_2", TASK_STKSZ,4,TASK_MODE);
			   ret = rt_task_create(&task_desc3,  "rt_task_3", TASK_STKSZ,3,TASK_MODE);
			   ret = rt_task_create(&task_desc4,  "rt_task_4", TASK_STKSZ,2,TASK_MODE);
			   ret = rt_task_create(&task_desc5,  "rt_task_5", TASK_STKSZ,1,TASK_MODE);

			if(ret<0) 
			{xnprintf("error in rt_task_create()_1...\n"); return ret;
			}
                
		//start task
               int ret1,ret2,ret3,ret4,ret5;
 
		ret1=rt_task_start(&task_desc1,&rt_task_1,NULL);
		ret2=rt_task_start(&task_desc2,&rt_task_2,NULL);
		ret3=rt_task_start(&task_desc3,&rt_task_3,NULL);
		ret4=rt_task_start(&task_desc4,&rt_task_4,NULL);
		ret5=rt_task_start(&task_desc5,&rt_task_5,NULL);
	
		if(ret1==0)
		{printk("task started 1");
		}		

		if(ret2==0)
                {printk("task started 2");
                } 

		if(ret3==0)
                {printk("task started 3");
                } 		

		if(ret4==0)
                {printk("task started 4");
                } 

		if(ret5==0)
                {printk("task started 5");
                } 
	//	printk("task started %d\n",i+1);	
			if(ret<0) 
			{	
                            xnprintf("error in rt_task_start()_1...\n"); 
                            rt_task_delete(&task_desc1); 
            		}

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

