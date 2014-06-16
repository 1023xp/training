#include <linux/module.h> 
#include <linux/kernel.h> 
#include <linux/init.h> 
#include <linux/proc_fs.h> 

MODULE_AUTHOR("Barry Song (author@linuxdriver.cn)"); 
MODULE_DESCRIPTION("proc exmaple"); 
MODULE_LICENSE("GPL"); 

unsigned int variable; 
struct proc_dir_entry *test_dir, *test_entry;

int test_proc_read(char *buf, char **start, off_t off, int count, 
                         int *eof, void *data) 
{ 
      unsigned int       *ptr_var = data; 
      return sprintf(buf, "%u\n", *ptr_var); 
} 

int test_proc_write(struct file *file, const char *buffer,
	unsigned long count, void *data) 
{ 
	unsigned int      *ptr_var = data; 
	
	printk(KERN_DEBUG "TEST: variable set to: %s", buffer); 
	*ptr_var = simple_strtoul(buffer, NULL, 10); 
	
	return count; 
} 

int register_proc_files(void) 
{
	test_dir = proc_mkdir("test_dir", NULL); 
	if (test_dir) {
		test_entry = create_proc_entry("test_rw", 0644, test_dir);
		if (test_entry) {
			test_entry->nlink = 1; 
			test_entry->data = (void *) &variable; 
			test_entry->read_proc = test_proc_read; 
			test_entry->write_proc = test_proc_write;
			return 0;
		}
	}

	return -ENOMEM;
} 

void unregister_proc_files(void) 
{ 
	remove_proc_entry("test_rw", test_dir); 
	remove_proc_entry("test_dir", NULL); 
} 

int test_proc_init(void) 
{ 
	return register_proc_files(); 
} 

void test_proc_cleanup(void) 
{ 
	unregister_proc_files(); 
} 

module_init(test_proc_init); 
module_exit(test_proc_cleanup); 
