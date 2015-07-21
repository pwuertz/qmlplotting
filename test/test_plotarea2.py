import numpy as np
from PyQt5 import QtCore, QtGui, QtQuick

app = QtGui.QGuiApplication([])
win = QtQuick.QQuickView()
win.setSource(QtCore.QUrl.fromLocalFile("test_plotarea2.qml"))
win.setResizeMode(win.SizeRootObjectToView)
win.show()

# get objects from qml context
root = win.rootObject()
items = ["source_img", "image", "plotarea", "plotwindow"]
source_img, image, plotarea, plotwindow = (root.findChild(QtCore.QObject, n) for n in items)

width = 512
height = 512
def test_image(img):
    h, w = img.shape
    Y, X = np.ogrid[-1:1:1j*h, -1:1:1j*w]
    img[:] = X * Y
    img += .2 * (np.random.random(img.shape) - .5)
    img[:, :10] = np.linspace(0, 1, h).reshape([h, 1])

img_buffer = np.empty((height, width), dtype=np.double)
test_image(img_buffer)
source_img.setData2D(img_buffer.ctypes.data, width, height)

image.setProperty("filter", "nearest")

app.exec_()

