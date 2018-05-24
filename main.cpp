/*************************************************************************************************
 * Priority-Flood Algorithm No.2 :: main module
 *
 * This code implements a main module for the priority-flood flood-fill Algorithm 2 described in article:
 * Barnes, Lehman, Mulla. "Priority-Flood: An Optimal Depression-Filling and Watershed-Labeling
 * Algorithm for Digital Elevation Models". Computers & Geosciences. Vol 62, Jan 2014, pp 117–127,
 * doi: 10.1016/j.cageo.2013.04.024.
 *
 * By Vincenzo De Florio, 2016-10-21.
 *
 * Version: see const string mversion
 *
 *************************************************************************************************/
#include "GSPriorityFlood.h"

#include <string.h>
#include <iostream>
#include <fstream>
#include <map>
#include <utility>		// std::pair
#include <iomanip>		// std::setprecision
#include <limits>		// std::numeric_limits
#include <vector>
#include <algorithm>	// std::max
#include <queue>

// OpenCV includes
//#include <cv.h>
//#include "opencv2/opencv.hpp"
//#include <highgui.h>
#include <iostream>
//#include <opencv2/imgproc/imgproc.hpp>
//#include <opencv2/core/core.hpp>
//#include <opencv2/highgui/highgui.hpp>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>

using namespace cv;
using namespace std;

const string mversion = "preliminary, 0.3, 2016-10-22";

#define NaN		-9999.0  // std::numeric_limits<Real>::min()

void printHelp();
void fromRGB2Mat(Mat& img, unsigned char **r, unsigned char **g, unsigned char **b);
void fromMat2RGB(Mat& img, unsigned char **r, unsigned char **g, unsigned char **b);
Boolean diffMat(Mat& dst, Mat& src);

int main(int argc, char *argv[])
{
	int i, j;
	string iFileName;
	string oFileName;
	string dFileName;
	Mat src, dst, diff;
	string XSDPath;
	int rows, cols;
	Boolean error;
	int verbose;

	verbose=0;
	// manage command-line args
	if (argc>1)
	for (i=1; i<argc; i++)
		if (argv[i][0] == '-')
		switch(argv[i][1]) {
		case 'v': verbose=1; break;
		case 'h': printHelp(); return 0;
		case 'i': iFileName = argv[++i]; break;
		case 'o': oFileName = argv[++i]; break;
		case 'd': dFileName = argv[++i]; break;
		case 'x': XSDPath = argv[++i]; break;

		default: std::cerr << "Unknown switch.\n" << std::endl;
				 printHelp();
				 return -1;
		}



	if (iFileName.empty()) {
		std::cerr << "Option -i <filename> must be present! Aborting...\n";
		return -1;
	}
	if (oFileName.empty()) {
		std::cerr << "Option -o <filename> is missing. Output file name is set to 'output.jpg'\n";
		oFileName = "output.jpg";
	}

	src = imread(iFileName, cv::IMREAD_COLOR);
	namedWindow("Input image", CV_WINDOW_AUTOSIZE );
	imshow("Input image", src );

	std::cout << "Image " << iFileName << " consists of " << src.channels() << " channels and "
		<< src.cols << "x" << src.rows << " pixels.\n";

	//dst = imread(iFileName, CV_LOAD_IMAGE_COLOR); //src.clone();
	src.copyTo(dst);

	cols = src.cols;
	rows = src.rows;

	unsigned char **r, **g, **b;
	unsigned char *buffr, *buffg, *buffb;

	r = new unsigned char *[rows];
	g = new unsigned char *[rows];
	b = new unsigned char *[rows];

	buffr = new unsigned char[ cols * rows ];
	buffg = new unsigned char[ cols * rows ];
	buffb = new unsigned char[ cols * rows ];

	for (int i=0, disp=0; i<rows; i++, disp += cols)
		r[i] = & buffr[disp];
	for (int i=0, disp=0; i<rows; i++, disp += cols)
		g[i] = & buffg[disp];
	for (int i=0, disp=0; i<rows; i++, disp += cols)
		b[i] = & buffb[disp];

	fromMat2RGB(src, r, g, b);

	/*
	unsigned char *origr, *origg, *origb;
	if (! dFileName.empty()) {
		origr = new unsigned char[ cols * rows ];
		origg = new unsigned char[ cols * rows ];
		origb = new unsigned char[ cols * rows ];
		int rxc = rows*cols;
		for (i=0; i<rxc; i++) {
			origr[i] = buffr[i];
			origg[i] = buffg[i];
			origb[i] = buffb[i];
		}
	}
	*/

	GSFloodFill<unsigned char> *floodFill;


	floodFill = new GSFloodFill<unsigned char>(r, rows, cols);
	floodFill->setVerbose(verbose);
	if (! floodFill->Transform()) {
		std::cerr << "floodFill.Transform (r) has failed! Aborting...\n";
		return -1;
	}
	delete floodFill;

	floodFill = new GSFloodFill<unsigned char>(g, rows, cols);
	//GSFloodFill<unsigned char> floodFill(g, rows, cols);
	floodFill->setVerbose(verbose);
	if (! floodFill->Transform()) {
		std::cerr << "floodFill.Transform (g) has failed! Aborting...\n";
		return -1;
	}
	delete floodFill;

	floodFill = new GSFloodFill<unsigned char>(b, rows, cols);
	//GSFloodFill<unsigned char> floodFill(b, rows, cols);
	floodFill->setVerbose(verbose);
	if (! floodFill->Transform()) {
		std::cerr << "floodFill.Transform (b) has failed! Aborting...\n";
		return -1;
	}
	delete floodFill;


	// Now r, g and b point to the flood-filled transformation of the three input bands

	// (r,g,b) => dst
	fromRGB2Mat(dst, r, g, b);

	namedWindow( "Flood-filled image", CV_WINDOW_AUTOSIZE );
	imshow("Flood-filled image", dst );

	imwrite("FloodFilled.jpg", dst );



	if (! dFileName.empty()) {
		dst.copyTo(diff);
		diffMat(diff, src);

		namedWindow( "Difference image", CV_WINDOW_AUTOSIZE );
		imshow("Difference image", diff );
		imwrite("Difference.jpg", diff );
	}

	waitKey(0);
	return 0;
}

void printHelp() {
std::cerr <<
					//LxDeco(c_color::YELLOW, c_decoration::NORMAL) <<
	" * Priority-Flood Algorithm No.1\n" <<
					//RxDeco() <<
	" *\n" <<
	" * This code implements the priority-flood flood-fill algorithm described in article:\n" <<
	" * Barnes, Lehman, Mulla. \"Priority-Flood: An Optimal Depression-Filling and Watershed-Labeling\n" <<
	" * Algorithm for Digital Elevation Models\". Computers & Geosciences. Vol 62, Jan 2014, pp 117–127,\n" <<
	" * doi: 10.1016/j.cageo.2013.04.024.\n" <<
	" *\n" <<
	" * By Vincenzo De Florio, 2016-10-20.\n" <<
	" *\n" <<
	" * Version: " << mversion << std::endl;
}

void fromRGB2Mat(Mat& img, unsigned char **r, unsigned char **g, unsigned char **b) {
	for(int y = 0; y < img.rows; y++){
		for(int x = 0; x < img.cols; x++){
			Vec3b& intensity = img.at<Vec3b>(y, x);
			uchar& blu = intensity.val[0];
			uchar& gre = intensity.val[1];
			uchar& red = intensity.val[2];

			blu = b[y][x];
			gre = g[y][x];
			red = r[y][x];
		}
	}
}

void fromMat2RGB(Mat& img, unsigned char **r, unsigned char **g, unsigned char **b) {
	for(int y = 0; y < img.rows; y++){
		for(int x = 0; x < img.cols; x++){
			Vec3b& intensity = img.at<Vec3b>(y, x);
			uchar& blu = intensity.val[0];
			uchar& gre = intensity.val[1];
			uchar& red = intensity.val[2];

			b[y][x] = blu;
			g[y][x] = gre;
			r[y][x] = red;
		}
	}
}

Boolean diffMat(Mat& dst, Mat& src) {
	for(int y = 0; y < dst.rows; y++){
		for(int x = 0; x < dst.cols; x++){
			Vec3b& dintensity = dst.at<Vec3b>(y, x);
			uchar& dblu = dintensity.val[0];
			uchar& dgre = dintensity.val[1];
			uchar& dred = dintensity.val[2];

			Vec3b& sintensity = src.at<Vec3b>(y, x);
			uchar& sblu = sintensity.val[0];
			uchar& sgre = sintensity.val[1];
			uchar& sred = sintensity.val[2];
			dblu -= sblu;
			dgre -= sgre;
			dred -= sred;
		}
	}
}
