
#include <native/pipe.h>
#include <native/task.h>


#define PIPE_MINOR 0


/* Kernel-side */

#define TASK_PRIO  0		/* Highest RT priority */
#define TASK_MODE  T_FPU|T_CPU(0)	/* Uses FPU, bound to CPU #0 */
#define TASK_STKSZ 4096		/* Stack size (in bytes) */

RT_TASK task_desc;

RT_PIPE pipe_desc;

void task_body(void *cookie)
{
	RT_PIPE_MSG *msgout, *msgin;
	int err, len, n;

        //if we are setting up this task as a real time task,
        //we must use rt_task_set_periodic() - this will ensure
        //that a new sw timer is created and our task is associated
        //with the sw timer - eventually, our sw timer is added to
        //the sw timer list - when our sw timer expires, 
        //our task will again be added to the ready queue !!! 
	
        //we may use rt_task_set_periodic() here to 
        //set up our task as a periodic real time task 
        //param1 - ptr to tcb
        //param2 - rt_timer_read() + x nanoseconds (in one shot mode)
        //         rt_timer_read() + x clock ticks(in periodic mode)
        //param3 - x nanoseconds as per application(in one shot mode)
        //         - x clock ticks as per application(in periodic mode) 
        
        for (;;) {
		/* ... */

		len = sizeof("Hello");
		/* Get a message block of the right size in order to
		   initiate the message-oriented dialog with the
		   user-space process. Sending a continuous stream of
		   bytes is also possible using rt_pipe_stream(), in
		   which case no message buffer needs to be
		   preallocated. */
		msgout = rt_pipe_alloc(&pipe_desc,len);

		if (!msgout);
		//	fail();

		/* Send prompt message "Hello" (the output buffer will be freed
		   automatically)... */
		strcpy(P_MSGPTR(msgout), "Hello");
		rt_pipe_send(&pipe_desc, msgout, len, P_NORMAL);

		/* Then wait for the reply string "World": */
		n = rt_pipe_receive(&pipe_desc, &msgin, TM_INFINITE);

		if (n < 0) {
			continue;
		}

		if (n == 0) {
			if (msgin == NULL) {
				continue;
			}

		} else

		/* Free the received message buffer. */
		rt_pipe_free(&pipe_desc, msgin);

                //may need to add rt_task_wait_period(), if task is periodic
                //

		/* ... */
	}
}

int init_module(void)
{
	int err;

	err = rt_pipe_create(&pipe_desc, "rt_pipe_jit", PIPE_MINOR,1024);

	if (err);
	//	fail();

	/* ... */

	err = rt_task_create(&task_desc,
			     "MyTaskName", TASK_STKSZ, TASK_PRIO, TASK_MODE);
	if (!err)


		rt_task_start(&task_desc, &task_body, NULL);

	/* ... */
        return 0;
}

void cleanup_module(void)
{
	rt_pipe_delete(&pipe_desc);
	rt_task_delete(&task_desc);
}

















