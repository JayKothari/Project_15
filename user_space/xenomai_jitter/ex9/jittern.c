#include<native/task.h>
#include<native/timer.h>
#include <stdlib.h>
#include <string.h>
#include<sys/mman.h>
#include<unistd.h>
#include<stdio.h>
#include<signal.h>

#define TASK_STKSZ	4096
#define TASK_MODE	(T_FPU	|   T_CPU(0))
#define TASK_MODE1       (T_FPU  |   T_CPU(1))
#define  m_NSEC		1000000

#define PERIOD_1	(m_NSEC)
#define PERIOD_2	(m_NSEC*5)
#define PERIOD_3	(m_NSEC*5)
#define PERIOD_4	(m_NSEC*5)
#define PERIOD_5	(m_NSEC*5)

#define WAKE_TIME	(m_NSEC*200)

#define ONE_SHOT_MODE	1

RT_TASK task_desc[50];
static int value=0;
int ret;
RTIME systime1=0,systime2=0;
//***********************************************task1**************************************************//
void rt_task_1(void *cookie)
{
//	printf("task 1 processing...........\n");
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
	
		for(i=1;i<2;++i)
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
//		printf("task1 ended.\n");


}
//**********************************************************************************************//
//***********************************task2******************************************************//
void rt_task_2(void *cookie)
{
//	printf("task 2 processing........\n");
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

	
		for(i=1;i<2;++i)
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
//		printf("task 2 ended\n");

}
//******************************************************************************************************************//
//**********************************task 3**************************************************************************//
void rt_task_3(void *cookie)
{
//printf("task 3 processing......\n"); 
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


                for(i=1;i<2;++i)
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
//		printf("task 3 ended\n");


}
//******************************************************************************************************************************************************************//
//**********************************************************************task4***************************************************************************************//

void rt_task_4(void *cookie)
{
//	printf("task 4 processing......\n");
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

                for(i=1;i<2;++i)
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
//		printf("task 4 ended\n");


}
//********************************************************************************************************************************************************************//
//*********************************************************task5******************************************************************************************************//
void rt_task_5(void *cookie)
{
//	printf("task 5 processing\n");
        RTIME reltime;
        RTIME task_pstart_time=0,expected_time=0,cur_sys_time=0,jitter=0;
        int i;
        int tmp_ret;
        systime1=0;systime2=0;
        systime2=rt_timer_read();
        task_pstart_time=systime2+WAKE_TIME;
         
                                ret=rt_task_set_periodic(NULL,task_pstart_time,PERIOD_5);
        
                if(ret<0)
                {
                perror("error in periodic task\n");
                }
        systime1=rt_timer_read();
                for(i=1;i<2;++i)
                {
                expected_time = task_pstart_time  + i*PERIOD_5;
                ret=rt_task_wait_period(NULL);
                        if(ret<0)
                        {perror("wait task1 error\n");
                        }
                cur_sys_time = rt_timer_read();
                jitter=cur_sys_time - expected_time;
                }
		printf("task5 jitter= %llu\n",jitter);
//		printf("task 5 ended \n");
}

void rt_task_6(void *cookie)
{
//	printf("task 6 processing.........\n");
        RTIME reltime;
        RTIME task_pstart_time=0,expected_time=0,cur_sys_time=0,jitter=0;
        int i;
        int tmp_ret;
        systime1=0;systime2=0;
        systime2=rt_timer_read();
        task_pstart_time=systime2+WAKE_TIME;

                                ret=rt_task_set_periodic(NULL,task_pstart_time,PERIOD_5);

                if(ret<0)
                {
                perror("error in periodic task\n");
                }
        systime1=rt_timer_read();
                for(i=1;i<2;++i)
                {
                expected_time = task_pstart_time  + i*PERIOD_5;
                ret=rt_task_wait_period(NULL);
                        if(ret<0)
                        {perror("wait task1 error\n");
                        }
                cur_sys_time = rt_timer_read();
                jitter=cur_sys_time - expected_time;
                }
                printf("task6 jitter= %llu\n",jitter);
//		printf("task 6 ended\n");
}

void rt_task_7(void *cookie)
{
//	printf("task 7 processing.......\n");
        RTIME reltime;
        RTIME task_pstart_time=0,expected_time=0,cur_sys_time=0,jitter=0;
        int i;
        int tmp_ret;
        systime1=0;systime2=0;
        systime2=rt_timer_read();
        task_pstart_time=systime2+WAKE_TIME;

                                ret=rt_task_set_periodic(NULL,task_pstart_time,PERIOD_5);

                if(ret<0)
                {
                perror("error in periodic task\n");
                }
        systime1=rt_timer_read();
                for(i=1;i<2;++i)
                {
                expected_time = task_pstart_time  + i*PERIOD_5;
                ret=rt_task_wait_period(NULL);
                        if(ret<0)
                        {perror("wait task1 error\n");
                        }
                cur_sys_time = rt_timer_read();
                jitter=cur_sys_time - expected_time;
                }
                printf("task7 jitter= %llu\n",jitter);
//		printf("task 7 ended.\n");
}

void rt_task_8(void *cookie)
{
//	printf("task 8 processing......\n");
        RTIME reltime;
        RTIME task_pstart_time=0,expected_time=0,cur_sys_time=0,jitter=0;
        int i;
        int tmp_ret;
        systime1=0;systime2=0;
        systime2=rt_timer_read();
        task_pstart_time=systime2+WAKE_TIME;

                                ret=rt_task_set_periodic(NULL,task_pstart_time,PERIOD_5);

                if(ret<0)
                {
                perror("error in periodic task\n");
                }
        systime1=rt_timer_read();
                for(i=1;i<2;++i)
                {
                expected_time = task_pstart_time  + i*PERIOD_5;
                ret=rt_task_wait_period(NULL);
                        if(ret<0)
                        {perror("wait task1 error\n");
                        }
                cur_sys_time = rt_timer_read();
                jitter=cur_sys_time - expected_time;
                }
                printf("task8 jitter= %llu\n",jitter);
//		printf("task 8 ended.\n");
}

void rt_task_9(void *cookie)
{
//	printf("task 9 processing.......\n");
        RTIME reltime;
        RTIME task_pstart_time=0,expected_time=0,cur_sys_time=0,jitter=0;
        int i;
        int tmp_ret;
        systime1=0;systime2=0;
        systime2=rt_timer_read();
        task_pstart_time=systime2+WAKE_TIME;

                                ret=rt_task_set_periodic(NULL,task_pstart_time,PERIOD_5);

                if(ret<0)
                {
                perror("error in periodic task\n");
                }
        systime1=rt_timer_read();
                for(i=1;i<2;++i)
                {
                expected_time = task_pstart_time  + i*PERIOD_5;
                ret=rt_task_wait_period(NULL);
                        if(ret<0)
                        {perror("wait task1 error\n");
                        }
                cur_sys_time = rt_timer_read();
                jitter=cur_sys_time - expected_time;
                }
                printf("task9 jitter= %llu\n",jitter);
//		printf("task 9 ended.\n");
}

void rt_task_10(void *cookie)
{
//	printf("task 10 processing......\n");
        RTIME reltime;
        RTIME task_pstart_time=0,expected_time=0,cur_sys_time=0,jitter=0;
        int i;
        int tmp_ret;
        systime1=0;systime2=0;
        systime2=rt_timer_read();
        task_pstart_time=systime2+WAKE_TIME;

                                ret=rt_task_set_periodic(NULL,task_pstart_time,PERIOD_5);

                if(ret<0)
                {
                perror("error in periodic task\n");
                }
        systime1=rt_timer_read();
                for(i=1;i<2;++i)
                {
                expected_time = task_pstart_time  + i*PERIOD_5;
                ret=rt_task_wait_period(NULL);
                        if(ret<0)
                        {perror("wait task1 error\n");
                        }
                cur_sys_time = rt_timer_read();
                jitter=cur_sys_time - expected_time;
                }
                printf("task10 jitter= %llu\n",jitter);

//		printf("task 10 ended\n");
}

int rt_task_prio[100] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80};


/*
rt_task_name is arrgument to rt_task_create.There should be diffrent name of each task created.But more than one task can call function at same time ....i.e. rt_task_1,rt_task_2 etc
*/
char *rt_task_name[100]={"rt_task_1","rt_task_2","rt_task_3","rt_task_4","rt_task_5","rt_task_6","rt_task_7","rt_task_8","rt_task_9","rt_task_10","rt_task_11","rt_task_12","rt_task_13","rt_task_14","rt_task_15","rt_task_16","rt_task_17","rt_task_18","rt_task_19","rt_task_20","rt_task_21","rt_task_22","rt_task_23","rt_task_24","rt_task_25","rt_task_26","rt_task_27","rt_task_28","rt_task_29","rt_task_30","rt_task_31","rt_task_32","rt_task_33","rt_task_34","rt_task_35","rt_task_36","rt_task_37","rt_task_38","rt_task_39","rt_task_40","rt_task_41","rt_task_42","rt_task_43","rt_task_44","rt_task_45","rt_task_46","rt_task_47","rt_task_48","rt_task_49","rt_task_50","rt_task_51","rt_task_52","rt_task_53","rt_task_54","rt_task_55","rt_task_56","rt_task_57","rt_task_58","rt_task_59","rt_task_60","rt_task_61","rt_task_62","rt_task_63","rt_task_64","rt_task_65","rt_task_66","rt_task_67","rt_task_68","rt_task_69","rt_task_70","rt_task_71","rt_task_72","rt_task_73","rt_task_74","rt_task_75","rt_task_76","rt_task_77","rt_task_78","rt_task_79","rt_task_80"};
void(*rt_task_body[100])(void *cookie) = {rt_task_1,rt_task_2,rt_task_3,rt_task_4,rt_task_5,rt_task_6,rt_task_7,rt_task_8,rt_task_9,rt_task_10,rt_task_1,rt_task_2,rt_task_3,rt_task_4,rt_task_5,rt_task_6,rt_task_7,rt_task_8,rt_task_9,rt_task_10};
void cleanup()
{
                int i=0;
                for(i<0;i<value;++i)
                {
                ret=rt_task_delete(&task_desc[i]);
                }
}

void catch_signal(int sig)
{		printf("real time task terminated\n");
}
int main(int argc,char* argv[])
{
		
		int i;
		signal(SIGTERM,catch_signal);
		signal(SIGINT,catch_signal);
		mlockall(MCL_CURRENT|MCL_FUTURE);
		ret=0;

		if(argc<2)
		{printf("need to enter argument\n");
		}
	
		value=atoi(argv[1]);
		printf("val=%d\n",value);
		
		ret=rt_timer_set_mode(TM_ONESHOT);

	for(i=0;i<value;i++)
	{
		ret=rt_task_create(&task_desc[i],rt_task_name[i],TASK_STKSZ,i,TASK_MODE);
		if(ret<0)
		{printf("error in create of task%d\n",i);}
	}

	for(i=0;i<value;i++)
	{
//		rt_task_start(&task_desc[i],rt_task_body[i],NULL);
		rt_task_start(&task_desc[i],&rt_task_1,NULL);
	}
	
	pause();
	
	cleanup();

return 0;
}
