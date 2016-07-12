#include <iostream>
#include <string>
#include <fstream>
#include <dirent.h>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

vector<int> annotations;
int         lastFrame;
bool        endProgram;
int         pathLength;
char*       imgPath;
string      source;

string appnd(string s1, string s2) {
    return s1+s2;
}

int init () {

    DIR *dir;
    struct dirent *ent;

    size_t idx = source.find_last_of("/\\");

    if ((dir = opendir (source.substr(0,idx).c_str())) == NULL) {
		printf("Invalid folder path.\n");
    	return -1;
    }

    /*
    int frameNumber;
    size_t percIdx = source.find_last_of("%");
    size_t dotIdx = source.find_last_of(".");
    int len = stoi( source.substr( percIdx+2, dotIdx-percIdx-3 ) );
    percIdx = percIdx-idx+1;
    
    int fileCount=0;
	while ((ent = readdir (dir)) != NULL ) {
	    fileCount++;
	}

	if (fileCount < 3) {
        printf("Folder is empty.\n");
		return -1;
    }*/

    closedir (dir);

    char buffer [source.size()+10];
    pathLength = sprintf(buffer, source.c_str(), 0);
    FILE * pFile;
    pFile = fopen(buffer, "r");
    if ( pFile == NULL ) {
        printf("Invalid filename format.\n");
        return -1;
    }
    fclose(pFile);

    annotations.resize(lastFrame+1);

    string labelFolder = source.substr(0,idx);
    idx = labelFolder.find_last_of("/\\");
    labelFolder = labelFolder.substr(0,idx);

    ifstream labelFile(labelFolder+"/label.txt");

    string line;
    if (labelFile.is_open()) {
        getline (labelFile,line);
        if ( lastFrame != stoi(line) ) {
            printf("Last frame id does not match with the label file.\n");
            return -1;
        }
        int i=0;
        while (getline (labelFile,line)) {
            annotations[i] = stoi(line);
            i++;
        }
        labelFile.close();
    }
    cout << "Label file is read." << endl;


	return 1;
}

bool loadFrame(Mat& frame, int frameNumber) {

    try {
        if ( frameNumber <= lastFrame ){
            //cout << "filenumber: " << frameNumber << ". file: " << fileList[frameNumber] << endl;

            sprintf(imgPath, source.c_str(), frameNumber);
            frame = imread(imgPath, CV_LOAD_IMAGE_COLOR);
            return true;
        }

    } catch (int e) {
        endProgram = true;
    }
    return false;
}

int main(int argc, char** argv )
{
    endProgram = false;

    if ( argc != 3 )
    {
        printf("Usage: annotator folderPath lastFrameIndex\n");
        return -1;
    }

    source = argv[1];
    lastFrame = stoi(argv[2]);
    cout << lastFrame << endl;
    if (init() < 0)
        return -1;

    imgPath = new char[pathLength+1];

    namedWindow("Annotator", WINDOW_AUTOSIZE );
    Mat image;

    int frameNumber = 0;
    char key;

    while( loadFrame(image, frameNumber) && !endProgram ) {

        if ( !image.data )
        {
            printf("Skipped file number %d. \n", frameNumber);
            frameNumber++;
        }
        else {

            putText(image, "Frame number: " + to_string(frameNumber), Point(5,15), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0,0,0));
            putText(image, "Frame label: " + to_string(annotations[frameNumber]), Point(5,30), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0,0,0));
            imshow("Annotator", image);
            
            key = waitKey(0);
            if (key == 'n') {
                frameNumber++;
            }
            else if (key == 'p') {
                frameNumber--;
            }
            else if (key >= '0' && key <= '9') {
                annotations.at(frameNumber) = key-'0';
                frameNumber++;
            }
            else if (key == 'j') {
                cout << "Jump to frame: ";
                int temp;
                cin >> temp;
                if (frameNumber >= 0 && frameNumber <= lastFrame) {
                    frameNumber = temp;
                } 
                else {
                    cout << "Entered frame number is not in range." << endl;
                }
            }
            else if (key == 'q') {
                endProgram = true;
            }

            // Stay in the loop
            if (frameNumber < 0)
                frameNumber = 0;
            else if (frameNumber > lastFrame)
                frameNumber = lastFrame;

        }
    }

    delete [] imgPath;


    size_t idx = source.find_last_of("/\\");
    string labelFolder = source.substr(0,idx);

    idx = labelFolder.find_last_of("/\\");
    labelFolder = labelFolder.substr(0,idx);

    ofstream labelFile (labelFolder + "/label.txt");
    if (labelFile.is_open()) {
        labelFile << lastFrame << "\n";
        for (int i=0; i<=lastFrame; i++)
           labelFile << annotations[i] << "\n";
        
        labelFile.close();
        cout << "Label file saved " << source.substr(0,idx) << "/label.txt" << endl;
    }
    else cout << "Couldn't save to label file";

    return 0;
}