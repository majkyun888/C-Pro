#include <iostream>
#include "CameraRef.h"
#include <queue>
#include <stack>
#include <vector>
Camera_struct bbc{ 0 };

vector<int> partitionLabels(string S) {
	vector<int> vec;
	for (int i = 0; i < S.size(); i++)
	{
		int flag = S.find_last_of(S[i]) - i;
		int maxNum = 0;
		for (int j = i + 1; j < flag; j++)
		{
			maxNum = max(flag, S.find_last_of(S[j]));
		}
		vec.push_back(maxNum - i + 1);
	}
	return vec;
}
int main()
{
	
	/*CcameraRefHikVision::GlobalInitialize();
	CcameraRefHikVision HikCamera(&bbc);

	HikCamera.initialize(0);
	HikCamera.SetStartGrabbing();
	
	for (int i = 0; i < 600; i++)
	{
		Sleep(100);
		HikCamera.RetrieveResult(1000);
		HikCamera.Image_Format();
	}
		
	HikCamera.closeCamera();
	CcameraRefHikVision::GlobalTerminate();*/

	string str = "ababcbacadefegdehijhklij";
	partitionLabels(str);
	
	system("pause");
	return 0;
}