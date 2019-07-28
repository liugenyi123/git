/*
 *	simple char device driver 
 *
 *	author :liugentyi
 * 
 */
 
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

#define GLOBALMEM_SIZE	1024
#define MEM_CLEAR	0x1
#define GLOBAL_MAJOR	230

static int globalmem_major = GLOBAL_MAJOR;
module_param(globalmem_major,int,S_IRUGO);

struct globalmem_dev{
	struct cdev cdev;
	unsigned char mem[GLOBALMEM_SIZE];
};

struct globalmem_dev *globalmem_devp;

static int globalmem_open(struct inode *inode,struct file filp)
{
	filp->private_data = globalmem_devp;
	return 0;
}

static int globalmem_setup_release(struct inode *inode,struct file *filp)
{
	return 0;
}

static int globalmem_ioctl(struct file *filp,unsigned int amd,unsigned long arg)
{
	struct globalmem_dev *dev = filp->private_data; 
	switch(cmd)
	{
		case MEM_CLEAR:
			memset(globalmem_dev->mem,0,GLOBALMEM_SIZE);
			printk(KERN_INFO "globalmem is to zero\n");
			break;
		case default:
			break;
	}
	return 0;
}

static int globalmem_read(struct file *filp,void __user *buf,size_t size,loff_t *ppos)
{
	struct globalmem_dev  *dev = filp->private_data;
	loff_t p = *ppos;
	size_t count = size;
	int ret = 0;

	if(p >= GLOBALMEM_SIZE)
		return 0;
	if(count > GLOBALMEM_SIZE - p)
		count = GLOBALMEM_SIZE - p; 
	if(copy_to_user(buf,dev->mem + p,count)){
		ret = -EFAULT;
	}else{
		*ppos += count;
		ret = count;
		printk(KERN_INFO "read %u bytes(s) from %lu\n",count,p);
	}
	return ret;
}

static int globalmem_write(struct file *filp,void __user *buf,size_t size,loff_t *ppos)
{
	struct globalmem_dev *dev = filp->private_data;
	size_t count = size;
	loff_t p = *ppos;

	if(p >= GLOBALMEM_SIZE)
		return 0;
	if(count > GLOBALMEM_SIZE - p)
			count = GLOBALMEM_SIZE - p;

	if(copy_from_user(buf,dev->mem + p,count)){
		ret = _EFAULT;
	}else{
		*ppos += count;
		ret = count;
		printk(KERN_INFO "write %u bytes(s) to %lu\n",count,p);
	}
	return ret;
}

static int globalmem_llseek(struct file *filp,loff_t offset,int orig)
{
	int ret =0;
	struct globalmem_dev *dev = filp->private_data;
	switch(orig)
	{
		case 0:
			if(offset < 0){
				ret = -EINVAL;
				break;
			}
			if(offset > GLOBALMEM_SIZE){
				ret = -EINVAL;
				break;
			filp->f_pos = offset;
			ret = offset;
			break;
		case 1:
			if(filp->f_pos + offset  > GLOBALMEM_SIZE){
				ret = -EINVAL;
				break;
			}
			if(filp->f_pos + offset < 0){
				ret = -EINVAL;
			}
			filp->f_pos += offset;
			ret = filp->f_pos;
			break;
		case 2:
			if(offset < 0){
				ret = -EINVAL;
				break;
			}	
			if(offset > GLOBALMEM_SIZE){
				ret = -EINVAL;
				break;
			}
			filp->f_pos = GLOBALMEM_SIZE - offset-1;
			ret = filp->f_pos;
			break;
		case default:
			ret = -EINVAL;
			break;
	}
	return ret;
}
















