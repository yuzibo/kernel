obj-m+=yubochar.o

all:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules
	$(CC) testyubochar.c -o testyubochar

clean:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) clean
