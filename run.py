from create2 import Create
from vision import find_yellow
from SimpleCV import Camera

front_left_sensor = 'CLIFF_FRONT_LEFT_SIGNAL'

robot = None
try:
    robot = Create('/dev/ttyUSB0')
except:
    robot = Create('/dev/ttyUSB1')

cam = Camera()

# drive up to ramp tracking blob location
while(robot.getSensor(front_left_sensor) < 1400):
    image = cam.getImage()
    blobs = image.findBlobs()
    if blobs:
        blob = find_yellow(blobs)
        if blob:
            x = blob.x
            robot.driveDirect(-(20 + (x - 200)), -(20 + (200 - x)))

# drive over ramp, updating front left cliff
while(robot.getSensor(front_left_sensor) > 1200):
    robot.go(-20)

robot.stop()
robot.shutdown()
