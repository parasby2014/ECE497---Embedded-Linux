/* Example Minimal Character Device Driver */
#include <linux/module.h>
static int debug_enable0 = 0;
static int debug_enable1 = 0;

/* Added driver parameters */
module_param(debug_enable0, int, 0); 
MODULE_PARM_DESC(debug_enable0, "Enable module debug0 mode.");

module_param(debug_enable1, int, 0); 
MODULE_PARM_DESC(debug_enable1, "Enable module debug1 mode.");

/*add new print values for new module*/
static int __init hello_init(void)
{
    printk(KERN_INFO "Hello Example Init - debug0 mode: %s debug1 mode: %s\n", 			debug_enable0 ? "enabled" : "disabled", debug_enable1 ? "enabled" : 			"disabled");

    return 0;
}

static void __exit hello_exit(void)
{
    printk(KERN_INFO "Hello Example Exit\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_AUTHOR("Chris Hallinan");
MODULE_DESCRIPTION("Hello World Example");
MODULE_LICENSE("GPL");
