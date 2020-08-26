/* Applied Video Sequence Analysis (AVSA)
 *
 *	LAB1.0: Background Subtraction - Unix version
 *	fgesg.cpp
 *
 * 	Authors: José M. Martínez (josem.martinez@uam.es), Paula Moral (paula.moral@uam.es) & Juan Carlos San Miguel (juancarlos.sanmiguel@uam.es)
 *	VPULab-UAM 2020
 */

#include <opencv2/opencv.hpp>
#include <stdlib.h>
#include "fgseg.hpp"

using namespace fgseg;

//default constructor
/*bgs::bgs(double threshold, bool rgb)
{
	_rgb=rgb;

	_threshold=threshold;
}*/

/*bgs::bgs(double threshold, double alpha, bool selective_bkg_update, int threshold_ghosts2, bool rgb){
	_rgb=rgb;

	_threshold=threshold;

	_selective_bkg_update = selective_bkg_update;

	_alpha = alpha;

	_threshold_ghosts2 = threshold_ghosts2;

	_counter = Mat::zeros(Size(_bgsmask.cols,_bgsmask.rows), CV_8UC1);
}*/

bgs::bgs(double threshold, double alpha, double threshold_ghosts2,bool rgb, double alpha_gaussian, float var){

	_rgb=rgb;

	_threshold=threshold;

	_alpha = alpha;

	_var = var;
	_alpha_gaussian = alpha_gaussian;

}

//default destructor
bgs::~bgs(void)
{
}

//method to initialize bkg (first frame - hot start)
void bgs::init_bkg(cv::Mat Frame)
{

	if (!_rgb){
		cv::cvtColor(Frame, Frame, COLOR_BGR2GRAY);

		_bkg = Mat::zeros(Size(Frame.cols,Frame.rows), CV_8UC1);
		Frame.copyTo(_bkg);

		cv::Mat cov;
		cv::Mat men;

		cv::meanStdDev(Frame, men, cov);

		float szam1 = (float) cov.at<double>(0,0);
		float szam2 = (float) men.at<double>(0,0);
		_varMask = Mat::ones(Size(Frame.cols,Frame.rows), CV_32FC1);
		_varMask = _varMask.mul(szam1);
		_varMask = _varMask.mul(_varMask);
		_meanMask = Mat::ones(Size(Frame.cols,Frame.rows), CV_32FC1);
		//_meanMask = _meanMask * szam2;
		Frame.copyTo(_meanMask);
		_meanMask.convertTo(_meanMask, CV_32FC1);

		//...
	}
	else{
		cout << "Colour currently not supported" << endl;
		exit(1);
	}

}

//method to perform BackGroundSubtraction
void bgs::bkgSubtraction(cv::Mat Frame)
{

	if (!_rgb){

		cv::cvtColor(Frame, Frame, COLOR_BGR2GRAY);
		_frame = Mat::zeros(Size(Frame.cols,Frame.rows), CV_8UC1); // void function for Lab1.0 - returns zero matrix
		_diff = Mat::zeros(Size(Frame.cols,Frame.rows), CV_8UC1); // void function for Lab1.0 - returns zero matrix
		_bgsmask = Mat::zeros(Size(Frame.cols,Frame.rows), CV_8UC1); // void function for Lab1.0 - returns zero matrix

		//ADD YOUR CODE HERE
		//...

		Frame.copyTo(_frame);
		/*cv::Mat bkg_temp = Mat::zeros(Size(_bgsmask.cols,_bgsmask.rows), CV_8UC1);
		_diff = abs(Frame - _bkg);

		_bgsmask = _diff > _threshold;*/

		//...
	}
	else{
		cout << "Colour currently not supported" << endl;
		exit(1);
	    }

}

//method to detect and remove shadows in the BGS mask to create FG mask
void bgs::removeShadows()
{
	if (!_rgb){
		_shadowmask = Mat::zeros(Size(_bgsmask.cols,_bgsmask.rows), CV_8UC1); // void function for Lab1.0 - returns zero matrix
		//_fgmask = Mat::zeros(Size(_bgsmask.cols,_bgsmask.rows), CV_8UC1); // void function for Lab1.0 - returns zero matrix

		}
		else{
			cout << "Colour currently not supported" << endl;
			exit(1);
		}


}

//ADD ADDITIONAL FUNCTIONS HERE
void bgs::updateBackground(){
}

void bgs::UnimodalupdateBackground(cv::Mat Frame){
	_frame.convertTo(_frame, CV_32F);
	cv::Mat simpleVariance = Mat::zeros(Size(Frame.cols,Frame.rows), CV_32F);

	sqrt(_varMask, simpleVariance);

	int numberOfTime = 2;
	cv::Mat lowerBoundaryMask = ((_meanMask - numberOfTime * simpleVariance) <= _frame);
	cv::Mat upperBoundaryMask = ((_meanMask + numberOfTime * simpleVariance) >= _frame);

	cv::Mat combinedMask = Mat::zeros(Size(Frame.cols,Frame.rows), CV_32F);
	bitwise_and(lowerBoundaryMask, upperBoundaryMask, combinedMask); // 255 if inside the gaussian -> we are in the background

	cv::Mat distance = (_frame - _meanMask);
	distance = distance.mul(distance);

	//SELECTIVE UPDATE
	/*_fgmask.convertTo(_fgmask, CV_32F);
	cv::Mat unchangedMask = (255 - _fgmask)/255;
	cv::Mat unchangedPartOfMean = _meanMask.mul(unchangedMask);
	cv::Mat unchangedPartOfVariance = _varMask.mul(unchangedMask);

	cv::Mat varTemp = _alpha_gaussian * squaredDistanceMap + (1-_alpha) * _varMask;
	_varMask = varTemp.mul(_fgmask) + unchangedPartOfVariance;

	cv::Mat meanTemp = _alpha_gaussian * _frame + (1-_alpha) * _meanMask;
	_meanMask = meanTemp.mul(_fgmask) + unchangedPartOfMean;*/

	//BlIND UPDATE
	_varMask = _alpha_gaussian * distance + (1-_alpha) * _varMask;
	_meanMask = _alpha_gaussian * _frame + (1-_alpha) * _meanMask;
	_meanMask.convertTo(_bkg, CV_8UC1);

	combinedMask.convertTo(combinedMask, CV_8UC1);
	bitwise_not(combinedMask, combinedMask);

	combinedMask.copyTo(_fgmask);
	combinedMask.copyTo(_bgsmask);
	combinedMask.copyTo(_shadowmask);

	//PixelWise Solution
	/*for(int j = 0; j < Frame.cols; j++){
		for(int i = 0; i < Frame.rows; i++){
			bool botIsTrue = false;

			double s = sqrt(_varMask.at<float>(i,j));

			double currentPixelOfFrame = (double) _frame.at<uchar>(i,j);

			if( (_meanMask.at<float>(i,j) - 1* s) < currentPixelOfFrame
					&& _meanMask.at<float>(i,j) + 1 * s > currentPixelOfFrame){
					_bgsmask.at<uchar>(i,j) = 0;

					_varMask.at<float>(i,j) = _alpha_gaussian
													* (currentPixelOfFrame - _meanMask.at<float>(i,j))
													* (currentPixelOfFrame - _meanMask.at<float>(i,j))
													+ (1 - _alpha_gaussian) * _varMask.at<float>(i,j);

					_meanMask.at<float>(i,j)  = _alpha_gaussian * currentPixelOfFrame
											+ (1- _alpha_gaussian) * _meanMask.at<float>(i,j) ;

					botIsTrue = true;
			}
			if(!botIsTrue){
				_bgsmask.at<uchar>(i,j) = 255;
			}
		}
	}
	_fgmask = _bgsmask;
	_meanMask.convertTo(_bkg,CV_8UC1);
	*/


}




