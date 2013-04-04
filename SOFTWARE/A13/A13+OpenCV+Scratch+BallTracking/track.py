#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
#  track.py
#  
#  Copyright 2013 OLIMEX Ltd <support@olimex.com>
#  
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#  
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#  
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software
#  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
#  MA 02110-1301, USA.


import cv
import sys
import scratch
import optparse


def main():
    s = scratch.Scratch()
    capture = cv.CaptureFromCAM(0)
    cv.NamedWindow("Track", 1)


    while True:
        #capture frame
        frame_o = cv.QueryFrame(capture)
	frame = cv.CreateImage((frame_o.width*3/8,
				frame_o.height*3/8),
				frame_o.depth,
				frame_o.nChannels)
	cv.Resize(frame_o, frame)
        cv.Smooth(frame, frame, cv.CV_GAUSSIAN,3,3)

        #Convert to HSV
        imgHSV = cv.CreateImage(cv.GetSize(frame), 8, 3)
        cv.CvtColor(frame, imgHSV, cv.CV_BGR2HSV)

        #Thresh
        imgThreshed = cv.CreateImage(cv.GetSize(frame), 8, 1)
        cv.InRangeS(imgHSV,
                cv.Scalar(0, 124, 221),
                cv.Scalar(10, 255, 256),
                imgThreshed)
        cv.Smooth(imgThreshed, imgThreshed, cv.CV_GAUSSIAN, 3, 3)

        mat = cv.GetMat(imgThreshed)
        moments = cv.Moments(mat)


        moment10 = cv.GetSpatialMoment(moments, 1, 0)
        moment01 = cv.GetSpatialMoment(moments, 0, 1)
        area = cv.GetCentralMoment(moments, 0, 0)
     
        if area > 1000:
            posX = int(moment10/area)
            posY = int(moment01/area)    

            if posX>=0 and posY>=0:
                print("X: " + str(posX) + ", Y: " + str(posY))
                cv.Rectangle(frame, (posX-10, posY-10), (posX+10, posY+10), cv.RGB(0, 255, 0))
                s.sensorupdate({'X': posX})
                s.sensorupdate({'Y': posY})
                
        cv.ShowImage("Track", frame)
        k = cv.WaitKey(70)
        if k % 0x100 == 27:
            break

def listen(s):
    while True:
        try:
            yield s.receive()
        except scratch.ScratchError:
            raise StopIteration


if __name__ == '__main__':
    main()
