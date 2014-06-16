#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0xc4e4328b, "module_layout" },
	{ 0x15692c87, "param_ops_int" },
	{ 0x37a0cba, "kfree" },
	{ 0xa475964c, "cdev_del" },
	{ 0x68dfc59f, "__init_waitqueue_head" },
	{ 0x7485e15e, "unregister_chrdev_region" },
	{ 0x12da5bb2, "__kmalloc" },
	{ 0x29537c9e, "alloc_chrdev_region" },
	{ 0xd8e484f0, "register_chrdev_region" },
	{ 0x9b27a557, "cdev_add" },
	{ 0xabdba397, "cdev_init" },
	{ 0x668074a, "kill_fasync" },
	{ 0x3a013b7d, "remove_wait_queue" },
	{ 0xe45f60d8, "__wake_up" },
	{ 0x2e60bace, "memcpy" },
	{ 0x2f287f0d, "copy_to_user" },
	{ 0x4292364c, "schedule" },
	{ 0xd7bd3af2, "add_wait_queue" },
	{ 0xffd5a395, "default_wake_function" },
	{ 0x7ce55186, "current_task" },
	{ 0x50eedeb8, "printk" },
	{ 0xc4554217, "up" },
	{ 0xdd1a2871, "down" },
	{ 0xdda7c7c7, "fasync_helper" },
	{ 0xb4390f9a, "mcount" },
	{ 0x37ff4c06, "copy_from_user_overflow" },
	{ 0x362ef408, "_copy_from_user" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "DE61D595ADBED7658B6078E");
