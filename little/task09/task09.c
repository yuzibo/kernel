#include <linux/kobject.h>
#include <linux/string.h>
#include <linux/sysfs.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/semaphore.h>
#include <linux/uaccess.h>
#include <linux/slab.h>


MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Bo YU tsu.yubo@gmail.com");
MODULE_DESCRIPTION("task 09\n");

#define BUF_LEN 14

/* define semaphore */

static DEFINE_SEMAPHORE(foo_sem);

/** a page of data */

static char foo_data[PAGE_SIZE];

#define MY_ID_LEN 13 /* include final NULL, so it is 13 */

static char *MY_ID = "e233f61cc9c6\n";

static int foo_len;


/*
 * create eudyptula dir, foo, jiffies, id[match my id]
 *
 */
/*
 *  Here doesn't need is eudyptula, in kobject_create_and_add
 *  method arg @1 is "names"
 */
static struct kobject *hello_kobj;

/*
 * foo file, written by root and read from user
 * use  down() && up(), maybe use mutex_lock()
 */

static ssize_t foo_show(struct kobject *kobj,
		struct kobj_attribute *attr,	char *buf)
{
	down(&foo_sem);
	strncpy(buf, foo_data, foo_len);
	up(&foo_sem);
	return foo_len;
}

static ssize_t foo_store(struct kobject *kobj, struct kobj_attribute *attr,
			const char *buf, size_t count)
{
	if (count > PAGE_SIZE)
		return -EINVAL;

	down(&foo_sem);
	strncpy(foo_data, buf, count);
	foo_len = count;
	up(&foo_sem);
	return count;
}


static ssize_t id_show(struct kobject *kobj,
		struct kobj_attribute *attr, char *buf)
{
	char *my_id_str = MY_ID;

	strncpy(buf, my_id_str, MY_ID_LEN);

	return MY_ID_LEN;
}

static ssize_t id_store(struct kobject *kobj,
		struct kobj_attribute *attr, const char *buf, size_t count)
{
	char *my_id_str = MY_ID;

	if ((count != MY_ID_LEN) || (strncmp(my_id_str, buf, MY_ID_LEN)))
		return -EINVAL;
	else
		return MY_ID_LEN;
}
static ssize_t jiffies_show(struct kobject *kobj,
		struct kobj_attribute *attr, char *buf)
{
	return sprintf(buf, "%lu\n", jiffies);
}
/*
 * Create a group of attributes so that we can create and destory them
 *
 */

static struct kobj_attribute foo_attribute =
		__ATTR_RW(foo);

static struct kobj_attribute id_attribute =
		__ATTR_RW(id);
/* if you only use jiffies_show, you can use __ATTR_RO() */

static struct kobj_attribute jiffies_attribute =
		__ATTR_RO(jiffies);
/*
 * need to NULL terminate the list of attributes
 */
static struct attribute *attrs[] = {
	&foo_attribute.attr,
	&id_attribute.attr,
	&jiffies_attribute.attr,
	NULL,
};


static struct attribute_group hello_group = {
	.attrs = attrs,
};


static int __init eudyptula_init(void)
{
	int retval;

	hello_kobj = kobject_create_and_add("eudyptula", kernel_kobj);

	if (!hello_kobj)
		return -ENOMEM;

	/*
	 * Create the files associated with this kobject dir
	 */

	retval = sysfs_create_group(hello_kobj, &hello_group);

	if (retval)
		kobject_put(hello_kobj);

	return retval;
}

static void __exit eudyptula_exit(void)
{
	kobject_put(hello_kobj);
}

module_init(eudyptula_init);
module_exit(eudyptula_exit);

