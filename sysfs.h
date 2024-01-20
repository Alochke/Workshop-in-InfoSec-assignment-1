#ifndef SYSFS
#define SYSFS

#include <linux/kernel.h> /* We're doing kernel work. */
#include <linux/module.h> /* We're adding a kernel module. */
#include <linux/fs.h> /* This and the next header are for sysfs kernel api to work. */
#include <linux/device.h> 
#include "hw2secws.h"

#define NAME_OF_DEVICE "fw_device"
#define NAME_OF_CLASS "fw_class"
#define SUCCESS 0
#define TRUE 1
#define NUMBER_OF_BYTES_TRANSFERED 2 * sizeof(unsigned int) /* Number of bytes transferd inside the display function in sysfs.c from kernel space to user-space. */
#define SYSFS_MINOR 0 // To use with MKDEV to generate a number for a sysfs device.

/* The next enum is for the clean_up function in sysfs.c. Items represent the state of the sysfs initialization the module is currently at. */
enum stage{FIRST = 0, SECOND = 1, THIRD = 2, FOURTH =3};

int sysfs_init(void);
void sysfs_exit(void);

#endif