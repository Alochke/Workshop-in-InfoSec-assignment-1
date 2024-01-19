obj-m += fw.o

#append other source files except hw2secws.c which would be include by default
fw-y := hw2secws.o sysfs.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean