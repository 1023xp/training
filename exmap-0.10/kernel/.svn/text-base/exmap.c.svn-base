/*
 * Dump info on all pages in all VMAs for a given PID.
 *
 * (c) John Berthels 2005 <jjberthels@gmail.com>. See COPYING for license.
 */

/*
 * Locking:
 * We need to ensure the mm_struct doesn't go away beneath us:
 * inc mm->mm_count/mmdrop()
 *
 * We can't vmalloc whilst holding mm->page_table_lock (otherwise
 * page_check_address() can deadlock against us).
 *
 * We have to hold mm->mmap_sem with read bias (to avoid the vmas changing)
 *
 * So we:
 * inc mm_count
 * down_read(mm->mmap_sem)
 * walk vmas, allocating space to hold per-page info
 * take page-table-lock
 * walk vmas, walking page tables within vma, filling in alloc'd space
 * release page_table_lock
 * release mmap_sem
 * mmdrop()
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/mm.h>
#include <linux/pagemap.h>
#include <linux/version.h>
#include <linux/vmalloc.h>
#include <linux/swap.h>
#include <linux/swapops.h>

/* Allow compilation on some kernels prior to 2.6.11 */

#undef HAVE_PUD_T
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,11)
#define HAVE_PUD_T
#endif



#define PROCFS_NAME "exmap"
MODULE_LICENSE ("GPL");
MODULE_AUTHOR ("John Berthels <jjberthels@gmail.com>");
MODULE_DESCRIPTION ("Show page-level information on all vmas within a task");

/* Not very nice. We save the data at write() time and then use it at read().
 * We also keep a cursor so that sequential reads work correctly.
 * I'm pretty sure that there are nicer ways to do this.
 */
struct exmap_data
{
	struct exmap_vma_data *vma_data;
	int num_vmas;
	/* Index to next vma_data to examine */
	int vma_cursor;
};

struct exmap_vma_data
{
	unsigned long vm_start;
	int start_shown;
	/* Our vmalloc'ed array of pte's */
	pte_t *ptes;
	unsigned long num_pages;
	/* Next page to examine, starting at zero for the first in the vma */
	unsigned long page_cursor;
};

static struct exmap_data local_data;

/* Only called once, at load time */
static void init_local_data(void)
{
	local_data.vma_data = NULL;
	local_data.num_vmas = 0;
	local_data.vma_cursor = 0;
}

static void clear_local_data(void)
{
	struct exmap_vma_data *vma_data;
	int i;

	if (local_data.vma_data == NULL) {
		init_local_data();
		return;
	}

	for (i = 0, vma_data = local_data.vma_data;
	     i < local_data.num_vmas;
	     ++i, ++vma_data) {
		if (vma_data->ptes != NULL) {
			vfree(vma_data->ptes);
		}
	}
	vfree(local_data.vma_data);
	init_local_data();
}

/* Modelled on __follow_page. Except we don't support HUGETLB and we
 * only actually use the pfn or pte, rather than getting hold of the
 * struct page. */
static int walk_page_tables(struct mm_struct *mm,
			    unsigned long address,
			    pte_t *pte_ret)
{
	pgd_t *pgd;
	pmd_t *pmd;
	pte_t *ptep;
#ifdef HAVE_PUD_T
	pud_t *pud;
#endif

	// No support for HUGETLB as yet
	//page = follow_huge_addr(mm, address, write);
	//if (! IS_ERR(page))
	//return page;

	pgd = pgd_offset(mm, address);
	if (pgd_none(*pgd) || unlikely(pgd_bad(*pgd)))
		goto out;

#ifdef HAVE_PUD_T
	pud = pud_offset(pgd, address);
	if (pud_none(*pud) || unlikely(pud_bad(*pud)))
		goto out;

	pmd = pmd_offset(pud, address);
#else
	pmd = pmd_offset(pgd, address);
#endif
	if (pmd_none(*pmd) || unlikely(pmd_bad(*pmd)))
		goto out;
	ptep = pte_offset_map(pmd, address);
	if (!ptep)
		goto out;

	*pte_ret = *ptep;
	pte_unmap(ptep);

	return 0;
out:
	return -1;
}


static void save_vma_page_info(struct vm_area_struct *vma,
			      struct exmap_vma_data *vma_data)
{
	int pageno;
	pte_t *ptep = vma_data->ptes;
	unsigned long page_addr = vma->vm_start;

	for (pageno = 0;
	     pageno < vma_data->num_pages;
	     ++pageno, page_addr += PAGE_SIZE, ptep++) {

		if (walk_page_tables(vma->vm_mm,
				     page_addr,
				     ptep) < 0) {

			/* This appears to happen. It looks like if
			 * high enough pages haven't been touched, the
			 * pmd doens't yet exist. */
			*ptep = __pte(0);
		}
	}
}


static int alloc_vmalist_info(struct vm_area_struct *vma_base)
{
	struct vm_area_struct *vma;
	struct exmap_vma_data *vma_data;
	unsigned long alloc_size;
	int errcode = 0;

	if (local_data.vma_data != NULL) {
		clear_local_data();
	}
	for (vma = vma_base; vma != NULL; vma = vma->vm_next) {
		++local_data.num_vmas;
	}

	/* Allocate one struct per vma */
	alloc_size = sizeof(struct exmap_vma_data) * local_data.num_vmas;
	local_data.vma_data = vmalloc(alloc_size);
	if (local_data.vma_data == NULL) {
		printk (KERN_ALERT "/proc/%s : vmalloc(%lu) failed\n",
			PROCFS_NAME, alloc_size);
		errcode = -ENOMEM;
		goto ErrExit;
	}

	/* For each vma_data struct, calculate the number of pages
	 * and allocate pte space */
	for (vma = vma_base, vma_data = local_data.vma_data;
	     vma != NULL;
	     vma = vma->vm_next, vma_data++) {

		vma_data->num_pages
			= (vma->vm_end - vma->vm_start) >> PAGE_SHIFT;
		alloc_size = sizeof(pte_t) * vma_data->num_pages;
		vma_data->ptes = vmalloc(alloc_size);
		if (vma_data->ptes == NULL) {
			printk (KERN_ALERT "/proc/%s : vma vmalloc(%lu) failed",
				PROCFS_NAME, alloc_size);
			errcode = -ENOMEM;
			goto ErrExit;
		}

		vma_data->page_cursor = 0;
		vma_data->vm_start = vma->vm_start;
		vma_data->start_shown = 0;
	}

	return 0;
ErrExit:
	if (local_data.vma_data != NULL) {
		clear_local_data();
	}
	return errcode;
}



static void store_vmalist_info(struct vm_area_struct *vma_base)
{
	struct vm_area_struct *vma;
	struct exmap_vma_data *vma_data;

	/* For each vma_data struct, fill in the ptes */
	for (vma = vma_base, vma_data = local_data.vma_data;
	     vma != NULL;
	     vma = vma->vm_next, vma_data++) {
		save_vma_page_info(vma, vma_data);
	}
}



/* Copied (and modded) from user_atoi in arch/frv somewhere */
static unsigned long user_atoul (const char __user * ubuf, int len)
{
	char buf[16];
	unsigned long ret;

	if (len > 15) {
		printk (KERN_ALERT "/proc/%s : user_atoul bad length %d\n",
			PROCFS_NAME, len);
		return -EINVAL;
	}

	if (copy_from_user (buf, ubuf, len)) {
		printk (KERN_ALERT "/proc/%s : user_atoul cfu failed\n",
			PROCFS_NAME);
		return -EFAULT;
	}

	buf[len] = 0;

/*	printk(KERN_INFO "user_atoul: examining buffer [%s]\n", buf); */

	ret = simple_strtoul (buf, NULL, 0);
	return ret;
}


/* Add the output line for the given page to the buffer, returning
 * number of chars added. Returns 0 if it can't fit into the
 * buffer. */
static int show_one_page(pte_t pte,
			 char *buffer,
			 int buflen)
{
	int len;
	unsigned long pfn = 0UL;
	swp_entry_t swap_entry;
	int present;
	int writable;
	unsigned long cookie;

	swap_entry.val = 0UL; /* All zeros not a valid entry */

	present = pte_present(pte);
	writable = pte_write(pte) ? 1 : 0;
	if (present) {
		pfn = pte_pfn(pte);
		if (!pfn_valid(pfn)) {
			pfn = 0;
		}
		cookie = pfn;
	}
	else {
		swap_entry = pte_to_swp_entry(pte);
		cookie = swap_entry.val;
	}

	len = snprintf (buffer,
			buflen,
			"%d %d %lx\n",
			present,
			writable,
			cookie);

	if (len >= buflen)
		goto ETOOLONG;

	return len;
 ETOOLONG:
	buffer[0] = '\0';
	return 0;
}

static int show_vma_start(struct exmap_vma_data *vma_data,
			  char *buffer,
			  int buflen)
{
	int len;

	len = snprintf (buffer,
			buflen,
			"VMA %lx %lu\n",
			vma_data->vm_start,
			vma_data->num_pages);

	if (len >= buflen)
		goto ETOOLONG;

	return len;
 ETOOLONG:
	buffer[0] = '\0';
	return 0;
}


static int exmap_show_next(char *buffer, int length)
{
	int offset = 0;
	struct exmap_vma_data *vma_data;
	pte_t pte;
	int line_len;

	while (local_data.vma_cursor < local_data.num_vmas) {
		vma_data = local_data.vma_data + local_data.vma_cursor;
//		printk (KERN_INFO
//			"exmap: examining vma %08lx [%d/%d] %d\n",
//			vma_data->vm_start,
//			local_data.vma_cursor,
//			local_data.num_vmas,
//			vma_data->start_shown);
		if (!vma_data->start_shown) {
//			printk (KERN_INFO
//				"exmap: svs\n");
			line_len = show_vma_start(vma_data,
						  buffer + offset,
						  length - offset);
			if (line_len <= 0)
				/* Cursor in right position for next read */
				goto Finished;
			offset += line_len;
			vma_data->start_shown = 1;
		}

		while (vma_data->page_cursor < vma_data->num_pages) {
			pte = vma_data->ptes[vma_data->page_cursor];
			line_len = show_one_page(pte,
						 buffer + offset,
						 length - offset);
			if (line_len <= 0)
				/* Cursor in right position for next read */
				goto Finished;
			offset += line_len;
			vma_data->page_cursor++;
		}
		local_data.vma_cursor++;
	}
Finished:
	return offset;
}


int setup_from_pid(pid_t pid)
{
	struct mm_struct *mm = NULL;
	struct task_struct *tsk;
	int errcode = -EINVAL;

	tsk = find_task_by_pid_type_ns(PIDTYPE_PID, pid, &init_pid_ns);
	if (tsk == NULL) {
		printk (KERN_ALERT
			"/proc/%s: can't find task for pid %d\n",
			PROCFS_NAME, pid);
		goto Exit;
	}
	if (tsk == current) {
		printk (KERN_ALERT
			"/proc/%s: can't self-examine %d\n",
			PROCFS_NAME, pid);
		goto Exit;
	}

	mm = get_task_mm(tsk);
	if (mm == NULL) {
		printk (KERN_ALERT
			"/proc/%s: can't get mm for task for pid %d\n",
			PROCFS_NAME, pid);
		goto Exit;
	}

	/* Stop the vma list changing.
	 * We can't take page_table_lock yet because we vmalloc */
	down_read(&mm->mmap_sem);
	if ((errcode = alloc_vmalist_info(mm->mmap)) < 0) {
		printk (KERN_ALERT
			"/proc/%s: failed to alloc for mm for pid %d\n",
			PROCFS_NAME, pid);
		goto Exit;
	}

	/* We've got our space allocated, fill it in */
	spin_lock(&mm->page_table_lock);
	store_vmalist_info(mm->mmap);
	spin_unlock(&mm->page_table_lock);

	errcode = 0;
Exit:
	if (mm) {
		up_read(&mm->mmap_sem);
		mmput(mm);
	}

	return errcode;
}

/*
 * Writes to the procfile should be of the form:
 * pid:0xdeadbeef\n
 * where deadbeef is the hex addr of the vma to examine
 * and pid is the (decimal) pid of the process to examine
 */
static int procfile_write (struct file *file,
			   const char __user *buffer,
			   unsigned long count,
			   void *data)
{
	pid_t pid;
	int errcode = -EINVAL;

	pid = user_atoul(buffer, count);
	if (pid <= 0) {
		printk (KERN_ALERT
			"/proc/%s:can't parse buffer to read pid\n",
			PROCFS_NAME);
		return errcode;
	}

	if ((errcode = setup_from_pid(pid)) < 0) {
		printk (KERN_ALERT
			"/proc/%s: failed save info for pid %d [%d]\n",
			PROCFS_NAME, pid, errcode);
		return errcode;
	}

	return count;
}

/*
 * Only support sequential reading of file from start to finish
 * (following a write() to set the pid to examine
 */
static int procfile_read (char *buffer,
			  char **buffer_location,
			  off_t offset,
			  int buffer_length,
			  int *eof,
			  void *data)
{
	int ret;

	if (local_data.vma_data == NULL) {
		printk (KERN_ALERT "/proc/%s: vma data not set\n",
			PROCFS_NAME);
		return -EINVAL;
	}

	ret = exmap_show_next(buffer, buffer_length);
	if (ret > 0) {
		*buffer_location = buffer;
	}
	return ret;
}

int init_module ()
{
	struct proc_dir_entry *exmap_proc_file;
	printk (KERN_INFO "/proc/%s: insert\n", PROCFS_NAME);

	exmap_proc_file = create_proc_entry (PROCFS_NAME,
							0644,
							NULL);

	if (exmap_proc_file == NULL) {
		remove_proc_entry (PROCFS_NAME, NULL);
		printk (KERN_ALERT "/proc/%s: could not initialize\n",
			PROCFS_NAME);
		return -ENOMEM;
	}

	exmap_proc_file->read_proc = procfile_read;
	exmap_proc_file->write_proc = procfile_write;
	exmap_proc_file->owner = THIS_MODULE;

	/*     exmap_proc_file->mode         = S_IFREG | S_IRUGO; */
	/* TODO - this is quite probably a security problem */
	exmap_proc_file->mode = 0666;

	exmap_proc_file->uid = 0;
	exmap_proc_file->gid = 0;
	exmap_proc_file->size = 0;

	init_local_data();
	return 0;
}

void cleanup_module ()
{
	printk (KERN_INFO "/proc/%s: remove\n", PROCFS_NAME);
	remove_proc_entry (PROCFS_NAME, NULL);
}
