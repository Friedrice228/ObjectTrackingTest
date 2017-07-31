
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgcodecs.hpp>


using namespace cv;

IplImage* img;
VideoCapture cap(0);
Mat matOriginal, matHSV, matThresh, clusters, matHeirarchy;

Scalar RED(0,0,255),
		BLUE(0,255,0),
		GREEN(255,0,0),
		YELLOW(0, 255,255);

Point pt = Point(700,300);

void processImage(Scalar LOWER_BOUNDS, Scalar UPPER_BOUNDS){

	std::vector<std::vector<Point>> contours;

	while (true){

		contours.clear();

		cap>>matOriginal;

		cvtColor(matOriginal, matHSV, COLOR_BGR2HSV);

		inRange(matOriginal, LOWER_BOUNDS, UPPER_BOUNDS, matThresh);

		findContours(matThresh, contours, matHeirarchy, RETR_EXTERNAL,
				CHAIN_APPROX_SIMPLE);

		for(std::vector<Point> mop : contours){

			Rect rec = boundingRect(mop);

			rectangle(matOriginal, rec.br(), rec.tl(), YELLOW);

		}

		circle(matOriginal, pt, 5, RED, 1, 8, 0);

		imshow("Image", matOriginal);

		std::cout << "B: " << (int)matOriginal.at<Vec3b>(700,300)[0]; // [0] is blue
		std::cout << " G: " << (int)matOriginal.at<Vec3b>(700,300)[1]; // [1] is green
		std::cout << " R: " << (int)matOriginal.at<Vec3b>(700,300)[2] << std::endl; // [2] is red

	}

}

int * average(){

	int tick = 0;

	int arrB [30];
	int arrG [30];
	int arrR [30];

	while (tick < 30){

		cap>>matOriginal;

		arrB[tick] = (int)matOriginal.at<Vec3b>(700,300)[0];
		arrG[tick] = (int)matOriginal.at<Vec3b>(700,300)[1];
		arrR[tick] = (int)matOriginal.at<Vec3b>(700,300)[2];

		std::cout << "B: " << (int)matOriginal.at<Vec3b>(700,300)[0]; // [0] is blue
		std::cout << " G: " << (int)matOriginal.at<Vec3b>(700,300)[1]; // [1] is green
		std::cout << " R: " << (int)matOriginal.at<Vec3b>(700,300)[2] << std::endl; // [2] is red

		circle(matOriginal, pt, 5, YELLOW, 1, 8, 0);

		imshow("Image", matOriginal);

		tick++;

		std::cout << tick << std::endl;

	}

	int total = 0;
	int avg;
	static int ret [3]; // 0 = b, 1 = G, 2 = R (BGR)

	for (int i = 0; i < 3; i++){

		for (int j = 0; j < 30; j++){

			if (i == 0){

				total += arrB[j];

			}
			else if (i == 1){

				total += arrG[j];

			}
			else {

				total += arrR[j];

			}

		}

		avg = (int)(total/30);

		ret[i] = avg;

		total = 0;

	}

	return ret;

}

int main(){

	namedWindow("Image", 1);

	cap.open(0);

	int *avg;

	avg = average();

	Scalar LOWER_BOUNDS(avg[1] - 15, avg[0] - 15, avg[2] - 15);  //GBR
	Scalar UPPER_BOUNDS(avg[1] + 15, avg[0] + 15, avg[2] + 15);

	while (true){

		cap.open(0);

		std::cout << "LOWER: " << LOWER_BOUNDS << std::endl;
		std::cout << "UPPER: " << UPPER_BOUNDS << std::endl;

		processImage(LOWER_BOUNDS, UPPER_BOUNDS);

	}

	cap.release();

	exit(0);

	return 0;
}


