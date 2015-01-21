#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/vmalloc.h>
#include<linux/list.h>
#include<linux/fs.h>
#include<linux/major.h>
#include<linux/blkdev.h>
#include<linux/cdev.h>
#include<asm/uaccess.h>

#define PLP_KMEM_BUFSIZE (1024*1024) /*1MB internel buffer*/

//***********************global variables**********************//
int ndevices = 1;
static char *plp_kmem_buffer;
static struct class *pseudo_class;//class under /sys/class
static dev_t plp_kmem_dev;        //dynamically assigned char device
static struct cdev *plp_kmem_cdev;//dynamically allocated at runtime

//************************func prototype***********************//

static int __init plp_kmem_init(void);
static void __exit plp_kmem_exit(void);

static int plp_kmem_open(struct inode *inode,struct file *file);
static int plp_kmem_release(struct inode *inode , struct file *file);
static ssize_t plp_kmem_read(struct file *file,char __user *buf,size_t count,loff_t *ppos);
static ssize_t plp_kmem_write(struct file *file,const char __user *buf,size_t count,loff_t *ppos);

//pseudo device instance is representaed by this private object instance
typedef struct priv_obj1{
       struct list_head list;
       struct cdev cdev;
}C_OBJ;

static struct file_operation plp_kmem_fops={
	.read	=	plp_kmem_read,
	.write	=	plp_kmem_write,
	.open	=	plp_kmem_open,
	.release=	plp_kmem_release,
	.owner	=	THIS_MODULE,
};
LIST_HEAD(dev_list);
//******************************open method*************//
static int plp_kmem_open(struct inode *inode,struct file *file)
{
C_OBJ	*obj;
//we extract our device instance's private object using system object
obj=container_of(inode->i_cdev,C_OBJ,cdev);
file->private_data = obj;//storing private objects's ptr in a private_data field of open file object of the active file

dump_stack();

//here the open is dummy - not always
#ifdef PLP_DEBUG
	printk(KERN_DEBUG "plp_kmem: opened device.\n");
#endif

	return 0;

}
//****************************release*********************//
static int plp_kmem_release(struct inode *inode, struct file *file)
{

//dummy here - not always 

#ifdef PLP_DEBUG
	printk(KERN_DEBUG "plp_kmem: device closed.\n");
#endif

	return 0;
}
//********************************************************//
//********************read*******************************//

static ssize_t plp_kmem_read(struct file *file, char __user *buf,
				size_t count, loff_t *ppos)
{
	size_t bytes = count;
	loff_t fpos  = *ppos;
	char *data;
	
	if (fpos >= PLP_KMEM_BUFSIZE)
		return 0;
	
	if (fpos+bytes >= PLP_KMEM_BUFSIZE)
		bytes = PLP_KMEM_BUFSIZE-fpos;

	if (0 == (data = kmalloc(bytes, GFP_KERNEL)))
		return -ENOMEM;

#ifdef PLP_DEBUG
	printk(KERN_DEBUG "plp_kmem: read %d bytes from device, offset %d.\n",
		bytes,(int)fpos);
#endif

	memcpy(data,plp_kmem_buffer+fpos,bytes);
	
	if (copy_to_user((void __user *)buf, data, bytes)) {
		printk(KERN_ERR "plp_kmem: cannot write data.\n");
		kfree(data);
		return -EFAULT;
	}
	
	*ppos = fpos+bytes;

	kfree(data);
	return bytes;
}
//*******************************************************//
//************************write*************************//
static ssize plp_kmem_write(struct file *file,const char __user *buf,size_t count,loff_t *ppos)
{
struct C_DEV *obj = file->private_data;
size_t bytes = count;
loff_t fpos = *ppos;
char *data;

if(fpos >= PLP_KMEM_BUFSIZE)
	return -ENOSPC;
if(fpos+bytes >= PLP_KMEM_BUFSIZE)
	bytes =PLP_KMEM_BUFSIZE - fpos;
if(0 == (data = kmalloc(bytes,GFP_KERNEL)))
	return -ENOMEM;

if (copy_from_user((void *)data, (const void __user *)buf, bytes)) {
		printk(KERN_ERR "plp_kmem: cannot read data.\n");
		kfree(data);
		return -EFAULT;
	}

#ifdef PLP_DEBUG
	printk(KERN_DEBUG "plp_kmem: write %d bytes to device, offset %d.\n",
		bytes,(int)fpos);
#endif
        //user-space data was earlier copies into a system-space 
        //buffer
        //from system-space buffer data is copied to the 
        //pseudo device built using vmalloc() - meaning, a system
        //space buffer is treated as device and managed by this driver
	memcpy(plp_kmem_buffer+fpos,data,bytes);

	*ppos = fpos+bytes;

	kfree(data);
	return bytes;  //must return the no of bytes written to the device
}

C_OBJ *my_dev;

module_param(ndevices,int,S_IRUGO);

static int __init plp_kmem_init(void)
{

int ret;
my_dev = kmalloc(sizeof(C_OBJ),GFP_KERNEL);
list_add_tail(&my_dev->list,&dev_list);
if (alloc_chrdev_region(&plp_kmem_dev, 0, ndevices, "pseudo_driver"))
	goto error;
cdev_init(&my_dev->cdev,&plp_kmem_fops);  
my_dev->cdev.owner = THIS_MODULE;
kobject_set_name(&(my_dev->cdev.kobj),"pseudo_dev0");
my_dev->cdev.ops = &plp_kmem_fops; 

if (cdev_add(&my_dev->cdev, plp_kmem_dev, 1)) {
		kobject_put(&(my_dev->cdev.kobj));
		unregister_chrdev_region(plp_kmem_dev, 1);
		kfree(my_dev);
		goto error;
	}

printk(KERN_INFO "plp_kmem: loaded.\n");

	return 0;
}

static void __exit plp_kmem_exit(void)
{
cdev_del(&(my_dev->cdev));
unregister_chrdev_region(plp_kmem_dev,1);
 kfree(my_dev); 
	printk(KERN_INFO "plp_kmem: unloading.\n");
}

module_init(plp_kmem_init);
module_exit(plp_kmem_exit);

/* define module meta data */

MODULE_DESCRIPTION("Demonstrate kernel memory allocation");

MODULE_ALIAS("memory_allocation");
MODULE_LICENSE("GPL");
MODULE_VERSION("0:1.0");







