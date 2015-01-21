
//#include<native/task.h>
//#include<linux/module.h>
//#include<linux/kernel.h>

//#define TASK_PRIO  90             /* Highest RT priority */
//#define TASK_MODE  T_FPU|T_CPU(0)  /* Uses FPU, bound to CPU #0 */
//#define TASK_STKSZ 4096            /* Stack size (in bytes) */

//RT_TASK task_desc;

//
//you can use the cookie accordingly, as needed !!!
//
//void task_body(void *cookie)

//{
//	printk("I am in RT-TASK\n");
    //for (;;) {
    /* actual real-time code comes here */
    //}
//}

//int init_module (void)

//{
//	int err;
//	printk("hello world\n");
    /* ... */

  //  err = rt_task_create(&task_desc,"MyTaskName",TASK_STKSZ,TASK_PRIO,TASK_MODE);
   // if (!err)
    //    rt_task_start(&task_desc,&task_body,NULL);

    /* .... handler errors  ... */
//}


//if a kernel module is to unloaded, rmmod utility is to be invoked
//once rmmod is invoked, system will execute clean-up module and 
//eventually, unload the module from system space 

//typically, cleanup_module() code must undo whatever done by 
//init_module() - meaning, destroy and free what was allocated
//and initialized in init_module() !!!


//void cleanup_module (void)
//{

    //undo what you have done in init_module()
//	    printk(KERN_ALERT"good bye\n");	
//	    rt_task_delete(&task_desc);
//}
//MODULES_LICENSE("GPL");       
#include <linux/module.h> /* Needed by all modules */
#include <linux/kernel.h> /* Needed for KERN_ALERT */
int init_module(void) {
printk("<1>Hello world 1.\n");
return 0; 
}
void cleanup_module(void) {
printk(KERN_ALERT "Goodbye world 1.\n");
}
MODULE_LICENSE("GPL"); //To access Xenomai symbols
           
