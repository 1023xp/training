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
	{ 0x7406f5c5, "kobject_put" },
	{ 0x43b433c6, "put_disk" },
	{ 0xa4014e61, "del_gendisk" },
	{ 0x1f673f43, "blk_cleanup_queue" },
	{ 0xb5a459dc, "unregister_blkdev" },
	{ 0x999e8297, "vfree" },
	{ 0xf662bbfb, "add_disk" },
	{ 0xb81960ca, "snprintf" },
	{ 0x462c6328, "alloc_disk" },
	{ 0xaeba449a, "blk_queue_logical_block_size" },
	{ 0xefca7187, "blk_init_queue" },
	{ 0xebdc7a4a, "blk_queue_make_request" },
	{ 0xeaabdc44, "blk_alloc_queue" },
	{ 0x593a99b, "init_timer_key" },
	{ 0xd6ee688f, "vmalloc" },
	{ 0x12da5bb2, "__kmalloc" },
	{ 0x71a50dbc, "register_blkdev" },
	{ 0x9b7ce92a, "bio_endio" },
	{ 0x596f11c7, "kmap_atomic" },
	{ 0xed93f29e, "__kunmap_atomic" },
	{ 0x9c63db6b, "blk_start_request" },
	{ 0x551beab1, "blk_peek_request" },
	{ 0x8b770e2, "__blk_end_request_all" },
	{ 0x2e60bace, "memcpy" },
	{ 0x50eedeb8, "printk" },
	{ 0xd5f2172f, "del_timer_sync" },
	{ 0x76ebea8, "pv_lock_ops" },
	{ 0xbe2c0274, "add_timer" },
	{ 0x7d11c268, "jiffies" },
	{ 0x67f7403e, "_raw_spin_lock" },
	{ 0x2bc95bd4, "memset" },
	{ 0xb4390f9a, "mcount" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "B13576BFAB21FCCF6AB809C");
