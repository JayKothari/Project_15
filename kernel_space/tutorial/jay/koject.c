#include<linux/module.h>
#include<linux/init.h>
#include<linux/kobject.h>
static struct kobject *ex_kobj;
static struct kset *example_kset;


static struct kset *kset1;

static int __init ex_init(void)
{
int i,j;
 printk(KERN_INFO "i am loading...n");
for(i=0;i<5;i++)
{
 
 ex_kobj = kobject_create_and_add("example", kernel_kobj);
 
//   j=kset_register(&kset1);

    example_kset = kset_create_and_add("kset_example", NULL, kernel_kobj);



 return 0;
}
 
}

static void __exit ex_exit(void)
{
 kobject_put(ex_kobj);

	
// kset_unregister(&kset1);

kset_unregister(example_kset);

 printk(KERN_INFO "i am unloading...n");
}
 
module_init(ex_init);
module_exit(ex_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("ravi teja");
