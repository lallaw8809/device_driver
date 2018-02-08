/* Program to statically allocate the major and minor number 
 * for character driver
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
#define MY_MAJOR       240
#define MY_MINOR         0

static dev_t first;            // Global variable for the first device number
unsigned int num_of_minor = 1; // Number of minor numbers

static int __init char_driver_init(void) /* Constructor */
{
	int ret;
	/* Statically allocate the major and minor number */
	first = MKDEV(MY_MAJOR, MY_MINOR);
	/* Register the charatcer driver with device name
	 * Major number will allocated at Statically
	 * Return : 0   : Success
	 *	    -ve : Fail
	 */
	if( (ret = register_chrdev_region(first, num_of_minor, DEVICE_NAME)) < 0)
	{
		printk(KERN_INFO "Unable to register the char driver\n");
		return ret;	
	}

	printk(KERN_INFO "%s is registered with <Major, Minor>: <%d, %d>\n", \
			DEVICE_NAME, MAJOR(first), MINOR(first));
	return 0;
}

static void __exit char_driver_exit(void) /* Destructor */
{
	first = MKDEV(MY_MAJOR, MY_MINOR);
	/* Unregister the character driver */
	unregister_chrdev_region(first, num_of_minor);
	printk(KERN_INFO "Unregister char driver\n");
}

module_init(char_driver_init);
module_exit(char_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Lal Bosco");
MODULE_DESCRIPTION("Our First Character Driver");
