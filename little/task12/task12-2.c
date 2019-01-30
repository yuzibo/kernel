// SPDX-License-Identifier: GPL-2.0

#include <linux/module.h>
#include <linux/list.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/spinlock.h>

struct identity {
	char	name[20];
	int	id;
	bool	busy;
	/* kerenl's list */
	struct list_head list;
};

/* this is a list point to identity */
/*
 * struct identity identity_list;
 * LIST_HEAD(identity_list);
 *
 * struct list_head head_list;
 */

LIST_HEAD(head_list);
static DEFINE_RWLOCK(list_lock);
/* it is necessary  to use lock */

/*there is another method to init the list*/
/*
 * identity_create:
 *  create the struct idnetity node
 *- Write a function that looks like:
 *   int identity_create(char *name, int id)
 *   that creates the structure "identity", copies in the 'name' and 'id'
 *   fields and sets 'busy' to false.  Proper error checking for out of
 *   memory issues is required.  Return 0 if everything went ok; an error
 *  value if something went wrong
 *
 */

static int identity_create(char *name, int id)
{
	struct identity *temp;

	temp = kmalloc(sizeof(struct identity), GFP_KERNEL);
	if (!temp)
		goto free_mem;

	strcpy(temp->name, name);
	temp->id = id;
	temp->busy = false;
	INIT_LIST_HEAD(&temp->list);
	write_lock(&list_lock);
	list_add_tail(&(temp->list), &head_list);
	write_unlock(&list_lock);

	return 0;

free_mem:
	kfree(temp);
	return -ENOMEM;
}
/**
 * Write a function that looks like:
 * struct identity *identity_find(int id);
 * that takes a given id, iterates over the list of all ids,and
 * return the proper 'struct identity' associated with it.If the
 * identity is not found, return NULL;
 */
struct identity *identity_find(int id)
{
	struct identity *current_tmp = NULL;
	//struct list_head *p;
	read_lock(&list_lock);
	list_for_each_entry(current_tmp, &head_list, list) {
		if (current_tmp->id == id) {
			read_unlock(&list_lock);
			return current_tmp;
		}
	}
	read_unlock(&list_lock);
	return NULL;

}
/*
 *  Void identity_destory(int id)
 */
void identity_destory(int id)
{
	struct identity *temp = NULL, *current_temp = NULL;

	write_lock(&list_lock);
	list_for_each_entry_safe(current_temp, temp, &head_list, list) {
		if (current_temp->id == id) {
			list_del(&current_temp->list);
			write_unlock(&list_lock);
			return;
		}
	}
	write_unlock(&list_lock);
}

static int __init identity_init(void)
{
	int ret = 0;
	struct identity *temp = NULL;

	ret =  identity_create("Alice", 1);
	if (ret)
		pr_debug("error\n");
	ret = identity_create("Bob", 2);
	if (ret)
		pr_debug("error\n");
	ret = identity_create("Dave", 3);
	if (ret)
		pr_debug("error\n");
	ret = identity_create("Gena", 10);
	if (ret)
		pr_debug("error\n");

	temp = identity_find(3);
	pr_debug("id 3 = %s\n", temp->name);

	temp = identity_find(42);
	if (temp == NULL)
		pr_debug("id 42 not found\n");
	identity_destory(2);
	identity_destory(1);
	identity_destory(10);
	identity_destory(42);
	identity_destory(3);

	return 0;


}
static void __exit identity_exit(void)
{
	pr_debug("exit the identity");
}

module_init(identity_init);
module_exit(identity_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("BO YU<tsu.yubo@gmail.com>");
MODULE_DESCRIPTION("task12");
