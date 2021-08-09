#include <linux/init.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_irq.h>
#include <linux/interrupt.h>



struct device_node *fskey;
u32 irqno[3];
char *irqname[3] = {"gpiof9", "gpiof7", "gpiof8"};

irqreturn_t key_irq_handle(int irqno, void *dev)
{
	int i = (int)dev;
	switch (i) {
		case 0:
			printk("key1 .............\n");
			break;
		case 1:
			printk("key2 #############\n");
			break;
		case 2:
			printk("key3 YYYYYYYYYYYYYY\n");
			break;	
	}
	return 0;
}


static int __init fskey_init(void)
{
	int i, ret;

	//从设备树中找到中断相关节点
	fskey = of_find_node_by_name(NULL, "key_irq");
	if(fskey == NULL) {
		printk("get node error\n");
		return -EAGAIN;
	}

	//从获取的节点中解析到中断号
	for(i=0;i<ARRAY_SIZE(irqno);i++) {
		irqno[i] = irq_of_parse_and_map(fskey,i);
		if(irqno[i] == 0) {
			printk("irq get and map error\n");
			return -EAGAIN;
		}
		//注册中断
		ret = request_irq(irqno[i], key_irq_handle,
			IRQF_TRIGGER_FALLING, irqname[i], (void *)i);
		if(ret) {
			printk("request %d irq error\n", irqno[i]);
			return ret;
		}	
	}

    return 0;
}

static void __exit fskey_exit(void)
{
	int i;
	for(i=0;i<ARRAY_SIZE(irqno);i++){
		free_irq(irqno[i],(void *)i);
	}
}
module_init(fskey_init);
module_exit(fskey_exit);
MODULE_LICENSE("GPL");