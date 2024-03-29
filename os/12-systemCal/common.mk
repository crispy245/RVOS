CROSS_COMPILE = riscv64-unknown-linux-gnu-
CFLAGS = -nostdlib -fno-builtin -march=rv32imazicsr -mabi=ilp32 -g -Wall

QEMU = qemu-system-riscv32
QFLAGS = -nographic -smp 1 -machine virt -bios none 
QFLAGS += -drive format=raw,file=os.img,id=x0
QFLAGS += -device virtio-blk-device,drive=x0,bus=virtio-mmio-bus.0

hdd.dsk:
	dd if=/dev/urandom of=hdd.dsk bs=1048576 count=32

GDB = gdb-multiarch
CC = ${CROSS_COMPILE}gcc
OBJCOPY = ${CROSS_COMPILE}objcopy
OBJDUMP = ${CROSS_COMPILE}objdump