= A13-OLinuXino + OpenCV + Scratch

1. Install scratchpy

  git clone git://github.com/pilliq/scratchpy.git
  cd scratchpy

**IMPORTANT**: Make sure that you date is set!

  date -s "3 APR 2013"
  python setup.py install

2. Install OpenCV

  apt-get install libopencv-dev python-opencv

3. Install Scratch

  apt-get install scratch

4. Open Scratch and open the demo project Eyes.sb

5. A message should appear that remote sensor connection is enabled.
If doesn't click on "Sensing", right click on sensor value and click "Enable remote sensor connection".

6. Open terminal and start the tracking program

  python track.py

7. This will track yellow ball and the eyes will look at this direction

8. If you want to use some other object with different color use config.py.
Start the config:

  python config.py

Move the sliders until only the desired color is white and everything else is black.
Open track.py and edit the followin lines:

```
 cv.InRangeS(imgHSV,
                cv.Scalar(0, 100, 255),
                cv.Scalar(40, 255, 256),
                imgThreshed)
```

Write your values, for example:

```
 cv.InRangeS(imgHSV,
                cv.Scalar(110, 98, 100),
                cv.Scalar(131, 255, 256),
                imgThreshed)
```

Start the script:

  python track.py
