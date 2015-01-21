#include <native/task.h>

#define TASK_PRIO  99              /* Highest RT priority */
#define TASK_MODE  T_FPU|T_CPU(0)  /* Uses FPU, bound to CPU #0 */
#define TASK_STKSZ 4096            /* Stack size (in bytes) */

RT_TASK task_desc;

//
//you can use the cookie accordingly, as needed !!!
//
void task_body (void *cookie)

{
    //for (;;) {
    /* actual real-time code comes here */
    //}
}
int init_module (void)

{
    int err;

    /* ... */

    err = rt_task_create(&task_desc,
                         "MyTaskName",
                         TASK_STKSZ,


                         TASK_PRIO,
                         TASK_MODE);
    if (!err)
        rt_task_start(&task_desc,&task_body,NULL);


}
void cleanup_module (void)
{

    //undo what you have done in init_module()

    rt_task_delete(&task_desc);
}
MODULE_LICENSE("GPL"); //To access Xenomai symbols

