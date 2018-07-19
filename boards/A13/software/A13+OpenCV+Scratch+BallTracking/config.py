# config.py - color separation tool, use the values to track the colored object

import cv

lowerH=0
lowerS=0
lowerV=0

upperH=180
upperS=256
upperV=256



def call_LH(a):
    global lowerH
    lowerH = a

def call_UH(a):
    global upperH
    upperH = a
    
def call_LS(a):
    global lowerS
    lowerS = a
    
def call_US(a):
    global upperS
    upperS = a
    
def call_LV(a):
    global lowerV
    lowerV = a
    
def call_UV(a):
    global upperV
    upperV = a




capture = cv.CaptureFromCAM(0)  
#cv.NamedWindow("Video")
cv.NamedWindow("Ball")
cv.CreateTrackbar("LowerH", "Ball", lowerH, 180, call_LH)
cv.CreateTrackbar("UpperH", "Ball", upperH, 180, call_UH)
cv.CreateTrackbar("LowerS", "Ball", lowerS, 256, call_LS)
cv.CreateTrackbar("UpperS", "Ball", upperS, 256, call_US)
cv.CreateTrackbar("LowerV", "Ball", lowerV, 256, call_LV)
cv.CreateTrackbar("UpperV", "Ball", upperV, 256, call_UV) 

while True:
 
     frame = cv.QueryFrame(capture)
     frame=cv.CloneImage(frame)

     imgHSV = cv.CreateImage(cv.GetSize(frame), 8, 3)
     cv.CvtColor(frame, imgHSV, cv.CV_BGR2HSV)
       
     imgThresh = cv.CreateImage(cv.GetSize(imgHSV), 8, 1)

 #    print("LH: " + str(lowerH) + ", LS: " + str(lowerS) + ", LV: " + str(lowerV))
 #    global lowerH
 #    global lowerS
 #    global lowerV
 #    global upperH
 #    global upperS
 #    gloval upperV
     print("LH: " + str(lowerH) + ", LS: " + str(lowerS) + ", LV: " + str(lowerV))
     cv.InRangeS(imgHSV, cv.Scalar(lowerH, lowerS, lowerV), cv.Scalar(upperH, upperS, upperV), imgThresh)
      
     cv.ShowImage("Ball", imgThresh)
#     cv.ShowImage("Video", frame)




     k = cv.WaitKey(10)
     if k % 0x100 == 27:
         break



cv.DestroyAllWindows()

