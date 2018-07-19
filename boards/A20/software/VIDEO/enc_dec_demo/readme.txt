1:CEDARX_PATH  需要指定为sdk中CedarX 的目录绝对路径
2:LOCAL_C_INCLUDES 中 CHIP_F23  替换为默认SDK 中 CedarX-Project\CedarAndroidLib\ 目录配置，例如
LIB_JB_F33  ,需要改成 CHIP_F33。
3:A31 和 A20 使用 sunxi mem 管理fb内存，需要设置 CEDARX_USE_SUNXI_MEM_ALLOCATOR:=Y

2013-5-31：
1：支持A10 A10S A13 A20。
2：A31 A31S 支持解码，不支持渲染。