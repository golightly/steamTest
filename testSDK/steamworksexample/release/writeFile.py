import sys
import subprocess
from PIL import Image
path = sys.argv[1]
inputPath = path + ".png"
outputPath = path + ".img"
file = Image.open(inputPath)
w, h = file.size
pixels = file.convert("RGBA")
pixelArray = [[0 for y in range(w)] for x in range(h)]
for one in range(h):
	for two in range(w):
		r,g,b,a = pixels.getpixel((two,one))
		pixelArray[one][two] = str(r) + " " + str(g) + " " + str(b) + " " + str(a) + " "
file2 = open(outputPath,"w")
for a in range(h):
	for b in range(w):
		file2.write(pixelArray[a][b])
file2.close()
subprocess.check_call(["editFile.exe", str(outputPath), str(w), str(h)])
