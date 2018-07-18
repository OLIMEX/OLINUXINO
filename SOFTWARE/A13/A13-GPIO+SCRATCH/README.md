= A13-OLinuXino GPIO control by Scratch

1. Install pyA13

  wget https://pypi.python.org/packages/source/p/pyA13/pyA13-0.1.12.tar.gz
  tar zxvf pyA13-0.1.12.tar.gz
  cd pyA13-0.1.12
  python setup.py install

2. Install scratchpy

  git clone https://github.com/pilliq/scratchpy.git
  cd scratchpy
  make install

3. Run SCRATCH and open GPIO.sb

4. Go to 'Sensing'.
Right click '... sensor value'.
Click: 'Enable remote sensor connections' (if it is not enabled).

5. Open a terminal and execute the python script

  python GPIO.py --host=xxx.xxx.xxx.xxx --port=xxxx --verbose

--host  -   IP if you're connecting remote computer
--port  -   Port number
--verbose - Print some debug information. 

For example:

  python GPIO.py --verbose --host=192.168.0.100 --port=40000

You should see message that the program is connected.

If you run SCRATCH on A13 just type in terminal:

  python GPIO.py

6. Run the Scratch block

7. Click on outputs to toggle GPIOs.
The inputs are software polled every 0.1 sec. 

Also you can play "Smoke on the water" with these four inputs.
Just press INPUT1, 2, 3, 1, 2, 4, 3, 1, 2, 3, 2, 1 and voila.

8. Open HelloWorld.sb and run watch on board GREEN LED blinking commanded by the Scratch cat.
