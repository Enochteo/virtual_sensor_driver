
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/random.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>
#include <linux/device.h>

#define DEVICE_NAME "virtual_sensor"
#define CLASS_NAME "vsensor"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Enoch Owoade");
MODULE_DESCRIPTION("A Linux char driver that simulates a virtual sensor");
MODULE_VERSION("0.2");

static int major;
static struct class* sensor_class = NULL;
static struct device* sensor_device = NULL;
static int dev_open(struct inode*, struct file*);
static ssize_t dev_read(struct file*, char*, size_t, loff_t*);
static int dev_release(struct inode*, struct file*);



static struct file_operations fops = {
	.open = dev_open,
	.read = dev_read,
	.release = dev_release,
};

static int __init virtual_sensor_init(void) {
	printk(KERN_INFO "VirtualSensor: Initializing...\n");
	// Register char device dynamically
	major = register_chrdev(0, DEVICE_NAME, &fops);
	if (major < 0) {
		printk(KERN_ALERT "VirtualSensor failed to register a major number\n");
		return major;
	}
	sensor_class = class_create(CLASS_NAME);
	if (IS_ERR(sensor_class)) {
		unregister_chrdev(major, DEVICE_NAME);
		printk(KERN_ALERT "Failed to register device class\n");
		return PTR_ERR(sensor_class);
	}
	
	sensor_device = device_create(sensor_class, NULL, MKDEV(major, 0), NULL, DEVICE_NAME);
	if (IS_ERR(sensor_device)) {
		class_destroy(sensor_class);
		unregister_chrdev(major, DEVICE_NAME);
		printk(KERN_ALERT "Failed to create device\n");
		return PTR_ERR(sensor_device);
	}
	printk(KERN_INFO "VirtualSensor: Registered /dev/%s with major %d\n", DEVICE_NAME, major);
	return 0;
}

// Module Exit
static void __exit virtual_sensor_exit(void) {
	device_destroy(sensor_class, MKDEV(major, 0));
	class_unregister(sensor_class);
	class_destroy(sensor_class);
	unregister_chrdev(major, DEVICE_NAME);
	printk(KERN_INFO "VirtualSensor: module unloaded.\n");
}


// File Ops
static int dev_open(struct inode* inodep, struct file* filep) {
	printk(KERN_INFO "VirtualSensor Device opened\n");
	return 0;
}

static ssize_t dev_read(struct file* filep, char* buffer, size_t len, loff_t* offset){
	int fake_value;
	static char sensor_data[64];

	if (*offset > 0) 
 		return 0;

	get_random_bytes(&fake_value, sizeof(fake_value));
	fake_value = (fake_value % 100) + 1;

	int bytes = snprintf(sensor_data, sizeof(sensor_data), "Sensor reading: %d\n", fake_value);
	if (copy_to_user(buffer, sensor_data, bytes) != 0) {
		printk(KERN_ERR "VirtualSensor; Failed to send data to user\n");
		return -EFAULT;
}
	*offset += bytes;
	printk(KERN_INFO "VirtualSensor: Sent %d bytes: %s", bytes, sensor_data);
	return bytes;
}


static int dev_release(struct inode* inodep, struct file* filep) {
	printk(KERN_INFO "VirtualSensor: Device successfully closed\n");
	return 0;
}


module_init(virtual_sensor_init);
module_exit(virtual_sensor_exit);

	


