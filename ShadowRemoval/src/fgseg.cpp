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

bgs::bgs(double tau, double alpha, double beta, bool selective_bkg_update, bool rgb, double sTh, double dTh){
	_rgb=rgb;

	_selective_bkg_update = selective_bkg_update;

	_alpha = alpha;
	_beta = beta;

	_sTh = sTh;
	_dTh = dTh;

	_threshold = tau;

}


//default destructor
bgs::~bgs(void)
{
}

//method to initialize bkg (first frame - hot start)
void bgs::init_bkg(cv::Mat Frame)
{

	if (!_rgb){
		_bkg = Mat::zeros(Size(Frame.cols,Frame.rows), CV_8UC1); // void function for Lab1.0 - returns zero matrix//ADD YOUR CODE HERE
		//...
		Frame.copyTo(_bkg);


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


		_frame = Mat::zeros(Size(Frame.cols,Frame.rows), CV_8UC1); // void function for Lab1.0 - returns zero matrix
		_diff = Mat::zeros(Size(Frame.cols,Frame.rows), CV_8UC1); // void function for Lab1.0 - returns zero matrix
		_bgsmask = Mat::zeros(Size(Frame.cols,Frame.rows), CV_8UC1); // void function for Lab1.0 - returns zero matrix

		//ADD YOUR CODE HERE
		//...

		Frame.copyTo(_frame);
		cv::Mat bkg_temp = Mat::zeros(Size(_bgsmask.cols,_bgsmask.rows), CV_8UC1);
		_bkg.copyTo(bkg_temp);
		cv::cvtColor(Frame, Frame, COLOR_BGR2GRAY);
		cv::cvtColor(bkg_temp, bkg_temp, COLOR_BGR2GRAY);
		_diff = abs(Frame - bkg_temp);

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
		_fgmask = Mat::zeros(Size(_bgsmask.cols,_bgsmask.rows), CV_8UC1); // void function for Lab1.0 - returns zero matrix

		//_bgsmask.copyTo(_fgmask);

			//ADD YOUR CODE HERE
			//...
		/*cv::Mat frame_temp = Mat::zeros(Size(_bgsmask.cols,_bgsmask.rows), CV_8UC1);
		cv::Mat bkg_temp = Mat::zeros(Size(_bgsmask.cols,_bgsmask.rows), CV_8UC1);

		_frame.copyTo(frame_temp);
		_bkg.copyTo(bkg_temp);

		cv::cvtColor(frame_temp, frame_temp, COLOR_BGR2HSV);
		cv::cvtColor(bkg_temp, bkg_temp, COLOR_BGR2HSV);

		cv::Mat sF[3];
		cv::Mat sI[3];
		split(frame_temp, sF);
		split(bkg_temp, sI);

		cv::Mat saturationDiff = Mat::zeros(Size(_bgsmask.cols,_bgsmask.rows), CV_8UC1);
		absdiff(sF[1],sI[1], saturationDiff);

		cv::Mat hueDiff = Mat::zeros(Size(_bgsmask.cols,_bgsmask.rows), CV_8UC1);
		cv::Mat hueDiff1 = Mat::zeros(Size(_bgsmask.cols,_bgsmask.rows), CV_8UC1);
		cv::Mat hueDiff2 = Mat::zeros(Size(_bgsmask.cols,_bgsmask.rows), CV_8UC1);

		absdiff(sF[0],sI[0], hueDiff1);
		absdiff(sF[0],sI[0], hueDiff2);
		hueDiff2 = 360 - hueDiff2;
		hueDiff = min(hueDiff1, hueDiff2);

		frame_temp.convertTo(frame_temp, CV_32FC1);
		bkg_temp.convertTo(bkg_temp, CV_32FC1);

		split(frame_temp, sF);
		split(bkg_temp, sI);

		cv::Mat vDiff = Mat::zeros(Size(_bgsmask.cols,_bgsmask.rows), CV_32FC1);
		cv::Mat vDiff1 = Mat::zeros(Size(_bgsmask.cols,_bgsmask.rows), CV_32FC1);
		cv::Mat vDiff2 = Mat::zeros(Size(_bgsmask.cols,_bgsmask.rows), CV_32FC1);

		divide(sF[2], sI[2], vDiff1);


		int tauS = 100;
		int tauH = 100;
		int alpha = 0.5;
		int beta = 0.9;
		bitwise_and(saturationDiff<=tauS, hueDiff<=tauH, saturationDiff);
		vDiff2 = vDiff1 >= alpha;
		vDiff1 = vDiff1 <= beta;
		bitwise_and(vDiff2, vDiff1, vDiff1);
		cv::Mat result = Mat::zeros(Size(_bgsmask.cols,_bgsmask.rows), CV_32FC1);
		bitwise_and(vDiff1, saturationDiff, result);
		result = result * 255;
		result.convertTo(_shadowmask, CV_8UC1);

		cv::Mat invShadow = Mat::zeros(Size(_bgsmask.cols,_bgsmask.rows), CV_32FC1);*/
		//invShadow = (255 - _shadowmask);
		/*std::cout << _bgsmask.cols << " " << _bgsmask.rows << std::endl;
		std::cout << invShadow.cols << " " << invShadow.rows << std::endl;
		std::cout << _fgmask.cols << " " << _fgmask.rows << std::endl;*/

		//cv::Mat filterMask = Mat::zeros(Size(_bgsmask.cols,_bgsmask.rows), CV_32FC1);
		//_bgsmask.copyTo(filterMask);
		//cv::Mat invShadow2 = invShadow.mul(filterMask);
		//cv::cvtColor(filterMask,_fgmask, COLOR_HSV2RGB);

		//bgsmask.at<uchar>(i,j)



		cv::Mat frame_temp = Mat::zeros(Size(_bgsmask.cols,_bgsmask.rows), CV_8UC1);
		cv::Mat bkg_temp = Mat::zeros(Size(_bgsmask.cols,_bgsmask.rows), CV_8UC1);
		_frame.copyTo(frame_temp);
		_bkg.copyTo(bkg_temp);

		cv::cvtColor(frame_temp, frame_temp, COLOR_BGR2HSV);
		cv::cvtColor(bkg_temp, bkg_temp, COLOR_BGR2HSV);


		for(int i = 0; i < frame_temp.rows; i ++){
			for(int j = 0; j < frame_temp.cols; j ++){
				Vec3b ipixel = frame_temp.at<Vec3b>(i,j);
				Vec3b bpixel = bkg_temp.at<Vec3b>(i,j);
				int diff1 = abs(ipixel.val[0] - bpixel.val[0]);
				int diff2 = 360 - abs(ipixel.val[0] - bpixel.val[0]);
				int Dh = min(diff1, diff2); //0.5, 0.9 70 volt mindketto
				if( _alpha < ((float) ipixel.val[2]/(float) bpixel.val[2])
						&& ((float) ipixel.val[2]/(float) bpixel.val[2]) < _beta
						&& abs((float) ipixel.val[1] - (float) bpixel.val[1]) < _sTh
						&& Dh < _dTh
						&& _bgsmask.at<uchar>(i,j) > 0
						){
					_shadowmask.at<uchar>(i,j) = 255;
				}
			}
		}
		_fgmask = _bgsmask - _shadowmask;

			//...
		}
		else{
			cout << "Colour currently not supported" << endl;
			exit(1);
		    }


}

//ADD ADDITIONAL FUNCTIONS HERE
void bgs::updateBackground(){
	if(_selective_bkg_update){

		cv::Mat background__logical_filter = (255-_bgsmask)/255;
		cv::Mat foreground_logical_filter = _bgsmask/255;

		cv::Mat temp2 = _frame.mul(background__logical_filter) * _alpha;

		cv::Mat temp3 = _bkg.mul(background__logical_filter) * (1-_alpha); //filter the whole image

		cv::Mat new_bkg = _bkg.mul(foreground_logical_filter);

		_bkg = temp3 + temp2 + new_bkg;


	}else{
		_bkg = _frame.mul(_alpha) + _bkg.mul((1-_alpha));

	}


	/*_counter = _bgsmask/255 + _counter;

	cv::Mat background__logical_filter = (255-_bgsmask)/255;

	cv::Mat tmp5 = _counter.mul(background__logical_filter);

	cv::Mat tmp6 = _counter;

	_counter = tmp6 - tmp5; // we do this becomes background

	cv::Mat mask_where_foregroundBecomesBackground = (_counter > _threshold_ghosts2)/255;

	_bkg = _frame.mul(mask_where_foregroundBecomesBackground) + _bkg.mul(1-mask_where_foregroundBecomesBackground);
	*/
}





