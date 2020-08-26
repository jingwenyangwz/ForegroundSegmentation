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

bgs::bgs(double threshold, double alpha, bool selective_bkg_update, double threshold_ghosts2,bool rgb){

	_rgb=rgb;

	_threshold=threshold;

	_selective_bkg_update = selective_bkg_update;

	_alpha = alpha;

	_threshold_ghosts2 = threshold_ghosts2;
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

		_counter = Mat::zeros(Size(Frame.cols,Frame.rows), CV_8UC1);
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
		cv::Mat bkg_temp = Mat::zeros(Size(_bgsmask.cols,_bgsmask.rows), CV_8UC1);
		_diff = abs(Frame - _bkg);

		_bgsmask = _diff > _threshold;

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
		_fgmask = _bgsmask;
		}
		else{
			cout << "Colour currently not supported" << endl;
			exit(1);
		}


}

//ADD ADDITIONAL FUNCTIONS HERE
void bgs::updateBackground(){
	if(_selective_bkg_update){
		//MATRIX OPERATIONS
		/*cv::Mat background__logical_filter = (255-_bgsmask)/255;

		cv::Mat foreground_logical_filter = _bgsmask/255;

		cv::Mat temp2 = _frame.mul(background__logical_filter) * _alpha;

		cv::Mat temp3 = _bkg.mul(background__logical_filter) * (1-_alpha);

		cv::Mat new_bkg = _bkg.mul(foreground_logical_filter);

		_bkg = temp3 + temp2 + new_bkg;*/

		//PIXEL-WISE OPERATIONS
		cv::Mat new_bkg = Mat::zeros(Size(_frame.cols,_frame.rows), CV_32FC1);
		for(int i = 0; i < _bkg.rows; i++){
			for(int j = 0; j < _bkg.cols; j++){
				if(_bgsmask.at<uchar>(i,i) == 0){
					_bkg.at<uchar>(i,j) = _frame.at<uchar>(i,j) *_alpha + _bkg.at<uchar>(i,j) * (1-_alpha);
				}
			}
		}


	}else{
		_bkg = _frame *_alpha + _bkg * (1-_alpha);

		_counter = _bgsmask/255 + _counter; //We add one to the counter where _bkgmask is not zero, aka there is foreground (the filter is defined the opposite way)

		cv::Mat background__logical_filter = (255-_bgsmask)/255; //This filter is the real background filter, which is zero a foreground, and one in background

		cv::Mat tmp5 = _counter.mul(background__logical_filter); //we calculate where the counter is not zero, but the pixel is background now

		cv::Mat tmp6 = _counter; //the actual counter 

		_counter = tmp6 - tmp5; // We substract from the whole counter where it is background, which results, the pixels detected background will turn to zero, only the pixels that are foregrond will remain nonzero

		cv::Mat mask_where_foregroundBecomesBackground = (_counter > _threshold_ghosts2)/255; //Finnaly we check if a pixel is not a foreground for too long, whcih means its counter value is too high

		_bkg = _frame.mul(mask_where_foregroundBecomesBackground) + _bkg.mul(1-mask_where_foregroundBecomesBackground);
		// if a pixel is foreground for too long, we change it to background, and update the background
	}


}



