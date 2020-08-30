// This code is written at BigVision LLC. It is based on the OpenCV project. It is subject to the license terms in the LICENSE file found in this distribution and at http://opencv.org/license.html



// Usage example:  ./object_detection_yolo.out --video=run.mp4

//                 ./object_detection_yolo.out --image=bird.jpg

#include <fstream>

#include <sstream>

#include <iostream>

#include<experimental/filesystem>

#include<dirent.h>



#include<iostream>

#include<vector>

#include<string>

#include<set>

#include<ftw.h>

#include<algorithm>

#include<string>

#include <opencv2/imgproc.hpp>

#include <opencv2/highgui.hpp>

#include <opencv2/dnn.hpp>



using namespace cv;

using namespace dnn;

using namespace std;



// Initialize the parameters

string classesFile = "coco.names";

String modelConfiguration = "yolov3.cfg";

String modelWeights = "yolov3.weights";

string pattern="Has({person, dog}), No({horse})";

string DirWithImages = ".";

float confThreshold = 0.5;

float nmsThreshold = 0.4;  

int inpWidth = 416; 

int inpHeight = 416; 

VideoCapture cap;

Mat frame, blob;

Net net ;

vector<string> classes;

void postprocess(Mat& frame, const vector<Mat>& out,set<string>&keywords);

vector<String> getOutputsNames(const Net& net);

 void checkFile(string filePath,set<string>&keywords);

 void scanDir(string dirPath,vector<string>&images);







void ParsePattern(string pattern, vector<string>&HasArray,vector<string>&NoArray)

{

        string has = pattern.substr(pattern.find("Has"),pattern.find("No"));

    has=has.substr(has.find("{")+1);

    has=has.substr(0,has.find("}"));

    

    string no = pattern.substr(pattern.find("No"));

    

    no = no.substr(no.find("{")+1);

    no=no.substr(0,no.find("}"));





    istringstream stringstr(has);

    string str;

    while(getline(stringstr,str,','))

    {

        str.erase(remove_if(str.begin(), str.end(), ::isspace),str.end());

        HasArray.push_back(str);

        

    }

    istringstream stringstr2(no);

    while(getline(stringstr2,str,','))

    {

         str.erase(remove_if(str.begin(), str.end(),::isspace),str.end());

        NoArray.push_back(str);

        

    }





}



bool PatternPass(set<string>keywords,vector<string>hasKeywords,vector<string>noKeywords)

{

    bool ret = true;

    for(string hasKeyword : hasKeywords)

    {

        if(find(keywords.begin(),keywords.end(),hasKeyword)== keywords.end()){

            ret = false;

            break;

        }



    }

    if(ret == true)

    {

        for(string noKeyword : noKeywords)

        {

            if(find(keywords.begin(),keywords.end(),noKeyword)!= keywords.end()){

                ret = false;

                break;

                }



        }

    }

    return ret;

}

int main(int argc, char** argv)

{



    ifstream ifs(classesFile.c_str());

    string line;

    while (getline(ifs, line)) classes.push_back(line);

    

    net =  readNetFromDarknet(modelConfiguration, modelWeights);

    net.setPreferableBackend(DNN_BACKEND_OPENCV);

    net.setPreferableTarget(DNN_TARGET_CPU);



    vector<string>has,no;

    ParsePattern(pattern,has,no);

   

    vector<string>images;

    scanDir(DirWithImages,images);

    set<string> keywords;

    vector<string>PassedImageFiles;

    for(string imagePath : images)

    {

        checkFile(imagePath,keywords);

       

       if(PatternPass(keywords,has,no))

       {

           PassedImageFiles.push_back(imagePath);

       }

       keywords.clear();

  }

  for(string str : PassedImageFiles)

  {

      std::cout<<str<<endl;

  }



    return 0;

}





void checkFile(string filePath,set<string>&keywords)

{

     try {  

            cap.open(filePath);

           

        }

    catch(...) {

        cout << "Could not open the input image/video stream" << endl;

      return;

    }



        // get frame from the video

        cap >> frame;



        // Create a 4D blob from a frame.

        blobFromImage(frame, blob, 1/255.0, cv::Size(inpWidth, inpHeight), Scalar(0,0,0), true, false);

        

        //Sets the input to the network

        net.setInput(blob);

        

        // Runs the forward pass to get output of the output layers

        vector<Mat> outs;

     

       net.forward(outs, getOutputsNames(net));



        postprocess(frame, outs,keywords);



    cap.release();

}

 void scanDir(string dirPath,vector<string>&images)

 {

     string jpgPath=dirPath+"/*.jpg";

   

   

     vector<cv::String> fn;

    glob(jpgPath, fn, true);

    for(cv::String str : fn)

    {

        images.push_back(str);

    }

    

    string jpegPath=dirPath+"/*.jpeg";

   

   

    // vector<cv::String> fn;

    glob(jpegPath, fn, true);

    for(cv::String str : fn)

    {

        images.push_back(str);

    }





    string pngPath=dirPath+"/*.png";

    

    

    //  vector<cv::String> fn;

    glob(pngPath, fn, true);

    for(cv::String str : fn)

    {

        images.push_back(str);

    }



 }



// Remove the bounding boxes with low confidence using non-maxima suppression

void postprocess(Mat& frame, const vector<Mat>& outs,set<string>&keywords)

{

    vector<int> classIds;

    vector<float> confidences;

    vector<Rect> boxes;

    

    for (size_t i = 0; i < outs.size(); ++i)

    {

       

        float* data = (float*)outs[i].data;

        for (int j = 0; j < outs[i].rows; ++j, data += outs[i].cols)

        {

            Mat scores = outs[i].row(j).colRange(5, outs[i].cols);

            Point classIdPoint;

            double confidence;

            // Get the value and location of the maximum score

            minMaxLoc(scores, 0, &confidence, 0, &classIdPoint);

            



            if (confidence > confThreshold)

            {

               

                keywords.insert(classes[classIdPoint.x]);

            }

        }

    }

    

   

}





// Get the names of the output layers

vector<String> getOutputsNames(const Net& net)

{

    static vector<String> names;

    if (names.empty())

    {

        //Get the indices of the output layers, i.e. the layers with unconnected outputs

        vector<int> outLayers = net.getUnconnectedOutLayers();

        

        //get the names of all the layers in the network

        vector<String> layersNames = net.getLayerNames();

        

        // Get the names of the output layers in names

        names.resize(outLayers.size());

        for (size_t i = 0; i < outLayers.size(); ++i)

        names[i] = layersNames[outLayers[i] - 1];

    }

    return names;

}