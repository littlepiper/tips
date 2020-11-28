//fitting of a polynomial
bool polynomialCurveFit(vector<Point> & training, int n, Mat& A)
{
	int N = training.size();
	Mat x = Mat::zeros(n + 1, n + 1, CV_64FC1);
	for (int i = 0; i < n + 1; i++)
		for (int j = 0; j < n + 1; j++)
			for (int k = 0; k < N; k++)
				x.at<double>(i, j) = x.at<double>(i, j) + pow(training[k].x, i + j);
	Mat Y = Mat::zeros(n + 1, 1, CV_64FC1);
	for (int i = 0; i < n + 1; i++)
		for (int k = 0; k < N; k++)
			Y.at<double>(i, 0) = Y.at<double>(i, 0) + pow(training[k].x, i)*training[k].y;
	A = Mat::zeros(n + 1, 1, CV_64FC1);
	solve(x, Y, A, DECOMP_LU);
	return true;
}

//Straightness calculation
void LineFitLeastSquares(vector<Point> pt, vector<float> &vResult)
{
	float A = 0.0;
	float B = 0.0;
	float C = 0.0;
	float D = 0.0;
	float E = 0.0;
	float F = 0.0;
	for (int i = 0; i < pt.size(); i++)
	{
		A += pt[i].x * pt[i].x;
		B += pt[i].x;
		C += pt[i].x * pt[i].y;
		D += pt[i].y;
	}
	//Calculate the slope a and intercept b
	float a, b, temp = 0;
	if (temp = (pt.size()*A - B*B))// denominator is not zero
	{
		a = (pt.size()*C - B*D) / temp;
		b = (A*D - B*C) / temp;
	}
	else
	{
		a = 1;
		b = 0;
	}
	// Calculated correlation coefficient r
	float Xmean, Ymean;
	Xmean = B / pt.size();
	Ymean = D / pt.size();
	float tempSumXX = 0.0, tempSumYY = 0.0;
	for (int i = 0; i<pt.size(); i++)
	{
		tempSumXX += (pt[i].x - Xmean) * (pt[i].x - Xmean);
		tempSumYY += (pt[i].y - Ymean) * (pt[i].y - Ymean);
		E += (pt[i].x - Xmean) * (pt[i].y - Ymean);
	}
	F = sqrt(tempSumXX) * sqrt(tempSumYY);
	float r;
	r = E / F;
	vResult.push_back(a);
	vResult.push_back(b);
	vResult.push_back(r*r);
}

Mat eulerAnglesToRotationMatrix(Mat theta)
{
	Mat R_x = (Mat_<float>(3, 3) <<
		1, 0, 0,
		0, cos(theta.at<float>(0, 0)), -sin(theta.at<float>(0, 0)),
		0, sin(theta.at<float>(0, 0)), cos(theta.at<float>(0, 0))
		);
	Mat R_y = (Mat_<float>(3, 3) <<
		cos(theta.at<float>(0, 1)), 0, sin(theta.at<float>(0, 1)),
		0, 1, 0,
		-sin(theta.at<float>(0, 1)), 0, cos(theta.at<float>(0, 1))
		);
	Mat R_z = (Mat_<float>(3, 3) <<
		cos(theta.at<float>(0, 2)), -sin(theta.at<float>(0, 2)), 0,
		sin(theta.at<float>(0, 2)), cos(theta.at<float>(0, 2)), 0,
		0, 0, 1);
	Mat R = R_z * R_y * R_x;
	Mat Rdata(1, 9, CV_32FC1);
	for (int i = 0; i < 3; i++)
	{
		Rdata.at<float>(0, i) = R.at<float>(0, i);
		Rdata.at<float>(0, i + 3) = R.at<float>(1, i);
		Rdata.at<float>(0, i + 6) = R.at<float>(2, i);
	}
	return Rdata;
}

Vec3f rotationMatrixToEulerAngles(Mat &R)
{
	float sy = sqrt(R.at<float>(0, 0) * R.at<float>(0, 0) + R.at<float>(1, 0) * R.at<float>(1, 0));
	bool singular = sy < 1e-6;
	float x, y, z;
	if (!singular)
	{
		x = atan2(R.at<float>(2, 1), R.at<float>(2, 2));
		y = atan2(-R.at<float>(2, 0), sy);
		z = atan2(R.at<float>(1, 0), R.at<float>(0, 0));
	}
	else
	{
		x = atan2(-R.at<float>(1, 2), R.at<float>(1, 1));
		y = atan2(-R.at<float>(2, 0), sy);
		z = 0;
	}
	x = x*180.0f / 3.141592653589793f;
	y = y*180.0f / 3.141592653589793f;
	z = z*180.0f / 3.141592653589793f;
	return Vec3f(x, y, z);
}

*.yuv files convert rgb Mat
Mat yuv2rgb(const char *_FileName, Size size = Size(1920,1080))
{
  FILE *fp = fopen(_FileName, "rb");
	if (NULL == fp)	return Mat();
	uchar *yuvdata = new uchar[size.height * 3 / 2 * size.width * sizeof(uchar)];
	fread(yuvdata, size.height * 3 / 2, size.width, fp);
	Mat yuvimg(size.height * 3 / 2, size.width, CV_8UC1, yuvdata);
	Mat rgbimg(size.height, size.width, CV_8UC3);
	cvtColor(yuvimg, rgbimg, CV_YUV2RGB_NV21);
	fclose(fp);
	delete[] yuvdata;
	return rgbimg;
}

region grow
void RegionGrow(Mat srcImage, Mat &growImage, Point pt)
{
	Point pToGrowing;
	int pGrowValue = 0;
	int pGrowlight = 0;
	int DIR[8][2] = { { -1,-1 },{ 0,-1 },{ 1,-1 },{ 1,0 },{ 1,1 },{ 0,1 },{ -1,1 },{ -1,0 } };
	vector<Point> growPtVector; 
	growPtVector.push_back(pt); 
	while (!growPtVector.empty())
	{
		pt = growPtVector.back();
		growPtVector.pop_back();
		for (int i = 0; i < 9; ++i)
		{
			pToGrowing.x = pt.x + DIR[i][0];
			pToGrowing.y = pt.y + DIR[i][1];
			if (pToGrowing.x < 0 || pToGrowing.y < 0 ||
				pToGrowing.x >(srcImage.cols - 1) || (pToGrowing.y > srcImage.rows - 1))
				continue;
			pGrowValue = growImage.at<uchar>(pToGrowing.y, pToGrowing.x);
			pGrowlight = srcImage.at<uchar>(pToGrowing.y, pToGrowing.x);
			if (pGrowValue == 0 && pGrowlight >= 240)
			{
				growImage.at<uchar>(pToGrowing.y, pToGrowing.x) = 255;
				growPtVector.push_back(pToGrowing);
			}
		}
	}
}

