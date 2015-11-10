#!/bin/bash

BACKTITLE="OlinuXino screen configurator"

# Due to different versions of the script.fex file,
# variable names may missmatch. So define them here
PARAM_SCREEN0_OUTPUT_TYPE="screen0_output_type"
PARAM_LCD_X="lcd_x"
PARAM_LCD_Y="lcd_y"
PARAM_LCD_DCLK_FREQ="lcd_dclk_freq"
PARAM_LCD_HBP="lcd_hbp"
PARAM_LCD_HT="lcd_ht"
PARAM_LCD_VBP="lcd_vbp"
PARAM_LCD_VT="lcd_vt"
PARAM_LCD_VSPW="lcd_vspw"
PARAM_LCD_HSPW="lcd_hspw"
PARAM_LCD_IF="lcd_if"
PARAM_LCD_LVDS_BITWIDTH="lcd_lvds_bitwidth"
PARAM_LCD_IO_CFG0="lcd_io_cfg0"
PARAM_LCD_BL_EN_USED="lcd_bl_en_used"
PARAM_FB0_SCALER_MODE_ENABLE="fb0_scaler_mode_enable"
PARAM_FB0_WIDTH="fb0_width"
PARAM_FBO_HEIGHT="fb0_height"
PARAM_LCD_LVDS_CH="lcd_lvds_ch"
PARAM_LCD_FRM="lcd_frm"
PARAM_LCD0_BACKLIGHT="lcd0_backlight"
PARAM_PWM_USED="pwm_used"

# Define directories
TEMP_DIR=${TEMP_DIR:="/tmp/screen"}
MMC_DIR=${MMC_DIR:="/tmp/mmc"}
SUNXI_DIR=${SUNXI_DIR:="/opt/sunxi-tools"}

BOOT_SOURCE=

# Define tools
BIN2FEX=${BIN2FEX:="$SUNXI_DIR/bin2fex"}
FEX2BIN=${FEX2BIN:="$SUNXI_DIR/fex2bin"}
DEVMEM=${DEVMEM:="/opt/mem/devmem"}

# Define script files
BIN_FILE=${BIN_FILE:="$MMC_DIR/script.bin"}
FEX_FILE=${FEX_FILE:="$TEMP_DIR/script.fex"}
RCLOCAL=${RCLOCAL:="/etc/rc.local"}
SOFTPWM=${SOFTPWM:="/opt/softpwm.ko"}

tempfile1=/tmp/dialog_1_$$
tempfile2=/tmp/dialog_2_$$
tempfile3=/tmp/dialog_3_$$
tempfile4=/tmp/dialog_4_$$

# Define some functions

# Find line number for a give word
#
# Parameters:
# $1	<Word> to be searched
#
# Return:
# <line> -> If word is found
# <null> -> There is no such word
#
# Note: Only the first coincidence is returned
function find_word
{
	echo $(grep -nr -m 1 "$1" $FEX_FILE | awk '{print$1}' FS=":")
}

# Find last match in file
#
# Parameters:
# $1 <File>
# $2 <Word> to be searched
#
# Return:
# <line> -> If word is found
# <null> -> There is no such word
#
function find_last_word
{
	echo $(grep -nr "$2" $1 | tail -1 | awk '{print$1}' FS=":")
}

# Insert into rc.local
function insert_rc_local
{
	# Check if the line is already inserted
	local var=$(find_last_word $RCLOCAL "$1")
	if [ ! -z $var ];
	then
		return
	fi

	local line=$(find_last_word $RCLOCAL "exit")
	if [ -z $line ];
	then
		dialog --infobox "Cannot find 'exit 0' in $RCLOCAL" 0 0
		sleep 2
		cleanup
		exit
	fi

	# Insert line
	sed -i $line'i'"$1" $RCLOCAL
}

function delete_rc_local
{
	local line=$(find_last_word $RCLOCAL $1)
	if [ ! -z $line ];
	then
		sed -i $line'd' $RCLOCAL
	fi
}

function change_parameter_in_section
{
	# Find section line
	local section=$(find_word $1)
	if [ -z $section ];
	then
		dialog --infobox "Cannot find $1 variable!" 0 0
		sleep 2
		cleanup
		exit
	fi

	grep -nr "$2" $FEX_FILE | awk '{print$1}' FS=":" | while read -r line;
	do
		local var=$line;
        if [ "$var" -gt "$section" ];
        then
        	# Replace parameter
			sed -i $var's/.*/'$2' = '$3'/' $FEX_FILE
            break
        fi
    done

}
# Find parameter and set its value
# Parameters:
#	$1	<parameter> to search for
#	$2	new value
#
# Note: if a parameter is present in multiple places only the first is replaced.
function change_parameter
{
	# Find line number
	local line=$(find_word $1)

	# Check if parameter is null
	if [ -z $line ];
	then
		dialog --infobox "Cannot find $1 variable!" 0 0
		sleep 2
		cleanup
		exit
	fi

	# Replace parameter
	sed -i $line's/.*/'$1' = '$2'/' $FEX_FILE
}


# Find parameter and delete it
# Parameters:
# $1 <parameter> to search for
# Note: only the first found parameter is deleted.
function delete_parameter
{
	# Find line number
	local line=$(find_word $1)

	# Check if parameter is null
	if [ ! -z $line ];
	then
		# Delete parameter
		sed -i $line'd' $FEX_FILE
	fi


}


# Display confirmation dialog
# Parameters:
#	$1	<Dialog> to be displayed
# Return:
#	0 -> "YES" is pressed
#	1 -> "NO" is pressed
function display_confirm
{
	local __result=$2
	dialog --title "Confirmation" --backtitle "$BACKTITLE" --yesno "$1" 0 0
	eval $__result=$?
}

# Insert a line after matching word
# Parameters:
# $1	<Word>	to be searched
# $2 	<Parameter> to be inserted
# $3	<Value>
function insert_after
{
	# Find word
	local line=$(find_word $1)

	# Chech if it's unset
	if [ -z $line ];
	then
		dialog --infobox "Cannot find $1 variable!" 0 0
		sleep 2
		cleanup
		exit
	else
		local next_line=$(sed -n $(($line+1))'p' $FEX_FILE)
		if [[ ! $next_line =~ .*$2.* ]];
		then
			sed -i $line'a\'$2' = '$3'' $FEX_FILE
		fi
	fi
}



set_screen_none() {
	display_confirm "Disable screen ?" result
	if [ $result -eq 0 ];
	then
		change_parameter $PARAM_SCREEN0_OUTPUT_TYPE "0"
		delete_rc_local "softpwm"
    	delete_rc_local "0x01c20118"
	fi
}

set_screen_hdmi() {

    dialog --backtitle "$BACKTITLE" \
    --menu "Select screen mode:" 0 0 0 \
    0	"480i"		 \
    1	"576i"		 \
    2	"480p"		 \
    3	"576p"		 \
    4	"720p50"	 \
    5	"720p60"	 \
    6	"1080i50"	 \
    7	"1080i60"	 \
    8	"1080p24"	 \
    9	"1080p50"	 \
    10	"1080p60"	 \
    11	"pal"		 \
    14	"ntsc"		2> $tempfile3

	retv=$?
	choice=$(cat $tempfile3)

	# Check if ESC of CANCLE are pressed
	if [ $retv -eq 1 -o $retv -eq 255 ];
	then
		clear
		cleanup
		exit
	fi

	case $choice in
		0)
			display_confirm "Set HDMI to 480i ?" result
    		if [ $result -eq 0 ];
			then
			   	change_parameter $PARAM_SCREEN0_OUTPUT_TYPE "3"
    			change_parameter "screen0_output_mode" $choice
    			delete_rc_local "softpwm"
    			delete_rc_local "0x01c20118"
			fi
    	;;
		1)
			display_confirm "Set HDMI to 576i ?" result
    		if [ $result -eq 0 ];
			then
			   	change_parameter $PARAM_SCREEN0_OUTPUT_TYPE "3"
    			change_parameter "screen0_output_mode" $choice
    			delete_rc_local "softpwm"
    			delete_rc_local "0x01c20118"
			fi
    	;;
		2)
			display_confirm "Set HDMI to 480p ?" result
    		if [ $result -eq 0 ];
			then
			   	change_parameter $PARAM_SCREEN0_OUTPUT_TYPE "3"
    			change_parameter "screen0_output_mode" $choice
    			delete_rc_local "softpwm"
    			delete_rc_local "0x01c20118"
			fi
    	;;
		3)
			display_confirm "Set HDMI to 576p ?" result
    		if [ $result -eq 0 ];
			then
			   	change_parameter $PARAM_SCREEN0_OUTPUT_TYPE "3"
    			change_parameter "screen0_output_mode" $choice
    			delete_rc_local "softpwm"
    			delete_rc_local "0x01c20118"
			fi
    	;;
		4)
			display_confirm "Set HDMI to 720p50 ?" result
    		if [ $result -eq 0 ];
			then
			   	change_parameter $PARAM_SCREEN0_OUTPUT_TYPE "3"
    			change_parameter "screen0_output_mode" $choice
    			delete_rc_local "softpwm"
    			delete_rc_local "0x01c20118"
			fi
    	;;
		5)
			display_confirm "Set HDMI to 720p60 ?" result
    		if [ $result -eq 0 ];
			then
			   	change_parameter $PARAM_SCREEN0_OUTPUT_TYPE "3"
    			change_parameter "screen0_output_mode" $choice
    			delete_rc_local "softpwm"
    			delete_rc_local "0x01c20118"
			fi
    	;;
		6)
			display_confirm "Set HDMI to 1080i50 ?" result
    		if [ $result -eq 0 ];
			then
			   	change_parameter $PARAM_SCREEN0_OUTPUT_TYPE "3"
    			change_parameter "screen0_output_mode" $choice
    			delete_rc_local "softpwm"
    			delete_rc_local "0x01c20118"
			fi
    	;;
		7)
			display_confirm "Set HDMI to 1080i60 ?" result
    		if [ $result -eq 0 ];
			then
			   	change_parameter $PARAM_SCREEN0_OUTPUT_TYPE "3"
    			change_parameter "screen0_output_mode" $choice
    			delete_rc_local "softpwm"
    			delete_rc_local "0x01c20118"
			fi
    	;;
		8)
			display_confirm "Set HDMI to 1080p24 ?" result
    		if [ $result -eq 0 ];
			then
			   	change_parameter $PARAM_SCREEN0_OUTPUT_TYPE "3"
    			change_parameter "screen0_output_mode" $choice
    			delete_rc_local "softpwm"
    			delete_rc_local "0x01c20118"
			fi
    	;;
		9)
			display_confirm "Set HDMI to 1080p50 ?" result
    		if [ $result -eq 0 ];
			then
			   	change_parameter $PARAM_SCREEN0_OUTPUT_TYPE "3"
    			change_parameter "screen0_output_mode" $choice
    			delete_rc_local "softpwm"
    			delete_rc_local "0x01c20118"
			fi
    	;;
		10)
			display_confirm "Set HDMI to 1080p60 ?" result
    		if [ $result -eq 0 ];
			then
			   	change_parameter $PARAM_SCREEN0_OUTPUT_TYPE "3"
    			change_parameter "screen0_output_mode" $choice
    			delete_rc_local "softpwm"
    			delete_rc_local "0x01c20118"
			fi
    	;;
		11)
			display_confirm "Set HDMI to pal ?" result
    		if [ $result -eq 0 ];
			then
			   	change_parameter $PARAM_SCREEN0_OUTPUT_TYPE "3"
    			change_parameter "screen0_output_mode" $choice
    			delete_rc_local "softpwm"
    			delete_rc_local "0x01c20118"
			fi
    	;;
		14)
			display_confirm "Set HDMI to ntsc ?" result
    		if [ $result -eq 0 ];
			then
			   	change_parameter $PARAM_SCREEN0_OUTPUT_TYPE "3"
    			change_parameter "screen0_output_mode" $choice
    			delete_rc_local "softpwm"
    			delete_rc_local "0x01c20118"
			fi
    	;;
	esac


}

set_screen_vga() {

    dialog --backtitle "$BACKTITLE" \
    --menu "Select screen mode:" 0 0 0 \
    0	"1680x1050"	 \
    1	"1440x900"	 \
    2	"1360x768"	 \
    3	"1280x1024"	 \
    4	"1024x768"	 \
    5	"800x600"	 \
    6	"640x480"	 \
    10	"1920x1080"	 \
    11	"1280x720"	 2> $tempfile4

	retv=$?
	choice=$(cat $tempfile4)

	# Check if ESC of CANCLE are pressed
	if [ $retv -eq 1 -o $retv -eq 255 ];
	then
		clear
		cleanup
		exit
	fi

	#sed -i $line's/.*/screen0_output_type = 4/' $TEMP_DIR/$FEX_FILE
	case $choice in
		0)
			display_confirm "Set VGA to 1680x1050 ?" result
    		if [ $result -eq 0 ];
			then
			   	change_parameter $PARAM_SCREEN0_OUTPUT_TYPE "4"
    			change_parameter "screen0_output_mode" $choice
    			delete_rc_local "softpwm"
    			delete_rc_local "0x01c20118"
			fi
    	;;
    	1)
			display_confirm "Set VGA to 1440x900 ?" result
			if [ $result -eq 0 ];
			then
			   	change_parameter $PARAM_SCREEN0_OUTPUT_TYPE "4"
    			change_parameter "screen0_output_mode" $choice
    			delete_rc_local "softpwm"
    			delete_rc_local "0x01c20118"
			fi
    	;;
    	2)
			display_confirm "Set VGA to 1360x768 ?" result
			if [ $result -eq 0 ];
			then
			   	change_parameter $PARAM_SCREEN0_OUTPUT_TYPE "4"
    			change_parameter "screen0_output_mode" $choice
    			delete_rc_local "softpwm"
    			delete_rc_local "0x01c20118"
			fi
    	;;
    	3)
			display_confirm "Set VGA to 1280x1024 ?" result
			if [ $result -eq 0 ];
			then
			   	change_parameter $PARAM_SCREEN0_OUTPUT_TYPE "4"
    			change_parameter "screen0_output_mode" $choice
    			delete_rc_local "softpwm"
    			delete_rc_local "0x01c20118"
			fi
    	;;
    	4)
			display_confirm "Set VGA to 1024x768 ?" result
			if [ $result -eq 0 ];
			then
			   	change_parameter $PARAM_SCREEN0_OUTPUT_TYPE "4"
    			change_parameter "screen0_output_mode" $choice
    			delete_rc_local "softpwm"
    			delete_rc_local "0x01c20118"
			fi
    	;;
    	5)
			display_confirm "Set VGA to 800x600 ?" result
			if [ $result -eq 0 ];
			then
			   	change_parameter $PARAM_SCREEN0_OUTPUT_TYPE "4"
    			change_parameter "screen0_output_mode" $choice
    			delete_rc_local "softpwm"
    			delete_rc_local "0x01c20118"
			fi
    	;;
    	6)
			display_confirm "Set VGA to 640x480 ?" result
			if [ $result -eq 0 ];
			then
			   	change_parameter $PARAM_SCREEN0_OUTPUT_TYPE "4"
    			change_parameter "screen0_output_mode" $choice
    			delete_rc_local "softpwm"
    			delete_rc_local "0x01c20118"
			fi
    	;;
    	10)
			display_confirm "Set VGA to 1920x1080 ?" result
			if [ $result -eq 0 ];
			then
			   	change_parameter $PARAM_SCREEN0_OUTPUT_TYPE "4"
    			change_parameter "screen0_output_mode" $choice
    			delete_rc_local "softpwm"
    			delete_rc_local "0x01c20118"
			fi
    	;;
    	11)
    		display_confirm "Set VGA to 1280x720 ?" result
    		if [ $result -eq 0 ];
    		then
			   	change_parameter $PARAM_SCREEN0_OUTPUT_TYPE "4"
    			change_parameter "screen0_output_mode" $choice
    			delete_rc_local "softpwm"
    			delete_rc_local "0x01c20118"
			fi
    	;;
	esac


}

set_screen_lcd() {
	dialog --backtitle "$BACKTITLE" --menu "Select screen mode:" 0 0 0 \
    "4.3"	"480x272"	 \
    "7.0"	"800x480"	 \
    "10.3"	"1024x600"	 \
    "15.6"	"1366x768"	 \
    "15.6-FHD"	"1920x1080" 2> $tempfile2

	retv=$?
	choice=$(cat $tempfile2)

	# Check if ESC of CANCLE are pressed
	if [ $retv -eq 1 -o $retv -eq 255 ];
	then
    	clear
    	cleanup
    	exit
    fi

	case $choice in
    "4.3")
	    x=480
	    y=272
	    freq=9
	    hbp=40
	    ht=525
	    vbp=8
	    vt=576
	    vspw=5
	    hspw=30
	    lcd_if=0
	    lcd_lvds_bitwidth=0
	    lcd_lvds_ch=0
	    lcd_frm=1
	    lcd_io_cfg0=268435456
	    lcd_bl_en_used=1
	    fb0_scaler_mode_enable=0
	    fb0_width=0
	    fb0_height=0
	    lcd_backlight=250
	    pwm_used=1
    ;;
    "7.0")
	    x=800
	    y=480
	    freq=33
	    hbp=46
	    ht=1055
	    vbp=23
	    vt=1050
	    vspw=1
	    hspw=30
	    lcd_backlight=250
	    lcd_if=0
	    lcd_lvds_bitwidth=0
	    lcd_lvds_ch=0
	    lcd_frm=1
	    lcd_io_cfg0=268435456
	    lcd_bl_en_used=1
	    fb0_scaler_mode_enable=0
	    fb0_width=0
	    fb0_height=250
	    pwm_used=1
    ;;
    "10.3")
	    x=1024
	    y=600
	    freq=45
	    hbp=160
	    ht=1200
	    vbp=23
	    vt=1250
	    vspw=2
	    hspw=10
	    lcd_if=0
	    lcd_lvds_bitwidth=0
	    lcd_lvds_ch=0
	    lcd_frm=1
	    lcd_io_cfg0=268435456
	    lcd_bl_en_used=1
	    fb0_scaler_mode_enable=0
	    fb0_width=0
	    fb0_height=0
	    lcd_backlight=250
	    pwm_used=1
    ;;
	"15.6")
	    x=1366
	    y=768
	    freq=70
	    hbp=54
	    ht=1440
	    vbp=23
	    vt=1616
	    vspw=0
	    hspw=0
	    lcd_if=3
	    lcd_lvds_bitwidth=1
	    lcd_lvds_ch=0
	    lcd_frm=1
	    lcd_io_cfg0=268435456
	    lcd_bl_en_used=0
	    fb0_scaler_mode_enable=1
	    fb0_width=1366
	    fb0_height=768
	    lcd_backlight=0
	    pwm_used=0
    ;;
    "15.6-FHD")
	    x=1920
	    y=1080
	    freq=152
	    hbp=100
	    ht=2226
	    vbp=23
	    vt=2284
	    vspw=0
	    hspw=0
	    lcd_if=3
	    lcd_lvds_bitwidth=1
	    lcd_lvds_ch=1
	    lcd_frm=1
	    lcd_io_cfg0=268435456
	    lcd_bl_en_used=0
	    fb0_scaler_mode_enable=1
	    fb0_width=1920
	    fb0_height=1080
	    lcd_backlight=0
	    pwm_used=0
    ;;
    esac

    display_confirm "Set LCD to '$x'x'$y' ?" result
    if [ $result -eq 0 ];
	then
	   	change_parameter $PARAM_SCREEN0_OUTPUT_TYPE "1"
	   	change_parameter $PARAM_LCD_X $x
	   	change_parameter $PARAM_LCD_Y $y
	   	change_parameter $PARAM_LCD_DCLK_FREQ $freq
	   	change_parameter $PARAM_LCD_HBP $hbp
	   	change_parameter $PARAM_LCD_HT $ht
	   	change_parameter $PARAM_LCD_VBP $vbp
	   	change_parameter $PARAM_LCD_VT $vt
	   	change_parameter $PARAM_LCD_VSPW $vspw
	   	change_parameter $PARAM_LCD_HSPW $hspw
	   	change_parameter $PARAM_LCD_IF $lcd_if
	   	change_parameter $PARAM_LCD_LVDS_BITWIDTH $lcd_lvds_bitwidth
	   	change_parameter $PARAM_LCD_IO_CFG0 $lcd_io_cfg0
	   	change_parameter $PARAM_LCD_BL_EN_USED $lcd_bl_en_used
	   	change_parameter $PARAM_FB0_SCALER_MODE_ENABLE $fb0_scaler_mode_enable
	   	change_parameter $PARAM_FB0_WIDTH $fb0_width
	   	change_parameter $PARAM_FBO_HEIGHT $fb0_height
	   	change_parameter $PARAM_LCD_LVDS_CH $lcd_lvds_ch
	   	change_parameter $PARAM_LCD_FRM $lcd_frm
	   	change_parameter $PARAM_LCD0_BACKLIGHT $lcd_backlight
	   	change_parameter_in_section "pwm0_para" $PARAM_PWM_USED $pwm_used

	   	if [ "$choice" = "15.6" ] || [ "$choice" = "15.6-FHD" ];
		then
			# Add pll3 parameter
			if [ -z $(find_word "pll3") ];
			then
				insert_after "\[clock]" "pll3" "297"
			else
				change_parameter "pll3" "297"
			fi

			# Change lcd pins to lvds
			change_parameter "lcdd0" "port:PD00<3><0><default><default>"
			change_parameter "lcdd1" "port:PD01<3><0><default><default>"
			change_parameter "lcdd2" "port:PD02<3><0><default><default>"
			change_parameter "lcdd3" "port:PD03<3><0><default><default>"
			change_parameter "lcdd4" "port:PD04<3><0><default><default>"
			change_parameter "lcdd5" "port:PD05<3><0><default><default>"
			change_parameter "lcdd6" "port:PD06<3><0><default><default>"
			change_parameter "lcdd7" "port:PD07<3><0><default><default>"
			change_parameter "lcdd8" "port:PD08<3><0><default><default>"
			change_parameter "lcdd9" "port:PD09<3><0><default><default>"
			change_parameter "lcdd10" "port:PD10<3><0><default><default>"
			change_parameter "lcdd11" "port:PD11<3><0><default><default>"
			change_parameter "lcdd12" "port:PD12<3><0><default><default>"
			change_parameter "lcdd13" "port:PD13<3><0><default><default>"
			change_parameter "lcdd14" "port:PD14<3><0><default><default>"
			change_parameter "lcdd15" "port:PD15<3><0><default><default>"
			change_parameter "lcdd16" "port:PD16<3><0><default><default>"
			change_parameter "lcdd17" "port:PD17<3><0><default><default>"
			change_parameter "lcdd18" "port:PD18<3><0><default><default>"
			change_parameter "lcdd19" "port:PD19<3><0><default><default>"
			change_parameter "lcdd20" "port:PD20<3><0><default><default>"
			change_parameter "lcdd21" "port:PD21<3><0><default><default>"
			# Delete useless parameters
			delete_parameter "lcdd22 = port:PD22"
			delete_parameter "lcdd23 = port:PD23"
			delete_parameter "lcdclk = port:PD24"
			delete_parameter "lcdde = port:PD25"
			delete_parameter "lcdhsync = port:PD26"
			delete_parameter "lcdvsync = port:PD27"
			insert_rc_local "insmod $SOFTPWM"

		else
			# Change pins to parallel port
			change_parameter "lcdd0" "port:PD00<2><0><default><default>"
			change_parameter "lcdd1" "port:PD01<2><0><default><default>"
			change_parameter "lcdd2" "port:PD02<2><0><default><default>"
			change_parameter "lcdd3" "port:PD03<2><0><default><default>"
			change_parameter "lcdd4" "port:PD04<2><0><default><default>"
			change_parameter "lcdd5" "port:PD05<2><0><default><default>"
			change_parameter "lcdd6" "port:PD06<2><0><default><default>"
			change_parameter "lcdd7" "port:PD07<2><0><default><default>"
			change_parameter "lcdd8" "port:PD08<2><0><default><default>"
			change_parameter "lcdd9" "port:PD09<2><0><default><default>"
			change_parameter "lcdd10" "port:PD10<2><0><default><default>"
			change_parameter "lcdd11" "port:PD11<2><0><default><default>"
			change_parameter "lcdd12" "port:PD12<2><0><default><default>"
			change_parameter "lcdd13" "port:PD13<2><0><default><default>"
			change_parameter "lcdd14" "port:PD14<2><0><default><default>"
			change_parameter "lcdd15" "port:PD15<2><0><default><default>"
			change_parameter "lcdd16" "port:PD16<2><0><default><default>"
			change_parameter "lcdd17" "port:PD17<2><0><default><default>"
			change_parameter "lcdd18" "port:PD18<2><0><default><default>"
			change_parameter "lcdd19" "port:PD19<2><0><default><default>"
			change_parameter "lcdd20" "port:PD20<2><0><default><default>"
			change_parameter "lcdd21" "port:PD21<2><0><default><default>"
			# Add needed pins for paralle lcds
			insert_after "lcdd21" "lcdd22" "port:PD22<2><0><default><default>"
			insert_after "lcdd22" "lcdd23" "port:PD23<2><0><default><default>"
			insert_after "lcdd23" "lcdclk" "port:PD24<2><0><default><default>"
			insert_after "lcdclk" "lcdde" "port:PD25<2><0><default><default>"
			insert_after "lcdde" "lcdhsync" "port:PD26<2><0><default><default>"
			insert_after "lcdhsync" "lcdvsync" "port:PD27<2><0><default><default>"
			delete_rc_local "softpwm"

		fi

		if [ "$choice" = "15.6-FHD" ];
		then
			insert_rc_local "$DEVMEM 0x01c20118 w 0xc2000000"

		else
			delete_rc_local "0x01c20118"

		fi
	fi



}

function check_tools
{
	# Checking if there is sunxi tools
	if [ ! -f $FEX2BIN -o ! -f $BIN2FEX ];
	then

		# In newer image releases sunxi tools are in /usr/bin
		SUNXI_DIR="/usr/bin"
		BIN2FEX="$SUNXI_DIR/bin2fex"
		FEX2BIN="$SUNXI_DIR/fex2bin"
		if [ ! -f $FEX2BIN -o ! -f $BIN2FEX ];
		then
			echo "There is no sunxi-tools installed."
			exit
		fi
	fi

	# Checking for mount directory
	if [ ! -d $MMC_DIR ];
	then
		mkdir -p $MMC_DIR
	fi

	# Checking for mount directory
	if [ ! -d $TEMP_DIR ];
	then
		mkdir -p $TEMP_DIR
	fi

	# Checking for devmem
	if [ ! -f $DEVMEM ];
	then

		# Check for devmem in /usr/bin
		DEVMEM="/usr/bin/devmem"

		if [ ! -f $DEVMEM ];
		then
			echo "There is no devmem installed."
			exit
		fi
	fi
}

function check_boot
{
	device=$(tail -1 "/etc/fstab" | awk '{print $1}')
	echo $device

	if [ "$device" == "/dev/mmcblk0p2" ]; then
		BOOT_SOURCE="MMC"
	elif [ "$device" == "/dev/nandb" ]; then
		BOOT_SOURCE="NAND"
		# Realocate target file
		BIN_FILE="/boot/script.bin"
	else
		echo "Unknown boot device"
		exit
	fi
}

function read_script
{
	if [ "$BOOT_SOURCE" == "MMC" ]; then
		# Unmount mmcblk0p1
		umount /dev/mmcblk0p1 > /dev/null 2>&1

		# Mounting
		mount /dev/mmcblk0p1 $MMC_DIR > /dev/null 2>&1

		# Converting
		($BIN2FEX $BIN_FILE > $FEX_FILE) > /dev/null 2>&1

		# Syncing
		sync

		sleep 1

		# Unmount
		umount /dev/mmcblk0p1 > /dev/null 2>&1
	else
		# Converting
		($BIN2FEX $BIN_FILE > $FEX_FILE) > /dev/null 2>&1
	fi


}

function write_script
{
	if [ "$BOOT_SOURCE" == "MMC" ]; then
		# Unmount mmcblk0p1
		umount /dev/mmcblk0p1 > /dev/null 2>&1

		# Mounting
		mount /dev/mmcblk0p1 $MMC_DIR > /dev/null 2>&1

		# Converting
		($FEX2BIN $FEX_FILE $BIN_FILE) > /dev/null 2>&1

		# Syncing
		sync

		sleep 1

		# Unmound
		umount /dev/mmcblk0p1 > /dev/null 2>&1
	else
		# Converting
		($FEX2BIN $FEX_FILE $BIN_FILE) > /dev/null 2>&1
	fi
}

function cleanup
{
	rm -rf $TEMP_DIR
	rm -rf $MMC_DIR

	rm -f $tempfile1
	rm -f $tempfile2
	rm -f $tempfile3
	rm -f $tempfile4
}

function main
{
	dialog --backtitle "$BACKTITLE" --menu "Select output type:" 0 0 0 \
	"None" "Disable all screens" \
	"LCD" "Set configuration for LCD" \
	"HDMI" "Set configuration for HDMI" \
	"VGA" "Set configuration for VGA" 2> $tempfile1

	retv=$?
	choice=$(cat $tempfile1)

	# Check if ESC of CANCLE are pressed
	if [ $retv -eq 1 -o $retv -eq 255 ];
	then
		cleanup
		clear
		exit
	fi

	read_script

	# Check selected option
	case $choice in
		"None")
			set_screen_none
		;;
	    "LCD")
	    	set_screen_lcd
	    ;;
	    "HDMI")
	    	set_screen_hdmi
	    ;;
	    "VGA")
	    	set_screen_vga
	    ;;
	esac
	trap "rm -f $tempfile1" 0 1 2 5 15

	write_script

	display_confirm "Reboot ?" result
    if [ $result -eq 0 ];
    then
    	cleanup
     	reboot
	fi

}
check_boot
check_tools

main
cleanup
clear
