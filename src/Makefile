obj-m += Main.o

all:
	make -C /lib/modules/`uname -r`/build M=`pwd` modules
clean:
	make -C /lib/modules/`uname -r`/build M=`pwd` clean
	rm Main.o.rc
test:
	sudo dmesg -C
	sudo sh ../load.sh
	sudo sh ../unload.sh
	dmesg
