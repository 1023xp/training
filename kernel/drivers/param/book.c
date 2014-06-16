/*
 * A kernel module: book
 * This example is to introduce module params
 *
 * The initial developer of the original code is Baohua Song
 * <author@linuxdriver.cn>. All Rights Reserved.
 */

#include <linux/init.h>
#include <linux/module.h>

static char *book_name = "dissecting Linux Device Driver";
static int num = 4000;

static int book_init(void)
{
	printk(KERN_INFO " book name:%s\n",book_name);
	printk(KERN_INFO " book num:%d\n",num);
	return 0;
}
static void book_exit(void)
{
	printk(KERN_INFO " Book module exit\n ");
}
module_init(book_init);
module_exit(book_exit);

module_param(num, int, S_IRUGO);
module_param(book_name, charp, S_IRUGO);

MODULE_AUTHOR("Song Baohua, author@linuxdriver.cn");
MODULE_LICENSE("Dual BSD/GPL");
MODULE_DESCRIPTION("A simple Module for testing module params");
MODULE_VERSION("V1.0");
