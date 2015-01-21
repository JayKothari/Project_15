#include<native/task.h>
#include<nucleus/xenomai.h>
#include<native/pipe.h>
#include<native/queue.h>
#include<native/timer.h>

#define TASK_PRIO 0
#define TASK_MODE T_FPU | T_CPU(0)
#define TASK_STKSIZE 4096

#define PIPE_SIZE 4096
#define QUEUE_SIZE 1024
SRTIME period_ns = 1000000;

RT_TASK task_desc1;
RT_TASK task_desc2;

RT_PIPE pipe_desc;
RT_QUEUE queue_desc;

RT_PIPE_MSG *msgout = NULL;
void *msgin = NULL;
void *msgout1 = NULL;
unsigned long overrun;
RTIME currTime,prevTime;


void task_periodic(void *args)
{
	int err;
	int itr = 0;

	char timestr[128];
	int length = 0;
		
        prevTime = rt_timer_read();
	xnprintf("in periodic task\n");    
    

	err = rt_task_set_periodic(&task_desc1,prevTime+DELTA,period_ns);
	if(err<0)
	{		
			
			xnprintf("cant set periodic task to periodic mode %d\n",err);
			return;
	}
	

	//while(itr++<5)
        while(1)
	{	       
		err = rt_task_wait_period(&overrun);
			if(err<0)
			{
				xnprintf("error in rt_task_wait_period %d\n",err);
				return;
			}
		
		xnprintf("rt_task_wait_period is done %d\n",itr);

		currTime = rt_timer_read();
			if(currTime<0)
			{
				xnprintf("timer read has problem\n");
				return;
			}	

                length = sprintf(timestr,"%llu",currTime);
	
		xnprintf("length is  %d\n",length+1);
		if(length<0)
		{
			xnprintf("problrmin finding the length\n");
			return;
		}

		/*
			 void* rt_queue_alloc(RT_QUEUE* q,size_t size)
			return:address of the allocated message buffer upon success
			upon fail:NULL
		*/
		msgout1 = rt_queue_alloc(&queue_desc,length+1);
		if(msgout1==NULL)
		{
			xnprintf("error in allocating queue buffer\n");
			return;
			

		}
		else
		{
			xnprintf("buffer is allocated successfully\n");
			strcpy(msgout1,timestr);
			/*
				int rt_queue_send(RT_QUEUE *q,void* mbuff,size_t size,int mode);
				return: upon success, this service returns the number of receivers
					which got awaken as a result the operation.
					if 0,no task is waiting on receiving side of the queue.
			*/
			err = rt_queue_send(&queue_desc,msgout1,length+1,P_NORMAL);
			if(err<0)
			{
				xnprintf("error writing to queue %d\n",err);
		                rt_queue_free(&queue_desc,msgout1);
				return;
			}
			xnprintf("mesage sending is success full to queue\n");
		}

		prevTime = currTime;


	}

}
void  task_aperiodic(void *args)
{
	int err;
	ssize_t recv;
	//int itr1 =  0;
	//char *recvtime =NULL;
	int length = 0;
	xnprintf("in aperiodic task\n");
        //while(itr1++<5)
        while(1)
	{
		/*
		
			 ssize_t rt_queue_receive(RT_QUEUE *q,void **buffp,RTIME timeout);
		
			 return: upon success , number of bytes available from the received message.
		*/
                //error checking for this must be improved !!
		recv = rt_queue_receive(&queue_desc,&msgin,TM_INFINITE);
		//recv = rt_queue_receive(&queue_desc,&msgin,500000);
		if(recv>0)
		{
			xnprintf("receive message\n");
			length = strlen(msgin)+1;
			
			/*

				RT_PIPE_MSG* rt_pipe_alloc(RT_PIPE * pipe,size_t size);
				
				return:upon success ,address of the allocated message buffer 
				
				upon fail:NULL
				
				the begining of available data area of size contigious bytes is accesible from
				P_MSGPTR(msg).
			*/

			
			msgout = rt_pipe_alloc(&pipe_desc,length);
			if(msgout==NULL)
			{
				xnprintf("error in allocating buffer for pipe\n");
				return;
				
			}
			else
			{
				
			
				strcpy(P_MSGPTR(msgout),msgin);
                                err=rt_queue_free(&queue_desc,msgin);
                                if(err<0)
				{
                                         xnprintf("error in freeing queue buff in aperiodic\n"); 
                                }
				xnprintf("queue is free\n");
				rr = rt_pipe_send(&pipe_desc,msgout,P_MSGSIZE(msgout),P_NORMAL);
				if(err<0)
				{
					xnprintf("error in writing to the pipe\n");
                                        rt_pipe_free(&pipe_desc,msgout);

					return;
				}
				xnprintf("message sending to pipe is successful\n");
				//err = rt_pipe_free(&pipe_desc,msgout);
				//if(err<0)
				//{
					xnprintf("error in freeing pipe buff in aperiodic\n");
				//}
				
				

			}
		}
		else
		{
			xnprintf("problem in receiving the message\n");
			return;		
		}

	}
}


int init_module()
{
	int ret = 12;
	/*
	 	int rt_tamer_set_mode(RTIME nstick);		
		set the system clock rate.
		return: upon success: 0
		otherwise:  -ENODEV
	*/
        
	xnprintf("entering into init module\n");
	ret = rt_timer_set_mode(TM_ONESHOT);  //set this to periodic, after setting up the 
                                              //real-time configuration !!!!
	if(ret<0)
	{
		xnprintf("problem in setting timer mode\n");
                return ret;
	}

	/*
		int rt_queue_create(RT_QUEUE *q,const char *name,size_t poolsize,size_t qlimit,int mode);
		return: upon success 0;
		
		poolsize: in byte
		
		qlimit: allows to limit maximum number of message which can be queued at any point of time.
		
		Q_UNLIMITED : can be pass to specify unlimited amount.

		mode: creation mode,(Q_FIFO |Q_PRIO |Q_SHARED |Q_DMA)
		
	*/
	ret = rt_queue_create(&queue_desc,"MY_QUEUE",QUEUE_SIZE,Q_UNLIMITED,
                                                               Q_FIFO );
	
        if(ret<0)
	{
		xnprintf("error in creating the queue\n");
                return ret;
	}
	else
	{
		xnprintf("queue created\n");
	}
	
	

	ret = rt_pipe_create(&pipe_desc,"MY_PIPE",0,PIPE_SIZE);
	if(ret<0)
	{


		xnprintf("error in creating the pipe\n");
                rt_queue_delete(&queue_desc);
                return ret;

	}
	else
	{
		xnprintf("pipe creadted\n");
	}


	ret =rt_task_create(&task_desc1,"periodic",TASK_STKSIZE,TASK_PRIO,
                                                                  TASK_MODE);
	if(ret<0)
	{
		xnprintf("error in creating the periodic task\n");
		rt_queue_delete(&queue_desc);
		rt_pipe_delete(&pipe_desc);
		return ret;
	}
	else
	{      
                xnprintf("task created periodic\n");
		ret = rt_task_start(&task_desc1,&task_periodic,NULL);
		if(ret<0)
		{
			xnprintf("task cant be started\n");
			rt_queue_delete(&queue_desc);
	                rt_pipe_delete(&pipe_desc);
			rt_task_delete(&task_desc1);
			return ret;

		}
		xnprintf("task started periodic\n");
	}

//*********************************************aperiodic create*******************************************//
	ret =rt_task_create(&task_desc2,"aperiodic_task",TASK_STKSIZE,TASK_PRIO,TASK_MODE);
	if(ret<0)
	{
		xnprintf("error in creating the aperiodic task\n");
		rt_queue_delete(&queue_desc);
		rt_pipe_delete(&pipe_desc);
		rt_task_delete(&task_desc1);
		return ret;
	}
	else
	{	xnprintf("task created aperodic\n");
		ret = rt_task_start(&task_desc2,&task_aperiodic,NULL);
		if(ret<0)
                {
                        xnprintf("task cant be started\n");
                        rt_queue_delete(&queue_desc);
                        rt_pipe_delete(&pipe_desc);
			rt_task_delete(&task_desc1);
			rt_task_delete(&task_desc2);
			return ret;
                }
		xnprintf("task started aperiodic\n");
	}


return 0;
}


void cleanup_module()
{
	rt_queue_delete(&queue_desc);
        rt_pipe_delete(&pipe_desc);
	rt_task_delete(&task_desc1);
	rt_task_delete(&task_desc2);

}
MODULE_LICENCE("GPL");//to access xenomai symbols
