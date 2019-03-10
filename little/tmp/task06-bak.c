#include <linux/module.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/major.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/sched.h>
#include <linux/kthread.h>
/*
*#include <linux/sched.h>   //wake_up_process()
* #include <linux/kthread.h> //kthread_create(), kthread_run()
* #include <err.h> //IS_ERR(), PTR_ERR()
*/


MODULE_AUTHOR("Bo YU<tsu.yubo@gmail.com>");

MODULE_DESCRIPTION("task 06\n");

MODULE_LICENSE("GPL");

#define BUF_LEN 60
#define MY_ID_LEN 12 /* include final NULL, so it is 13 */

static char *my_id = "e233f61cc9c6";
static char read_id[BUF_LEN] = {0};
static struct task_struct *wee_wait;

/**
static ssize_t dev_read(struct file *file, char __user *userbuf,
			size_t len, loff_t *f_pos)
{

	return simple_read_from_buffer(userbuf, len,
				f_pos, my_id, strlen(my_id));
}
**/

static ssize_t dev_write(struct file *file, const char __user *userbuf,
			size_t len, loff_t *pos)
{
	ssize_t ret;

	ret = simple_write_to_buffer(read_id, BUF_LEN, pos, userbuf, len);

	if (ret < 0)
		return ret;

	else if ((ret != MY_ID_LEN) || strncmp(my_id, read_id, len - 1))
		return -EINVAL;
	else
		return len;
}


static const struct file_operations dev_fops = {
	.write = dev_write,
};



struct miscdevice misc_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "eudyptula",
	.fops = &dev_fops,
};

int thread_function(void *data)
{
	while(!kthread_should_stop()){
		schedle();
	}
	return var;
}

static int __init hello_init(void)
{
	int ret;

	ret = misc_register(&misc_device);

	if (ret) {
		pr_err("can't register misc device\n'");
		return ret;
	}
/*
	wee_wait = kthread_create(wee_thread, NULL, "wee_test");
	if (IS_ERR(wee_wait)) {
		pr_info("Unable to start kernel thread.\n");
		ret = PTR_ERR(wee_wait);
		wee_wait = NULL;
		return ret;
	}

	wake_up_process(wee_wait);
*/
	return 0;
}
static void __exit hello_exit(void)
{
	misc_deregister(&misc_device);
	/*
	if (wee_wait) {
		kthread_stop(wee_wait);
		wee_wait = NULL;
	}
	*/
}

module_init(hello_init);

module_exit(hello_exit);

