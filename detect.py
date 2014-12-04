from SimpleCV import Image, Color

cam = Camera()

while True:
    img = cam.getImage()
    yellowDist = img.colorDistance(Color.YELLOW)
    onlyYellow = img - yellowDist
    dist = onlyYellow.colorDistance(Color.YELLOW)
    dist = dist.binarize(200).invert()
    dist.erode(2)
    result = img - dist
    result.show()