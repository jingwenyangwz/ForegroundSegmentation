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
#include <numeric>
#include<vector>
#include <algorithm>
#include <functional>
#include <iostream> //system libraries

using namespace fgseg;
using namespace std; //to avoid using'std' to declare std functions and variables (e.g., std::out -> out)

bgs::bgs(double threshold, double alpha, bool rgb, int k, double alpha_gaussian, float var, float weight_threshold){

	_rgb = rgb;
	_threshold = threshold;
	_alpha = alpha;

	_k = k;
	_var = var;
	_alpha_gaussian = alpha_gaussian;
	_weight_threshold = weight_threshold;

}

//default destructor
bgs::~bgs(void)
{
}

//method to initialize bkg (first frame - hot start)
void bgs::init_bkg(cv::Mat Frame)
{

	if (!_rgb){

		/*if(Frame.channels() == 3 ){
		cv::cvtColor(Frame, Frame, COLOR_BGR2GRAY);
		}*/
		//cv::cvtColor(Frame, Frame, COLOR_BGR2GRAY);
		_bkg = Mat::zeros(Size(Frame.cols, Frame.rows), CV_8UC1); // void function for Lab1.0 - returns zero matrix//ADD YOUR CODE HERE
		//...
		Frame.copyTo(_bkg);
		cv::cvtColor(Frame, Frame, COLOR_BGR2GRAY);

		//create a vector of matrixes for mean
		//create a vector of matrixes for var
		//create a vector of matrixes for weight
		//int size[3] = { Frame.cols,Frame.rows, _k };
		//vector<cv::Mat> _meanMask(size[2]);
		//vector<cv::Mat> _varMask(size[2]);
		//vector<cv::Mat> _weightMask(size[2]);

		//initialize the first gaussian with the mean and variance value of the first frame
		cv::Mat cov;
		cv::Mat men;
		cv::meanStdDev(Frame, cov, men);

		float szam1 = (float)cov.at<double>(0, 0);
		float szam2 = (float)men.at<double>(0, 0);

		_meanMask[0] = Mat::ones(Size(Frame.cols, Frame.rows), CV_32FC1);
		_meanMask[0] = _meanMask[0].mul(szam2);

		_varMask[0] = Mat::ones(Size(Frame.cols, Frame.rows), CV_32FC1);
		_varMask[0] = _varMask[0].mul(szam1);

		//initialize the second gaussian distribution, assigning random values to mean/var, 0 weights
		_meanMask[1] = Mat::zeros(Size(Frame.cols, Frame.rows), CV_32FC1);
		_varMask[1] = Mat::zeros(Size(Frame.cols, Frame.rows), CV_32FC1);
		randu(_meanMask[1], 0, 255);
		randu(_varMask[1], 25, 50);

		//initialize the third gaussian distribution,assigning random values to mean/var, 0 weights
		_meanMask[2] = Mat::zeros(Size(Frame.cols, Frame.rows), CV_32FC1);
		_varMask[2] = Mat::zeros(Size(Frame.cols, Frame.rows), CV_32FC1);
		randu(_meanMask[2], 0, 255);
		randu(_varMask[2], 25, 50);

		//create a 3D matrix for weight
		//int size[3] = { Frame.cols,Frame.rows, _k };
		//cv::Mat _weight(3, size, CV_32FC1, cv::Scalar(0));

		// assign 100% weights to the first gaussian, zeros for the other two gaussians
		_weightMask[0] = Mat::ones(Size(Frame.cols, Frame.rows), CV_32FC1);
		_weightMask[1] = Mat::zeros(Size(Frame.cols, Frame.rows), CV_32FC1);
		_weightMask[2] = Mat::zeros(Size(Frame.cols, Frame.rows), CV_32FC1);

		//cout << _meanMask[2] <<endl;

	}
	else{
		cout << "Colour currently not supported" << endl;
		exit(1);
	}

}

//method to perform BackGroundSubtraction
void bgs::bkgSubtraction(cv::Mat Frame)
{

	if (!_rgb)
	{
		_frame = Mat::zeros(Size(Frame.cols, Frame.rows), CV_8UC1); // void function for Lab1.0 - returns zero matrix
		_diff = Mat::zeros(Size(Frame.cols, Frame.rows), CV_8UC1); // void function for Lab1.0 - returns zero matrix
		_bgsmask = Mat::zeros(Size(Frame.cols, Frame.rows), CV_8UC1); // void function for Lab1.0 - returns zero matrix

		//ADD YOUR CODE HERE
		//...
		Frame.copyTo(_frame);
		cv::Mat bkg_temp = Mat::zeros(Size(_bgsmask.cols, _bgsmask.rows), CV_8UC1);
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

//ADD ADDITIONAL FUNCTIONS HERE
void bgs::updateBackground(){
}

/*
vector<int> sort_indexes(const vector<float> &v)
{

vector<int> idx(v.size());
iota(idx.begin(),idx.end(),0);

stable_sort(idx.begin(),idx.end(),
[&v](int i1,int i2){return v[i1] < v[i2];});

return idx;
}
*/

void bgs::removeShadows()
{
	if (!_rgb){
		_shadowmask = Mat::zeros(Size(_bgsmask.cols, _bgsmask.rows), CV_8UC1); // void function for Lab1.0 - returns zero matrix

	}
	else{
		cout << "Colour currently not supported" << endl;
		exit(1);
	}


}

vector<int> bgs::order(float w1, float w2, float w3)
{

	vector<int> idx(3);

	if (w1 >= w2)
	{
		if (w3 >= w1)
		{
			idx = { 2, 0, 1 };//3 1 2

		}
		else if (w3 >= w2)
		{
			idx = { 0, 2, 1 };//1 3 2

		}
		else
		{
			idx = { 0, 1, 2 };//1 2 3

		}
	}

	else{

		if (w3 >= w2)
		{
			idx = { 2, 1, 0 };//3 2 1

		}
		else if (w3 >= w1)
		{
			idx = { 1, 2, 0 };//2 3 1

		}
		else
		{
			idx = { 1, 0, 2 };//2 1 3
		}


	}


	return idx;
}


vector<size_t> bgs::sort_indexes(const vector<float> &v) {

	// initialize original index locations
	vector<size_t> idx(v.size());
	iota(idx.begin(), idx.end(), 0);

	// sort indexes based on comparing values in v
	// using std::stable_sort instead of std::sort
	// to avoid unnecessary index re-orderings
	// when v contains elements of equal values
	stable_sort(idx.begin(), idx.end(),
		[&v](size_t i1, size_t i2) {return v[i1] < v[i2]; });

	return idx;

}

void bgs::updateGaussian(int row, int col, int idx)
{

	_varMask[idx].at<float>(row, col) = sqrt(_alpha_gaussian
		* (_frame.at<uchar>(row, col) - _meanMask[idx].at<float>(row, col))
		* (_frame.at<uchar>(row, col) - _meanMask[idx].at<float>(row, col))
		+ (1 - _alpha_gaussian) * _varMask[idx].at<float>(row, col));

	_meanMask[idx].at<float>(row, col) = _alpha_gaussian * _frame.at<uchar>(row, col)
		+ (1 - _alpha_gaussian) * _meanMask[idx].at<float>(row, col);

	for (int i = 0; i <_k; i++)
	{
		if (i != idx)
		{
			_weightMask[i].at<float>(row, col) = (1 - _alpha_gaussian) * _weightMask[i].at<float>(row, col);

		}
		else
		{
			_weightMask[idx].at<float>(row, col) = (1 - _alpha_gaussian) * _weightMask[idx].at<float>(row, col)
				+ _alpha_gaussian;

		}
	}

	//normalize the weights
	float sum = 0;

	for (int i = 0; i <_k; i++)
	{
		sum = _weightMask[i].at<float>(row, col);
	}

	for (int i = 0; i <_k; i++)
	{
		_weightMask[i].at<float>(row, col) = _weightMask[i].at<float>(row, col) / sum;
	}

}



void bgs::GMMupdateBackground(cv::Mat Frame){
	cv::cvtColor(Frame, Frame, COLOR_BGR2GRAY);

	//cout << typeid(_weightMask[0]).name() << endl;

	for (int j = 0; j < Frame.cols; j++){
		for (int i = 0; i < Frame.rows; i++){

			//_frame.at<uchar>(i, j)
			//cout<< _weightMask[0].at<float>(i,j) << endl;
			//cout<< typeid(_weightMask[0].at<float>(i,j)).name() << endl;

			// how wide is the range of variance do we want to  compare
			int tau = 1;

			vector<float> v = { _weightMask[0].at<float>(i, j), _weightMask[1].at<float>(i, j), _weightMask[2].at<float>(i, j) };
			vector<size_t> idx(3);
			idx = sort_indexes(v);

			//vector<int> idx = order(_weightMask[0].at<float>(i, j), _weightMask[1].at<float>(i, j), _weightMask[2].at<float>(i, j));


			int place = 0;

			if ((_meanMask[0].at<float>(i, j) - _varMask[0].at<float>(i, j)*tau) < _frame.at<uchar>(i, j)
				&& _meanMask[0].at<float>(i, j) + _varMask[0].at<float>(i, j)*tau > _frame.at<uchar>(i, j))
			{

				//_bgsmask.at<uchar>(i,j) = 0;
				place = 1;
				updateGaussian(i, j, 0);

			}
			else if ((_meanMask[1].at<float>(i, j) - _varMask[1].at<float>(i, j)*tau) < _frame.at<uchar>(i, j)
				&& _meanMask[1].at<float>(i, j) + _varMask[1].at<float>(i, j)*tau > _frame.at<uchar>(i, j))
			{

				//_bgsmask.at<uchar>(i,j) = 0;
				place = 2;
				updateGaussian(i, j, 1);

			}
			else if ((_meanMask[2].at<float>(i, j) - _varMask[2].at<float>(i, j)*tau) <  _frame.at<uchar>(i, j)
				&& _meanMask[2].at<float>(i, j) + _varMask[2].at<float>(i, j)*tau >  _frame.at<uchar>(i, j))
			{

				//_bgsmask.at<uchar>(i,j) = 0;
				place = 3;
				updateGaussian(i, j, 2);

			}

			else
			{

				_meanMask[idx[0]] = _frame.at<uchar>(i, j);

				_weightMask[idx[2]].at<float>(i, j) = (1 - _alpha_gaussian) * _weightMask[idx[2]].at<float>(i, j);

				_weightMask[idx[1]].at<float>(i, j) = (1 - _alpha_gaussian) * _weightMask[idx[1]].at<float>(i, j);

				_weightMask[idx[0]].at<float>(i, j) = _alpha_gaussian;

				float sum = _weightMask[idx[0]].at<float>(i, j)
					+ _weightMask[idx[1]].at<float>(i, j)
					+ _weightMask[idx[2]].at<float>(i, j);

				_weightMask[idx[0]].at<float>(i, j) = _weightMask[idx[0]].at<float>(i, j) / sum;
				_weightMask[idx[1]].at<float>(i, j) = _weightMask[idx[1]].at<float>(i, j) / sum;
				_weightMask[idx[2]].at<float>(i, j) = _weightMask[idx[2]].at<float>(i, j) / sum;

			}

			if (_weightMask[idx[2]].at<float>(i, j) > _weight_threshold)
				// only one gaussian is used
			{
				// if the pixel is in any of the gaussian
				if (place == 3 && place == 2 && place == 1)
				{
					_bgsmask.at<uchar>(i, j) = 0;// we set it as background pixel
					_bkg.at<uchar>(i, j) = _meanMask[idx[2]].at<float>(i, j);//we update the background with the mean of the selected gaussian
				}
			}

			else if (_weightMask[idx[2]].at<float>(i, j) + _weightMask[idx[1]].at<float>(i, j) > _weight_threshold)
				// two gausssian with higher weights are used
			{
				if (place == 3 && place == 2 && place == 1)
				{
					_bgsmask.at<uchar>(i, j) = 0;// we set it as background pixel
					_bkg.at<uchar>(i, j) = _meanMask[idx[2]].at<float>(i, j)*_weightMask[idx[2]].at<float>(i, j)
						+ _meanMask[idx[1]].at<float>(i, j)*_weightMask[idx[1]].at<float>(i, j);//we update the background with the mean of the selected two gaussian according to the weights
				}
			}
			else
				// three gaussians are used
			{
				if (place == 3 && place == 2 && place == 1)
				{
					_bgsmask.at<uchar>(i, j) = 0;// we set it as background pixel
					_bkg.at<uchar>(i, j) = _meanMask[idx[2]].at<float>(i, j)*_weightMask[idx[2]].at<float>(i, j)
						+ _meanMask[idx[1]].at<float>(i, j)*_weightMask[idx[1]].at<float>(i, j)
						+ _meanMask[idx[0]].at<float>(i, j)*_weightMask[idx[0]].at<float>(i, j);//we update the background with the mean of the selected two gaussian according to the weights
				}

			}


		}//end of for loop
	}//end of for loop

	_fgmask = _bgsmask;
	//_meanMask.convertTo(_bkg,CV_8UC1);

}

