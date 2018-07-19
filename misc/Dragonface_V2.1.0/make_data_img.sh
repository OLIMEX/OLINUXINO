#!/system/bin/sh

function get_owner()
{
	a=$1
	b=$2
	ret=`cd $a; ls -l |busybox grep -w "${b}$" | busybox awk '{print $2}'`
	echo $ret
}

function get_group()
{
	a=$1
	b=$2
	ret=`cd $a; ls -l |busybox grep -we "${b}$" | busybox awk '{print $3}'`
	echo $ret
}

function get_owner_group()
{
	owner=`get_owner $1 $2`
	group=`get_group $1 $2`
	ret=`echo "${owner}:${group}"`
	echo $ret
}

function get_fspart_kbytes_size()
{
	a=$1
	ret=`busybox df -k | busybox grep -we "${a}$" | busybox awk '{print $1}' | busybox cut -d$'\n' -f2`
	let ret=$ret+16*1024
	let ret=$ret/16/1024
	let ret=$ret*16*1024
	echo $ret
}



function ensure_path_mounted()
{
	dev=$1
	mount_point=$2
	fstype=$3
	
	ret1=`busybox df | busybox grep ${dev}`
	ret2=`busybox df | busybox grep ${mount_point}`
	if [ -z "$ret1" -a -z "$ret2" ]; then
		mkdir -p ${mount_point}
		mount -t ${fstype} ${dev} ${mount_point}
	fi
	
	ret=`busybox df | busybox grep ${dev}`
	if [ -z "$ret" ]; then
		return 1
	else
		return 0
	fi
}

mkdir -p /cache/data/data/com.android.providers.settings/databases/
mkdir -p /cache/data/data/com.android.launcher/databases/
mkdir -p "/cache/data/system/users/0"
chown `get_owner_group "/" "data"`  /cache/data
chown `get_owner_group "/data" "data"` /cache/data/data
chown `get_owner_group "/data/data" "com.android.providers.settings"`  /cache/data/data/com.android.providers.settings
chown `get_owner_group "/data/data/com.android.providers.settings/" "databases"`  /cache/data/data/com.android.providers.settings/databases
chown `get_owner_group "/data/data" "com.android.launcher"`  /cache/data/data/com.android.launcher
chown `get_owner_group "/data/data/com.android.launcher/" "databases"`  /cache/data/data/com.android.launcher/databases
chown `get_owner_group  "/data" "system"` /cache/data/system
chown `get_owner_group "/data/system/" "users"` /cache/data/system/users
chown `get_owner_group "/data/system/users" "0"` "/cache/data/system/users/0"
chmod -R 771 /cache/data/
cp -pf /data/data/com.android.providers.settings/databases/* /cache/data/data/com.android.providers.settings/databases/
echo "delete from secure where name = 'android_id';" > /system/bin/d.sql
sqlite3 /cache/data/data/com.android.providers.settings/databases/settings.db < /system/bin/d.sql
rm /system/bin/d.sql
cp -pf /data/data/com.android.launcher/databases/launcher.db /cache/data/data/com.android.launcher/databases/
#cp -prf /data/app /cache/data/
cp -pf /data/system/users/0/wallpaper_info.xml /cache/data/system/users/0/
cp -pf /data/system/users/0/wallpaper /cache/data/system/users/0/

mkdir -p /cache/databk
ensure_path_mounted "/dev/block/by-name/databk"  "/databk"  "ext4"
if [ -f "/databk/data_backup.tar" ]; then
	busybox tar -pxf /mnt/sdcard/data_backup.tar -C /cache/data
fi
cd /cache
busybox tar -pcf /mnt/sdcard/data_backup.tar ./data
cd -

rm -rf /cache/databk
rm -rf /cache/data

