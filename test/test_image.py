import numpy as np
from PyQt5 import QtCore, QtGui, QtQuick

# setup qml application
app = QtGui.QGuiApplication([])
win = QtQuick.QQuickView()
win.setSource(QtCore.QUrl.fromLocalFile("test_image.qml"))
win.setResizeMode(win.SizeRootObjectToView)
win.show()

# get objects from qml context
root = win.rootObject()
items = ["source", "image", "button"]
source, image, button = (root.findChild(QtCore.QObject, n) for n in items)

# default image size
width = 512
height = 512

# define test function
def update_image(img):
    h, w = img.shape
    Y, X = np.ogrid[-1:1:1j*h, -1:1:1j*w]
    img[:] = X * Y
    img += .2 * (np.random.random(img.shape) - .5)

# update example1: use numpy array as DataSource
img_buffer = np.empty((height, width), dtype=np.double)
def update_data1():
    update_image(img_buffer)
    source.setData2D(img_buffer.ctypes.data, width, height)

# update example2: modify array allocated/owned by DataSource
def update_data2():
    ptr = source.allocateData2D(width, height)
    ptr.setsize(width * height * np.dtype(np.double).itemsize)
    img_buffer = np.frombuffer(ptr, dtype=np.double, count=width*height)
    img_buffer = img_buffer.reshape([height, width])
    update_image(img_buffer)
    source.commitData()

# update image on button clicked
button.clicked.connect(update_data2)
update_data2()

app.exec_()

