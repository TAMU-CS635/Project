from create2 import Create
from SimpleCV import Camera, Color
import time

robot = Create('/dev/ttyUSB0')

robot.go(10)
robot.stop()
cam = Camera()

while True:
    img = cam.getImage()
    yellowDist = img.colorDistance(Color.YELLOW)
    onlyYellow = img - yellowDist
    dist = onlyYellow.colorDistance(Color.YELLOW)
    dist = dist.binarize(200)
    dist.erode(2)
    
    blobs = dist.findBlobs()
    if blobs:
        blobs.draw()
	img.show()
	blob = blobs[-1]
	print "largest Blob area %s x %s" % (blob.area(), blob.x)
	if blob.area() > 1000 and blob.x > 280 and blob.x < 360:
		robot.go(20, 0)
		time.sleep(1)
		robot.stop()
	else:
		robot.go(0, 20)
		time.sleep(1)
		robot.stop()
        
#result = img - dist
#result.show()
