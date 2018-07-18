= MOD-IO + OLINUXINO-A13 + SCRATCH

1. Install python-smbus:

  apt-get install python-smbus

2. Install scratchpy

  git clone https://github.com/pilliq/scratchpy.git
  cd scratchpy
  make install

3. Run SCRATCH and open MOD-IO.sb

4. Go to 'Sensing'.
Right click '... sensor value'
Click 'Enable 
remote sensor connections' (if it not enabled).

5. To to terminal and start the python script

  python IO.py -a 0x58 -i 2 --host=xxx.xxx.xxx.xxx --port=xxxx --verbose

-i        - I2C bus that you're using
-a        - Address of MOD-IO (default 0x58)
--host    - IP if you're connecting remote computer
--port    - Port number
--verbose - Print some debug information. 

For example:

  python IO.py -a 0x58 -i 2 --verbose --host=192.168.0.100 --port=40000

You should see message that the program is connected.

6. Run the Scratch block

7. Press '1', '2', '3' or '4' to toggle relays
