obj-m += task12-2.o
all:
		make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
CFLAGS_task12-2.o := -DDEBUG

clean:
		make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
