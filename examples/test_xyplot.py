import numpy as np
from PyQt5 import QtCore, QtGui, QtQuick

# setup qml application
app = QtGui.QGuiApplication([])
win = QtQuick.QQuickView()
win.setSource(QtCore.QUrl.fromLocalFile("test_xyplot.qml"))
win.setResizeMode(win.SizeRootObjectToView)
win.show()

# get objects from qml context
root = win.rootObject()
items = ["source", "xyplot", "button"]
source, xyplot, button = (root.findChild(QtCore.QObject, n) for n in items)

# default number of points
n_points = 100

# define test function
def update_plot(data):
    n = data.size/2
    X = np.linspace(-1., 1., n)
    data[0::2] = X
    data[1::2] = .5 + .4*np.sin(X * 2.*np.pi/0.2)
    data[1::2] *= 1.-X**2

# update example1: use numpy array as DataSource
xydata_buffer = np.empty(2*n_points, dtype=np.double)
def update_data1():
    update_plot(xydata_buffer)
    source.setData1D(xydata_buffer.ctypes.data, xydata_buffer.size)

# update example2: modify array allocated/owned by DataSource
def update_data2():
    ptr = source.allocateData1D(xydata_buffer.size)
    ptr.setsize(xydata_buffer.size*np.dtype(np.double).itemsize)
    buf = np.frombuffer(ptr, dtype=np.double, count=xydata_buffer.size)
    update_plot(buf)
    source.commitData()

# update plot on button clicked
button.clicked.connect(update_data2)
update_data2()

app.exec_()

