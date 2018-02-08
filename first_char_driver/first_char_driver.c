/* Program to write basic character driver
 * 
 * Author : Lal Bosco Lawrence   
 * Date   : 08-Feb-2018
 */

#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>

#define DEVICE_NAME "CharDriver"

static dev_t major;            // Global variable for the first device number
unsigned int baseminor    = 0; // First of the requested range of minor number
unsigned int num_of_minor = 1; // Number of minor numbers required 

static int __init char_driver_init(void) /* Constructor */
{
	int ret;

        /* Register charatcer driver with device name
	 * Return : 0   : Success
	 *	    -ve : Fail
	 */
	if ((ret = alloc_chrdev_region(&major, baseminor, num_of_minor, DEVICE_NAME)) < 0)
	{
		printk(KERN_INFO "Unable to register the driver\n");
		return ret;
	}
	printk(KERN_INFO "Driver is Registered\n");
	return 0;
}

static void __exit char_driver_exit(void) /* Destructor */
{
	/* Unregistered the driver */
	unregister_chrdev_region(major, num_of_minor);
	printk(KERN_INFO "Driver is Unregistered\n");
}

module_init(char_driver_init);
module_exit(char_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Lal Bosco");
MODULE_DESCRIPTION("First Character Driver");
