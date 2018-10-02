#include <linux/module.h>
#include <linux/list.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/spinlock.h>


struct identity{
	char	name[20];
	int	id;
	bool	busy;
	/* kernel's list */
	struct list_head list;
};

/* this is a list point to identity */
/*
 * struct identity identity_list;
 *LIST_HEAD(identity_list);
 */
/*struct list_head head_list;*/

LIST_HEAD(head_list);
static DEFINE_RWLOCK(list_lock);
/* it is necessary to use lock */


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
	if(!temp)
		goto free_mem;

	strcpy(temp->name, name);
	temp->id = id;
	temp->busy = false;
	INIT_LIST_HEAD(&temp->list);
	write_lock(&list_lock);
	list_add_tail(&(temp->list), &head_list);
	write_unlock(&list_lock);
	//kfree(temp);
	pr_info("here \n");
	return 0;

free_mem:
	pr_info("free_mem in create \n");
	kfree(temp);
	return -ENOMEM;
}

/*
 * Write a function that looks like:
 * struct identity *identity_find(int id);
 * that takes a given id, iterates over the list of all ids, and
 * returns the proper 'struct identity' associated with it.  If the
 * identity is not found, return NULL.
 */
struct identity *identity_find(int id)
{
	struct identity *current_tmp = NULL;
	struct list_head *p;
	read_lock(&list_lock);
	list_for_each(p, &head_list)
	{
		current_tmp = list_entry(p, struct identity, list);
		if(current_tmp->id == id){
			pr_info("The id is %d\t", current_tmp->id);
			read_unlock(&list_lock);
			return current_tmp;
			break;
		}
	}
	read_unlock(&list_lock);
	return NULL;
}

/*
 * Void identity_destroy(int id)
 * Write a function: that given an id, finds the proper
 * 'struct identity' and removes it from the system.
 */
void identity_destroy(int id)
{
	struct identity *temp = NULL;
	struct list_head *p;

	write_lock(&list_lock);
	list_for_each(p, &head_list){
		pr_info("Have being in del");
		temp = list_entry(p, struct identity, list);
		if (temp->id == id){
			list_del(p);
			pr_info("del the id %d\n", temp->id);
			write_unlock(&list_lock);
			return ;
		}
	}
	write_unlock(&list_lock);
	return ;
}
static int __init identity_init(void)
{
	int ret = 0;
	struct identity *temp;

	pr_info("init the identity\n");
	ret = identity_create("Alice", 1);
	pr_info("the ret is %d\n", ret);
	if(ret)
		goto err;
	ret = identity_create("Bob", 2);
	if(ret)
		goto err;
	ret = identity_create("Dave", 3);
	if(ret)
		goto err;
	ret = identity_create("Gena", 10);
	if(ret)
		goto err;

	temp = identity_find(3);
	pr_info("id 3 = %s\n", temp->name);

	temp = identity_find(42);
	if(temp == NULL)
		pr_info("id 42 not found\n");

	identity_destroy(2);
	identity_destroy(1);
	identity_destroy(10);
	identity_destroy(42);
	identity_destroy(3);

	return 0;
err:
	pr_info("There are some errors occurs\n");
	return -ENOMEM;

}
static void __exit identity_exit(void)
{
	pr_info("exit the identity\n");
}

module_init(identity_init);
module_exit(identity_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Bo YU<tsu.yubo@gmail.com>");
MODULE_DESCRIPTION("task12");

