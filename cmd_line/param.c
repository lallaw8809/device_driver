/* Program for pass parameters from cmd line (module)
 * 
 * Author : Lal Bosco Lawrence   
 * Date   : 05-Feb-2018
 */
#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/moduleparam.h>

/* Global declaration */
static short int count = 1;
static int my_array[100]={0,0};

static char *name    = "lal Bosco";
static int int_value = 1;
static int att       = 0;

/*  The module_param() macro takes 3 arguments: 

    arg1 : The name of the variable.
    arg2 : Its data type
    arg3 : Permissions for the corresponding file in sysfs. 
*/
module_param(int_value, int,   S_IRUGO);
module_param(count,     short, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);;
module_param(name,      charp, S_IRUGO);
module_param_array(my_array, int,&att, S_IRUGO);

/* LKM module initialiation */
static int hello_init(void)
{
	int index;
	/* Print the command line argument array */
	for (index = 0; index < count; index++)
	{
		printk(KERN_INFO "myintArray[%d] = %d\n", index, my_array[index]);
	}

	/* Print the command line argument int,char pointer */
	printk(KERN_INFO "Integer Value = %d\n",int_value);
	printk(KERN_INFO "Char Pointer  = %s\n",name);

	return 0;
}

/* LKM module exit */
static void hello_exit(void)
{
	printk(KERN_INFO "Removed the MODULE\n");
}

module_init(hello_init);
module_exit(hello_exit);
MODULE_LICENSE("GPL");
