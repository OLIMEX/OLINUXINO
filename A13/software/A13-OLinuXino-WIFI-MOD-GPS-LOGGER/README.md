# A13-OLinuXino-WIFI + MOD-GPS GPS data logger with Google maps vsualization

A13-OLinuXino-WIFI can connect directly to MOD-GPS via UEXT connector.
Also, the A13-OLinuXino-WIFI has a LiPo battery connector and can charge and operate off of Lithium Polymer batteries.
This allow us to make very nice GPS logger demo with just A13-OLinuXino-WIFI + LiPo battery + MOD-GPS.

This project reads GPS info and log the coordinates then create flypath with them.

To start server use following commands:

```bash
apt-get install update
apt-get install apache2
```

(Or other server on your choice...)

```bash
apt-get install php5
```

Then you can configure it as you want... or not. 

If you're using DHCP check the ip:

```bash
ifconfig -a
```

You should view the current IP address, for example 192.168.0.100.

Open some browser and type 192.168.0.100. The page should read "It works!".

Go to /var/www/ dir and extract the project:

```bash
cd /var/www/
tar -zxvf /path/to/archieve/GPS.tar.gz .
cd /GPS
chmod 777 GPS2.py LOG
ls -l *
```

And you can see permitions.

Open index.php and find the line:

```php
        <script type="text/javascript" src="https://maps.googleapis.com/maps/api/js?key=YOUR_GOOGLE_MAPS_KEY&sensor=false">
```
        
Replace ``YOUR_GOOGLE_MAPS_KEY`` with your own key.
Visit [this page](https://developers.google.com/maps/documentation/javascript/tutorial#api_key) for a tutorial.


Once you enter your key and save the file, open your browser again and enter the address "192.168.0.100/GPS".
A google maps should load.

UART1 MUST be disabled.

```bash
vi /etc/inittab
```

Comment the line: ``T0:23:respawn:/sbin/getty -L -a root ttyS0 115200 vt102``
Save the file and reboot.
**Before rebooting you should have working SSH!!**


After the reboot, login via SSH and connect A13 with MOD-GPS.

To log coords type:

```bash
cd /var/www/GPS
```

Remove demo coords:

```bash
rm LOG
python GPS2.py
```

This will collect data every 1 minute.
While this is running (or in the background) go for a walk. :)

Once you're back home from your lovely walk, stop the process.
Go to your browser and enter the demo address.

Click load file.
Now you should see flypath and the logged coordinates. :)

01.03.2013

Olimex LTD

support@olimex.com

