#include<linux/module.h>
#include<linux/kernel.h>

MODULE_LICENSE("Dual BSD/GPL");
static int hello_init(void)
{
	printk(KERN_INFO "Hello,World!\n");
	return 0;
}
static void hello_exit(void)
{
	printk(KERN_INFO "exit sucessfully!\n");
}

/*
 * initialize of modules
 *
 */
module_init(hello_init);
/**
 * exit of modules
 */
module_exit(hello_exit);
<<<<<<< HEAD

=======
============================
	Makefile
============================
>>>>>>> 30e8c0d5a6a476ed94b3a700b99ad7d1652ea9ff

ifneq ($(KERNELRELEASE),)
	obj-m := hello.o
else
	KDIR ?= /lib/modules/`uname -r`/build
	CFLAGS_hello.o := -DDEBUG
	PWD := $(shell pwd)
default:
	$(MAKE) -C $(KDIR) M=$$PWD
clean:
	make -C $(KDIR) M=$(PWD) clean

endif
