# Introduction
CppMT is a method for visual object tracking.
It is the C++ implementation of CMT that was originally developed in Python by myself.
Details can be found on the [project page](http://www.gnebehay.com/cmt).
The implementation in this repository is platform-independent and runs
on Linux, Windows and OS X.

# License
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
* OpenCV (>= 2.4.8), OpenCV 3 is supported

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

## Note for Windows users
These steps are necessary to get CppMT running on Windows:
* Download this repository.
* Download and install
[Visual Studio](https://www.visualstudio.com/en-us/downloads/download-visual-studio-vs.aspx).
* Download and install the latest [OpenCV 2.4.x release](http://opencv.org/downloads.html).
* Download and install the latest [CMake release](http://www.cmake.org/download/).
* Run CMake and configure the project [like so](http://www.gnebehay.com/cmt/cmake.png).
The only thing you actually have to specify yourself is the location of your OpenCV installation.
* Add the OpenCV DLLs to the PATH environment variable [like so](http://www.gnebehay.com/cmt/path.png).
The vcXX part depends on the edition of visual studio that you are using.
For example, vc12 is to be used for Visual Studio 2013.
* Open the Visual Studio solution file (CMT.sln), hit F7 to build and F5 to Run/Debug.

# Usage
```
usage: ./cmt [--challenge] [--no-scale] [--with-rotation] [--bbox BBOX]
             [--skip N] [--skip-msecs N] [--output-file FILE]
             [--verbose] [inputpath]
```
## Optional arguments
* `inputpath` The input path.
* `--challenge` Enter challenge mode.
* `--no-scale` Disable scale estimation
* `--with-rotation` Enable rotation estimation
* `--bbox BBOX` Specify initial bounding box. Format: x,y,w,h
* `--skip N` Skip N frames of the video input
* `--skip-msecs N` Skip N milliseconds of the video input
* `--output-file FILE` Save data to a file in CSV format
* `--verbose`, `-v` Turn on debugging console output

Trying to skip both frames and milliseconds at the start of a video will raise
an error.

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
cmt test.avi
```
If your input consists of numbered image files (e.g. im_0001.png, im_0002.png, ...), you can use printf syntax:
```
cmt im_%04d.png
```

It is also possible to specify the initial bounding box on the command line:
```
cmt --bbox=123,85,60,140 test.avi
```

You can output data (number of active points, bounding box parameters) to a
file in CSV format:
```
cmt --output-file=/path/to/file.csv test.avi
```
The data file includes header data so that it can be loaded into Excel or R
directly.

[1]: http://en.wikipedia.org/wiki/BSD_licenses#2-clause_license_.28.22Simplified_BSD_License.22_or_.22FreeBSD_License.22.29
