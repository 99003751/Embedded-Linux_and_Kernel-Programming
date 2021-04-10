obj-m += sema.o

#KSRC=/lib/modules/$(shell uname -r)/build
KSRC=/home/user/workspace/kernel_ws/linux-4.14.227
#ARCH=arm
#CROSS_COMPILE_PREFIX=arm-linux-gnueabi-

all:
	make -C ${KSRC} M=${PWD} modules ARCH=arm CROSS_COMPILE=arm-linux-gnueabi-
clean:
	make -C ${KSRC} M=${PWD} clean  ARCH=arm CROSS_COMPILE=arm-linux-gnueabi-

