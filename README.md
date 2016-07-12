# labeler
Generate numeric labels for image sequences. 

OpenCV is required (Version 3.1). <br />
Coder: Burak Kakillioglu.

Usage
-----
> ./labeler path/to/images/folder/filename%0Xd.jpg lastFileID

where X is the file id length.

Example
-------

files0000.jpg	< First file <br />
files0001.jpg <br />
files0002.jpg <br />
... <br />
files8552.jpg <br />
... <br />
files9500.jpg   < Last file <br />

> ./bin/annotator path/to/images/folder/files%04d.jpg 9500

Specifications
--------------
* Press 0-9 to label current image.
* Press 'n' to go next frame.
* Press 'p' to go previous frame.
* Press 'j' to jump to a specific frame (Enter file id in command line).
* Press 'q' to quit.
* Maximum 10 labels (0 to 9) can be used. (Higher label numbers can be achieved by altering the code though. It's gonna be pretty straightforward.)
* All labels are initially 0.
* Quitting automatically saves label.txt file.
* Running the program with same configuration loads previous label.txt file if exists.
