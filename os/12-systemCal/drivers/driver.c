#include "os.h"

console_t console;
disk_t disk;

static void console_init() {
    deviceCB_t *console_dev = serial_init();
    if (console_dev) {
        device_open(console_dev, FLAG_INT_RX);
        console.dev = console_dev;
        lock_init(&console.lock);
    }else{
        while(1);
    }
}

static void disk_init(){
    deviceCB_t *disk_dev = disk_dev_init();
    if(disk_init){
        device_open(disk_dev,FLAG_DMA_RX);
        disk.dev = disk_dev;
        lock_init(&disk.lock);
    }
    else{
        while (1);
    }
}

void drivers_init() {
    console_init();
    disk_init();
    kprintf("Console and Disk initiated...\n");
}