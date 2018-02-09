/* User space program to communicate with linux kernel module
 * It pass the string to the LKM and reads the response from LKM.
 * user space and kernel space
 * 
 * Author : Lal Bosco Lawrence   
 * Date   : 09-Feb-2018
 */

#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>

#define DEVICE_NAME "/dev/CharDriver"

#define BUFFER_LENGTH 256
static char rcv_msg[BUFFER_LENGTH];

int main(){
	int ret, fd;
	char stringToSend[BUFFER_LENGTH];

	/* Opne the char device */
	fd = open(DEVICE_NAME, O_RDWR);
	if (fd < 0){
		printf("Failed to open the device...\n");
		return errno;
	}

	printf("Type in a short string to send to the kernel module:\n");
	scanf("%[^\n]%*c", stringToSend);
	printf("Writing message to the device [%s].\n", stringToSend);
	/* Write a string into LKM */
	ret = write(fd, stringToSend, strlen(stringToSend)); 
	if (ret < 0){
		printf("Failed to write a message to the device.");
		return errno;
	}

	printf("Press ENTER to read back from the char device...\n");
	getchar();

	printf("Reading from the device...\n");
	ret = read(fd, rcv_msg, BUFFER_LENGTH);        // Read the response from the LKM
	if (ret < 0){
		printf("Failed to read the message from the char device.");
		return errno;
	}
	printf("The received message is: [%s]\n", rcv_msg);
	printf("End of the program\n");
	return 0;
}
