#include <cstdio>
#include <iostream>
#include "opencv2/opencv.hpp"
#include "utils/improcess.hpp"

using namespace cv;

int main(int argc, char** argv) {
    // Define the arguments
    const String keys =
        "{help   | | Print this message}"
        "{@input | | Path to the image}";

    // Construct the command line parser and parse the arguments
    CommandLineParser parser(argc, argv, keys);
    parser.about("Rotate the image");

    if (parser.has("help")) {
        parser.printMessage();
        printf("Example:\n");
        printf("improcess-rotate butterfly.jpg\n");
        return 0;
    }

    String input_path = parser.get<String>(0);

    if (!parser.check()) {
        parser.printErrors();
        return -1;
    }

    // Load the image
    Mat img = imread(input_path);
    if (img.empty()) {
        printf("ERR: Cannot load the image\n");
        return -1;
    }
    imshow("Original", img);
    waitKey();

    // Calculate the center of image
    Point2f center(static_cast<float>(img.cols * 0.5), 
                   static_cast<float>(img.rows * 0.5));

    // Rotate the image by 45 degrees
    Mat M = getRotationMatrix2D(center, 45.0, 1.0);
    Mat rotated;
    warpAffine(img, rotated, M, img.size());
    imshow("Rotated by 45 Degrees", rotated);
    waitKey();
    
    // Print the rotation matrix
    std::cout << "Rotate the image by 45 degrees - Rotation matrix:" << std::endl;
    std::cout << M << std::endl;

    // Rotate the image by 90 degrees
    M = getRotationMatrix2D(center, -90.0, 1.0);
    warpAffine(img, rotated, M, img.size());
    imshow("Rotated by -90 Degrees", rotated);
    waitKey();

    // Print the rotation matrix
    std::cout << "Rotate the image by -90 degrees - Rotation matrix:" << std::endl;
    std::cout << M << std::endl;

    // Rotate the image around an arbitrary point
    Point2f pt(static_cast<float>(img.cols * 0.5 - 50),
               static_cast<float>(img.rows * 0.5 - 50));
    M = getRotationMatrix2D(pt, 45.0, 1.0);
    warpAffine(img, rotated, M, img.size());
    imshow("Rotated by offset and 45 Degrees", rotated);
    waitKey();

    // Print the rotation matrix
    std::cout << "Rotated by offset and 45 Degrees - Rotation matrix:" << std::endl;
    std::cout << M << std::endl;

    // Rotate the image by 180 degrees
    // using the helper function
    rotate(img, rotated, 180.0);
    imshow("Rotated by 180 Degrees", rotated);
    waitKey();

    return 0;
}
