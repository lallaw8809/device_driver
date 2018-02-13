/* Program to write a character driver, create a device node and pass data between
 * user space and kernel space
 * 
 * Author : Lal Bosco Lawrence   
 * Date   : 13-Feb-2018
 */

#include <linux/init.h>     // Macros used to mark up functions e.g. __init __exit
#include <linux/module.h> 
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/device.h>
#include <linux/fs.h>       //File system structure
#include <linux/uaccess.h>  //Required for the copy to user function


#define DEVICE_NAME "CharDriver"
#define CLASS_NAME  "Char-class"

static int    majorNumber;
static int    count = 0;
static char   message[256] = {0}; //Memory for the string that is passed from userspace
static short  size_of_message;
static struct class*  charClass  = NULL; ///< The device-driver class struct pointer
static struct device* charDevice = NULL; ///< The device-driver device struct pointer

// The prototype functions for the character driver
static int     char_dev_open(struct inode *, struct file *);
static int     char_dev_release(struct inode *, struct file *);
static ssize_t char_dev_read(struct file *, char *, size_t, loff_t *);
static ssize_t char_dev_write(struct file *, const char *, size_t, loff_t *);

/* Devices are represented as a file structure in the kernel. 
 * The file_operations structure is defined in linux/fs.h
 * Char devices usually implement open, read, write and release calls
 */
static struct file_operations fops =
{
	.open    = char_dev_open,
	.read    = char_dev_read,
	.write   = char_dev_write,
	.release = char_dev_release,
};

static int __init char_driver_init(void) /* Constructor */
{
	/* Register the char driver 
	 * Dynamically allocate a major number for the device
	 */
	majorNumber = register_chrdev(0, DEVICE_NAME, &fops);
	if (majorNumber<0){
		printk(KERN_ALERT "Failed to register the driver with a major number\n");
		return majorNumber;
	}
	printk(KERN_INFO "Successfully register the [%s] driver with major number %d\n" \
					,DEVICE_NAME, majorNumber);

	/* Create a device node */
	/* Device node can be handled by udev. 
	 * Udev is used to create/remove the device node under /dev directory dynamically.
	 */

	/* Create a device class 
	 * This will create the struct class for our device driver under /sys/class/
	 */
	charClass = class_create(THIS_MODULE, CLASS_NAME);
	if (IS_ERR(charClass)){           // Validation of error
		unregister_chrdev(majorNumber, DEVICE_NAME);
		printk(KERN_ALERT "Failed to register device class\n");
		return -1;
	}

	/* Create a device : Create the device node /dev/ */
	charDevice = device_create(charClass, NULL, MKDEV(majorNumber, 0), NULL, DEVICE_NAME);
	if (IS_ERR(charDevice)){          // Validation of error
		class_destroy(charClass);
		unregister_chrdev(majorNumber, DEVICE_NAME);
		printk(KERN_ALERT "Failed to create the device\n");
		return -1;
	}

	return 0;
}

static void __exit char_driver_exit(void) /* Destructor */
{
	/* Remove the device */
	device_destroy(charClass, MKDEV(majorNumber, 0));
	/* Unregister the device class */
	class_unregister(charClass);
	/* Remove the device class */
	class_destroy(charClass);
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

/*  This function is called whenever device is being read from user space i.e. data is
 *  being sent from the device to the user. In this case is uses the copy_to_user() function to
 *  send the buffer string to the user and captures any errors.
 *  @param fp is a pointer to a file object (defined in linux/fs.h)
 *  @param buffer The pointer to the buffer to which this function writes the data
 *  @param len The length of the b
 *  @param offset The offset if required
 */
static ssize_t char_dev_read(struct file *fp, char *buffer, size_t len, loff_t *offset){
	int error = 0;
	// copy_to_user has the format ( * to, *from, size) and returns 0 on success
	error = copy_to_user(buffer, message, size_of_message);

	if (error==0){            // if true then have success
		printk(KERN_INFO "Sent %d characters to the user\n", size_of_message);
		return 0;  // clear the position to the start and return 0
	}
	else {
		printk(KERN_INFO "Failed to send %d characters to the user\n", error);
		return -EFAULT;
	}
}

/** This function is called whenever the device is being written to from user space i.e.
 *  data is sent to the device from the user. The data is copied to the message[] array in this
 *  LKM using the sprintf() function along with the length of the string.
 *  @param fp A pointer to a file object
 *  @param buffer The buffer to that contains the string to write to the device
 *  @param len The length of the array of data that is being passed in the const char buffer
 *  @param offset The offset if required
 */
static ssize_t char_dev_write(struct file *fp, const char *buffer, size_t len, loff_t *offset){
	// sprintf(message, "%s(%zu letters)", buffer, len);   
	copy_from_user( message, buffer, len);   // appending received string with its length
	size_of_message = strlen(message);                 // store the length of the stored message
	printk(KERN_INFO "Received %zu characters from the user\n", len);
	return len;
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
MODULE_DESCRIPTION("First Character Driver");

//Reference
//http://derekmolloy.ie/writing-a-linux-kernel-module-part-2-a-character-device/


