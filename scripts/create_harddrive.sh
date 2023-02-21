#!/bin/bash
#
# How to build a boot image 
# NOTE: This script cant run properly yet 
# Things described here should be done manually for now
# 
# COPYRIGHT © Nigel Barink 2023
#

echo "Building a FAT16 filesystem"

dd if=/dev/zero of=disk.img bs=512 count=131072
fdisk disk.img
# Use the following options in fdisk (Format Disk Tool)
# We want to create a MBR (NOT GPT) Partition table containing 1 logical disk 
# with a primary FAT16 partition marked bootable

#OPTIONs

# Create new DOS disklabel
# o
# Create new partition
# n
# Choose Primary as partition type
# p
# hit enter to choose default for the other options

# Mark partition 1 as bootable
# a

# Change partition type to FAT16
# t
# Choose Partition 1
# 1
# Choose HEX 6 for FAT16
# 6

# Sync and write changes to disk
# w

# Create a "block" device from the disk.img
losetup /dev/loop9 disk.img

# Format the partition on the disk as FAT16
mkdosfs -F16 /dev/loop9

# Mount the disk to a folder on our dev machine
mount /dev/loop9 /mnt

# Install the grub bootloader onto the disk 
grub-install --no-floppy --modules="normal multiboot" /dev/loop9 --target=i386-pc --boot-directory=/mnt/boot --force

# copy the necessary OS files 
cp root/boot/myos.bin /mnt/boot/myos.bin
cp root/boot/grub/grub.cfg /mnt/boot/grub/grub.cfg

# Unmount the device
umount /mnt

# Destroy the loop device
losetup -d /dev/loop9
