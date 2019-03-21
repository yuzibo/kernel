// SPDX-License-Identifier: GPL-2.0

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
#include <linux/printk.h>
/*
 * #include <linux/sched.h>   //wake_up_process()
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
static struct task_struct *eudyptula_task;

DECLARE_WAIT_QUEUE_HEAD(wee_wait);

static ssize_t dev_write(struct file *file, const char __user *userbuf,
			size_t len, loff_t *pos)
{
	ssize_t ret;

	pr_info("hello, write\n");
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
	.mode = 0222,
};

static int thread_function(void *data)
{
	int time_count = 0;

	do {
		pr_info("kill thread from ps -ef==%d\n", ++time_count);
		if (kthread_should_stop())
			break;
		if (wait_event_interruptible(wee_wait, kthread_should_stop()))
			return -ERESTARTSYS;
	} while (time_count <= 30);

	return time_count;
}

static int __init hello_init(void)
{
	int ret;

	ret = misc_register(&misc_device);

	pr_info("hello,task17\n");
	if (ret) {
		pr_err("can't register misc device\n'");
		return ret;
	}

	eudyptula_task = kthread_run(&thread_function, NULL,
					"eudyptula");
	if (IS_ERR(eudyptula_task)) {
		pr_info("Unable to start kernel thread.\n");
		ret = PTR_ERR(eudyptula_task);
		eudyptula_task = NULL;
		return ret;
	}
	pr_info("Kernel thread: %s\n", eudyptula_task->comm);
//	wake_up_process(eudyptula_task);
//	condition = 1;
//	wake_up_interruptible(&wee_wait);

	return 0;
}
static void __exit hello_exit(void)
{
	misc_deregister(&misc_device);

	if (eudyptula_task) {
		kthread_stop(eudyptula_task);
		eudyptula_task = NULL;
	}

	pr_info("exit task17\n");

}

module_init(hello_init);

module_exit(hello_exit);

