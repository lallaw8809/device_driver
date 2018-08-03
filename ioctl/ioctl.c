/* Program to write a program to transfer the data between 
 * user space and kernel space using ioctl
 * 
 * Author : Lal Bosco Lawrence   
 * Date   : 03-Aug-2018
 */

#include <linux/init.h>     // Macros used to mark up functions e.g. __init __exit
#include <linux/module.h> 
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>       //File system structure
#include <linux/uaccess.h>  //Required for the copy to user function
#include <linux/ioctl.h>

#include "ioctl.h"

#define DEVICE_NAME "ioctl_driver"

static int    majorNumber;
static int    count = 0;
int 	      value;

// The prototype functions for the character driver
static int     char_dev_open(struct inode *, struct file *);
static int     char_dev_release(struct inode *, struct file *);

#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,35))
static int my_ioctl(struct inode *i, struct file *f, unsigned int cmd, unsigned long arg)
#else
static long my_ioctl(struct file *f, unsigned int cmd, unsigned long arg)
#endif
{
	switch(cmd)
	{
		/* MACRO to identify write */
		case WR_VALUE:
			copy_from_user(&value ,(int32_t*) arg, sizeof(value));
			printk(KERN_INFO "Value received from user space = %d\n", value);
			break;
		/* MACRO to identify read */
		case RD_VALUE:
			printk(KERN_INFO "Value send to user space = %d\n", value);
			copy_to_user((int32_t*) arg, &value, sizeof(value));
			break;
	}

	return 0;
}

/* Devices are represented as a file structure in the kernel. 
 * The file_operations structure is defined in linux/fs.h
 * Char devices usually implement open, read, write and release calls
 */
static struct file_operations fops =
{
	.open    = char_dev_open,
	.release = char_dev_release,
	
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,35))
	.ioctl = my_ioctl
#else
	.unlocked_ioctl = my_ioctl
#endif
};

/* Init module */
static int __init char_driver_init(void) /* Constructor */
{
	/* Register the char driver 
	 * Dynamically allocate a major number for the device
	 * Major device number or 0 for dynamic allocation 
	 */
	majorNumber = register_chrdev(0, DEVICE_NAME, &fops);
	if (majorNumber<0){
		printk(KERN_ALERT "Failed to register the driver with a major number\n");
		return majorNumber;
	}
	printk(KERN_INFO "Successfully register the [%s] driver with major number %d\n" \
					,DEVICE_NAME, majorNumber);
	printk(KERN_INFO "Create a device node...\n");
	printk(KERN_INFO "----------------------------------------------------\n");
	printk(KERN_INFO "sudo mknod /dev/%s c %d 0\n",DEVICE_NAME, majorNumber);
	printk(KERN_INFO "-----------------------------------------------------\n");

	return 0;
}

static void __exit char_driver_exit(void) /* Destructor */
{
	/* Unregister the driver with a major number */
	unregister_chrdev(majorNumber, DEVICE_NAME);
	printk(KERN_INFO "Unregistered the driver!\n");
}

/*  This function is called whenever device is opened from user space
 *  This will only increment the counter in this case.
 *  @param inodep A pointer to an inode object (defined in linux/fs.h)
 *  @param fp A pointer to a file object (defined in linux/fs.h)
 */
static int char_dev_open(struct inode *inodep, struct file *fp){
	count++;
	printk(KERN_INFO "Device has been opened %d time(s)\n", count);
	return 0;
}

/** The device release function that is called whenever the device is closed/released
 *  from the user spave
 *  @param inodep A pointer to an inode object (defined in linux/fs.h)
 *  @param fp is a pointer to a file object (defined in linux/fs.h)
 */
static int char_dev_release(struct inode *inodep, struct file *fp){
	printk(KERN_INFO "Device is successfully closed\n");
	return 0;
}

module_init(char_driver_init);
module_exit(char_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Lal Bosco");
MODULE_DESCRIPTION("ioctl Character Driver");


