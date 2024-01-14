obj-m += hw2secws.o

#append other source files except hw2secws.c which would be include by default
hw2secws-objs := sysfs.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean