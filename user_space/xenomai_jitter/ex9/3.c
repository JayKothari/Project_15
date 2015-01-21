#include<native/task.h>
#include<native/timer.h>

#include<sys/mman.h>
#include<unistd.h>
#include<stdio.h>
#include<signal.h>

#define TASK_STKSZ	4096
#define TASK_MODE	(T_FPU	|   T_CPU(0))
#define TASK_MODE1       (T_FPU  |   T_CPU(1))
#define  m_NSEC		1000000

#define PERIOD_1	(m_NSEC*1)
#define PERIOD_2	(m_NSEC*2)
#define PERIOD_3	(m_NSEC*3)
#define PERIOD_4	(m_NSEC*4)
#define PERIOD_5	(m_NSEC*5)

#define WAKE_TIME	(m_NSEC*200)

#define ONE_SHOT_MODE	1

RT_TASK task_desc1,task_desc2,task_desc3,task_desc4,task_desc5;
int ret;
RTIME systime1=0,systime2=0;
//***********************************************task1**************************************************//
void rt_task_1(void *cookie)
{
	RTIME reltime;
	RTIME task_pstart_time=0,expected_time=0,cur_sys_time=0,jitter=0;
	int i;
	int tmp_ret;
	systime1=0;systime2=0;
	systime2=rt_timer_read();
//	systime1=rt_timer_ticks2ns(systime2);

//	printf("task1 started at %llu\n",systime2);

	task_pstart_time=systime2+WAKE_TIME;
	
				ret=rt_task_set_periodic(NULL,task_pstart_time,PERIOD_1);
	
		if(ret<0)
		{
		perror("error in periodic task\n");
		}
	systime1=rt_timer_read();

//	printf("task1 after periodic at %llu\n",systime1);
	
		for(i=1;i<3;++i)
		{
		expected_time = task_pstart_time  + i*PERIOD_1;
		ret=rt_task_wait_period(NULL);
			if(ret<0)
			{perror("wait task1 error\n");
			}
		cur_sys_time = rt_timer_read();
		jitter=cur_sys_time - expected_time;
//		printf("task1 jitter= %llu\n",jitter);
		}
		printf("task1 jitter= %llu\n",jitter);


}
//**********************************************************************************************//
//***********************************task2******************************************************//
void rt_task_2(void *cookie)
{
	RTIME task_pstart_time=0,expected_time=0,cur_sys_time=0,jitter=0;
	int i;
	int tmp_ret;
	systime1=0;systime2=0;
	systime2 = rt_timer_read();

//	printf("task2 started at %llu\n",systime2);

	
	task_pstart_time = systime2 + WAKE_TIME;
	
				ret= rt_task_set_periodic(NULL,task_pstart_time,PERIOD_2);
	
	systime1=rt_timer_read();

//	printf("task2 after periodic at %llu\n",systime1);

	
		for(i=1;i<3;++i)
		{
		expected_time = task_pstart_time + i*PERIOD_2;
		ret=rt_task_wait_period(NULL);
			if(ret<0)
			{perror("task wait 2 error\n");
			ret=0;
			}
	 	cur_sys_time = rt_timer_read();
		jitter=cur_sys_time - expected_time;
//	        printf("task2 jitter= %llu\n",jitter);
		}
		printf("task2 jitter= %llu\n",jitter);

}
//******************************************************************************************************************//
//**********************************task 3**************************************************************************//
void rt_task_3(void *cookie)
{

RTIME task_pstart_time=0,expected_time=0,cur_sys_time=0,jitter=0;
        int i;
        int tmp_ret;
        systime1=0;systime2=0;
        systime2 = rt_timer_read();

  //      printf("task3 started at %llu\n",systime2);


        task_pstart_time = systime2 + WAKE_TIME;

                                ret= rt_task_set_periodic(NULL,task_pstart_time,PERIOD_3);

        systime1=rt_timer_read();

    //    printf("task3 after periodic at %llu\n",systime1);


                for(i=1;i<3;++i)
                {
                expected_time = task_pstart_time + i*PERIOD_3;
                ret=rt_task_wait_period(NULL);
                        if(ret<0)
                        {perror("task wait 2 error\n");
                        ret=0;
                        }
                cur_sys_time = rt_timer_read();
                jitter=cur_sys_time - expected_time;
//		 printf("task3 jitter= %llu\n",jitter);
                }
		printf("task3 jitter= %llu\n",jitter);


}
//******************************************************************************************************************************************************************//
//**********************************************************************task4***************************************************************************************//

void rt_task_4(void *cookie)
{
        RTIME reltime;
        RTIME task_pstart_time=0,expected_time=0,cur_sys_time=0,jitter=0;
        int i;
        int tmp_ret;
        systime1=0;systime2=0;
        systime2=rt_timer_read();
//      systime1=rt_timer_ticks2ns(systime2);
      
//        printf("task4 started at %llu\n",systime2);

        task_pstart_time=systime2+WAKE_TIME;
         
                                ret=rt_task_set_periodic(NULL,task_pstart_time,PERIOD_4);
        
                if(ret<0)
                {
                perror("error in periodic task\n");
                }
        systime1=rt_timer_read();

  //      printf("task4 after periodic at %llu\n",systime1);

                for(i=1;i<3;++i)
                {
                expected_time = task_pstart_time  + i*PERIOD_4;
                ret=rt_task_wait_period(NULL);
                        if(ret<0)
                        {perror("wait task1 error\n");
                        }
                cur_sys_time = rt_timer_read();
                jitter=cur_sys_time - expected_time;
//		 printf("task4 jitter= %llu\n",jitter);
                }
		printf("task4 jitter= %llu\n",jitter);


}
//********************************************************************************************************************************************************************//
//*********************************************************task5******************************************************************************************************//
void rt_task_5(void *cookie)
{
        RTIME reltime;
        RTIME task_pstart_time=0,expected_time=0,cur_sys_time=0,jitter=0;
        int i;
        int tmp_ret;
        systime1=0;systime2=0;
        systime2=rt_timer_read();
//      systime1=rt_timer_ticks2ns(systime2);
      
//        printf("task5 started at %llu\n",systime2);

        task_pstart_time=systime2+WAKE_TIME;
         
                                ret=rt_task_set_periodic(NULL,task_pstart_time,PERIOD_5);
        
                if(ret<0)
                {
                perror("error in periodic task\n");
                }
        systime1=rt_timer_read();

//        printf("task5 after periodic at %llu\n",systime1);

                for(i=1;i<3;++i)
                {
                expected_time = task_pstart_time  + i*PERIOD_5;
                ret=rt_task_wait_period(NULL);
                        if(ret<0)
                        {perror("wait task1 error\n");
                        }
                cur_sys_time = rt_timer_read();
                jitter=cur_sys_time - expected_time;
//		 printf("task5 jitter= %llu\n",jitter);
                }
		printf("task5 jitter= %llu\n",jitter);
}

void cleanup()
{
ret=rt_task_delete(&task_desc1);
ret=rt_task_delete(&task_desc2);
ret=rt_task_delete(&task_desc3);
ret=rt_task_delete(&task_desc4);
ret=rt_task_delete(&task_desc5);
}
void catch_signal(int sig)
{printf("real time task terminated\n");
}
int main(int argc,char* argv[])
{
signal(SIGTERM,catch_signal);
signal(SIGINT,catch_signal);
mlockall(MCL_CURRENT|MCL_FUTURE);
ret=0;
	
	ret=rt_timer_set_mode(TM_ONESHOT);
//	systime1=rt_timer_read();
	
	ret=rt_task_create(&task_desc1,"rt_task_1",TASK_STKSZ,1,TASK_MODE);
	if(ret<0)
	{perror("error in create of task1\n");}
	
	ret=rt_task_create(&task_desc2,"rt_task_2",TASK_STKSZ,20,TASK_MODE);
	if(ret<0)
	{perror("error in create of task2\n");}

	ret=rt_task_create(&task_desc3,"rt_task_3",TASK_STKSZ,34,TASK_MODE);
        if(ret<0)
        {perror("error in create of task3\n");}

	ret=rt_task_create(&task_desc4,"rt_task_4",TASK_STKSZ,40,TASK_MODE);
        if(ret<0)
        {perror("error in create of task4\n");}

	ret=rt_task_create(&task_desc5,"rt_task_5",TASK_STKSZ,99,TASK_MODE);
        if(ret<0)
        {perror("error in create of task5\n");}

	rt_task_start(&task_desc1,&rt_task_1,NULL);
	rt_task_start(&task_desc2,&rt_task_2,NULL);
	rt_task_start(&task_desc3,&rt_task_3,NULL);
	rt_task_start(&task_desc4,&rt_task_4,NULL);
        rt_task_start(&task_desc5,&rt_task_5,NULL);

	pause();
	
	cleanup();
return 0;
}
