obj-m           := tut01-skeleton-drv.o
KDIR            := /lib/modules/$(shell uname -r)/build
PWD             := $(shell if [ "$$PWD" != "" ]; then echo $$PWD; else pwd; fi)
EXTRA_CFLAGS    := -I$(KDIR)/include/xenomai -I$(KDIR)/include/xenomai/posix $(ADD_CFLAGS)

all:      
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules
clean:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) clean


