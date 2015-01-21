#include<stdio.h>
#include<signal.h>
#include<unistd.h>
#include<sys/mman.h>
//#include <native/sem.h>

#include<native/task.h>
#include<native/timer.h>
#include<native/pipe.h>

#define TASK_MODE       (T_FPU|T_CPU(0))
#define TASK_STKSZ      4096
#define u_NSEC          1000000

#define SETUP_TICK_NSEC         1000000

#define PERIOD_1        (u_NSEC*1)
#define PERIOD_2        (u_NSEC*2)
#define PERIOD_3        (u_NSEC*3)
#define PERIOD_4        (u_NSEC*4)
#define PERIOD_5        (u_NSEC*5)

#define ONE_SHOT_MODE   1

//#define         SEM_INIT        1 /* Initial semaphore count */
//#define         SEM_MODE        S_FIFO 

RT_TASK task_desc1,task_desc2,task_desc3,task_desc4,task_desc5;

//RT_SEM sem_desc;

int i=0;
static RTIME exp=0;
void rt_task_1(void *cookie)
{	unsigned long overrun1_t;
	int ret;
//	rt_sem_p(&sem_desc,TM_INFINITE);

//        printf("\t task 1 started \t\n");
	int err;
	RTIME systime1=0,systime2=0,expected=0;	
	systime2= rt_timer_read();                    
  //      printf("initial task1 system time ....value  %llu \n", systime2);
	ret = rt_task_set_periodic(&task_desc1, TM_NOW, PERIOD_1);
	if(ret<0)
	{ 
	printf("error1 in rt_task_set_periodic()  %d ...\n",ret); return ;
	}
	
	expected= systime2 + PERIOD_2;
//	while(1)
	{
	err=rt_task_wait_period(&overrun1_t);        
//	printf("overrun1=%lu\n",overrun1_t); 
	if(err<0)
	{perror("task wait error\n");
	}
	systime1 = rt_timer_read();   
	exp=systime1;                  
//	printf("actual system time after first task1 wake-up....value %llu \n",systime1);
//	printf(" task1 time%lld \n", systime1-systime2);

//	printf("jitter after task1.....%lld\n",systime1-expected);
	expected=systime1;
	}
//	printf("\ttask 1 ended\t\n");	
														     
//	  rt_sem_v(&sem_desc);
	
}

void rt_task_2(void *cookie)
{
int overrun2_t;
int ret,ret1;		    	
//rt_sem_p(&sem_desc,TM_INFINITE);

	printf("\t task 2 started  \t\n");
        int err,i;
        RTIME systime1=0,systime2=0,expected=0;
        systime2= rt_timer_read();
//        printf("initial task1 system time ....value  %llu \n", systime2);
	
        ret1 = rt_task_set_periodic(&task_desc1, exp, PERIOD_2);
        if(ret<0)
        {
        printf("error2 in rt_task_set_periodic()  %d ...\n",ret); return ;
        }
	        expected= exp +PERIOD_2;
	        err=rt_task_wait_period(&overrun2_t);
		printf("overrun2=%lu\n",overrun2_t);

	        if(err<0)
	        {perror("task 2 wait error\n");
	        }
	
        systime1 = rt_timer_read();
//        printf("actual system time after first task1 wake-up....value %llu \n",systime1);
   //     printf(" task1 time%lld \n", systime1-systime2);
 //       printf("jitter after task2.....%lld\n",systime1-expected);
	
//	printf("\ttask 2 ended\t\n");
//	  rt_sem_v(&sem_desc);

}

/*
void rt_task_3(void *cookie)
{
	unsigned long overrun3_t;
	int ret;

	printf("\t task 3 started  \t\n");

        int err,i;

        RTIME systime1=0,systime2=0,expected=0;
        systime2= rt_timer_read();
 //       printf("initial task1 system time ....value  %llu \n", systime2);


        ret = rt_task_set_periodic(&task_desc1, TM_NOW, PERIOD_3);
        if(ret<0)
        {
        printf("error3 in rt_task_set_periodic()  %d ...\n",ret); return ;
        }
	
	
        expected= systime2 +PERIOD_3;
        err=rt_task_wait_period(&overrun3_t);
	printf("overrun=%lu\n",overrun3_t);
        if(err<0)
        {perror("wait error 3\n");
        }
        systime1 = rt_timer_read();
 //       printf("actual system time after first task1 wake-up....value %llu \n",systime1);
        printf(" task3 time%lld \n", systime1-systime2);
        printf("jitter after task3.....%lld\n",systime1-expected);

	
	printf("\ttask 3 ended\t\n");
//	  rt_sem_v(&sem_desc);
}


void rt_task_4(void *cookie)
{
	unsigned long overrun4_t;
	int ret;
	printf("\t task 4 started\t\n");
        int err;
        RTIME systime1=0,systime2=0,expected=0;
        systime2= rt_timer_read();
//        printf("initial task1 system time ....value  %llu \n", systime2);
        ret = rt_task_set_periodic(&task_desc1, TM_NOW, PERIOD_1);
        if(ret<0)
        {
        printf("error4 in rt_task_set_periodic()  %d ...\n",ret); return ;
        }
        expected= systime2 + PERIOD_4;
        err=rt_task_wait_period(&overrun4_t);
	printf("overrun4=%lu\n",overrun4_t);
        if(err<0)
        {//printf("error4 in rt_task_wait_period%d\n",err);
	-ETIMEDOUT ;
        }
        systime1 = rt_timer_read();
//        printf("actual system time after first task1 wake-up....value %llu \n",systime1);
        printf(" task4 time%lld \n", systime1-systime2);
        printf("jitter after task4.....%lld\n",systime1-expected);

printf("\ttask 4 ended\t\n");
}

void rt_task_5(void *cookie)
{
	int ret;
	unsigned long overrun5_t;
//	 rt_sem_p(&sem_desc,TM_INFINITE);


	printf("\t task 5 started \t\n");

        int err;

        RTIME systime1=0,systime2=0,expected=0;



        systime2= rt_timer_read();
//        printf("initial task1 system time ....value  %llu \n", systime2);


        ret = rt_task_set_periodic(&task_desc1, TM_NOW, PERIOD_5);
        if(ret<0)
        {
        printf("error5 in rt_task_set_periodic()  %d ...\n",ret); return ;
        }

        expected= systime2 + PERIOD_5;
        err=rt_task_wait_period(&overrun5_t);
	printf("overrun5_t=%lu\n",overrun5_t);
        if(err<0)
        {printf("error5 in rt_task_wait_period%d\n",err);
        }
        systime1 = rt_timer_read();
  //      printf("actual system time after first task1 wake-up....value %llu \n",systime1);
        printf(" task5 time%lld \n", systime1-systime2);
        printf("jitter after task5.....%lld\n",systime1-expected);

printf("\ttask 5 ended\t\n");
//	  rt_sem_v(&sem_desc);

}
*/

void catch_signal(int sig)
{printf("task is completed\n");
}
int main(int argc,char* argv[])
{
	int ret;
	signal(SIGTERM,catch_signal);
	signal(SIGINT,catch_signal);

	mlockall(MCL_CURRENT | MCL_FUTURE);

//	int err = rt_sem_create(&sem_desc,"MySemaphore",SEM_INIT,SEM_MODE);
//        if(err<0)
//	{printf("semaphore not created\n");
//	}

#if ONE_SHOT_MODE == 1
	        ret = rt_timer_set_mode(TM_ONESHOT);//sets timer MODE
		printf("timer base in one-shot mode  ***********");
#else
		ret = rt_timer_set_mode(SETUP_TICK_NSEC);
		printf("timer base in periodic mode  *********");
#endif

//	ret=rt_task_create(&task_desc1,"rt_task_1",TASK_STKSZ,5,TASK_MODE);
//	ret=rt_task_create(&task_desc2,"rt_task_2",TASK_STKSZ,4,TASK_MODE);
//	ret=rt_task_create(&task_desc3,"rt_task_3",TASK_STKSZ,3,TASK_MODE);
//	ret=rt_task_create(&task_desc4,"rt_task_4",TASK_STKSZ,2,TASK_MODE);
//	ret=rt_task_create(&task_desc5,"rt_task_5",TASK_STKSZ,1,TASK_MODE);
	


//	printf("real time task is created\n");
	 ret=rt_task_create(&task_desc1,"rt_task_1",TASK_STKSZ,5,TASK_MODE);
	rt_task_start(&task_desc1,&rt_task_1,NULL);
//	printf("************************************task 1 started*******************************************\n");	
//	   ret=rt_task_suspend(&task_desc2);
  //       if(ret<0)
    //             perror("suspend error\n");
      //  ret=rt_task_suspend(&task_desc3);
//         if(ret<0)
  //               perror("suspend error\n");
    //    ret=rt_task_suspend(&task_desc4);
      //   if(ret<0)
//                 perror("suspend error\n");
  //      ret=rt_task_suspend(&task_desc5);
    //     if(ret<0)
      //           perror("suspend error\n");

//	pause();
//	  ret = rt_task_delete(&task_desc1);
//	ret=rt_task_resume(&task_desc2);
  //      if(ret<0)
    //             perror("suspend error\n");
  //      ret=rt_task_resume(&task_desc3);
 //       if(ret<0)
   //              perror("suspend error\n");
//        ret=rt_task_resume(&task_desc4);
 //       if(ret<0)
   //              perror("suspend error\n");
//        ret=rt_task_resume(&task_desc5);
  //      if(ret<0)
    //             perror("suspend error\n");           

	ret=rt_task_create(&task_desc2,"rt_task_2",TASK_STKSZ,4,TASK_MODE);
	rt_task_start(&task_desc2,&rt_task_2,NULL);
//	printf("************************************task 2 started*******************************************\n");

//	          ret=rt_task_resume(&task_desc2);

//	  ret=rt_task_suspend(&task_desc1);
  //       if(ret<0)
    //             perror("suspend error\n");
//        ret=rt_task_suspend(&task_desc3);
  //       if(ret<0)
    //             perror("suspend error\n");
  //      ret=rt_task_suspend(&task_desc4);
//         if(ret<0)
  //               perror("suspend error\n");
    //    ret=rt_task_suspend(&task_desc5);
      //   if(ret<0)
        //         perror("suspend error\n");


	pause();
//	ret = rt_task_delete(&task_desc2);


//	ret=rt_task_resume(&task_desc1);
  //      if(ret<0)
    //             perror("suspend error\n");
//        ret=rt_task_resume(&task_desc3);
  //      if(ret<0)
    //             perror("suspend error\n");
//        ret=rt_task_resume(&task_desc4);
  //      if(ret<0)
    //             perror("suspend error\n");
//        ret=rt_task_resume(&task_desc5);
  //      if(ret<0)
    //             perror("suspend error\n");



//	 ret=rt_task_create(&task_desc3,"rt_task_3",TASK_STKSZ,3,TASK_MODE);
//	rt_task_start(&task_desc3,&rt_task_3,NULL);
//	printf("************************************task 3 started********************************************\n");

//	        ret=rt_task_suspend(&task_desc3);
  //       if(ret<0)
    //             perror("suspend error\n");
  //      ret=rt_task_suspend(&task_desc4);
    //     if(ret<0)
      //           perror("suspend error\n");
//        ret=rt_task_suspend(&task_desc5);
  //       if(ret<0)
    //             perror("suspend error\n");

//	pause();


//	ret=rt_task_resume(&task_desc3);
  //      if(ret<0)
    //             perror("suspend error\n");
  //      ret=rt_task_resume(&task_desc4);
    //    if(ret<0)
      //           perror("suspend error\n");
//        ret=rt_task_resume(&task_desc5);
  //      if(ret<0)
    //             perror("suspend error\n");

//	ret=rt_task_create(&task_desc4,"rt_task_4",TASK_STKSZ,2,TASK_MODE);
//	rt_task_start(&task_desc4,&rt_task_4,NULL);
//	printf("************************************task 4 started********************************************\n");

//	 ret=rt_task_suspend(&task_desc4);
  //       if(ret<0)
   //              perror("suspend error\n");
  //      ret=rt_task_suspend(&task_desc5);
    //     if(ret<0)
      //           perror("suspend error\n");

//	pause();

//	ret=rt_task_resume(&task_desc4);
  //      if(ret<0)
    //             perror("suspend error\n");
//        ret=rt_task_resume(&task_desc5);
  //      if(ret<0)
    //             perror("suspend error\n");


//	 ret=rt_task_create(&task_desc5,"rt_task_5",TASK_STKSZ,1,TASK_MODE);
//	rt_task_start(&task_desc5,&rt_task_5,NULL);
//	printf("************************************task 5 started********************************************\n");

//	printf("real time task is started\n");
         
        pause(); 

	 ret = rt_task_delete(&task_desc1);
	 ret = rt_task_delete(&task_desc2);
//	 ret = rt_task_delete(&task_desc3);
//	 ret = rt_task_delete(&task_desc4);
//	 ret = rt_task_delete(&task_desc5);
	
//	 ret = rt_sem_delete(&sem_desc);
//	if(ret<0){printf("error in deleteing real time task\n")};
	 printf("all task deleted\n");
}
