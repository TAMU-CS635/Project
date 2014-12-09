from SimpleCV import Camera, Color
def main():
	cam = Camera()

	while True:
		image = cam.getImage()
		blobs = image.findBlobs()
		if blobs:
			blobs.draw()
			blob = find_yellow(blobs)
			if blob:
				image.drawCircle((blob.x, blob.y), 10, color=Color.RED)
		image.show()

def find_yellow(image):
    ydist = image.colorDistance(Color.YELLOW)
    onlyYellow = image - ydist
    blobs = onlyYellow.findBlobs()
    if blobs:
	    return blobs[-1]
    else:
        return None
