obj-m += kll_traverse.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

#KDIR = 	/usr/src/linux-headers-4.10.0-33-generic

#all:
#	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
#	rm -rf *.o *.ko *.mod.* *.symvers *.order
