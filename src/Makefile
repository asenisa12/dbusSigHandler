DIR_SDK=/home/devteam/cite/ls1021/openwrt
DIR_TOOLCHAIN=$(DIR_SDK)/staging_dir/toolchain-arm_cortex-a7+neon_gcc-5.3.0_musl-1.1.14_eabi/bin

CC=$(DIR_TOOLCHAIN)/arm-openwrt-linux-gcc
LD=$(DIR_TOOLCHAIN)/arm-openwrt-linux-ld

DIR_TARGET=$(DIR_SDK)/staging_dir/target-arm_cortex-a7+neon_musl-1.1.14_eabi/usr

INCLUDE_DIR=$(DIR_TARGET)/include
LIB_DIR=$(DIR_SDK)/staging_dir/target-arm_cortex-a7+neon_musl-1.1.14_eabi/usr/lib

DBUS_DIR1=$(INCLUDE_DIR)/dbus-1.0
DBUS_DIR2=$(LIB_DIR)/dbus-1.0/include

LDFLAGS=-L$(LIB_DIR) -ldbus-1

SOURCES=main.c

OBJS=$(SOURCES:.c=.o)

all: all

%.o: %.c
	$(CC) -c  -I $(DBUS_DIR1) -I $(DBUS_DIR2) -o $@ $<
all: $(OBJS)
	$(CC) -o  dbusHandler $(OBJS) $(LDFLAGS)

clean:
	rm *.o 
