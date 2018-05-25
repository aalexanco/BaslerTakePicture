#!/usr/bin/env python3

import cv2 as cv
import sys
import os
import libtakepicture as poc

camera = poc.Camera()

try:
   frame = camera.captureImage()
except:
   pass

if frame is not None:
    cv.imshow('img', frame)
    cv.waitKey(0)
