obj-m += fw.o

# Object files needed for linkage of the module.
fw-y := hw2secws.o sysfs.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean