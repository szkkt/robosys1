#include<linux/module.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/device.h>
#include<linux/uaccess.h>
#include<linux/io.h>
#include<linux/delay.h>

MODULE_AUTHOR("Suzuki Keita and Ryuichi Ueda");//
MODULE_DESCRIPTION("driver for LED control");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.0.1");

static dev_t dev;
static struct cdev cdv; 
static struct class *cls =NULL;
static volatile u32 *gpio_base=NULL;

static ssize_t led_write(struct file* filp, const char* buf, size_t count, loff_t* pos)
{
	char c;
	if(copy_from_user(&c, buf, sizeof(char)))
		return -EFAULT;

	if(c=='0'){       
	gpio_base[7]=1<<25; //1
        __delay(50*1000*1000);
    	gpio_base[10]=1<<25;gpio_base[7]=1<<18;//01
      	__delay(50*1000*1000);
	gpio_base[7]=1<<25;gpio_base[7]=1<<18;//11  
	__delay(50*1000*1000);
	gpio_base[10]=1<<25;gpio_base[10]=1<<18;gpio_base[7]=1<<22;//001
	__delay(50*1000*1000);
	gpio_base[7]=1<<25;gpio_base[10]=1<<18;gpio_base[7]=1<<22;//101
	__delay(50*1000*1000);
        gpio_base[10]=1<<25;gpio_base[7]=1<<18;gpio_base[7]=1<<22;//011
	__delay(50*1000*1000);
 	gpio_base[7]=1<<25;gpio_base[7]=1<<18;gpio_base[7]=1<<22;//111
	__delay(50*1000*1000); 
	gpio_base[10]=1<<25;gpio_base[10]=1<<18;gpio_base[10]=1<<22;gpio_base[7]=1<<23;//0001
	__delay(50*1000*1000); 
	gpio_base[7]=1<<25;gpio_base[10]=1<<18;gpio_base[10]=1<<22;gpio_base[7]=1<<23;//1001
	__delay(50*1000*1000);
	gpio_base[10]=1<<25;gpio_base[7]=1<<18;gpio_base[10]=1<<22;gpio_base[7]=1<<23;//0101
	__delay(50*1000*1000);
	gpio_base[7]=1<<25;gpio_base[7]=1<<18;gpio_base[10]=1<<22;gpio_base[7]=1<<23;//1101
	__delay(50*1000*1000);
        gpio_base[10]=1<<25;gpio_base[10]=1<<18;gpio_base[7]=1<<22;gpio_base[7]=1<<23;//0011
	__delay(50*1000*1000);
        gpio_base[7]=1<<25;gpio_base[10]=1<<18;gpio_base[7]=1<<22;gpio_base[7]=1<<23;//1011
	__delay(50*1000*1000);
        gpio_base[10]=1<<25;gpio_base[7]=1<<18;gpio_base[7]=1<<22;gpio_base[7]=1<<23;//0111
	__delay(50*1000*1000);
	gpio_base[7]=1<<25;gpio_base[7]=1<<18;gpio_base[7]=1<<22;gpio_base[7]=1<<23;//1111
	__delay(50*1000*1000);
        gpio_base[10]=1<<25;gpio_base[10]=1<<18;gpio_base[10]=1<<22;gpio_base[10]=1<<23;//00001
	gpio_base[7]=1<<24;
	}
	else if(c=='1'){
	gpio_base[10]=1<<25;
	gpio_base[10]=1<<18;
        gpio_base[10]=1<<22;
        gpio_base[10]=1<<23;
	gpio_base[10]=1<<24;
	}
	return 1;	
}

static struct file_operations led_fops = {
	.owner = THIS_MODULE,
	.write = led_write,
};

static int __init init_mod(void)
{
	int retval;
	retval=alloc_chrdev_region(&dev, 0, 1, "myled");
	if(retval<0){
	printk(KERN_ERR "alloc_chrdev_region failled\n");
	return retval;
	}
        printk(KERN_INFO "%s is loaded. major:%d\n", __FILE__,MAJOR(dev));  	
	cdev_init(&cdv, &led_fops);
	retval = cdev_add(&cdv, dev, 1);
	if(retval<0){
        printk(KERN_ERR "cdev_add failled. major:%d,minor:%d\n",MAJOR(dev),MINOR(dev));
	return retval;
	}
	cls = class_create(THIS_MODULE, "myled");
	if(IS_ERR(cls)){  
		printk(KERN_ERR "class?create failled.");
		return PTR_ERR(cls);
		}
	device_create(cls, NULL, dev, NULL, "myled%d",MINOR(dev));

	gpio_base=ioremap_nocache(0xfe200000,0xA0);

	const u32 led0 = 25;
	const u32 index0 = led0/10;
	const u32 shift0 = (led0%10)*3;
	const u32 mask0 = ~(0x7<<shift0);
	gpio_base[index0] = (gpio_base[index0] & mask0) | (0x1 << (shift0));
	const u32 led1 = 18;
        const u32 index1 = led1/10;
	const u32 shift1 = (led1%10)*3;
 	const u32 mask1 = ~(0x7<<shift1);
	gpio_base[index1] = (gpio_base[index1] & mask1) | (0x1 << (shift1));
        const u32 led2 = 22;
        const u32 index2 = led2/10;	 
        const u32 shift2 = (led2%10)*3;
        const u32 mask2 = ~(0x7<<shift2);
        gpio_base[index2] = (gpio_base[index2] & mask2) | (0x1 << (shift2));
        const u32 led3 = 23;
        const u32 index3 = led3/10;
        const u32 shift3 = (led3%10)*3;
        const u32 mask3 = ~(0x7<<shift3);
        gpio_base[index3] = (gpio_base[index3] & mask3) | (0x1 << (shift3));
	const u32 led4 = 24;
        const u32 index4 = led4/10;
        const u32 shift4 = (led4%10)*3;
	const u32 mask4 = ~(0x7<<shift4);
        gpio_base[index4] = (gpio_base[index4] & mask4) | (0x1 << (shift4));

	return 0;
}


static void __exit cleanup_mod(void)
{
 	cdev_del(&cdv);
	device_destroy(cls, dev);
	class_destroy(cls);
	unregister_chrdev_region(dev, 1);
	printk(KERN_INFO "%s is unloaded. major:%d\n", __FILE__,MAJOR(dev)); 
}
module_init(init_mod);
module_exit(cleanup_mod);

