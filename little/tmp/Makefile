obj-m += 17.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
CFLAGS_17.o := -DDEBUG

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

