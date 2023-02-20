#!/bin/bash
echo "Mount harddrive image as block device"
sudo losetup /dev/loop9 disk.img
sudo mount /dev/loop9 /mnt

echo "Copy over kernel binary"
sudo cp build/kernel/myos.bin /mnt/boot/myos.bin

echo "unmount image"
sudo umount /mnt
sudo losetup -d /dev/loop9