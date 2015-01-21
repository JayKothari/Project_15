#include<native/task.h>
#include<native/timer.h>
#include<pthread.h>
#include<native/queue.h>
#include<string.h>
#include <stdlib.h>

#include<sys/mman.h>
#include<unistd.h>
#include<stdio.h>
#include<signal.h>

#define TASK_STKSZ	4096
#define TASK_MODE	(T_FPU	|   T_CPU(0))
#define TASK_MODE1       (T_FPU  |   T_CPU(1))
#define  m_NSEC		1000000
#define QUEUE_SIZE	 1024

#define PERIOD_1	(m_NSEC*1)
#define PERIOD_2	(m_NSEC*2)
#define PERIOD_3	(m_NSEC*3)
#define PERIOD_4	(m_NSEC*4)
#define PERIOD_5	(m_NSEC*5)

#define WAKE_TIME	(m_NSEC*200)

#define ONE_SHOT_MODE	1
pthread_t thread1,thread2;
RT_QUEUE queue_desc1;

int len;
RT_TASK task_desc1,task_desc2,task_desc3,task_desc4,task_desc5;
int ret;
RTIME systime1,systime2;
void *msgin = NULL;
void *msgout1 = NULL;
//***********************************************task1**************************************************//
void rt_task_1(void *cookie)
	{
		RTIME reltime;
		RTIME task_pstart_time=0,expected_time=0,cur_sys_time=0,jitter=0;
		int i;
		char timestr[128];	
		int length=0;
		systime1=0;systime2=0;

		int err = rt_queue_bind(&queue_desc1,"MY_QUEUE", TM_INFINITE);
   		 if (err != 0)
   		 { rt_printf ("Producer: Unable to bind to queue%d\n",err); exit(-1); }
   

		systime2=rt_timer_read();

		task_pstart_time=systime2+WAKE_TIME;
	
		ret=rt_task_set_periodic(NULL,task_pstart_time,PERIOD_1);
	
		if(ret<0)
			{
			perror("error in periodic task\n");
			}
		systime1=rt_timer_read();
	
		for(i=1;i<3;++i)
		{
			expected_time = task_pstart_time  + i*PERIOD_1;
			ret=rt_task_wait_period(NULL);
			if(ret<0)
				{	perror("wait task1 error\n");
				}
			cur_sys_time = rt_timer_read();
			jitter=cur_sys_time - expected_time;
	
			printf("Time since last turn: %ld.%06ld ms\n",
                       (long)(jitter) / 1000000,
                       (long)(jitter) % 1000000);

//		printf("task1 jitter= %llu\n",jitter);
		}
//		printf("task1 jitter= %llu\n",jitter);
//		length=sizeof(jitter);
		  length = sprintf(timestr,"%llu",jitter);
         	msgout1 = rt_queue_alloc(&queue_desc1,length+1);


		if(msgout1==NULL)
		{
			perror("error in allocating queue buffer\n");
			return;			
		}
		else
		{
	//		printf("buffer is allocated successfully\n");


//			printf("length is  %d\n",length+1);

			strcpy(msgout1,timestr);
//			printf("timestr=%s\n",(char *)msgout1);
			/*
				int rt_queue_send(RT_QUEUE *q,void* mbuff,size_t size,int mode);
				return: upon success, this service returns the number of receivers
					which got awaken as a result the operation.
					if 0,no task is waiting on receiving side of the queue.
			*/

//			length=strlen(msgout1)+1;
			 err = rt_queue_send(&queue_desc1,msgout1,length+1,Q_NORMAL);
			if(err<0)
			{
				perror("error writing to queue %d\n");
		                rt_queue_free(&queue_desc1,msgout1);
				return;
			}
//			rt_queue_free(&queue_desc,msgout1);


		}
}
//**********************************************************************************************//
//***********************************task2******************************************************//
void rt_task_2(void *cookie)
{
	RTIME task_pstart_time=0,expected_time=0,cur_sys_time=0,jitter=0;
	int i;
//	int tmp_ret;
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
//		printf("task2 jitter= %llu\n",jitter);

}
//******************************************************************************************************************//
//**********************************task 3**************************************************************************//
void rt_task_3(void *cookie)
{

RTIME task_pstart_time=0,expected_time=0,cur_sys_time=0,jitter=0;
        int i;
      //  int tmp_ret;
       systime1=0;systime2=0;
        systime2 = rt_timer_read();

//       printf("task3 started at %llu\n",systime2);


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
//		printf("task3 jitter= %llu\n",jitter);


}
//******************************************************************************************************************************************************************//
//**********************************************************************task4***************************************************************************************//

void rt_task_4(void *cookie)
{
        RTIME reltime;
        RTIME task_pstart_time=0,expected_time=0,cur_sys_time=0,jitter=0;
        int i;
    //    int tmp_ret;
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
//		printf("task4 jitter= %llu\n",jitter);


}
//********************************************************************************************************************************************************************//
//*********************************************************task5******************************************************************************************************//
void rt_task_5(void *cookie)
{
                int recv=0;
                msgin = NULL;

                int  err = rt_queue_bind(&queue_desc1,"MY_QUEUE",TM_INFINITE);
                if (err) {rt_printf("Consumer: Unable to bind to queue"); exit(-1); }

                recv = rt_queue_receive(&queue_desc1,&msgin,TM_INFINITE);
                printf("recv=%d\n",recv);
                if(recv>0)
                {
                        printf("message is recevied\n");
//              length = strlen(msgin)+1;
//              printf("length r%d\n",length);
//              strcpy(message,msgin);

                        printf("message=%s\n",(const char *)msgin);

                         err=rt_queue_free(&queue_desc1,msgin);
                         if(err<0)
                                {
                                printf("error in freeing queue buff in aperiodic\n");
                                }
                }
                else
                {       perror("queue message not rececied\n");
                }
                 rt_queue_unbind(&queue_desc1);
}
//**********************************************************************************************************************************************************//
//*********************************************************thread function*********************************************************************************//
/*
void *print_message_function(void *ptr)
{		char message[10];
		int recv=0;

//		message=(char *)ptr;
//		printf("%s\n",message);

		msgin = NULL;

		int  err = rt_queue_bind(&queue_desc,"MY_QUEUE",TM_INFINITE);
		if (err) {rt_printf("Consumer: Unable to bind to queue"); exit(-1); }

		recv = rt_queue_receive(&queue_desc,&msgin,TM_INFINITE);
		printf("recv=%d\n",recv);
		if(recv>0)
		{
			printf("message is recevied\n");
//		length = strlen(msgin)+1;
//		printf("length r%d\n",length);
			strcpy(message,msgin);

			printf("message=%s\n",(const char *)msgin);

			 err=rt_queue_free(&queue_desc,msgin);
			 if(err<0)
				{
				printf("error in freeing queue buff in aperiodic\n"); 
				}
		}
		else
		{	perror("queue message not rececied\n");
		}
		 rt_queue_unbind(&queue_desc);
}	
*/
//******************************************************cleanup*****************************************************//
void cleanup()
	{
	printf("deleteing...........\n");
	ret=rt_task_delete(&task_desc1);
	ret=rt_task_delete(&task_desc2);
	ret=rt_task_delete(&task_desc3);
	ret=rt_task_delete(&task_desc4);
	ret=rt_task_delete(&task_desc5);
//	pthread_exit(NULL);
//	pthread_detach(thread1);
//	pthread_detach(thread2);
	rt_queue_delete(&queue_desc1);
	}
//*****************************************************************************************************************//
//****************************************************signal*******************************************************//
void catch_signal(int sig)
	{printf("real time task terminated\n");
	}
//*****************************************************************************************************************//

int main(int argc,char* argv[])
{
signal(SIGTERM,catch_signal);
signal(SIGINT,catch_signal);
//mlockall(MCL_CURRENT|MCL_FUTURE);
ret=0;

//int iret1,iret2;
//const char *message1="Thread 1";
//const char *message2="Thread 2";

mlockall(MCL_CURRENT|MCL_FUTURE);


	   ret = rt_queue_create(&queue_desc1,"MY_QUEUE",QUEUE_SIZE,Q_UNLIMITED,Q_FIFO|Q_SHARED );

        if(ret<0)
        {
                perror("error in message queue\n");
        }


	ret=rt_timer_set_mode(TM_ONESHOT);
//	systime1=rt_timer_read();
	
	ret=rt_task_create(&task_desc1,"rt_task_1",TASK_STKSZ,99,TASK_MODE);
	if(ret<0)
	{perror("error in create of task1\n");}
	
	ret=rt_task_create(&task_desc2,"rt_task_2",TASK_STKSZ,50,TASK_MODE);
	if(ret<0)
	{perror("error in create of task2\n");}

	ret=rt_task_create(&task_desc3,"rt_task_3",TASK_STKSZ,14,TASK_MODE);
        if(ret<0)
        {perror("error in create of task3\n");}

	ret=rt_task_create(&task_desc4,"rt_task_4",TASK_STKSZ,30,TASK_MODE);
        if(ret<0)
        {perror("error in create of task4\n");}

	ret=rt_task_create(&task_desc5,"rt_task_5",TASK_STKSZ,1,TASK_MODE);
        if(ret<0)
        {perror("error in create of task5\n");}

//	ret = rt_queue_create(&queue_desc,"MY_QUEUE",QUEUE_SIZE,Q_UNLIMITED,Q_FIFO );
//	
  //      if(ret<0)
//	{
//		perror("error in message queue\n");
//	}


	rt_task_start(&task_desc1,&rt_task_1,NULL);
	rt_task_start(&task_desc2,&rt_task_2,NULL);
	rt_task_start(&task_desc3,&rt_task_3,NULL);
	rt_task_start(&task_desc4,&rt_task_4,NULL);
        rt_task_start(&task_desc5,&rt_task_5,NULL);
	
//	 printf ("Joining\n");
//   	 rt_task_join(&task_desc1);
	
//	iret1=pthread_create(&thread1,NULL,print_message_function,(void*) message1);
//	iret1=pthread_create(&thread2,NULL,print_message_function,(void*) message2);
	
	pause();
	
	cleanup();
return 0;
}

