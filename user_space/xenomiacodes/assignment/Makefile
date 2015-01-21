obj-m           := 4ass.o
KDIR            := /lib/modules/$(shell uname -r)/build
PWD             := $(shell pwd)
EXTRA_CFLAGS    := -I/usr/xenomai/include -I/usr/include/

all:      

	  $(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules
clean:

	   $(MAKE) -C $(KDIR) SUBDIRS=$(PWD) clean

