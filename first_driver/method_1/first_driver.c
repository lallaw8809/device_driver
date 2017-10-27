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
int init_module()
{
	printk(KERN_INFO "Module is inserted\n");
	return 0;
}

/* LKM module exit */
void cleanup_module()
{
	printk(KERN_INFO "Removed the module\n");
}
