#obj-m = bmp085-i2c.o
#KVERSION = $(shell uname -r)
#all:
#	make -C /lib/modules/$(KVERSION)/build M=$(PWD) modules
#clean:
#	make -C /lib/modules/$(KVERSION)/build M=$(PWD) clean



#export ARCH=arm
#export CROSS_COMPILE=arm-linux-gnueabihf-
obj-m := bmp085-i2c.o
KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)
all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
#       $(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
#       $(MAKE) -C $(KDIR) SUBDIRS=$(PWD) clean
