#include <cstdio>
#include "opencv2/opencv.hpp"

using namespace cv;

int main(int argc, char** argv) {
    // Define the arguments
    const String keys =
        "{ help   | | Print this message }"
        "{ @input | | Path to the image  }";

    // Construct the command line parser and parse the arguments
    CommandLineParser parser(argc, argv, keys);
    parser.about("Image arithmetic");

    if (parser.has("help")) {
        parser.printMessage();
        printf("Example:\n");
        printf("improcess-arithmetic butterfly.jpg\n");
        return 0;
    }

    String input_path = parser.get<String>(0);

    if (!parser.check()) {
        parser.printErrors();
        return -1;
    }

    // Add and subtract
    // The value will be modulos (wrap around)
    uchar n1[] = { 200 };
    uchar n2[] = { 100 };
    uchar n3[] = { 50 };
    uchar n_added = n1[0] + n2[0];
    uchar n_subtracted = n3[0] - n2[0];
    printf("wrap around: %d\n", n_added);
    printf("wrap around: %d\n", n_subtracted);

    // Add and subtract
    // The value will be clipped to the range [0, 255]
    Mat m1(1, 1, CV_8UC1, n1);
    Mat m2(1, 1, CV_8UC1, n2);
    Mat m3(1, 1, CV_8UC1, n3);
    Mat m_added = m1 + m2;
    Mat m_subtracted = m3 - m2;
    printf("max of 255: %d\n", m_added.at<uchar>(0));
    printf("min of 0: %d\n", m_subtracted.at<uchar>(0));

    // Load the image
    Mat img = imread(input_path);
    if (img.empty()) {
        printf("ERR: Cannot load the image\n");
        return -1;
    }
    imshow("Original", img);
    waitKey();

    // Increase the intensity of all pixels in the image by 100
    Mat M = Mat(img.size(), CV_8UC3, Scalar(100.0, 100.0, 100.0));
    Mat added = img + M;
    imshow("Added", added);
    waitKey();

    // Decrease the intensity of all pixels in the image by 50
    M = Mat(img.size(), CV_8UC3, Scalar(50.0, 50.0, 50.0));
    Mat subtracted = img - M;
    imshow("Subtracted", subtracted);
    waitKey();

    return 0;
}
