obj-m += test.o

ccflags-y := -Wunused-function

all:
	make -C /lib/modules/`uname -r`/build M=`pwd` modules
clean:
	make -C /lib/modules/`uname -r`/build M=`pwd` clean
test:
	sudo dmesg -C
	sudo insmod test.ko
	sudo rmmod test
	dmesg
