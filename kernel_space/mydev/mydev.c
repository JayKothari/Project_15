#include<linux/module.h>
#include<linux/string.h>
#include<linux/fs.h>
#include<linux/uaccess.h>
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("DEVICE DRIVER DEMO");
MODULE_AUTHOR("JAY AND JAY");
//module_init(init_module);
//module_exit(cleanup_module);
	static char msg[100]={0};
	static short readp=0;
	static int times=0;

	static int dev_open(struct inode *,struct file *);
	static int dev_rls(struct inode *,struct file *);

	static ssize_t dev_read(struct file *,char *,size_t ,loff_t *);
	static ssize_t dev_write(struct file *,char *,size_t ,loff_t *);

	static struct file_operations fops =
	{
		.read=dev_read,
		.open=dev_open,
		.write=dev_write,
		.release=dev_rls,
	};
       
	int init_module(void)
	{
		int t=register_chrdev(89,"mydev",&fops);
		if(t<0)
		{
			printk("DEVICE REGISTRATION FAILED\n");
		}
		else
		{
			printk("device register\n");
		}
		return t;
		}


	void cleanup_module(void)
	{
		unregister_chrdev(89,"mydev");
	}


	static int dev_open(struct inode *inod,struct file *fil)
	{
		times=times+1;
		printk("device open");
		return 0;
	}	


	static ssize_t dev_read(struct file *filp,char *buff,size_t len,loff_t *off)
	{
		short count=0;
		while(len && (msg[readp]!=0))
		{
		put_user(msg[readp],buff++);
		count++;
		len--;
		readp++;
		}
	return count;
	}

	static int dev_rls(struct inode *inod,struct file *fil)
	{
		printk("device closed");
	return 0;
	}


	 static ssize_t dev_write(struct file *filp,char *buff,size_t len,loff_t *off)
	 {
                printk("i am write method");
		short ind = len-1;
                short count=0;
                memset(msg,0,100);
		readp=0;
		while(len>0)
		{
			msg[count++]=buff[ind--];
	//		get_user(msg[readp],buff++);
	//		len--;
	//		readp++;

		}
		
                return count;
       	 }

