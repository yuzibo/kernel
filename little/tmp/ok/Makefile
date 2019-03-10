obj-m += 20.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
CFLAGS_20.o := -DDEBUG

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

