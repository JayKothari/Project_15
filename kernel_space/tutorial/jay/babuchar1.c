/*
 * plp_kmem.c - Minimal example kernel module.
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/vmalloc.h>
#include <linux/list.h>
#include <linux/fs.h>
#include <linux/major.h>
#include <linux/blkdev.h>
#include <linux/cdev.h>

#include <asm/uaccess.h>

#define PLP_KMEM_BUFSIZE (1024*1024) /* 1MB internal buffer */

/* global variables */

int ndevices = 1;

static char *plp_kmem_buffer;

static struct class *pseudo_class;	/* pretend /sys/class */
static dev_t plp_kmem_dev;		/* dynamically assigned char device */
static struct cdev *plp_kmem_cdev;	/* dynamically allocated at runtime. */

/* function prototypes */

static int __init plp_kmem_init(void);
static void __exit plp_kmem_exit(void);

static int plp_kmem_open(struct inode *inode, struct file *file);
static int plp_kmem_release(struct inode *inode, struct file *file);
static ssize_t plp_kmem_read(struct file *file, char __user *buf,
				size_t count, loff_t *ppos);
static ssize_t plp_kmem_write(struct file *file, const char __user *buf,
				size_t count, loff_t *ppos);

/* file_operations */

//typical private object may contain system objects and other fields 
//a pseudo device instance is represented by this private object instance !!!
//as per the rules !!!

typedef struct priv_obj1{
	struct list_head list;
	struct cdev cdev;      //this field is mandatory 
                               //other fields may be added as per requirments 
}C_OBJ;


//these methods are typically known as driver methods !!!
//
static struct file_operations plp_kmem_fops = {
	.read		= plp_kmem_read,
	.write		= plp_kmem_write,
	.open		= plp_kmem_open,
	.release	= plp_kmem_release,
	.owner		= THIS_MODULE,//this is a must for the system
                                      //ptr to this module's object/structure
};

LIST_HEAD(dev_list);

/*
 * plp_kmem_open: Open the kmem device
 */
//driver's custom open method ???
static int plp_kmem_open(struct inode *inode, struct file *file)
{

  C_OBJ *obj;
  //we extract our device instance's private object using 
  //system object - this is the reason for their co-existence !!!

  //once we extract our device instance's private object, driver
  //method will use this object to work directly with specific
  //device instance - this ensures that our driver methods are
  //re-entrant - this ensures that our driver methods can work
  //with several device instances, concurrently !!!
  //
  //system is free to use cdev{} and pass it to us via struct inode{}
  //,but we should not interfere with its contents 
  //
  //however, we are free to extract and work with our private object !!!
  //
  //however, system does not maintain private object, only cdev{} 
  //refer to chapter 3 of LDD/3
  //refer to <ksrc>/include/linux/kernel.h  
  obj = container_of(inode->i_cdev,C_OBJ, cdev);  //system macro 

  //as per conventions and patterns, we must store our private object's
  //ptr in a private_data field of open file object of our active file - 
  //
  //place is private_data field in file { } object !!!

  file->private_data = obj;

  dump_stack(); //complete flow of code/control to our driver/device
                //will be clear ???


//here the open is dummy - not always
#ifdef PLP_DEBUG
	printk(KERN_DEBUG "plp_kmem: opened device.\n");
#endif

	return 0;
}

/*
 * plp_kmem_release: Close the kmem device.
 */

static int plp_kmem_release(struct inode *inode, struct file *file)
{

//dummy here - not always 

#ifdef PLP_DEBUG
	printk(KERN_DEBUG "plp_kmem: device closed.\n");
#endif

	return 0;
}

/*
 * plp_kmem_read: Read from the device.
 */

static ssize_t plp_kmem_read(struct file *file, char __user *buf,
				size_t count, loff_t *ppos)
{
	size_t bytes = count;
	loff_t fpos = *ppos;
	char *data;

       //fetching the current device object/context

        //struct pseudo_dev_obj *obj = file->private_data ; 

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

/*
 * plp_kmem_write: Write to the device.
 */

static ssize_t plp_kmem_write(struct file *file, const char __user *buf,
				size_t count, loff_t *ppos)
{
        

        //fetching the current device object/context

        struct C_DEV *obj = file->private_data ;  

        //write to the kfifo, as per the standard rules
        //and return appropriately

        //the special case of blocking

        //all operations are based on the private object 

        

	size_t bytes = count;
	loff_t fpos = *ppos;
	char *data;

	if (fpos >= PLP_KMEM_BUFSIZE)
		return -ENOSPC;

	if (fpos+bytes >= PLP_KMEM_BUFSIZE)
		bytes = PLP_KMEM_BUFSIZE-fpos;

	if (0 == (data = kmalloc(bytes, GFP_KERNEL)))
		return -ENOMEM;
        //this will copy data from user-space to system-space
        //this will also verify  that the user-space buffer is
        //really user-space buffer 
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

/*
 * plp_kmem_init: Load the kernel module into memory
 */
C_OBJ *my_dev;

module_param(ndevices,int,S_IRUGO);

//everything in init module, exit module and other methods are 
//based on implementation and rules !!! there is hardly any 
//theory - however, implementation is based on strong design 
//principles, which are fairly universal !!!

static int __init plp_kmem_init(void)
{
	int ret;

        //allocate a private object for a single device !!
        //GFP_KERNEL is used when we allocate memory in 
        //process context - meaning, this call can block
        //the current process, if needed - we will see 
        //other flags in other contexts, as needed !!!
	my_dev = kmalloc(sizeof(C_OBJ),GFP_KERNEL);
        //add your own error checking code !!!	
	list_add_tail(&my_dev->list,&dev_list); //private list 
        //the first param. is the storage for the first device no
        //allocated dynamically

        //second param. is the minor no. associated with the first
        //device no. allocated dynamically

        //third param. is the no. of dynamic device ids. requested
        //last param. is a logical name 

        //if this system API succeeds, we will be allocated character
        //device ids in the range plp_kmem_dev -> plp_kmem_dev+ndevices-1,
        //with minor no. of the first device id (plp_kmem_dev) is 
        // set to 0 !!!
	if (alloc_chrdev_region(&plp_kmem_dev, 0, ndevices, "pseudo_driver"))
		goto error;

        //we are requesting for a system defined structure 
        //from a slab allocator in the KMA dedicated for
        //struct cdev 
	//if (0 == (plp_kmem_cdev = cdev_alloc()))
	//	goto error;

        //you may ask the system to allocate a system object or 
        //you may provide
        //the structure as a global data of your module - this
        //approach of maintaining system object in a private
        //object is more flexible !!!

        //we need to allocate/initialize/provide a cdev system object to 
        //manage our device as well as driver - if we are managing
        //multiple devices, we need to repeat this several times, 
        //as per rules !!!
        cdev_init(&my_dev->cdev,&plp_kmem_fops);  
       
//to manage module's resource usage count !!!
         //THIS_MODULE expands to extract ptr to the struct module {}
         //object of this module (device driver)
         my_dev->cdev.owner = THIS_MODULE;

	//we are dealing with certain special objects of the I/O subsystem 
        kobject_set_name(&(my_dev->cdev.kobj),"pseudo_dev0");
        //we are passing the file-operations supported by
        //our driver to the system - this is known as 
        //passing hooks - registering our driver's characteristics
        //with the system - this step is redundant - however, just 
        //keep it !!!
	my_dev->cdev.ops = &plp_kmem_fops; /* file up fops */
        //we are registering our system object to the system !!!
	//ptr to cdev
        //specific device's device id
        //no of devices to be managed by cdev - typically it must 
        //be 1 only - some rare cases may use more than 1 

        //at the end of cdev_add(), our device's system object is
        //added to a system data structure, which may be used, in 
        //the future !!! this implies, cdevs registered with the 
        //system will be managed, in a system data structure !!!
        //since our system object is part of the device private
        //object, we are indirectly connecting our private object 
        //to the system as well - this combination will effectively
        //help the system to manage our device !!!
        
        if (cdev_add(&my_dev->cdev, plp_kmem_dev, 1)) {
		kobject_put(&(my_dev->cdev.kobj));
		unregister_chrdev_region(plp_kmem_dev, 1);
		kfree(my_dev);
		goto error;
	}

        //currently, do not include this section - we will use
        //after understanding the LDD model and sysfs(other layers)

	/*pseudo_class = class_create(THIS_MODULE, "pseudo_class");
	if (IS_ERR(pseudo_class)) {
		printk(KERN_ERR "plp_kmem: Error creating class.\n");
		cdev_del(plp_kmem_cdev);
		unregister_chrdev_region(plp_kmem_dev, 1);
                //ADD MORE ERROR HANDLING
		goto error;
	}
	device_create(pseudo_class, NULL, plp_kmem_dev, "pseudo_dev0");*/

	printk(KERN_INFO "plp_kmem: loaded.\n");

	return 0;

error:
	printk(KERN_ERR "plp_kmem: cannot register device.\n");

        //return appropriate negative error code
	return 1;
}

/*
 * plp_kmem_exit: Unload the kernel module from memory
 */


static void __exit plp_kmem_exit(void)
{

//      after LDD model and sysfs 
	//device_destroy(pseudo_class, plp_kmem_dev);
	//class_destroy(pseudo_class);

        //removing the registration of my driver
	cdev_del(&(my_dev->cdev));//remove the registration of the driver/device

        //freeing the logical resources - device nos.
	unregister_chrdev_region(plp_kmem_dev,1);

        //freeing the system-space buffer

        kfree(my_dev); 
	printk(KERN_INFO "plp_kmem: unloading.\n");
}

/* declare init/exit functions here */

module_init(plp_kmem_init);
module_exit(plp_kmem_exit);

/* define module meta data */

MODULE_DESCRIPTION("Demonstrate kernel memory allocation");

MODULE_ALIAS("memory_allocation");
MODULE_LICENSE("GPL");
MODULE_VERSION("0:1.0");

