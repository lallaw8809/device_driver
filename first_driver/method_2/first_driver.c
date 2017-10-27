/* Program for basic device driver (module)
 * 'Hello world' Program
 * 
 * Author : Lal Bosco Lawrence   
 * Date   : 27-oct-2017
 */

/* Module : Each piece of that can be added into a kernel at run time.
 * Loadable kernel module (LKM) that can display a message
 * in the /var/log/kern.log file when the module is loaded and removed.
 * We can also see the print log using $dmesg
 * Insert module
 *	insmod first_driver.ko
 * Remove the module
 * 	rmmod first_driver
*/

#include <linux/init.h>             // Macros used to mark up functions e.g., init_module
#include <linux/module.h>           // Core header for loading LKMs into the kernel
#include <linux/kernel.h>           // Contains types, macros, functions for the kernel

/* LKM module initialiation */
static int __init first_driver_init(void)
{
	printk(KERN_INFO "Module is inserted\n");
	return 0;
}

/* LKM module exit */
static void __exit first_driver_exit(void)
{
	printk(KERN_INFO "Removed the module\n");
}

/*  A module must use the module_init() and module_exit() macros from linux/init.h, which
 *  identify the initialization function at insertion time and the cleanup function (as
 *  listed above)
 */
module_init(first_driver_init);
module_exit(first_driver_exit);

MODULE_LICENSE("GPL");              // The license type 
MODULE_AUTHOR("Lal Bosco");      // The author name
MODULE_DESCRIPTION("A simple Linux driver codr.");  // The description 
MODULE_VERSION("0.1");              // The version of the module

