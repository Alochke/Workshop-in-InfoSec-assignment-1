#include "sysfs.h"

MODULE_SIGNATURE

static int major_number;
static struct class* sysfs_class = NULL;
static struct device* sysfs_device = NULL;

extern unsigned int accepted;
extern unsigned int dropped;

static struct file_operations fops = {
	.owner = THIS_MODULE
};

/*
	The implementation of show.
	Puts inside of address- buf the values of accepted and dropped,
	which are defined in hw2secws.h and then returns the number of bytes written succefuly.
	On failure, zeros the values at adresses [buff, buf + 2 * sizeof(unsigned int)).
*/
ssize_t display(struct device *dev, struct device_attribute *attr, char *buf)
{
	unsigned int *uibuf_accepted = (unsigned int*) buf;
	unsigned int *uibuf_dropped = (unsigned int*) (buf + sizeof(unsigned int));
	*uinbuf_accepted = accepted;
	*uinbuf_dropped = dropped;
	return NUMBER_OF_BYTES_TRANSFERED;
}

/*
	The implementation of store.
	Zeros accepted and dropped which are defined in hw2secws.h.
*/
ssize_t modify(struct device *dev, struct device_attribute *attr, const char *buf, size_t count){
	accepted = 0;
	dropped = 0;
	return 0;
}

// Links the device with display as show and modify as store (both funtions defined in sysfs.c), in a sysfs manner.
static DEVICE_ATTR(sysfs_att, S_IWUSR | S_IRUSR , display, modify);

/*
	Cleans the sysfs part of the module.

	Parameters:
    - stg (enum stage): A designated enum's member that represents the stage of initialization the sysfs part of the module is in.
*/
void cleanup(enum stage stg)
{
	// We use the enum- stage, defined in sysfs.h to choose action based on the state of the sysfs initialization the module is currently at. 
	switch (stg)
	{
	case FOURTH:
		device_remove_file(sysfs_device, (const struct device_attribute *)&dev_attr_sysfs_att.attr);
	case THIRD:
		device_destroy(sysfs_class, MKDEV(major_number, SYSFS_MINOR));
	case SECOND:
		class_destroy(sysfs_class);
	case FIRST:
		unregister_chrdev(major_number, NAME_OF_DEVICE);
	}
}

/*
	Initializes the sysfs part of the module.
*/
int sysfs_init(void)
{
	int err = 0; // Will be used to store the value returned from device_create_file, so we'll be able to return it in case of an error.

	//create char device
	ERR_CHECK((major_number = register_chrdev(0, NAME_OF_DEVICE, &fops)) < 0, printk(KERN_ERR "register_chrdev failed."), major_number)

	//create sysfs class
	ERR_CHECK(IS_ERR(sysfs_class = class_create(THIS_MODULE, NAME_OF_CLASS)), cleanup(FIRST); printk(KERN_ERR "class_create failed."), (int) sysfs_class)

	//create sysfs device
	ERR_CHECK(IS_ERR(sysfs_device = device_create(sysfs_class, NULL, MKDEV(major_number, 0), NULL, NAME_OF_CLASS "_" NAME_OF_DEVICE)), cleanup(SECOND); printk(KERN_ERR "device_create failed"), (int) sysfs_device)

	//create sysfs file attributes
	ERR_CHECK((err = device_create_file(sysfs_device, (const struct device_attribute *)&dev_attr_sysfs_att.attr)), cleanup(THIRD); printk(KERN_ERR "device_create_file failed"), err)
	
	return SUCCESS;
}

/*
	A wrapper function of cleanup, that serves as an abstraction layer of the clean up process of the sysfs part of the module.
	In case the initialization of that part of the module is done.
*/
void sysfs_exit(void)
{
	cleanup(FOURTH);
}