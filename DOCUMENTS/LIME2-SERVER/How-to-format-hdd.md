First of all you need to create a partition, *then* you can make the filesystem.

1. Create a new partition

   ```sudo fdisk /dev/sda```

   This will bring up the `fdisk` menu. You should familiarize yourself with it, search for tutorials ([here's one](http://www.howtogeek.com/106873/how-to-use-fdisk-to-manage-partitions-on-linux/)). The basic options are:

```
Command action
   a   toggle a bootable flagL
   b   edit bsd disklabel
   c   toggle the dos compatibility flag
   d   delete a partition
   l   list known partition types
   m   print this menu
   n   add a new partition
   o   create a new empty DOS partition table
   p   print the partition table
   q   quit without saving changes
   s   create a new empty Sun disklabel
   t   change a partition's system id
   u   change display/entry units
   v   verify the partition table
   w   write table to disk and exit
   x   extra functionality (experts only)
```

If all goes well, by pressing ```N```, you will be given the option of creating a new partition in the empty space. If the only unallocated space is the one you want to use, the default choices should be fine and you can just let `fdisk` choose.

2. Since you want to create an `ext` partition, you don't need to do anything. If you wanted to create a different type (swap or NTFS or whatever) you would need to use `t` to change the partition type. I am mentioning this just in case.
3. Check that your changes are what you expected by hitting ```P``` to print the partition table.
4. If everything is OK, write the new partition table to the disk (```W```) and exit (```Q```).

Now that you have your new, empty partition, you can create its filesystem. So, if you just created `/dev/sdaX` (where `X` is the number of the partition you created, for example `/dev/sda2`), run this:

```sudo mkfs.ext4 /dev/sda1```

