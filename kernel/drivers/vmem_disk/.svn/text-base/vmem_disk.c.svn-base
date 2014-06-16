/*
 * Sample disk driver, from the beginning.
 */

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>

#include <linux/sched.h>
#include <linux/kernel.h>       /* printk() */
#include <linux/slab.h>         /* kmalloc() */
#include <linux/fs.h>           /* everything... */
#include <linux/errno.h>        /* error codes */
#include <linux/timer.h>
#include <linux/types.h>        /* size_t */
#include <linux/fcntl.h>        /* O_ACCMODE */
#include <linux/hdreg.h>        /* HDIO_GETGEO */
#include <linux/kdev_t.h>
#include <linux/vmalloc.h>
#include <linux/genhd.h>
#include <linux/blkdev.h>
#include <linux/buffer_head.h>  /* invalidate_bdev */
#include <linux/bio.h>

MODULE_LICENSE("Dual BSD/GPL");

static int vmem_disk_major;
module_param(vmem_disk_major, int, 0);
static int hardsect_size = 512;
module_param(hardsect_size, int, 0);
static int nsectors = 1024;     /* How big the drive is */
module_param(nsectors, int, 0);
static int ndevices = 4;
module_param(ndevices, int, 0);

/*
 * The different "request modes" we can use.
 */
enum {
	RM_SIMPLE  = 0, /* The extra-simple request function */
	RM_FULL    = 1, /* The full-blown version */
	RM_NOQUEUE = 2, /* Use make_request */
};
static int request_mode = RM_FULL;
module_param(request_mode, int, 0);

/*
 * Minor number and partition management.
 */
#define vmem_disk_MINORS    16
#define MINOR_SHIFT     4
#define DEVNUM(kdevnum) ((MINOR(kdev_t_to_nr(kdevnum)) >> MINOR_SHIFT)

	/*
	 * We can tweak our hardware sector size, but the kernel talks to us
	 * in terms of small sectors, always.
	 */
#define KERNEL_SECTOR_SIZE      512

	/*
	 * After this much idle time, the driver will simulate a media change.
	 */
#define INVALIDATE_DELAY        (30*HZ)

	/*
	 * The internal representation of our device.
	 */
	struct vmem_disk_dev {
	int size;                       /* Device size in sectors */
	u8 *data;                       /* The data array */
	short users;                    /* How many users */
	short media_change;             /* Flag a media change? */
	spinlock_t lock;                /* For mutual exclusion */
	struct request_queue *queue;    /* The device request queue */
	struct gendisk *gd;             /* The gendisk structure */
	struct timer_list timer;        /* For simulated media changes */
	};

static struct vmem_disk_dev *devices;

/*
 * Handle an I/O request.
 */
static void vmem_disk_transfer(struct vmem_disk_dev *dev, unsigned long sector,
	unsigned long nsect, char *buffer, int write)
{
	unsigned long offset = sector*KERNEL_SECTOR_SIZE;
	unsigned long nbytes = nsect*KERNEL_SECTOR_SIZE;

	if ((offset + nbytes) > dev->size) {
		printk (KERN_NOTICE "Beyond-end write (%ld %ld)\n", offset, nbytes);
		return;
	}
	if (write)
		memcpy(dev->data + offset, buffer, nbytes);
	else
		memcpy(buffer, dev->data + offset, nbytes);
}

/*
 * The simple form of the request function.
 */
static void vmem_disk_request(struct request_queue *q)
{
	struct request *req;

	while ((req = blk_peek_request(q)) != NULL) {
		struct vmem_disk_dev *dev = req->rq_disk->private_data;
		if (req->cmd_type != REQ_TYPE_FS) {
			printk (KERN_NOTICE "Skip non-fs request\n");
			blk_start_request(req);
			__blk_end_request_all(req, -EIO);
			continue;
		}

		blk_start_request(req);
		vmem_disk_transfer(dev, blk_rq_pos(req), blk_rq_cur_sectors(req),
			req->buffer, rq_data_dir(req));

		__blk_end_request_all(req, 0);
	}
}


/*
 * Transfer a single BIO.
 */
static int vmem_disk_xfer_bio(struct vmem_disk_dev *dev, struct bio *bio)
{
	int i;
	struct bio_vec *bvec;
	sector_t sector = bio->bi_sector;

	/* Do each segment independently. */
	bio_for_each_segment(bvec, bio, i) {
		char *buffer = __bio_kmap_atomic(bio, i, KM_USER0);
		vmem_disk_transfer(dev, sector, bio_cur_bytes(bio) >> 9,
			buffer, bio_data_dir(bio) == WRITE);
		sector += bio_cur_bytes(bio) >> 9;
		__bio_kunmap_atomic(bio, KM_USER0);
	}
	return 0; /* Always "succeed" */
}

/*
 * Transfer a full request.
 */
static int vmem_disk_xfer_request(struct vmem_disk_dev *dev, struct request *req)
{

	struct req_iterator iter;
	int nsect = 0;
	struct bio_vec *bvec;

	/* Macro rq_for_each_bio is gone.
	 * In most cases one should use rq_for_each_segment.
	 */
	rq_for_each_segment(bvec, req, iter) {
		char *buffer = __bio_kmap_atomic(iter.bio, iter.i, KM_USER0);
		sector_t sector = iter.bio->bi_sector;
		vmem_disk_transfer(dev, sector, bio_cur_bytes(iter.bio) >> 9,
			buffer, bio_data_dir(iter.bio) == WRITE);
		sector += bio_cur_bytes(iter.bio) >> 9;
		__bio_kunmap_atomic(iter.bio, KM_USER0);
		nsect += iter.bio->bi_size/KERNEL_SECTOR_SIZE;
	}
	return nsect;
}


/*
 * Smarter request function that "handles clustering".
 */
static void vmem_disk_full_request(struct request_queue *q)
{
	struct request *req;
	int sectors_xferred;
	struct vmem_disk_dev *dev = q->queuedata;

	while ((req = blk_peek_request(q)) != NULL) {
		if (req->cmd_type != REQ_TYPE_FS) {
			blk_start_request(req);
			printk (KERN_NOTICE "Skip non-fs request\n");
			__blk_end_request_all(req, -EIO);
			continue;
		}
		blk_start_request(req);
		sectors_xferred = vmem_disk_xfer_request(dev, req);
		__blk_end_request_all(req, 0);
	}
}

/*
 * The direct make request version.
 */
static void vmem_disk_make_request(struct request_queue *q, struct bio *bio)
{
	struct vmem_disk_dev *dev = q->queuedata;
	int status;

	status = vmem_disk_xfer_bio(dev, bio);
	bio_endio(bio, status);
}


/*
 * Open and close.
 */

static int vmem_disk_open(struct block_device *bdev, fmode_t mode)
{
	struct vmem_disk_dev *dev = bdev->bd_disk->private_data;

	del_timer_sync(&dev->timer);
	spin_lock(&dev->lock);
	dev->users++;
	spin_unlock(&dev->lock);

	return 0;
}

static int vmem_disk_release(struct gendisk *disk, fmode_t mode)
{
	struct vmem_disk_dev *dev = disk->private_data;

	spin_lock(&dev->lock);
	dev->users--;

	if (!dev->users) {
		dev->timer.expires = jiffies + INVALIDATE_DELAY;
		add_timer(&dev->timer);
	}
	spin_unlock(&dev->lock);

	return 0;
}

/*
 * Look for a (simulated) media change.
 */
int vmem_disk_media_changed(struct gendisk *gd)
{
	struct vmem_disk_dev *dev = gd->private_data;

	return dev->media_change;
}

/*
 * Revalidate.  WE DO NOT TAKE THE LOCK HERE, for fear of deadlocking
 * with open.  That needs to be reevaluated.
 */
int vmem_disk_revalidate(struct gendisk *gd)
{
	struct vmem_disk_dev *dev = gd->private_data;

	if (dev->media_change) {
		dev->media_change = 0;
		memset (dev->data, 0, dev->size);
	}
	return 0;
}

/*
 * The "invalidate" function runs out of the device timer; it sets
 * a flag to simulate the removal of the media.
 */
void vmem_disk_invalidate(unsigned long ldev)
{
	struct vmem_disk_dev *dev = (struct vmem_disk_dev *) ldev;

	spin_lock(&dev->lock);
	if (dev->users || !dev->data)
		printk (KERN_WARNING "vmem_disk: timer sanity check failed\n");
	else
		dev->media_change = 1;
	spin_unlock(&dev->lock);
}

/*
 * The ioctl() implementation
 */

static int vmem_disk_getgeo(struct block_device *bdev, struct hd_geometry *geo)
{
	long size;
	struct vmem_disk_dev *dev = bdev->bd_disk->private_data;

	size = dev->size*(hardsect_size/KERNEL_SECTOR_SIZE);
	geo->cylinders = (size & ~0x3f) >> 6;
	geo->heads = 4;
	geo->sectors = 16;
	geo->start = 4;

	return 0;
}

/*
 * The device operations structure.
 */
static struct block_device_operations vmem_disk_ops = {
	.owner           = THIS_MODULE,
	.open            = vmem_disk_open,
	.release         = vmem_disk_release,
	.media_changed   = vmem_disk_media_changed,
	.revalidate_disk = vmem_disk_revalidate,
	.getgeo          = vmem_disk_getgeo,
};


/*
 * Set up our internal device.
 */
static void setup_device(struct vmem_disk_dev *dev, int which)
{
	/*
	 * Get some memory.
	 */
	memset (dev, 0, sizeof (struct vmem_disk_dev));
	dev->size = nsectors*hardsect_size;
	dev->data = vmalloc(dev->size);
	if (dev->data == NULL) {
		printk (KERN_NOTICE "vmalloc failure.\n");
		return;
	}
	spin_lock_init(&dev->lock);

	/*
	 * The timer which "invalidates" the device.
	 */
	init_timer(&dev->timer);
	dev->timer.data = (unsigned long) dev;
	dev->timer.function = vmem_disk_invalidate;

	/*
	 * The I/O queue, depending on whether we are using our own
	 * make_request function or not.
	 */
	switch (request_mode) {
	case RM_NOQUEUE:
		dev->queue = blk_alloc_queue(GFP_KERNEL);
		if (dev->queue == NULL)
			goto out_vfree;
		blk_queue_make_request(dev->queue, vmem_disk_make_request);
		break;

	case RM_FULL:
		dev->queue = blk_init_queue(vmem_disk_full_request, &dev->lock);
		if (dev->queue == NULL)
			goto out_vfree;
		break;

	default:
		printk(KERN_NOTICE "Bad request mode %d, using simple\n", request_mode);
		/* fall into.. */

	case RM_SIMPLE:
		dev->queue = blk_init_queue(vmem_disk_request, &dev->lock);
		if (dev->queue == NULL)
			goto out_vfree;
		break;
	}
	blk_queue_logical_block_size(dev->queue, hardsect_size);
	dev->queue->queuedata = dev;
	/*
	 * And the gendisk structure.
	 */
	dev->gd = alloc_disk(vmem_disk_MINORS);
	if (!dev->gd) {
		printk (KERN_NOTICE "alloc_disk failure\n");
		goto out_vfree;
	}
	dev->gd->major = vmem_disk_major;
	dev->gd->first_minor = which*vmem_disk_MINORS;
	dev->gd->fops = &vmem_disk_ops;
	dev->gd->queue = dev->queue;
	dev->gd->private_data = dev;
	snprintf (dev->gd->disk_name, 32, "vmem_disk%c", which + 'a');
	set_capacity(dev->gd, nsectors*(hardsect_size/KERNEL_SECTOR_SIZE));
	add_disk(dev->gd);
	return;

out_vfree:
	if (dev->data)
		vfree(dev->data);
}


static int __init vmem_disk_init(void)
{
	int i;
	/*
	 * Get registered.
	 */
	vmem_disk_major = register_blkdev(vmem_disk_major, "vmem_disk");
	if (vmem_disk_major <= 0) {
		printk(KERN_WARNING "vmem_disk: unable to get major number\n");
		return -EBUSY;
	}
	/*
	 * Allocate the device array, and initialize each one.
	 */
	devices = kmalloc(ndevices*sizeof (struct vmem_disk_dev), GFP_KERNEL);
	if (!devices)
		goto out_unregister;
	for (i = 0; i < ndevices; i++)
		setup_device(devices + i, i);

	return 0;

out_unregister:
	unregister_blkdev(vmem_disk_major, "sbd");
	return -ENOMEM;
}

static void vmem_disk_exit(void)
{
	int i;

	for (i = 0; i < ndevices; i++) {
		struct vmem_disk_dev *dev = devices + i;

		del_timer_sync(&dev->timer);
		if (dev->gd) {
			del_gendisk(dev->gd);
			put_disk(dev->gd);
		}
		if (dev->queue) {
			if (request_mode == RM_NOQUEUE)
				kobject_put (&dev->queue->kobj);
			/* blk_put_queue() is no longer an exported symbol */
			else
				blk_cleanup_queue(dev->queue);
		}
		if (dev->data)
			vfree(dev->data);
	}
	unregister_blkdev(vmem_disk_major, "vmem_disk");
	kfree(devices);
}

module_init(vmem_disk_init);
module_exit(vmem_disk_exit);
