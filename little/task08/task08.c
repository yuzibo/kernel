#include <linux/module.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/debugfs.h>
#include <linux/uaccess.h>
#include <linux/timer.h>
#include <linux/semaphore.h>


MODULE_AUTHOR("Bo YU<tsu.yubo@gmail.com>");

MODULE_DESCRIPTION("task 08\n");

MODULE_LICENSE("GPL");

#define	BUF_LEN 14

/* define semaphore */

static DEFINE_SEMAPHORE(foo_sem);

static char foo_data[PAGE_SIZE];

/*
 * This directory will point to '/sys/kernel/debug/example'
 */

static struct dentry *dir;

#define MY_ID_LEN 12 /* include final NULL, so it is 13 */

static char read_id[BUF_LEN] = {0};

static char *my_id = "e233f61cc9c6";

/* File '/sys/kernel/debug/eudyptula/id ' point to this variable
 */

static u32 j;

/*
 * Reading jiffies value, as if use u32
 *
 */

static ssize_t debug_read(struct file *file, char __user *userbuf,
		size_t len, loff_t *f_pos)
{
	return simple_read_from_buffer(userbuf, len, f_pos,
			my_id, strlen(my_id));
}

static ssize_t debug_write(struct file *file, const char __user *userbuf,
		size_t len, loff_t *f_pos)
{
	ssize_t ret;

	ret = simple_write_to_buffer(read_id, BUF_LEN, f_pos, userbuf, len);

	if (ret < 0)
		return ret;

	else if ((ret != MY_ID_LEN) || strncmp(my_id, read_id, len - 1))
		return -EINVAL;

	else
		return len;
}

static const struct file_operations debug_fops = {
	.read = debug_read,
	.write = debug_write,
};

static ssize_t foo_write(struct file *file, const char __user *buf,
		size_t count, loff_t *ppos)
{

	ssize_t ret;

	down(&foo_sem);

	ret = simple_write_to_buffer(foo_data, PAGE_SIZE, ppos, buf, count);

	up(&foo_sem);

	if (ret < 0)
		return ret;
	else
		return count;

}

static ssize_t foo_read(struct file *file, char __user *buf,
		size_t count, loff_t *ppos)
{
	int ret;

	down(&foo_sem);

	ret = simple_read_from_buffer(buf, count, ppos, foo_data, PAGE_SIZE);
	up(&foo_sem);

	return ret;
}

/*
 * foo file : write and read with lock,
 *
 */


static const struct file_operations foo_fops = {
	.read = foo_read,
	.write = foo_write,
};
int init_module(void)
{
	struct dentry *debug_file, *debug_jiffies, *foo;
	/*
	 * in /sys/kernel/debug create eudyptula
	 */
	dir = debugfs_create_dir("eudyptula", 0);

	if (!dir) {
		pr_err("Failed to create /sys/kernel/debug/eudyptula");
		return -ENOMEM;
	}

	/*
	 * create file /sys/kernel/debug/eudyptula/id
	 * read/write operation on the file result in read/write operation
	 *  in the variable
	 */
	debug_file = debugfs_create_file("id", 0666, dir, NULL, &debug_fops);

	/*
	 * checkpatch will compiant above debugfs_create_file()
	 */
	if (!debug_file) {
		pr_err("Failed to create file named 'id' in /sys/../eudyptula\n");
		return -ENOMEM;
	}

	j = jiffies;

	debug_jiffies = debugfs_create_u32("jiffies", 0444, dir, &j);

	if (!debug_jiffies) {
		pr_err("Failed to create file named foo\n");

		return -ENOMEM;
	}

	foo = debugfs_create_file("foo", 0644, dir, NULL, &foo_fops);

	if (!foo) {
		pr_err("Failed to create file named foo");

		return -ENOMEM;
	}

	return 0;
}

void cleanup_module(void)
{
	debugfs_remove_recursive(dir);
}
