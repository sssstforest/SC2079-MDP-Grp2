from datetime import timedelta
import cv2
import numpy as np
import os

SAVING_FRAMES_PER_SECOND = 10

def main():
  video_name = "movie.mp4" 
  vidcap = cv2.VideoCapture(video_name)
  success,image = vidcap.read()
  count = 0
  
  while success:
    cv2.imwrite("frame%d.jpg" % count, image)     # save frame as JPEG file      
    success,image = vidcap.read()
    print('Read a new frame: ', success)
    count += 1