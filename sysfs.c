#include "sysfs.h"

static int major_number;
static struct class* sysfs_class = NULL;
static struct device* sysfs_device = NULL;

static struct file_operations fops = {
	.owner = THIS_MODULE
};


/*
	The implementation of show.
	Puts inside of buf the values of accepted and dropped, which are defined in hw2secws.h and then returns the number of bytes written succefuly.
*/
ssize_t display(struct device *dev, struct device_attribute *attr, char *buf)
{
	unsigned int temp = 0;
	int bool1 = 0;
	int bool2 = 0;
	INT_TRANSFER(temp, buf, bool1)
	INT_TRANSFER(temp, buf + sizeof(unsigned int), bool2)
	if(bool1 && bool2)
	{
		user_put((int) accepted, buf)
		user_put((int) dropped + sizeof(unsigned int), buf)
	}
	return NUMBER_OF_BYTES_TRANSFERED
}

/*
	The implementation of store.
	zeros accepted and dropped which are defined in hw2secws.h.
*/
ssize_t modify(struct device *dev, struct device_attribute *attr, const char *buf, size_t count){
	accepted = 0;
	dropped = 0;
	return 0;
}

static DEVICE_ATTR(sysfs_att, S_IWUSR | S_IRUSR , display, modify);

void cleanup(stage stg)
{
	switch (stg)
	{
	case fourth:
		device_remove_file(sysfs_device, (const struct device_attribute *)&dev_attr_sysfs_att.attr);
	case third:
		device_destroy(sysfs_class, MKDEV(major_number, SYSFS_MINOR));
	case second:
		class_destroy(sysfs_class);
	case first:
		unregister_chrdev(major_number, "FW_Device");
	}
}

int sysfs_example_init(void)
{
	int err = 0; // Will be used store the value returned from device_create_file, so we'll be able to return it in case of an error.

	//create char device
	ERR_CHECK((major_number = register_chrdev(0, "FW_Device", &fops)) < 0, printk(KERN_ERR "register_chrdev failed."), major_number)

	//create sysfs class
	ERR_CHECK(IS_ERR(sysfs_class = class_create(THIS_MODULE, "FW_class")), cleanup(first); printk(KERN_ERR "class_create failed."), sysfs_class)
	
	//create sysfs device
	ERR_CHECK(IS_ERR(sysfs_device = device_create(sysfs_class, NULL, MKDEV(major_number, 0), NULL, "FW_class" "_" "FW_Device")), cleanup(second); printk(KERN_ERR "device_create failed"), sysfs_device)

	//create sysfs file attributes
	ERR_CHECK((err = device_create_file(sysfs_device, (const struct device_attribute *)&dev_attr_sysfs_att.attr)), cleanup(third); printk(KERN_ERR "device_create_file failed"), err)
	
	return SUCCESS;
}

void sysfs_exit(void)
{
	cleanup(fourth);
}
