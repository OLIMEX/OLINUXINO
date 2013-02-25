A13 WEB SERVER with MOD-RGB = REMOTE WEB CONTROLLED LED RGB STRIPE

Prepare A13-OLinuXino-WIFI for web serving:

Update repository
$apt-get update

Install apache and php
$apt-get install apache2 php5 libapache2-mod-php5

Open browser and type A13 board internal network IP address. For instance http://192.168.0.30 . You should get message "It Works"

Extract demo files into apache dir. For example /var/www/RGB


Compile i2c-tool
$gcc i2c-tool

Make executable
$chmod 777 a.out

Change permitions of i2c-bus
$chmod 777 /dev/i2c*

Now open index.html. You should see color picker. Move mouse. Enjoy :)


