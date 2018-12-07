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
from PyQt5 import QtGui
from PyQt5 import QtXml

app = QtGui.QGuiApplication(sys.argv)

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
					rgba = QtGui.qRgba(red, green, blue, 255)
					if rgba not in colorList:
						colorList.append(rgba)
					else:
						print("Error: The color {},{},{} is used more than once in the kgm file".format(red, green, blue))

					divisionTag = divisionTag.nextSiblingElement("division");

				image = QtGui.QImage(imagePath)
				ct = image.colorTable()
				error = len(image.colorTable()) == 0
				if error:
					print("Error: The PNG file should be in indexed color mode")
				ac = image.convertToFormat(QtGui.QImage.Format_Alpha8)
				act = ac.colorTable()
				usedColors = set()
				notfoundcolors = {}
				for x in range(0, image.width()):
					for y in range(0, image.height()):
						rgbcolor = image.pixel(x, y)
						ipix = image.pixelIndex(x, y)
						c = ct[ipix]
						al0 = ac.pixel(x, y)
						al = QtGui.qAlpha(al0)
						rgba = QtGui.qRgba(QtGui.qRed(rgbcolor), QtGui.qGreen(rgbcolor), QtGui.qBlue(rgbcolor), al)
						usedColors.add(rgba)
						if rgbcolor not in colorList:
							tri = notfoundcolors.get(rgba)
							if tri is None:
								notfoundcolors[rgba] = [(x, y), 1, (x, y)]
							else:
								tri[1] += 1
								tri[2] = (x, y)

				error |= len(notfoundcolors) > 0
				for rgba, tri in notfoundcolors.items():
					qcolor = QtGui.QColor.fromRgba(rgba)
					first, c, last = tri
					x, y = first
					print ("Error: The pixel (%d ,%d) has color rgba %d,%d,%d,%d that is not defined in the kgm file" % (
						x, y, qcolor.red(), qcolor.green(), qcolor.blue(), qcolor.alpha()))

				nonUsedColors = set(colorList)
				nonUsedColors.difference_update(usedColors)
				error |= len(nonUsedColors) > 0
				for rgba in nonUsedColors:
					qcolor = QtGui.QColor.fromRgba(rgba)
					print("Error: the rgb(a) color {},{},{},({}) is absent from the png pixels".format(
						qcolor.red(), qcolor.green(), qcolor.blue(), qcolor.alpha()))

				if not error:
					print("The map is correctly formed")
			xmlFile.close();

sys.exit(0)
