#include <linux/init.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_gpio.h>
#include <linux/gpio.h>

//led control driver code
//MIKE TAN

int core_led[3] = {0};
int extend_led[3] = {0};

struct device_node *fsled;

int get_fsled_node(void)
{
	fsled = of_find_node_by_path("/fsled");
	if(fsled == NULL){
		printk("get fsled node error\n");
		return -ENODEV;
	}
	return 0;
}
int fsled_all_init(void)
{
	int i,ret;
	for(i=0;i<ARRAY_SIZE(core_led);i++){
        
        //
		core_led[i] = of_get_named_gpio(fsled,"core_led",i);
		if(core_led[i] < 0){
			printk("get node gpio error\n");
			return core_led[i];
		}
        //
		extend_led[i] = of_get_named_gpio(fsled,"extend_led",i);
		if(extend_led[i] < 0){
			printk("get node gpio error\n");
			return extend_led[i];
		}

		printk("core_led[%d] = %d,extend_led[%d] = %d\n",
			i,core_led[i],i,extend_led[i]);

        //
		gpio_free(core_led[i]);
		ret = gpio_request(core_led[i] ,NULL);
		if(ret){
			printk("gpio %d request error\n",core_led[i]);
			return ret;
		}
		gpio_direction_output(core_led[i],1);

        //
		gpio_free(extend_led[i]);
		ret = gpio_request(extend_led[i] ,NULL);
			if(ret){
				printk("gpio %d request error\n",extend_led[i]);
				return ret;
		}
		gpio_direction_output(extend_led[i],1);
		
	}

	return 0;
}


static int __init fsled_init(void)
{
    int ret;
	
	if((ret = get_fsled_node())<0){
		return ret;
	}
	if((ret = fsled_all_init())<0){
		return ret;
	}
    return 0;
}

static void __exit fsled_exit(void)
{
    int i, j;
    for(i = 0;i<ARRAY_SIZE(core_led);i++) {
        gpio_free(core_led[i]);
    }
    for(j = 0;j<ARRAY_SIZE(extend_led);j++) {
        gpio_free(extend_led[i]);
    }

}
module_init(fsled_init);
module_exit(fsled_exit);
MODULE_LICENSE("GPL");
