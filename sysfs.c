#include "sysfs.h"

MODULE_SIGNATURE

static int major_number;
static struct class* sysfs_class = NULL;
static struct device* sysfs_device = NULL;

static struct file_operations fops = {
	.owner = THIS_MODULE
};

/*
	The implementation of show.
	Puts inside of address- buf the values of accepted and dropped,
	which are defined in hw2secws.h and then returns the number of bytes written succefuly.

	Returns: The number of bytes transfered.
*/
ssize_t display(struct device *dev, struct device_attribute *attr, char *buf)
{
	unsigned int *uibuf_accepted = (unsigned int*) buf;
	unsigned int *uibuf_dropped = (unsigned int*) (buf + sizeof(unsigned int));

	/* 
		I intentionally write manually into the buffer. A research I have conducted showed that buf is within the kernel space allocated by the sysfs API and therefore this is totaly ok.
		Sources: https://docs.kernel.org/filesystems/sysfs.html Also, the fact that the address of buf is diffrent than the address of a buffer given to a write system call that writes to
		the sysfs attribute and the fact that put_user fails to transfer data to buf.
		Also, becuase I want to transfer the actual values of accepted and dropped to the buffer,
		using a transferring fuctions that uses string formatting would make things unnecessarily complex.
	*/
	*uibuf_accepted = accepted;
	*uibuf_dropped = dropped;
	return SHOW_BYTES;
}

/*
	The implementation of store.
	Zeros accepted and dropped which are defined in hw2secws.h.

	Returns: 0, i.e, the bytes from the sysfs buffer modified, according to standard practice with the sysfs api.
*/
ssize_t modify(struct device *dev, struct device_attribute *attr, const char *buf, size_t count){
	accepted = 0;
	dropped = 0;
	return STORE_BYTES;
}

// Links the device with display as show and modify as store (both funtions defined in sysfs.c), in a sysfs manner.
static DEVICE_ATTR(sysfs_att, S_IWUSR | S_IRUSR, display, modify);

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