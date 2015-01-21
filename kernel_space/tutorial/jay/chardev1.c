#include <linux/init.h>		
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
/*	fs/char_dev.c, drivers/base/class.c
 *	drivers/base/core.c
 */
#include <linux/slab.h>
#include <linux/kfifo.h>
#include<linux/errno.h>
#include "query_ioctl.h"
#include<linux/version>

#define MAX_SIZE 4096
int ndevices=5;
module_param(ndevices,int,S_IRUGO);

typedef struct private_object
{
	struct cdev cdev;
	unsigned char* pbuf;
	struct kfifo kf1;
	struct list_head lentry;
}C_DEV;
LIST_HEAD(phead);
dev_t pseudo_dev_id;

struct class *pseudo_cls;

//query_arg_t q;


//******************************************open()**************************************************************//
static int pseudo_open(struct inode *inode,struct file *file)
{
	 C_DEV* probj=container_of(inode->i_cdev,C_DEV,cdev);
	printk("pseudo open method\n");
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
	unsigned char* tbuf=kmalloc(count+1,GFP_KERNEL);

	C_DEV* pdev=file->private_data;
	int len=kfifo_len(&pdev->kf1);

	int ret,nbytes;
	printk("pseudo read method entered\n");
	
	if(count>len)
		count=len;//min(count,length)//
	if(count==0) return -EAGAIN;
	
	nbytes=kfifo_out(&pdev->kf1,tbuf,count);
	printk("read:nbytes=%d\t,count=%d\t,off=%d\t,kfifo length=%d\n",nbytes,count,*ppos,kfifo_len(&pdev->kf1));
	tbuf[nbytes]=0;
	printk("read:tbuf=%s\n",tbuf);
	ret=copy_to_user(ubuf,tbuf,nbytes);
//	printf("tbub%s\n",*tbuf);
	printk("read::ret=%d\n",ret);
	if(ret) return -EFAULT;
//	*ppos += nbytes;
	kfree(tbuf);
	return nbytes;
}
//*****************************************************************************************************************//
//**********************************write()***********************************************************************//
static ssize_t pseudo_write(struct file *file, const char __user *ubuf,size_t count, loff_t *ppos)
{
	C_DEV* pdev=file->private_data;
	
	int nbytes;
	
	int remain=kfifo_avail(&pdev->kf1);
	unsigned char* tbuf=kmalloc(count,GFP_KERNEL);

	printk("write:pseudo write method entered,remain=%d\n",remain);
	if(remain==0) 
		return -EAGAIN;
	if(count > remain)
		count=remain;	//min(count,remain)
	
	if(copy_from_user(tbuf,ubuf,count))
		return -EFAULT;
//	       q->buffer  =tbuf;
	  nbytes=kfifo_in(&(pdev->kf1),tbuf,count);

	
	printk("nbytes=%d\t,count=%d\t,off=%d\t,kfifo length=%d\t",nbytes,count,*ppos,kfifo_len(&pdev->kf1));
//	*ppos += nbytes;
	kfree(tbuf);
	return nbytes;
}

//*****************************************************************************************************************//
//*************************************file operation**************************************************************//
static struct file_operations pseudo_fops=
{
	.open=pseudo_open,
	.release=pseudo_release,
	.read=pseudo_read,
	.write=pseudo_write,
	.owner=THIS_MODULE,
////#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,35))
//	.ioctl = my_ioctl
//#else
//	.unlocked_ioctl = my_ioctl
//#endif
};
//******************************************************************************************************************//

//*********************************************init function()*****************************************************//
static int pseudo_init(void)	
{
	int ret,i=0;
	C_DEV* pdev; 	//struct private_object* pdev;
	ret=alloc_chrdev_region(&pseudo_dev_id,0,ndevices,"pseudo_char_driver");
	if(ret<0)	return -EFAULT;
	printk("driver registered,major=%d\n",
			MAJOR(pseudo_dev_id));
	pseudo_cls=class_create(THIS_MODULE,"pseudo_class");
	//per device initialization
	for(i=0;i<ndevices;i++) 
	{
	 pdev=kmalloc(sizeof(C_DEV),GFP_KERNEL);
	 pdev->pbuf=kmalloc(MAX_SIZE,GFP_KERNEL);
	 kfifo_init(&(pdev->kf1),pdev->pbuf,MAX_SIZE);
	// kfifo_alloc(&pdev->kf1,MAX_SIZE,GFP_KERNEL);//*
	 cdev_init(&pdev->cdev,&pseudo_fops);
	 kobject_set_name(&(pdev->cdev.kobj),"my_pseudo_dev%d",i);
	 ret=cdev_add(&pdev->cdev,pseudo_dev_id+i,1);
	 if(ret<0) 
	{
		printk("cdev_add failed\n");
		kfifo_free(&pdev->kf1);
		//unregister the driver
		return -EFAULT;
	}
	list_add_tail(&pdev->lentry,&phead);
	device_create(pseudo_cls,NULL,pseudo_dev_id+i,NULL,"pchardev%d",i);
	printk("device initialized:%d\n",i);
        }
        printk("Pseudo Char Driver registered successfully\n");
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
	list_for_each_safe(ptemp,qtemp,&phead)
	{
		pdev=container_of(ptemp,C_DEV,lentry);
		kfifo_free(&pdev->kf1);
		cdev_del(&pdev->cdev);
		kfree(pdev);
		device_destroy(pseudo_cls,pseudo_dev_id+i);
		printk("cleaned device:%d\n",i);
		i++;
	}

	class_destroy(pseudo_cls);
	unregister_chrdev_region(pseudo_dev_id,5);

	printk("Pseudo Char Driver:Bye\n");	
}
//********************************************************************************************************//
/*
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,35))
static int my_ioctl(struct inode *i, struct file *f, unsigned int cmd, unsigned long arg)
#else
static long my_ioctl(struct file *f, unsigned int cmd, unsigned long arg)
#endif
{
query_arg_t q;
switch(cmd)
{
	case QUERY_SET_VARIABLES:
	 q.status =sizeof( q->buffer);
	break;
	default:
            return -EINVAL; 
}
}
*/

module_init(pseudo_init); 
module_exit(pseudo_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jay Kothari");
MODULE_DESCRIPTION("driver for n number of device");


