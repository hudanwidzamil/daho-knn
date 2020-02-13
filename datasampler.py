import numpy as np
import cv2 as cv
import csv

cap = cv.VideoCapture('flied20001-0060.avi') #import video
noframe = 0
with open('sampledata.csv', mode='w') as sample_data:
    sample_data = csv. writer(sample_data,delimiter=',')

    while(cap.isOpened()):
        #capture frame by frame
        ret, frame = cap.read()
        #print(noframe)
        #print(" ")

        frame = cv.resize(frame, (640, 480))
        gray = cv.cvtColor(frame, cv.COLOR_BGR2GRAY)
        edges = cv.Canny(gray,50,150,apertureSize = 3)
        lines = cv.HoughLines(edges,1,np.pi/180,100)

        data = [[0,0],[0,0],[0,0]]
        for line in lines:
            rho, theta = line[0]
            if rho > 300 :
                data[0][0] = rho
                data[0][1] = theta
            elif rho >0 :
                data[1][0] = rho
                data[1][1] = theta
            else :
                data[2][0] = rho
                data[2][1] = theta

            a = np.cos(theta)
            b = np.sin(theta)
            x0 = a*rho
            y0 = b*rho
            x1 = int(x0 + 1000*(-b))
            y1 = int(y0 + 1000*(a))
            x2 = int(x0 - 1000*(-b))
            y2 = int(y0 - 1000*(a))
            cv.line(frame,(x1,y1),(x2,y2),(0,0,255),2)

        #data.sort(reverse = True)
        sample_data.writerow([data[0][0],data[0][1],data[1][0],data[1][1],data[2][0],data[2][1],noframe+1])
        #for i in range (3):
        #
        #   if i== 0:
        #       print("arr[",noframe,"].x = ",data[i][0],";")
        #       print("arr[",noframe,"].y = ",data[i][1],";")
        #   elif i==1:
        #       print("arr[",noframe,"].a = ",data[i][0],";")
        #       print("arr[",noframe,"].b = ",data[i][1],";")
        #   else:
        #       print("arr[",noframe,"].c = ",data[i][0],";")
        #       print("arr[",noframe,"].d = ",data[i][1],";")


        #print("arr[",noframe,"].val = ",noframe+1,";")
        noframe +=1
        #display
        #cv.imshow('edges',edges)
        #cv.imshow('frame',frame)
        #if cv.waitKey(120) & 0xFF == ord('q'):
        #    break

    #release the capture
    cap.release()
    cv.destroyAllWindows()
