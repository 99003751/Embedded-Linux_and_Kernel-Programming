obj-m += dummy_serial.o
KSRC=/home/user/workspace/kernel_ws/linux-4.14.227
#replace KSRC if building for other kernel version
all:
	make -C ${KSRC} M=${PWD} modules ARCH=arm CROSS_COMPILE=arm-linux-gnueabi-
clean:
	make -C ${KSRC} M=${PWD} clean ARCH=arm CROSS_COMPILE=arm-linux-gnueabi-
