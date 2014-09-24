import numpy as np
from PyQt5 import QtCore, QtGui, QtQuick

app = QtGui.QGuiApplication([])
win = QtQuick.QQuickView()
win.setSource(QtCore.QUrl.fromLocalFile("test.qml"))
win.setResizeMode(win.SizeRootObjectToView)
win.show()

root = win.rootObject()
source, image, selection = root.getItems()

# test image
width = 512
height = 512
def update_image(img):
    h, w = img.shape
    Y, X = np.ogrid[-1:1:1j*h, -1:1:1j*w]
    img[:] = X * Y
    img += .2 * (np.random.random(img.shape) - .5)

# example1: use numpy array as DataSource
img_buffer = np.empty((height, width), dtype=np.double)
update_image(img_buffer)
source.setData2D(img_buffer.ctypes.data, width, height)

# example2: modify array allocated/owned by DataSource
ptr = source.allocateData2D(width, height)
ptr.setsize(width * height * np.dtype(np.double).itemsize)
img_buffer = np.frombuffer(ptr, dtype=np.double, count=width*height)
img_buffer = img_buffer.reshape([height, width])
update_image(img_buffer)
source.commitData()

app.exec_()

