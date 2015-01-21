/*
 * Copyright (c) 2012  Bosch Sensortec GmbH
 * Copyright (c) 2012  Unixphere AB
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/err.h>
#include "bmp085.h"

#define BMP085_I2C_ADDRESS	0x77

static const unsigned short normal_i2c[] = { BMP085_I2C_ADDRESS,
							I2C_CLIENT_END };

static int bmp085_i2c_detect(struct i2c_client *client,
			     struct i2c_board_info *info)
{
	if (client->addr != BMP085_I2C_ADDRESS)
		return -ENODEV;

	return bmp085_detect(&client->dev);
}

static int bmp085_i2c_probe(struct i2c_client *client,
				      const struct i2c_device_id *id)
{
	int err;
	struct regmap *regmap = devm_regmap_init_i2c(client,
						     &bmp085_regmap_config);

	if (IS_ERR(regmap)) {
		err = PTR_ERR(regmap);
		dev_err(&client->dev, "Failed to init regmap: %d\n", err);
		return err;
	}

	return bmp085_probe(&client->dev, regmap);
}

static int bmp085_i2c_remove(struct i2c_client *client)
{
	return bmp085_remove(&client->dev);
}

static const struct i2c_device_id bmp085_id[] = {
	{ BMP085_NAME, 0 },
	{ "bmp180", 0 },
	{ }
};
MODULE_DEVICE_TABLE(i2c, bmp085_id);

/*
struct module *owner; — set to the value THIS_MODULE that allows the proper module reference counting.

char name[I2C_NAME_SIZE]; — set to a descriptive name of the I2C chip driver. This value shows up in the sysfs file name created for every I2C chip device.

unsigned int flags; — set to the value I2C_DF_NOTIFY in order for the chip driver to be notified of any new I2C devices loaded after this driver is loaded. This field probably will go away soon, as almost all drivers set this field.

int (*attach_adapter)(struct i2c_adapter *); — called whenever a new I2C bus driver is loaded in the system. This function is described in more detail below.

int (*detach_client)(struct i2c_client *); — called when the i2c_client device is to be removed from the system. More information about this function is provided below. 
*/

static struct i2c_driver bmp085_i2c_driver = {
	.driver = {
		.owner	= THIS_MODULE,
		.name	= BMP085_NAME,
	},
	.id_table	= bmp085_id,
	.probe		= bmp085_i2c_probe,
	.remove		= bmp085_i2c_remove,

	.detect		= bmp085_i2c_detect,
	.address_list	= normal_i2c
};

/*


After the I2C chip driver is registered, the attach_adapter function callback is called when an I2C bus driver is loaded. This function checks to see if any I2C devices are on this I2C bus to which the client driver wants to attach. Almost all I2C chip drivers call the core I2C function i2c_detect to determine this. For example, the tiny_i2c_chip.c driver does this:

static int
chip_attach_adapter(struct i2c_adapter *adapter)
{
    return i2c_detect(adapter, &addr_data,
                      chip_detect);
}

*/

//this macro is used in place of init and exit which include i2c_add_driver
module_i2c_driver(bmp085_i2c_driver);

MODULE_AUTHOR("Eric Andersson <eric.andersson@unixphere.com>");
MODULE_DESCRIPTION("BMP085 I2C bus driver");
MODULE_LICENSE("GPL");
