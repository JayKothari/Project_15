#include <linux/init.h>		
#include <linux/module.h>
#include <linux/kernel.h>
#include<linux/kobject.h>

#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
/*	fs/char_dev.c, drivers/base/class.c
 *	drivers/base/core.c
 */
#include <linux/slab.h>
#include <linux/kfifo.h>//for kfifo
#include<linux/spinlock.h>//for spinlock

#define MAX_SIZE 4096

int ndevices=5;
module_param(ndevices,int,S_IRUGO);

typedef struct private_object
{
	struct cdev cdev;
	unsigned char* pbuf;
	struct kfifo kf1;
	struct list_head lentry;
	struct kobject kobj;
	spinlock_t my_lock;         
}C_DEV;
LIST_HEAD(phead);//initialized at compile time:
dev_t pseudo_dev_id;

struct class *pseudo_cls;//class pointer


//******************************************open()**************************************************************//
static int pseudo_open(struct inode *inode,struct file *file)
{
//by using container_of() we get pointer to C_DEV structure from pointer to member cdev of C_DEV
	 C_DEV* probj=container_of(inode->i_cdev,C_DEV,cdev);
	printk("pseudo open method\n");
        //storing the private_object to the private_data field in the file.This all things are done by pointer.
	file->private_data=probj;    
	return 0;
}
//close(fd);
static int pseudo_release(struct inode *inode,struct file *file)
{
	printk("pseudo close method\n");
	return 0;
}
//****************************************read()******************************************************************//
static ssize_t pseudo_read(struct file *file, char __user *ubuf,size_t count, loff_t *ppos)
{
	unsigned char* tbuf=kmalloc(count+1,GFP_KERNEL);    //allocates memory for tbuf

	C_DEV* pdev=file->private_data;                     //pointer of private object to private_data in file
	int len=kfifo_len(&pdev->kf1);                      //length of kfifo

	int ret,nbytes;                
	printk("pseudo read method entered\n");
	
	if(count>len)
		count=len;//min(count,length)//
	if(count==0) return -EAGAIN;
	
	nbytes=kfifo_out(&pdev->kf1,tbuf,count);            //get data from kfifo
	printk("read:nbytes=%d\t,count=%d\t,off=%d\t,kfifo length=%d\n",nbytes,count,*ppos,kfifo_len(&pdev->kf1));
	tbuf[nbytes]=0;                                     
	printk("read:tbuf=%s\n",tbuf);
	ret=copy_to_user(ubuf,tbuf,nbytes);                      
//	printf("tbub%s\n",*tbuf);
	printk("read::ret=%d\n",ret);
	if(ret) return -EFAULT;
//	*ppos += nbytes;
	kfree(tbuf);                                         //free the tbuf memory
	return nbytes;
}
//*****************************************************************************************************************//
//**********************************write()***********************************************************************//
static ssize_t pseudo_write(struct file *file, const char __user *ubuf,size_t count, loff_t *ppos)
{
	C_DEV* pdev=file->private_data;     
	
	int nbytes;
	
	int remain=kfifo_avail(&pdev->kf1);                  //number of bytes available in kfifo
	unsigned char* tbuf=kmalloc(count,GFP_KERNEL);       //allocates tbuf

	printk("write:pseudo write method entered,remain=%d\n",remain);
	if(remain==0) 
		return -EAGAIN;
	if(count > remain)
		count=remain;                           	//min(count,remain)
	
	if(copy_from_user(tbuf,ubuf,count))                     
		return -EFAULT;
	  nbytes=kfifo_in(&(pdev->kf1),tbuf,count);             //put data in kfifo

	
	printk("nbytes=%d\t,count=%d\t,off=%d\t,kfifo length=%d\t",nbytes,count,*ppos,kfifo_len(&pdev->kf1));
//	*ppos += nbytes;
	kfree(tbuf);                                           //free the tbuf
	return nbytes;
}
//*****************************************************************************************************************//
//*************************************************ioctl***********************************************************//
/*
static int pseudo_ioctl(struct file *f, unsigned int cmd, unsigned long arg)
{
printk("resetting pseudo device and querying a pseudo device for number of data bytes currently in device\n");
	switch(cmd)
	{
		case RESET:
		{
			kfifo_free(&pdev->kf1);
			break;
		}

		case NO_DATA_BYTES:
		{
			 C_DEV* pdev;
			pdev->pbuf
			break;
		}
	
	}	



}
*/
//*****************************************************************************************************************//
//*************************************file operation**************************************************************//
static struct file_operations pseudo_fops=
{
	.open=pseudo_open,
	.release=pseudo_release,
	.read=pseudo_read,
	.write=pseudo_write,
	.owner=THIS_MODULE,
//	.ioctl=pseudo_ioctl
};

//******************************************************************************************************************//

//*********************************************init function()*****************************************************//
static int pseudo_init(void)	
{
	struct kobject *temp;	
	int ret,i=0;
	C_DEV* pdev; 	//struct private_object* pdev;
        
        //allocates memory	
	ret=alloc_chrdev_region(&pseudo_dev_id,0,ndevices,"pseudo_char_driver");
	if(ret<0)	return -EFAULT;

	printk("driver registered,major=%d\n",MAJOR(pseudo_dev_id));
        //creates class 
	pseudo_cls=class_create(THIS_MODULE,"pseudo_class");
	//per device initialization                        
	for(i=0;i<ndevices;i++)                             
	{
		 pdev=kmalloc(sizeof(C_DEV),GFP_KERNEL);
		 pdev->pbuf=kmalloc(MAX_SIZE,GFP_KERNEL);                 //allocating memory to buffer in private object
		 kfifo_init(&(pdev->kf1),pdev->pbuf,MAX_SIZE);            //init the kfifo
		// kfifo_alloc(&pdev->kf1,MAX_SIZE,GFP_KERNEL);//*

//		   kobject_init(&(pdev->cdev.kobj),&(pdev->cdev.kobj.ktype));   

		 cdev_init(&pdev->cdev,&pseudo_fops);                     //init the charecter device

		
//		 struct kobject *temp;
		 temp=kobject_create_and_add("example", kernel_kobj);
		 pdev->kobj=*temp;


//		 memset(pdev->kobj,0,sizeof(pdev->kobj));
//		 kobject_init(&(pdev->kobj),&(pdev->kobj.ktype));			  //init koject and sets ref count to 1
//		int kobject_add(struct kobject *kobj);
//		int err=kobject_add(&(pdev->kobj),0,0);
//		if(err<0)
//		{	printk("koject adding error\n");	
//		}
	
		 kobject_set_name(&(pdev->cdev.kobj),"my_pseudo_dev%d",i);//sets the "name" field of koject  



		 ret=cdev_add(&pdev->cdev,pseudo_dev_id+i,1);             //adds the character device
		 if(ret<0) 
			{
				printk("cdev_add failed\n");
				kfifo_free(&pdev->kf1);
				//unregister the driver
				return -EFAULT;
			}
		list_add_tail(&pdev->lentry,&phead);//adds new entry to list||void list_add_tail(struct list_head * new,struct list_head * head);
		device_create(pseudo_cls,NULL,pseudo_dev_id+i,NULL,"pchardev%d",i);//creates a device and registers it with sysfs
		printk("device initialized:%d\n",i);
        }
        printk("Pseudo Char Driver registered successfully\n");

	spin_lock_init(&(pdev->my_lock));//init of spinlock
        return 0;
}
//*****************************************************************************************************//
//*****************************************exit*******************************************************//
static void pseudo_exit(void)	
{
	struct list_head *ptemp,*qtemp;
	C_DEV* pdev;
	int i=0;
	//per device cleanup/deallocation
	list_for_each_safe(ptemp,qtemp,&phead)//iterate over a list safe against removal of list entry || list_for_each_safe(pos,n,head)
	{		
		pdev=container_of(ptemp,C_DEV,lentry);
		kfifo_free(&pdev->kf1);
		cdev_del(&pdev->cdev);
		kfree(pdev);
		device_destroy(pseudo_cls,pseudo_dev_id+i);
		printk("cleaned device:%d\n",i);
//		void kobject_del(struct kobject *kobj);
//		kobject_del(&(pdev->kobj));
		kobject_put(&(pdev->kobj));
//		  kobject_put(&(pdev->kobj));
//		  kobject_put(&(pdev->kobj));
//		  kobject_put(&(pdev->kobj));
//		  kobject_put(&(pdev->kobj));

		printk(KERN_INFO "i am unloading...n");
	
		i++;

	}

	class_destroy(pseudo_cls);
	unregister_chrdev_region(pseudo_dev_id,5);

	printk("Pseudo Char Driver:Bye\n");	
}
//********************************************************************************************************//

module_init(pseudo_init); 
module_exit(pseudo_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jay Kothari");
MODULE_DESCRIPTION("driver for n number of device");
//MODULE_PARM(ndevices, "i");
MODULE_PARM_DESC(ndevices, ":Total number of devices");
