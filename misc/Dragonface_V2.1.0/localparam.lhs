; 文件路径参数

; bootfs文件夹存放的路径，相对于工具执行目录
[bootfs]
local=fsop\bootfs

; 固件修改FAT 文件夹存放的路径，相对于工具执行目录
[vfatfs]
local=fsop\vfatfs

; APK存放目录，相对于固件修改FAT 分区路径
[apk]
system=\Resources\apk\system
vendor=\Resources\apk\preinstall

;动画文件路径，相对于固件修改FAT分区
[bootmanimation]
local=\Resources\bootanimation\bootanimation.zip

[wallpaper];桌面背景,相对于固件修改FAT分区
local=\Resources\DefaultWallpapaer\default_wallpaper.jpg

[buildprop]
local=\build.prop

;setting参数文件存放路径， 相对于固件修改FAT分区
[setting]
local=\settings.ini

[keymap]
local=\Resources\keylayout
