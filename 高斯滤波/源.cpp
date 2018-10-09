#include <iostream>
//#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;


int main()
{
	return 0;
}



Mat TransBufferToMat(unsigned char* pBuffer, int nWidth, int nHeight, int nBandNum, int nBPB = 1)
{
	Mat mDst;
	if (nBandNum == 4)
	{
		if (nBPB == 1)
		{
			mDst = Mat::zeros(cv::Size(nWidth, nHeight), CV_8UC4);
		}
		else if (nBPB == 2)
		{
			mDst = Mat::zeros(cv::Size(nWidth, nHeight), CV_16UC4);
		}
	}
	else if (nBandNum == 3)
	{
		if (nBPB == 1)
		{
			mDst = Mat::zeros(cv::Size(nWidth, nHeight), CV_8UC3);
		}
		else if (nBPB == 2)
		{
			mDst = Mat::zeros(cv::Size(nWidth, nHeight), CV_16UC3);
		}
	}
	else if (nBandNum == 1)
	{
		if (nBPB == 1)
		{
			mDst = Mat::zeros(cv::Size(nWidth, nHeight), CV_8UC1);
		}
		else if (nBPB == 2)
		{
			mDst = Mat::zeros(cv::Size(nWidth, nHeight), CV_16UC1);
		}
	}

	for (int j = 0; j < nHeight; ++j)
	{
		unsigned char* data = mDst.ptr<unsigned char>(j);
		unsigned char* pSubBuffer = pBuffer + (nHeight - 1 - j) * nWidth? * nBandNum * nBPB;
		memcpy(data, pSubBuffer, nWidth * nBandNum * nBPB);
	}
	if (nBandNum == 1)
	{
		cvtColor(mDst, mDst, CV_GRAY2BGR);
	}
	else if (nBandNum == 3)
	{
		cvtColor(mDst, mDst, CV_RGB2BGR);
	}
	else if (nBandNum == 4)
	{
		cvtColor(mDst, mDst, CV_RGBA2BGR);
	}

	return mDst;
}

int TransMatToBuffer(Mat mSrc, unsigned char** ppBuffer, int& nWidth, int& nHeight, int& nBandNum, int& nBPB)
{
	if (*ppBuffer)
	{
		delete[] * ppBuffer;
		*ppBuffer = nullptr;
	}

	nWidth = mSrc.cols;
	//nWidth = ((nWidth + 3) / 4) * 4;
	nHeight = mSrc.rows;
	nBandNum = mSrc.channels();
	nBPB = (mSrc.depth() >> 1) + 1;

	size_t nMemSize = nWidth * nHeight * nBandNum * nBPB;
	//这样可以改变外部*pBuffer的值
	*ppBuffer = new uchar[nMemSize];
	memset(*ppBuffer, 0, nMemSize);
	uchar* pT = *ppBuffer;
	for (int j = 0; j < nHeight; ++j)
	{
		unsigned char* data = mSrc.ptr<unsigned char>(j);
		unsigned char* pSubBuffer = *ppBuffer + (j)* nWidth? * nBandNum * nBPB;
		memcpy(pSubBuffer, data, nWidth * nBandNum * nBPB);
	}

	return 0;
}
