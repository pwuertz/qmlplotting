import numpy as np
from PyQt5 import QtCore, QtGui, QtQuick

app = QtGui.QGuiApplication([])
win = QtQuick.QQuickView()
win.setSource(QtCore.QUrl.fromLocalFile("test_plotarea.qml"))
win.setResizeMode(win.SizeRootObjectToView)
win.show()

# get objects from qml context
root = win.rootObject()
items = ["source1", "xyplot1", "xyplot2", "plotarea"]
source1, xyplot1, xyplot2, plotarea = (root.findChild(QtCore.QObject, n) for n in items)

n_points = 50

def test_func(buffer):
    X = np.linspace(-1., 1., n_points)
    buffer[0::2] = X
    buffer[1::2] = 1e3 * np.exp(-X**2 / (2*.1)**2)
    buffer[1::2] *= np.logical_or(buffer[1::2] > 20, np.random.rand(n_points) > .5)
    buffer[1::2] = np.log10(buffer[1::2])

def autoscale_view(buffer):
    X = buffer[0::2][np.isfinite(buffer[0::2])]
    Y = buffer[1::2][np.isfinite(buffer[1::2])]
    xmin, xamp = X.min(), X.max() - X.min()
    ymin, yamp = Y.min(), Y.max() - Y.min()
    view_rect = QtCore.QRectF(xmin-xamp*.05, ymin-yamp*.05, xamp*1.1, yamp*1.1)
    return view_rect

xydata_buffer1 = np.empty(2*n_points, dtype=np.double)
test_func(xydata_buffer1)
view_rect = autoscale_view(xydata_buffer1)

source1.setData1D(xydata_buffer1.ctypes.data, xydata_buffer1.size)
plotarea.setProperty("yLabel", "Log10 data")
plotarea.setProperty("xLabel", "x")
plotarea.setProperty("viewRect", view_rect)
xyplot1.setProperty("markerEnabled", True)
xyplot1.setProperty("markerColor", "#cccc8888")
xyplot1.setProperty("markerSize", 8)
xyplot2.setProperty("visible", False)

app.exec_()

