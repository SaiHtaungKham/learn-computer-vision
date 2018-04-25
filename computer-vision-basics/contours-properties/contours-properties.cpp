#include <cstdio>
#include "opencv2/opencv.hpp"
#include "cvutils/colors.hpp"

using namespace cv;

int main(int argc, char** argv) {
    // Define the arguments
    const String keys =
        "{ help   | | Print this message }"
        "{ @input | | Path to the image  }";

    // Construct the command line parser and parse the arguments
    CommandLineParser parser(argc, argv, keys);
    parser.about("Find properties of contours");

    if (parser.has("help")) {
        parser.printMessage();
        printf("Example:\n");
        printf("contours-properties shapes-2.png\n");
        return 0;
    }

    String input_path = parser.get<String>(0);

    if (!parser.check()) {
        parser.printErrors();
        return -1;
    }

    // Load the image
    Mat image = imread(input_path);
    if (image.empty()) {
        printf("ERR: Cannot load the image\n");
        return -1;
    }
    imshow("Original", image);
    waitKey();

    // Convert the input image to grayscale
    Mat gray;
    cvtColor(image, gray, COLOR_BGR2GRAY);

    // Find the external contours in the image
    std::vector<std::vector<Point> > contours;
    findContours(gray.clone(), contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    // For each external contour, 
    // compute its moments which then is used to compute the centroid
    Mat clone = image.clone();
    for (unsigned int i = 0; i < contours.size(); i++) {
        Moments M = moments(contours[i]);
        int cX = static_cast<int>(M.m10 / M.m00);
        int cY = static_cast<int>(M.m01 / M.m00);
        Point centroid(cX, cY);
        circle(clone, centroid, 4, COLORNAME_YELLOW, FILLED);
    }
    imshow("Centroids", clone);
    waitKey();
    destroyAllWindows();

    // For each external contour,
    // compute the contour area and perimeter
    clone = image.clone();
    for (unsigned int i = 0; i < contours.size(); i++) {
        double area = contourArea(contours[i]);
        double perimeter = arcLength(contours[i], true);
        printf("Contour #%d -- area: %.2f, perimeter: %.2f\n", i + 1, area, perimeter);

        // For visualization
        drawContours(clone, contours, i, COLORNAME_WHITE, 2);
        Moments M = moments(contours[i]);
        int cX = static_cast<int>(M.m10 / M.m00);
        int cY = static_cast<int>(M.m01 / M.m00);
        Point org(cX - 15, cY);
        putText(clone, format("#%d", i + 1), org, FONT_HERSHEY_SIMPLEX, 0.75, COLORNAME_YELLOW, 2);
    }
    imshow("Contours", clone);
    waitKey();
    destroyAllWindows();

    // For each external contour,
    // find its bounding box
    clone = image.clone();
    for (unsigned int i = 0; i < contours.size(); i++) {
        Rect box = boundingRect(contours[i]);
        rectangle(clone, box, COLORNAME_GREEN, 2);
    }
    imshow("Bounding Boxes", clone);
    waitKey();
    destroyAllWindows();

    // For each external contour,
    // find its rotated bounding box
    clone = image.clone();
    for (unsigned int i = 0; i < contours.size(); i++) {
        RotatedRect rotated_box = minAreaRect(contours[i]);
        
        Point2f p[4];
        std::vector<Point> pts;
        std::vector<std::vector<Point> > points;
        rotated_box.points(p);
        for (unsigned int j = 0; j < 4; j++) {
            pts.push_back(static_cast<Point>(p[j]));
        }
        points.push_back(pts);
        drawContours(clone, points, -1, COLORNAME_GREEN, 2);
    }
    imshow("Rotated Bounding Boxes", clone);
    waitKey();
    destroyAllWindows();

    // For each external contour,
    // find its minimum enclosing circle
    clone = image.clone();
    for (unsigned int i = 0; i < contours.size(); i++) {
        Point2f center;
        float radius;
        minEnclosingCircle(contours[i], center, radius);
        circle(clone, static_cast<Point>(center), cvRound(radius), COLORNAME_GREEN, 2);
    }
    imshow("Min-Enclosing Circles", clone);
    waitKey();
    destroyAllWindows();

    // For each external contour,
    // fit an ellipse
    clone = image.clone();
    for (unsigned int i = 0; i < contours.size(); i++) {
        if (contours[i].size() > 5) {   //  A contour must have at least 5 points for an ellipse to be computed
            RotatedRect rotated_box = fitEllipse(contours[i]);
            ellipse(clone, rotated_box, COLORNAME_GREEN, 2);
        }
    }
    imshow("Ellipses", clone);
    waitKey();

    return 0;
}
