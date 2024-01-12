#ifndef SYSFS
#define SYSFS

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/uaccess.h>  /* for get_user put_user */
#include "hw2secws.h"

#define TRUE 1
#define ERROR -1
#define SUCCESS 0
#define NUMBER_OF_BYTES_TRANSFERED 2 * sizeof(unsigned int) /* Number of bytes transferd inside the display function in sysfs.c from kernel space to user-space. */
#define SYSFS_MINOR 0 // To use with MKDEV to generate a number for a sysfs device.

/* The next enum is for the clean_up function in sysfs.c. */
enum stage{first = 0, second = 1, third = 2, fourth =3};

int sysfs_init(void);
void sysfs_exit(void);

/* 
    A safe way to transfer an int from userspace to kernel sapce,
    Sets flag to TRUE on success.
*/
#define INT_TRANSFER(kernel_buf, user_buf , flag)   \
    if(!put_user((int) kernel, buff))               \
	{                                               \
		flag = TRUE                                 \
	}                                               \

#endif