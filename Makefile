#If KERNELRELEASE is defined, we've been invoked from the
#kernel build system and its language can be used.
ifneq ($(KERNELRELEASE),)
		obj-m := main.o

#Otherwise we are called directely from the command
#line, invoke kernel build system
else
		KERNELDIR ?= /lib/modules/$(shell uname -r)/build
		PWD := $(shell pwd)

default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean

endif

