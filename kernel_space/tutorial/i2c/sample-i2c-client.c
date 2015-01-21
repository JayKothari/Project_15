#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/i2c.h>
#include <linux/interrupt.h>

#include <linux/i2c-pb206x-platform.h>

struct sample_device {
        struct i2c_client *client;
        /* TODOi */
};


/* TODOi */

static int __devinit sample_i2c_probe(struct i2c_client *client,
                const struct i2c_device_id *id)
{

//regmap_get_i2c_bus
        struct sample_device *dev;
        int ret;

        if (!i2c_check_functionality(client->adapter,
                I2C_FUNC_SMBUS_BYTE_DATA | I2C_FUNC_SMBUS_WORD_DATA |
                I2C_FUNC_SMBUS_I2C_BLOCK)) {
                printk(KERN_ERR "%s: needed i2c functionality is not supported\n", __func__);
                return -ENODEV;
        }
        
        dev = kzalloc(sizeof(struct sample_device), GFP_KERNEL);
        if (dev == NULL) {
                printk(KERN_ERR "%s: no memory\n", __func__);
                return -ENOMEM;
        }

        dev->client = client;
        i2c_set_clientdata(client, dev);
       /*are used to get/set the void *driver_data pointer that is part of the struct device, itself part of struct i2c_client.*/

        // pdata = client->dev.platform_data;
        
        /* TODOi: do something */

        return 0;
}

static int __devexit sample_i2c_remove(struct i2c_client *client)
{
        struct sample_client *dev = i2c_get_clientdata(client);

        /* TODOi: do something */

        kfree(dev);
        return 0;
}

#ifdef CONFIG_PM
static int sample_i2c_suspend(struct i2c_client *client, pm_message_t msg)
{
        struct sample_device *dev = i2c_get_clientdata(client);
        
        return 0;
}

static int sample_i2c_resume(struct i2c_client *client)
{
        struct sample_device *dev = i2c_get_clientdata(client);

        return 0;
}
#else
#define sample_i2c_suspend NULL
#define sample_i2c_resume  NULL
#endif

static const struct i2c_device_id sample_i2c_id[] = {
        { "sample-i2c-client", 0 },
        { }
};

static struct i2c_driver sample_i2c_driver = {
        .probe    = sample_i2c_probe,
        .remove   = __devexit_p(sample_i2c_remove),
        .id_table = sample_i2c_id,
        .suspend  = sample_i2c_suspend,
        .resume   = sample_i2c_resume,
        .driver   = {
                .name = "sample-i2c-client",
        },
};

static int __init sample_i2c_init_driver(void)
{
        return i2c_add_driver(&sample_i2c_driver);
}

static void __exit sample_i2c_exit_driver(void)
{
        i2c_del_driver(&sample_i2c_driver);
}

module_init(sample_i2c_init_driver);
module_exit(sample_i2c_exit_driver);

MODULE_DESCRIPTION("Sample I2C client driver");
MODULE_LICENSE("GPL");
MODULE_DEVICE_TABLE(i2c,sample-i2c-id);
MODULE_ALIAS("sample-i2c-client");

