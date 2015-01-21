#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <asm/uaccess.h>

#include <linux/interrupt.h>
#include <linux/sched.h>
#include <linux/wait.h>
#include <linux/ioport.h>
#include <asm/io.h>
#include <linux/delay.h>	

#include "Serial_reg_set.h"

#define serial MySerialDevice

//structure for device specific
struct resource *serial;

//Function Prototypes
int serial_open(struct inode *inode, struct file *filp);
int serial_release(struct inode *indoe, struct file *filp);
ssize_t serial_write(struct file *filp, const char __user *Ubuff, size_t count, loff_t  *offp);
ssize_t serial_read(struct file *filp, char __user *Ubuff, size_t count, loff_t  *offp);

//Serial Interrupt Handler declaration

irqreturn_t serial_interrupt(int , void * );

//Structure that defines the operations that the driver provides
struct file_operations fops = 
{
	.owner   = THIS_MODULE,
	.open    = serial_open, 
	.read    = serial_read,
	.write   = serial_write,
	.release = serial_release,
};

//Structure for a character driver
struct cdev *my_cdev;

dev_t Mydev;


static int __init SerialDevice_init(void)
{
	int result;
	int MAJOR,MINOR;



	result = alloc_chrdev_region(&Mydev , 0 , 1 , "MySerialDevice");
	if(result<0)
	{
		printk(KERN_ALERT "\nThe Region requested for is not obtainable\n");
		return(-1);
	}
	MAJOR=MAJOR(Mydev);
	MINOR=MINOR(Mydev);
	printk("\nThe Major Number is %d...THe Minor Number is %d\n",MAJOR,MINOR);
	
	
	
	my_cdev = cdev_alloc();//allocate memory to Serial Device structure
	my_cdev->ops = &fops;//link our file operations to the char device

	result=cdev_add(my_cdev,Mydev,1);//Notify the kernel abt the new device
	if(result<0)
	{
		printk(KERN_ALERT "\nThe Serial Devide has not been created......\n");
		return (-1);
	}

	
	return 0;
}


void __exit SerialDevice_exit(void)
{

	unregister_chrdev_region(Mydev,1);//unregister the device numbers and the device created
	cdev_del(my_cdev);
	printk(KERN_ALERT "\nI have unregistered the stuff that was allocated.....Goodbye for ever.....\n");
	return;
}


int serial_open(struct inode *inode, struct file *filp)
{
	int result;
//	int retval;
	printk(KERN_ALERT "\nThis is the Kernel....Open Call\n");
	serial = request_region( BASE , 8 , "MySerialDevice");


	//auto configuration of the serial port BR=9600, 8bit , no parity , 1 stop bit .

	
	outb_p(0x00,LCR);	//set DLAB=0 for  accessing IER register
	outb_p(0x00,IER);	//Disable the Interrupts	


	outb_p(0x80,LCR);	//Setting DLAB = 1 for setting the Baudrate

	outb_p(0x0c,BaudRegL);	//Low Byte for 9600 LB 0x0c HB 0x00
	outb_p(0x00,BaudRegH);	//High Byte


	outb_p(0x03,LCR);	//setting 8bits Noparity 1Stop bit for the device	

	outb_p(0x87,FCR);	/*Enable FIFO bit 0 ,clear Tx n Rx FIFO's bit(1 n 2)
				  Interrupt Trigger Level for 8 bytes bit 7:6 = 1 0 */

	outb_p(0x0b,MCR);	//Enable Aux output 2, Force Req To Send , Force Data Terminal Ready 		
	
	//Request for the irq (COM1 i.e irq is 4)
		
	
	result = request_irq(Serial_IRQ, serial_interrupt ,IRQF_DISABLED, "MySerialDevice" , NULL );

	if(result)
	 {
                printk(KERN_INFO"parport:can't get assigned %d\n",Serial_IRQ);
                return -1;
        }
        else
                printk(KERN_INFO"parport: get assigned %d\n",Serial_IRQ);
	  		
	//Enable the Tx and Rx interrupts
	outb_p(0x03,IER);

	return 0;
}


//Close System Call
int serial_release(struct inode *indoe, struct file *filp)
{

	free_irq(Serial_IRQ , NULL);
	release_region(BASE , 8);
	printk(KERN_ALERT "\nThis is the release method of my Serialacter Driver\n");
	return 0;
} 


//Write Functionality
ssize_t serial_write(struct file *filp, const char __user *Ubuff, size_t count, loff_t  *offp)
{
	char Kbuff[80];
	int result;
	ssize_t retval;
		
	result=copy_from_user((char *)Kbuff , (char *)Ubuff , count);

	if(result == 0)
	{
		printk(KERN_ALERT "\nMessage from the user......\n %s\n",Kbuff);
                printk(KERN_ALERT "\n %d bytes of data Successfully Written.....\n",count);
		
		outb_p(0xFF ,TxBuf); 
		mdelay(1000);
		outb_p(0xFF ,TxBuf); 
		mdelay(1000);
		outb_p(0xFF ,TxBuf); 
		mdelay(1000);
		outb_p(0xFF ,TxBuf); 
                retval=count;
                return retval;

	}
	else
	{
		printk(KERN_ALERT "\n Error Writing Data\n");
                retval=-EFAULT;
                return retval;

	}
}


//read Functionality	
ssize_t serial_read(struct file *filp, char __user *Ubuff, size_t count, loff_t  *offp)
{
	char Kbuff[]="Iam sending from Kernel buffer";
	int result;
	ssize_t retval;

	result=copy_to_user((char *)Ubuff,(const char *)Kbuff,sizeof(Kbuff));
	if(result == 0)
	{
		printk(KERN_INFO"copy to user space is successfully written%d\n",sizeof(Kbuff));
		retval = sizeof(Kbuff);
		return retval;
	}	
	else
        {
                printk(KERN_ALERT"\n Error Writing Data to User\n");
                retval=-EFAULT;
                return retval;
        }

	
}

//irqreturn_t serial_interrupt(int irq , void *dev_id , struct pt_regs *regs)
irqreturn_t serial_interrupt(int irq , void *dev_id )
{
	unsigned char LineStatusReg;
//	unsigned int IntIdentReg;

	LineStatusReg = inb(LSR);
//	IntIdentReg = (inb(IER));

	printk(KERN_INFO"Iam in interrupt Handler with IRQ %d\n",irq);
	printk(KERN_INFO"Interrupt Identification Reg %x\tLine Status Reg%x\n",(inb(IER)),LineStatusReg);

	if(LineStatusReg & 0x01)
//	if((inb(LSR)) & 0x01)
	{
		printk(KERN_INFO"Data is in Reciever Buffer\n");
		printk(KERN_INFO"Data Rx is %x\n",inb_p(RxBuf));
	}	
	
	else if((inb(IER)) & 0x02)
//	else if(IntIdentReg & 0x02)
	{
		printk(KERN_INFO"Transmit buffer is Empty\n");
	}


	return IRQ_HANDLED;
}
	
//Module over ride functions
module_init(SerialDevice_init);
module_exit(SerialDevice_exit);
MODULE_LICENSE("GPL");
