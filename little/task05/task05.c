#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/usb.h>
#include <linux/usb/input.h>
#include <linux/hid.h>

MODULE_AUTHOR("Bo YU<tsu.yubo@gmail.com>");

MODULE_DESCRIPTION("task 05\n");

MODULE_LICENSE("GPL");

static struct usb_device_id hello_id_table[] = {

	{ USB_INTERFACE_INFO(USB_INTERFACE_CLASS_HID,
	  USB_INTERFACE_SUBCLASS_BOOT,
	  USB_INTERFACE_PROTOCOL_KEYBOARD) },
	{ }, /* Here are entry */
};

/* 向内核注册 */
MODULE_DEVICE_TABLE(usb, hello_id_table);




static int __init hello_init(void)
{

	pr_info("Hello, module Task 05, hello, world!\n");

	return 0;

}
static void __exit hello_exit(void)
{
	pr_info("Hello, Module task 05: Exit\n");
}

module_init(hello_init);

module_exit(hello_exit);
