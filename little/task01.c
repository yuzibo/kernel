#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

static int __init hello_init(void)
{
	pr_debug("Hello,world\n");
	return 0;
}

static void __exit hello_exit(void)
{
	pr_debug("GoodBye\n");
}

module_init(hello_init);

module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Bo Yu<tsu.yubo@gmail.com>")
MODULE_DESCRIPTION("hello_module")
