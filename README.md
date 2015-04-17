# Introduction
CppMT is a method for visual object tracking.
It is the C++ implementation of CMT that was originally developed in Python by myself.
Details can be found on the [project page](http://www.gnebehay.com/cmt).
The implementation in this repository is platform-independent and runs
on Linux, Windows and OS X.

#License
CppMT is freely available under the [Simplified BSD license][1],
meaning that you can basically do with the code whatever you want.
If you use our algorithm in scientific work, please cite our publication
```
@inproceedings{Nebehay2015CVPR,
    author = {Nebehay, Georg and Pflugfelder, Roman},
    booktitle = {Computer Vision and Pattern Recognition},
    month = jun,
    publisher = {IEEE},
    title = {Clustering of {Static-Adaptive} Correspondences for Deformable Object Tracking},
    year = {2015}
}
```

# Dependencies
* OpenCV (>= 2.4.8, < 3)

# Building
CppMT uses cmake for building.
In its most simple form, calling
```
cmake .
```
from the source directory should setup everything that is necessary.
On Linux, you will probably call
```
make
```
afterwards, while on Windows you will open the project file in Visual Studio and start the build there.

# Usage
```
usage: ./cmt [--challenge] [--no-scale] [--no-rotation] [--bbox BBOX] [inputpath]
```
## Optional arguments
* `inputpath` The input path.
* `--challenge` Enter challenge mode.
* `--no-scale` Disable scale estimation
* `--with-rotation` Enable rotation estimation
* `--bbox BBOX` Specify initial bounding box. Format: x,y,w,h

## Object Selection
Press any key to stop the preview stream. Left click to select the
top left bounding box corner and left click again to select the bottom right corner.

## Examples
When using a webcam, no arguments are necessary:
```
cmt
```

When using a video, the path to the file has to be given as an input parameter:
```
cmt /home/cmt/test.avi
```

It is also possible to specify the initial bounding box on the command line.
```
cmt --bbox=123,85,60,140 /home/cmt/test.avi
```

[1]: http://en.wikipedia.org/wiki/BSD_licenses#2-clause_license_.28.22Simplified_BSD_License.22_or_.22FreeBSD_License.22.29
