__author__ = 'stefan'


class Colors:
    def __init__(self):
        self = self

    def test(self):
        print "Regular"
        print self.Black + "Black" + self.Color_Off
        print self.Red + "Red" + self.Color_Off
        print self.Green + "Green" + self.Color_Off
        print self.Yellow + "Yellow" + self.Color_Off
        print self.Blue + "Blue" + self.Color_Off
        print self.Purple + "Purple" + self.Color_Off
        print self.Cyan + "Cyan" + self.Color_Off
        print self.White + "White" + self.Color_Off
        print ""

        print "Bold"
        print self.BBlack + "BBlack" + self.Color_Off
        print self.BRed + "BRed" + self.Color_Off
        print self.BGreen + "BGreen" + self.Color_Off
        print self.BYellow + "BYellow" + self.Color_Off
        print self.BBlue + "BBlue" + self.Color_Off
        print self.BPurple + "BPurple" + self.Color_Off
        print self.BCyan + "BCyan" + self.Color_Off
        print self.BWhite + "BWhite" + self.Color_Off
        print ""

        print "Underline"
        print self.UBlack + "UBlack" + self.Color_Off
        print self.URed + "URed" + self.Color_Off
        print self.UGreen + "UGreen" + self.Color_Off
        print self.UYellow + "UYellow" + self.Color_Off
        print self.UBlue + "UBlue" + self.Color_Off
        print self.UPurple + "UPurple" + self.Color_Off
        print self.UCyan + "UCyan" + self.Color_Off
        print self.UWhite + "UWhite" + self.Color_Off
        print ""

        print "Background"
        print self.On_Black + "On_Black" + self.Color_Off
        print self.On_Red + "On_Red" + self.Color_Off
        print self.On_Green + "On_Green" + self.Color_Off
        print self.On_Yellow + "On_Yellow" + self.Color_Off
        print self.On_Blue + "On_Blue" + self.Color_Off
        print self.On_Purple + "On_Purple" + self.Color_Off
        print self.On_Cyan + "On_Cyan" + self.Color_Off
        print self.On_White + "On_White" + self.Color_Off
        print ""

        print "High Intensity"
        print self.IBlack + "IBlack" + self.Color_Off
        print self.IRed + "IRed" + self.Color_Off
        print self.IGreen + "IGreen" + self.Color_Off
        print self.IYellow + "IYellow" + self.Color_Off
        print self.IBlue + "IBlue" + self.Color_Off
        print self.IPurple + "IPurple" + self.Color_Off
        print self.ICyan + "ICyan" + self.Color_Off
        print self.IWhite + "IWhite" + self.Color_Off
        print ""

        print "Bold High Intensity"
        print self.BIBlack + "BIBlack" + self.Color_Off
        print self.BIRed + "BIRed" + self.Color_Off
        print self.BIGreen + "BIGreen" + self.Color_Off
        print self.BIYellow + "BIYellow" + self.Color_Off
        print self.BIBlue + "BIBlue" + self.Color_Off
        print self.BIPurple + "BIPurple" + self.Color_Off
        print self.BICyan + "BICyan" + self.Color_Off
        print self.BIWhite + "BIWhite" + self.Color_Off
        print ""

        print "High Intensity backgrounds"
        print self.On_IBlack + "On_IBlack" + self.Color_Off
        print self.On_IRed + "On_IRed" + self.Color_Off
        print self.On_IGreen + "On_IGreen" + self.Color_Off
        print self.On_IYellow + "On_IYellow" + self.Color_Off
        print self.On_IBlue + "On_IBlue" + self.Color_Off
        print self.On_IPurple + "On_IPurple" + self.Color_Off
        print self.On_ICyan + "On_ICyan" + self.Color_Off
        print self.On_IWhite + "On_IWhite" + self.Color_Off
        print ""

    # Reset
    Color_Off = '\033[0m'  # Text Reset

    Black = '\033[0;30m'  # Black
    Red = '\033[0;31m'  # Red
    Green = '\033[0;32m'  # Green
    Yellow = '\033[0;33m'  # Yellow
    Blue = '\033[0;34m'  # Blue
    Purple = '\033[0;35m'  # Purple
    Cyan = '\033[0;36m'  # Cyan
    White = '\033[0;37m'  # White

    # Bold
    BBlack = '\033[1;30m'  # Black
    BRed = '\033[1;31m'  # Red
    BGreen = '\033[1;32m'  # Green
    BYellow = '\033[1;33m'  # Yellow
    BBlue = '\033[1;34m'  # Blue
    BPurple = '\033[1;35m'  # Purple
    BCyan = '\033[1;36m'  # Cyan
    BWhite = '\033[1;37m'  # White

    # Underline
    UBlack = '\033[4;30m'  # Black
    URed = '\033[4;31m'  # Red
    UGreen = '\033[4;32m'  # Green
    UYellow = '\033[4;33m'  # Yellow
    UBlue = '\033[4;34m'  # Blue
    UPurple = '\033[4;35m'  # Purple
    UCyan = '\033[4;36m'  # Cyan
    UWhite = '\033[4;37m'  # White

    # Background
    On_Black = '\033[40m'  # Black
    On_Red = '\033[41m'  # Red
    On_Green = '\033[42m'  # Green
    On_Yellow = '\033[43m'  # Yellow
    On_Blue = '\033[44m'  # Blue
    On_Purple = '\033[45m'  # Purple
    On_Cyan = '\033[46m'  # Cyan
    On_White = '\033[47m'  # White

    # High Intensity
    IBlack = '\033[0;90m'  # Black
    IRed = '\033[0;91m'  # Red
    IGreen = '\033[0;92m'  # Green
    IYellow = '\033[0;93m'  # Yellow
    IBlue = '\033[0;94m'  # Blue
    IPurple = '\033[0;95m'  # Purple
    ICyan = '\033[0;96m'  # Cyan
    IWhite = '\033[0;97m'  # White

    # Bold High Intensity
    BIBlack = '\033[1;90m'  # Black
    BIRed = '\033[1;91m'  # Red
    BIGreen = '\033[1;92m'  # Green
    BIYellow = '\033[1;93m'  # Yellow
    BIBlue = '\033[1;94m'  # Blue
    BIPurple = '\033[1;95m'  # Purple
    BICyan = '\033[1;96m'  # Cyan
    BIWhite = '\033[1;97m'  # White

    # High Intensity backgrounds
    On_IBlack = '\033[0;100m'  # Black
    On_IRed = '\033[0;101m'  # Red
    On_IGreen = '\033[0;102m'  # Green
    On_IYellow = '\033[0;103m'  # Yellow
    On_IBlue = '\033[0;104m'  # Blue
    On_IPurple = '\033[0;105m'  # Purple
    On_ICyan = '\033[0;106m'  # Cyan
    On_IWhite = '\033[0;107m'  # White