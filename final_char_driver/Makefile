obj-m = final_char_driver.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) modules
	gcc -o app application.c

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) clean
	rm -rf *~ app
 

