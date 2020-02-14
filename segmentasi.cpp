// Muhamad Hudan Widzamil (18218003)
// Information System and Technology, ITB
// Dago Hoogeschool 2019/2020

#include "opencv2/opencv.hpp"
#include <iostream>
#include <fstream>
#include "opencv2/ml/ml.hpp"

using namespace std;
using namespace cv;

// knn algorithm implementation source :
// https://www.geeksforgeeks.org/k-nearest-neighbours/
// with some modification

struct knnPoint 
{ 
	int val;	 // Group of point 
	double x, y, a, b, c, d;	 // Co-ordinate of point 
	double distance; // Distance from test point 
}; 

// Used to sort an array of points by increasing order of distance 
bool comparison(knnPoint a, knnPoint b) 
{ 
	return (a.distance < b.distance); 
} 

// This function finds classification of point p using k nearest neighbour algorithm.
int classifyAPoint(knnPoint arr[], int n, int k, knnPoint p) 
{ 
	// Fill distances of all points from p 
	for (int i = 0; i < n; i++) 
		arr[i].distance = 
			sqrt((arr[i].x - p.x) * (arr[i].x - p.x) + 
				(arr[i].y - p.y) * (arr[i].y - p.y)+
				(arr[i].a - p.a) * (arr[i].a - p.a)+ 
				(arr[i].b - p.b) * (arr[i].b - p.b)+
				(arr[i].c - p.c) * (arr[i].c - p.c)+ 
				(arr[i].d - p.d) * (arr[i].d - p.d)); 

	// Sort the Points by distance from p 
	sort(arr, arr+n, comparison);  

	int freq[n];
	for (int i = 0; i < n; i++)
	{
		freq[i]=0;
	}
	
	for (int i = 0; i < k; i++) 
	{ 
		freq[arr[i].val]++;
	 	 	 	 	 	
	} 
	int classifier;

	classifier = 1;
	for (int i = 1; i < n; i++)
	{
		if (freq[i]>freq[classifier-1])
		{
			classifier = i + 1; 
		}
		
	}
	
	return classifier;
} 

void swap(double *xp, double *yp){ 
			double temp = *xp; 
			*xp = *yp; 
			*yp = temp; 
		} 

void csvreader(knnPoint *arr){
	fstream inputfile;

	inputfile.open("sampledata.csv",ios::in);
	
	
	vector<string> row;
	string line,word,temp;

	while (getline(inputfile,line)){
		stringstream s(line);
		double rawdata[7];
		int counter = 0;
    	while (getline(s,word,','))
		{			
			rawdata[counter] = stod(word);
			counter++;			
		}
		arr[int(rawdata[6])-1].x = rawdata[0]; 
		arr[int(rawdata[6])-1].y = rawdata[1]; 
		arr[int(rawdata[6])-1].a = rawdata[2]; 
		arr[int(rawdata[6])-1].b = rawdata[3];
		arr[int(rawdata[6])-1].c = rawdata[4]; 
		arr[int(rawdata[6])-1].d = rawdata[5];
		arr[int(rawdata[6])-1].val = int (rawdata[6]);
	}
    inputfile.close();
}

int main(){

	// Create a VideoCapture object and open the input file
    // If the input is the web camera, pass 0 instead of the video file name
	VideoCapture cap("samplevid.mp4");
	//VideoCapture cap(0);

    // Check if camera opened successfully
    if(!cap.isOpened()){
        cout << "Error opening video stream or file" << endl;
        return -1;
    }
    //initial state
    int hHSV = 60; //Hue Lower Bound
    int sHSV = 0; //Saturation Lower Bound
    int vHSV = 50;  //Value Lower Bound 

	int max_hHSV = 255; //Hue Upper Bound
    int max_sHSV = 80; //Saturation Upper Bound
    int max_vHSV = 255;  //Value Upper Bound 

    int threshold = 105; //Threshold value for hough line
	int k = 1; 			//k parameter in kNN
	int min_rho = 280;  //Rho lower bound to detect most left line
	int gamma = 110; //

	int houghRho = 1;
	int srn,stn = 0 ;
	int min_theta = 0;
    
    //create trackbar
    namedWindow("Parameter",WINDOW_NORMAL);
    createTrackbar("MIN H_HSV","Parameter",&hHSV,255);
    createTrackbar("MIN S_HSV","Parameter",&sHSV,255);
    createTrackbar("MIN V_HSV","Parameter",&vHSV,255);

	createTrackbar("MAX H_HSV","Parameter",&max_hHSV,255);
    createTrackbar("MAX S_HSV","Parameter",&max_sHSV,255);
    createTrackbar("MAX V_HSV","Parameter",&max_vHSV,255);

	createTrackbar("min rho for left side","Parameter",&min_rho,400);
	createTrackbar("k","Parameter",&k,60);
	
	namedWindow("HoughLine Parameter",WINDOW_NORMAL);
	createTrackbar("threshold","HoughLine Parameter",&threshold,255);
	createTrackbar("PI/gamma","HoughLine Parameter",&gamma,400);
	createTrackbar("Rho","HoughLine Parameter",&houghRho,40);
	createTrackbar("srn","HoughLine Parameter",&srn,20);
	createTrackbar("stn","HoughLine Parameter",&stn,20);
	createTrackbar("min theta","HoughLine Parameter",&min_theta,CV_PI);

    int n = 60; // Number of data points 
	
	//Sample data
	knnPoint arr[n]; 

	csvreader(arr);
	
	int framecounter = 0;
    while (1)
    {
        Mat img;

		cap >> img;

        Mat imgtoshow;
        //resize
        float width = 640;
        float imgScale = width/img.cols;
        float newWidth = img.cols*imgScale;
        float newHeight = img.rows*imgScale;
        resize(img,img,Size(newWidth,newHeight));
        resize(img,imgtoshow,Size(newWidth,newHeight));

        //Blur
        GaussianBlur(img,img,Size(3,3),3,0);
        //GaussianBlur(img,img,Size(5,5),5,0);
		//GaussianBlur(img,img,Size(9,9),9,0);
        
		//convert colorspace
        Mat hsv;
        cvtColor(img,hsv,COLOR_BGR2HSV);

        imshow("hsv",hsv);

        //CV inRange bound
        Mat hsv2;
        Vec3b lowerHSV(hHSV,sHSV,vHSV);
        Vec3b upper(max_hHSV,max_sHSV,max_vHSV);
        inRange(hsv,lowerHSV,upper,hsv2);
        imshow("hsv2",hsv2);

        //Mat hsv3;
        //erode(hsv2,hsv3,Mat(),Point(-1,-1),3,1,1);
        //imshow("hsv3",hsv3);
        
        //Mat cannyedge;
        //Canny(hsv2,cannyedge,100,200);
		//imshow("canny",cannyedge);

        //houghline
        vector<Vec2f>lines;
        //HoughLines(hsv2,lines,1,CV_PI/gamma,threshold);
		HoughLines(hsv2,lines,houghRho,CV_PI/gamma,threshold,srn,stn,min_theta);
        
        //drawline
        Mat outlineH;
        imgtoshow.copyTo(outlineH);
    
		double input_data[3][2];
		input_data[0][0] = 0;
		input_data[0][1] = 0;
        input_data[1][0] = 0;
		input_data[1][1] = 0;
		input_data[2][0] = 0;
		input_data[2][1] = 0;

        int pointcounter= 0;

        for (size_t i = 0; i < lines.size(); i++)
        {
            float rho = lines[i][0], theta = lines[i][1];
            
			if (rho > min_rho)
			{
				input_data[0][0] = rho;
				input_data[0][1] = theta;
			}else if (rho > 0)
			{
				input_data[1][0] = rho;
				input_data[1][1] = theta;
			}else
			{
				input_data[2][0] = rho;
				input_data[2][1] = theta;
			}
			
            Point pt1, pt2;
            double a = cos(theta), b = sin(theta);
            double x0 = a*rho, y0 = b*rho;
            
            pt1.x = cvRound(x0 + 1000*(-b));
            pt1.y = cvRound(y0 + 1000*(a));
            pt2.x = cvRound(x0 - 1000*(-b));
            pt2.y = cvRound(y0 - 1000*(a));
            
            line(outlineH, pt1, pt2, Scalar(0,0,255), 2, LINE_AA); 
            
            pointcounter++;
 
        }
		
        imshow("line",outlineH);

		knnPoint p; 
            p.x = input_data[0][0]; 
            p.y = input_data[0][1];
            p.a = input_data[1][0];
            p.b = input_data[1][1];
            p.c = input_data[2][0];
            p.d = input_data[2][1];

		framecounter++;
		cout<<"Frame ["<<framecounter<<"] "; 
        cout<<p.x<<" "<<p.y<<" "<<p.a<<" "<<p.b<<" "<<p.c<<" "<<p.d<<endl;
        
		// Parameter to decide group of the testing point 
        cout<<"k = "<<k<<" "<<", Segmen = "<<classifyAPoint(arr, n, k, p)<<endl;

        // press ESC to exit
        char c=(char)waitKey(100);
        if(c==27)
        break;

    }
	cap.release();
    destroyAllWindows();
    return 0;
}