from create import Create

robot = Create('/dev/ttyUSB0')

robot.go(10)
robot.stop()
