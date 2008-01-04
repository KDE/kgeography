#!/usr/bin/python

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
from PyQt4 import QtCore
from PyQt4 import QtGui
from PyQt4 import QtXml

app = QtGui.QApplication(sys.argv)

if len(sys.argv) == 1:
	print "You have to specify the file to check"
	sys.exit(1)

for fileIndex in range(1, len(sys.argv)):
	path = sys.argv[fileIndex]
	print "Processing " + path
	xmlFile = QtCore.QFile(path)
	
	if xmlFile.exists():
		if (xmlFile.open(QtCore.QIODevice.ReadOnly)):
			doc = QtXml.QDomDocument()
			doc.setContent(xmlFile.readAll())
			root = doc.documentElement()
			if (root.tagName() == "map"):
				imagePath = QtCore.QFileInfo(path).absolutePath() + "/" + root.firstChildElement("mapFile").text()
				
				if not QtCore.QFile.exists(imagePath):
					print QtCore.QString("Map file %1 does not exist").arg(imagePath)
					sys.exit(2)
				
				colorList = []
				divisionTag = root.firstChildElement("division");
				while (not divisionTag.isNull()):
					colorTag = divisionTag.firstChildElement("color");
					red = colorTag.firstChildElement("red").text().toInt()[0]
					green = colorTag.firstChildElement("green").text().toInt()[0]
					blue = colorTag.firstChildElement("blue").text().toInt()[0]
					colorList.append(QtGui.QColor(red, green, blue))
					divisionTag = divisionTag.nextSiblingElement("division");
				
				image = QtGui.QImage(imagePath)
				error = False
				for x in range(0, image.width()):
					for y in range(0, image.height()):
						rgbcolor = image.pixel(x,y)
						qcolor = QtGui.QColor(rgbcolor)
						contains = colorList.count(qcolor)
						if contains == 0:
							print QtCore.QString("The pixel (%1,%2) has color %3,%4,%5 that is not defined in the kgm file").arg(x).arg(y).arg(qcolor.red()).arg(qcolor.green()).arg(qcolor.blue())
							error = True
				
				if not error:
					print "All colors used in the image are defined in the kgm file"
			else:
				print "The map description file should begin with the map tag"
			xmlFile.close();
		else:
			print QtCore.QString("Could not open %1 for reading").arg(path)
	else:
		print QtCore.QString("File %1 does not exist").arg(path)

sys.exit(0)
