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

def find_yellow(blobs):
	for blob in blobs:
		rgb = blob.meanColor()
		if rgb[-1] < 50:
			return blob
	return None

