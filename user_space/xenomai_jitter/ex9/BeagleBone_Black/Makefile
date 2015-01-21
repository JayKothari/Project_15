###### CONFIGURATION ######

### List of applications to be build
APPLICATIONS = 1_trivial-periodic
 
ifeq ($(KERNELRELEASE),)
ifneq ($(APPLICATIONS),)

XENO ?= /usr/xenomai

XENOCONFIG=$(shell PATH=$(XENO):$(XENO)/bin:$(PATH) which xeno-config 2>/dev/null)

ifeq ($(XENOCONFIG),)
all::
	@echo ">>> Invoke make like this: \"make XENO=/path/to/xeno-config\" <<<"
	@echo
endif


CC=$(CROSS_COMPILE)gcc

CFLAGS=$(shell $(XENOCONFIG) --xeno-cflags) $(MY_CFLAGS) 

LDFLAGS=$(shell $(XENOCONFIG) --xeno-ldflags) $(MY_LDFLAGS) -lnative 

LDFLAGS+=-Xlinker -rpath -Xlinker $(shell $(XENOCONFIG) --libdir)

LDFLAGS+=-lxenomai
all:: $(APPLICATIONS)

clean::
	$(RM) $(APPLICATIONS) *.o

endif
endif



