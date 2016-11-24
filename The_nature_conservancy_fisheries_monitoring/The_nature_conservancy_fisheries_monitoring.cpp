// The_ nature_conservancy_fisheries_monitoring.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace std;
using namespace cv;

/** Global variables */
string window_name = "Showing Image";
String fish_cascade_name = "C://opencv//build//etc//haarcascades//haarcascade_frontalface_alt.xml";
String info_file_path =		"C://Users//sanand2//Dropbox//MyProjects//kaggle//The_nature_conservancy_fisheries_monitoring//resources//test_data.txt";
//String fish_cascade_name =	"C://Users//sanand2//Dropbox//MyProjects//kaggle//The_nature_conservancy_fisheries_monitoring//resources//faces.xml";
CascadeClassifier fish_cascade;
RNG rng(12345);


void process_image(string file_path)
{
	Mat image;
	std::vector<Rect> fishes;

	image = imread(file_path, IMREAD_GRAYSCALE);
	//cvtColor(image, image_gray, CV_BGR2GRAY);
	equalizeHist(image, image);
	//imshow(window_name, image);


	//-- Detect fishes
	fish_cascade.detectMultiScale(image, fishes, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));

	//Draw bounding boxes
	Mat final_image(image);
	for (size_t i = 0; i < fishes.size(); i++)
	{
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		rectangle(final_image, fishes[i].tl(), fishes[i].br(), color, 2, 8, 0);
		//imshow("Contours", final_image);
	}

	std::size_t index_backslash = file_path.find_last_of("\\");
	std::size_t index_dot = file_path.find_last_of(".");
	std::size_t length = index_dot - index_backslash - 1;
	string file_name = file_path.substr(index_backslash + 1, length);

	imwrite(file_name + "_marked.jpg", final_image);

}

int main()
{
	//Load the trained cascades
	if (!fish_cascade.load(fish_cascade_name))
	{ 
		printf("--(!)Error loading\n"); 
		return -1; 
	};

	ifstream info_file(info_file_path);
	string line;
	vector<string> test_image_paths;
	while (info_file.good())
	{
		getline(info_file, line);
		test_image_paths.push_back(line);
	}



	for (size_t i = 0; i < test_image_paths.size(); i++)
	{
		cout << "Processing image " << test_image_paths[i] << endl;
		process_image(test_image_paths[i]);
	}

	//while (char(waitKey(1) != 'q')) //waiting for the 'q' key to finish the execution
	//{

	//}

    return 0;
}

