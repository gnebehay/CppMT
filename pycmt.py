
import numpy as np
import ctypes as ct
import cv2

_libccmt = np.ctypeslib.load_library('libccmt', '.')

_libccmt.newCMT.argtypes = None
_libccmt.newCMT.restype = ct.c_void_p

_libccmt.delCMT.argtypes = [ct.c_void_p]
_libccmt.delCMT.restype = None

_libccmt.cmt_initialize.argtypes = [
        ct.c_void_p, np.ctypeslib.ndpointer(dtype=np.uint8),
        ct.c_size_t, ct.c_size_t,
        ct.c_int, ct.c_int, ct.c_int, ct.c_int]
_libccmt.cmt_initialize.restype = None

_libccmt.processFrame.argtypes = [
        ct.c_void_p, np.ctypeslib.ndpointer(dtype=np.uint8),
        ct.c_size_t, ct.c_size_t,
        ct.POINTER(ct.c_int), ct.POINTER(ct.c_int),
        ct.POINTER(ct.c_int), ct.POINTER(ct.c_int)]
_libccmt.processFrame.restype = None

class CMT(object):
    def __init__(self):
        self.cmt = _libccmt.newCMT()

    def __del__(self):
        _libccmt.delCMT(self.cmt)

    def initialize(self, image, rect):
        image = image.astype(np.uint8)
        shape = image.shape
        _libccmt.cmt_initialize(self.cmt, image, shape[0], shape[1],
                                rect[0], rect[1], rect[2], rect[3])

    def processFrame(self, image):
        image = image.astype(np.uint8)
        shape = image.shape
        x = ct.c_int()
        y = ct.c_int()
        w = ct.c_int()
        h = ct.c_int()
        _libccmt.processFrame(self.cmt, image, shape[0], shape[1],
                             ct.byref(x), ct.byref(y), ct.byref(w), ct.byref(h))
        return [x.value, y.value, w.value, h.value]
