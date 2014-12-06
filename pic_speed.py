from SimpleCV import Camera, Color

import time

c = Camera()

average = 0
for i in range(0, 20):
	start = time.time()
	i = c.getImage()
	ydist = i.colorDistance(Color.YELLOW)
	only = i = ydist
	dist = only.colorDistance(Color.YELLOW)
	dist = dist.binarize(200)
	dist = dist.erode(2)
	blobs = dist.findBlobs()
	#dist.show()
	end = time.time()
	difference = end - start
	average = average + difference

average = average / 20

print average
