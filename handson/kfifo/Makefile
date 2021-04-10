obj-m += kfifo.o

all:
	make -C /home/user/workspace/kernel_ws/linux-4.14.227 M=$(PWD) modules ARCH=arm CROSS_COMPILE=arm-linux-gnueabi-
clean:
	make -C /home/user/workspace/kernel_ws/linux-4.14.227 M=$(PWD) clean ARCH=arm CROSS_COMPILE=arm-linux-gnueabi-
