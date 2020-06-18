#!/usr/bin/python
# -*- coding: utf-8 -*-

# Copyright 2008 Albert Astals Cid <aacid@kde.org>

# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation; either version 2 of 
# the License, or (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

import sys
from PyQt5 import QtCore
from PyQt5 import QtXml
from PIL import Image

app = QtCore.QCoreApplication(sys.argv)

if len(sys.argv) == 1:
	print("Error: You have to specify the file to check")
	sys.exit(1)

for fileIndex in range(1, len(sys.argv)):
	path = sys.argv[fileIndex]
	print("Processing " + path)
	xmlFile = QtCore.QFile(path)

	if not xmlFile.exists():
		print("Error: File {} does not exist".format(path))
	else:
		if not xmlFile.open(QtCore.QIODevice.ReadOnly):
			print("Error: Could not open {} for reading".format(path))
		else:
			doc = QtXml.QDomDocument()
			doc.setContent(xmlFile.readAll())
			root = doc.documentElement()
			if root.tagName() != "map":
				print("Error: The map description file should begin with the map tag")
			else:
				imagePath = QtCore.QFileInfo(path).absolutePath() + "/" + root.firstChildElement("mapFile").text()

				if not QtCore.QFile.exists(imagePath):
					print ("Error: Map file {} does not exist".format(imagePath))
					sys.exit(2)

				colorList = [];
				divisionTag = root.firstChildElement("division");
				while not divisionTag.isNull():
					colorTag = divisionTag.firstChildElement("color");
					red = int(colorTag.firstChildElement("red").text())
					green = int(colorTag.firstChildElement("green").text())
					blue = int(colorTag.firstChildElement("blue").text())
					rgba = (red, green, blue, 255)
					if rgba not in colorList:
						colorList.append(rgba)
					else:
						print("Error: The color {},{},{} is used more than once in the kgm file".format(red, green, blue))

					divisionTag = divisionTag.nextSiblingElement("division");

				image = Image.open(imagePath)
				error = image.mode != 'P'
				if error:
					print("Error: The PNG file should be in indexed color mode")
				image = image.convert('RGBA')
				usedColors = set([rgba for count, rgba in image.getcolors()])
				notFoundColors = usedColors - set(colorList)

				error |= len(notFoundColors) > 0
				if notFoundColors:
					pixels = image.load()
					width, height = image.size
					notFoundColorsToSearch = notFoundColors
					for x in range(width):
						for y in range(height):
							rgba = pixels[x, y]
							if rgba in notFoundColorsToSearch:
								print ("Error: The pixel (%d ,%d) has color rgba %d,%d,%d,%d that is not defined in the kgm file" % (
									x, y, rgba[0], rgba[1], rgba[2], rgba[3]))
								notFoundColorsToSearch.remove(rgba)

				nonUsedColors = set(colorList)
				nonUsedColors.difference_update(usedColors)
				error |= len(nonUsedColors) > 0
				for rgba in nonUsedColors:
					print("Error: the rgb(a) color {},{},{},({}) is absent from the png pixels".format(
						rgba[0], rgba[1], rgba[2], rgba[3]))

				if not error:
					print("The map is correctly formed")
			xmlFile.close();

sys.exit(0)
