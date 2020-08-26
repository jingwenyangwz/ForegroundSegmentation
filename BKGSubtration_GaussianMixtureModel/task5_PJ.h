/* Applied Video Sequence Analysis (AVSA)
*
*	LAB1.0: Background Subtraction - Unix version
*	fgesg.hpp
*
* 	Authors: José M. Martínez (josem.martinez@uam.es), Paula Moral (paula.moral@uam.es) & Juan Carlos San Miguel (juancarlos.sanmiguel@uam.es)
*	VPULab-UAM 2020
*/
#include <opencv2/opencv.hpp>
#include <numeric>
#include<vector>
#include <algorithm>

#ifndef FGSEG_H_INCLUDE
#define FGSEG_H_INCLUDE

using namespace cv;
using namespace std;

namespace fgseg {

	//Declaration of FGSeg class based on BackGround Subtraction (bgs)
	class bgs{
	public:

		//constructor with parameter "threshold"

		bgs(double threshold, double alpha, bool rgb, int k, double alpha_gaussian, float var, float weight_threshold);

		//destructor
		~bgs(void);

		//method to initialize bkg (first frame - hot start)
		void init_bkg(cv::Mat Frame);

		//method to perform BackGroundSubtraction
		void bkgSubtraction(cv::Mat Frame);
		//
		void updateBackground();

		//returns the BG image
		cv::Mat getBG(){ return _bkg; };

		//returns the DIFF image
		cv::Mat getDiff(){ return _diff; };

		//returns the BGS mask
		cv::Mat getBGSmask(){ return _bgsmask; };

		//returns the binary mask with detected shadows
		cv::Mat getShadowMask(){ return _shadowmask; };

		//returns the binary FG mask
		cv::Mat getFGmask(){ return _fgmask; };

		void removeShadows();

		void updateGaussian(int i, int j, int idx);

		void GMMupdateBackground(cv::Mat Frame);

		vector<int> order(float w1, float w2, float w3);
		vector<size_t> sort_indexes(const vector<float> &v);

		//ADD ADITIONAL METHODS HERE
		//...
	private:
		cv::Mat _bkg; //Background model
		cv::Mat	_frame; //current frame
		cv::Mat _diff; //abs diff frame
		cv::Mat _bgsmask; //binary image for bgssub (FG)
		cv::Mat _shadowmask; //binary mask for detected shadows
		cv::Mat _fgmask; //binary image for foreground (FG)

		bool _rgb;
		double _threshold;
		//ADD ADITIONAL VARIABLES HERE

		double _alpha;

		//vector<cv::Mat> _meanMask;
		cv::Mat _meanMask[5];
		cv::Mat _varMask[5];
		cv::Mat _weightMask[5];

		int _k;// the number of gaussian models
		double _alpha_gaussian;
		float _var;
		float _weight_threshold;

		//...

	};//end of class bgs

}//end of namespace

#endif




