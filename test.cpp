//
// Created by vmachado on 28/11/17.
//

int ___main(){
    // Read the images to be aligned
    Mat im1 = imread("/home/vmachado/Videos/polarys/clips/v1_1/rgb001.jpg");
    Mat im2 = imread("/home/vmachado/Videos/polarys/clips/v1_1/tir001.jpg");



//    cv::resize(im1,im1,cv::Size(0,0), 0.35, 0.35);
//    cv::resize(im2,im2,cv::Size(0,0), 0.35, 0.35);
//
//
//    cv::resize(im2,im2,cv::Size(0,0), 1.05, 1.05);
//    Mat roi(im1, Rect(0.97*(im1.cols-im2.cols)/2 ,1.55*(im1.rows-im2.rows)/2, im2.cols, im2.rows));
//    roi.copyTo(im1);
//
//    cv::imshow("rgb", im1);
//    cv::imshow("tir", im2);
//    cv::waitKey();
//    return 0;


// Convert images to gray scale;
    Mat im1_gray, im2_gray;
    cvtColor(im1, im1_gray, CV_BGR2GRAY);
    cvtColor(im2, im2_gray, CV_BGR2GRAY);


// Define the motion model
    const int warp_mode = MOTION_HOMOGRAPHY;//MOTION_EUCLIDEAN;

// Set a 2x3 or 3x3 warp matrix depending on the motion model.
    Mat warp_matrix;

// Initialize the matrix to identity
    if ( warp_mode == MOTION_HOMOGRAPHY )
        warp_matrix = Mat::eye(3, 3, CV_32F);
    else
        warp_matrix = Mat::eye(2, 3, CV_32F);

// Specify the number of iterations.
    int number_of_iterations = 5000;

// Specify the threshold of the increment
// in the correlation coefficient between two iterations
    double termination_eps = 1e-1;

// Define termination criteria
    TermCriteria criteria (TermCriteria::COUNT+TermCriteria::EPS, number_of_iterations, termination_eps);

// Run the ECC algorithm. The results are stored in warp_matrix.
    findTransformECC(
            im1_gray,
            im2_gray,
            warp_matrix,
            warp_mode,
            criteria
    );

// Storage for warped image.
    Mat im2_aligned;

    if (warp_mode != MOTION_HOMOGRAPHY)
        // Use warpAffine for Translation, Euclidean and Affine
        warpAffine(im2, im2_aligned, warp_matrix, im1.size(), INTER_LINEAR + WARP_INVERSE_MAP);
    else
        // Use warpPerspective for Homography
        warpPerspective (im2, im2_aligned, warp_matrix, im1.size(),INTER_LINEAR + WARP_INVERSE_MAP);

// Show final result
    imshow("Image 1", im1);
    imshow("Image 2", im2);
    imshow("Image 2 Aligned", im2_aligned);
    waitKey(0);
}

