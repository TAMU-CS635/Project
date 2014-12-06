from create2 import Create
from vision import find_yellow
from SimpleCV import Camera

try:
    robot = Create('/dev/ttyUSB0')
except:
    robot = Create('/dev/ttyUSB1')

cam = Camera()

sensors = robot.sensors()

# drive up to ramp tracking blob location
while(sensors[create2.CLIFF_FRONT_LEFT_SIGNAL] < 1400):
    image = cam.getImage()
    blobs = image.findBlobs()
    if blobs:
        blob = find_yellow(blobs)
        if blob:
            x = blob.x
            robot.driveDirect(-(20 + (x - 200)), -(20 + (200 - x)))
    sensors = robot.sensors()

# drive over ramp, updating front left cliff
while(sensors[create2.CLIFF_FRONT_LEFT_SIGNAL] > 1200):
    robot.go(-20)
    sensors = robot.sensors()

robot.stop()
robot.shutdown()
