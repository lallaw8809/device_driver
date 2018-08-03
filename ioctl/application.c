/* User space application to communicate with linux kernel module using ioctl
 * It pass the integer to the LKM and reads the response from LKM.
 * 
 * Author : Lal Bosco Lawrence   
 * Date   : 03-Aug-2018
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include<sys/ioctl.h>

#include "ioctl.h"

#define DEVICE_NAME "/dev/ioctl_driver"
 
int main()
{
        int fd;
        int32_t value, number;
 
        printf("Opening Driver\n");
        fd = open(DEVICE_NAME, O_RDWR);
        if(fd < 0) {
                printf("Unable to open device file...\n");
                return 0;
        }
 
        printf("Enter the Value to send\n");
        scanf("%d",&number);
        printf("Writing Value to Driver\n");
        ioctl(fd, WR_VALUE, (int32_t*) &number); 
 
        printf("Reading Value from Driver\n");
        ioctl(fd, RD_VALUE, (int32_t*) &value);
        printf("Value is %d\n", value);
 
        printf("Closing Driver\n");
        close(fd);
}
