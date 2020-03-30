
void Calculate_diff_XYZ(Mat& I, Mat& test)
{
	CV_Assert(I.depth() == CV_8U);
	I.copyTo(test);
	const int channels = I.channels();
	cout << I.isContinuous() << endl;
	Mat_<Vec3b> _I = I;
	Mat_<Vec3b> _T = test;
	float dif_xz1 = 0, dif_yz1 = 0;
	for (int i = 0; i < I.rows; i++)
	{
		for (int j = 0; j < I.cols; j++)
		{
			float dxz = 0, dyz = 0; //
			unsigned char R = _I(i, j)[2];
			unsigned char G = _I(i, j)[1];
			unsigned char B = _I(i, j)[0];
			long int X = 199049 * B + 394494 * G + 455033 * R + 524288;
			long int Y = 75675 * B + 749900 * G + 223002 * R + 524288;
			long int Z = 915161 * B + 114795 * G + 18621 * R + 524288;

			X = X >> 20;
			Y = Y >> 20;
			Z = Z >> 20;
			dxz = X - Z < 5 ? 0 : X - Z;
			dyz = Y - Z < 5 ? 0 : Y - Z;

			if (dyz > 5.0000 && dyz < 10.00000)
				dyz *= 0.001;
			else if (dxz > 10.0000)
			{
				dxz *= 3.0;
				_T(i, j)[0] = 0;
				_T(i, j)[1] = 0;
				_T(i, j)[2] = 255;
			}
			else
			{
				dxz *= 0.001;
				_T(i, j)[0] = 0;
				_T(i, j)[1] = 0;
				_T(i, j)[2] = 0;
			}

			dif_xz1 = dif_xz1 + dxz;
			dif_yz1 = dif_yz1 + dyz;
		}
	}
	int t = dif_xz1 * 0.1;
	return;
}