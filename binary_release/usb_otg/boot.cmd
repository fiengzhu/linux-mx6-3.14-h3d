setenv bootdelay 3 
setenv ethaddr "d0:63:b4:00:4b:50"
saveenv
setenv bootargs "root=/dev/mmcblk0p1 rootfstype=ext4 rootwait console=ttymxc0,115200 console=tty1 video=mxcfb0:dev=ldb,LDB-320P60,if=RGB666,bpp=18 raid=noautodetect quiet g_mass_storage.removable=1 g_mass_storage.idVendor=0x15a2 g_mass_storage.idProduct=0x7b g_mass_storage.iSerialNumber=123456abcdef g_mass_storage.luns=1"
ext2load mmc 0 ${fdt_addr} /boot/dtb/${fdt_file}
ext2load mmc 0 ${loadaddr} /boot/zImage
bootz ${loadaddr} - ${fdt_addr}
# Recompile with:
# mkimage -C none -A arm -T script -d boot.cmd boot.scr
