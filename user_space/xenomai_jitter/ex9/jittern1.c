#include<native/task.h>
#include<native/timer.h>
#include<native/pipe.h>

#include<stdlib.h>
#include<string.h>

#include<sys/mman.h>

#include<unistd.h>
#include<stdio.h>
#include<signal.h>

#define PIPE_MINOR	 0
#define TASK_STKSZ	4096
#define TASK_MODE	(T_FPU	|   T_CPU(0))
#define TASK_MODE1      (T_FPU  |   T_CPU(1))
#define  m_NSEC		1000000

#define PERIOD_1	(m_NSEC*1)
#define PERIOD_2	(m_NSEC*2)
#define PERIOD_3	(m_NSEC*3)
#define PERIOD_4	(m_NSEC*4)
#define PERIOD_5	(m_NSEC*5)

#define WAKE_TIME	(m_NSEC*200)

#define ONE_SHOT_MODE	1

RT_TASK task_desc[50];

RT_PIPE pipe_desc;
static int value=0;
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
        task_pstart_time=systime2+WAKE_TIME;
         
                                ret=rt_task_set_periodic(NULL,task_pstart_time,PERIOD_5);
        
                if(ret<0)
                {
                perror("error in periodic task\n");
                }
        systime1=rt_timer_read();
                for(i=1;i<3;++i)
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
}

void rt_task_6(void *cookie)
{
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
                for(i=1;i<3;++i)
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
}

void rt_task_7(void *cookie)
{
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
                for(i=1;i<3;++i)
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
}

void rt_task_8(void *cookie)
{
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
                for(i=1;i<3;++i)
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
}

void rt_task_9(void *cookie)
{
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
                for(i=1;i<3;++i)
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
}



void rt_task_10(void *cookie)
{
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
                for(i=1;i<3;++i)
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
}

int rt_task_prio[100] = {99,89,79,69,59,49,39,29,19,9,5,1};


/*
rt_task_name is arrgument to rt_task_create.There should be diffrent name of each task created.But more than one task can call function at same time ....i.e. rt_task_1,rt_task_2 etc
*/
char *rt_task_name[100]={"rt_task_1","rt_task_2","rt_task_3","rt_task_4","rt_task_5","rt_task_6","rt_task_7","rt_task_8","rt_task_9","rt_task_10"};
void(*rt_task_body[100])(void *cookie) = {rt_task_1,rt_task_2,rt_task_3,rt_task_4,rt_task_5,rt_task_6,rt_task_7,rt_task_8,rt_task_9,rt_task_10};
void cleanup()
{
                int i=0;
                for(i<0;i<value;++i)
                {
                ret=rt_task_delete(&task_desc[i]);
                }
		rt_pipe_delete(&pipe_desc);
}

void catch_signal(int sig)
{		printf("real time task terminated\n");
}
int main(int argc,char* argv[])
{
		mlockall(MCL_CURRENT|MCL_FUTURE);		
		int i,err;
		RT_PIPE_MSG  *msgout, *msgin;
		signal(SIGTERM,catch_signal);
		signal(SIGINT,catch_signal);

		ret=0;

		if(argc<2)
		{printf("need to enter argument\n");
		}
	
		value=atoi(argv[1]);
		printf("val=%d\n",value);
		
		int len=sizeof(argv[1]);
		printf("len=%d\n",len);
		msgout = rt_pipe_alloc(&pipe_desc,len);
		if (!msgout)
		{perror("allocation problem\n");
		}

		ret=rt_timer_set_mode(TM_ONESHOT);

		err = rt_pipe_create(&pipe_desc, "rt_pipe_jit", PIPE_MINOR,1024);
		if(err<0)
		{	prror("pipe creation error\n");
		}


		for(i=0;i<value;i++)
		{
			ret=rt_task_create(&task_desc[i],rt_task_name[i],TASK_STKSZ,rt_task_prio[i],TASK_MODE);
			if(ret<0)
			{printf("error in create of task%d\n",i);}
		}

		for(i=0;i<value;i++)
		{
			rt_task_start(&task_desc[i],rt_task_body[i],NULL);
		}

	strcpy(msgout, argv[1]);

	rt_pipe_send(&pipe_desc, msgout, len, P_NORMAL);

	err=rt_pipe_receive(&pipe_desc, &msgin, TM_INFINITE);
		if(err<0)
		{	printf("error in receive\n");
		}
	printf("msgin=%s\n",msgin);

	rt_pipe_free(&pipe_desc, msgin);

	pause();

	cleanup();

return 0;
}
