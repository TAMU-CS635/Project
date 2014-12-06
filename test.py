from create2 import Create
from SimpleCV import Camera, Color


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
    while blobs[-1].x != 640 and blobs[-1].y != 512:
        robot.go(0,20)
        
    robot.go(20,0)
#result = img - dist
#result.show()