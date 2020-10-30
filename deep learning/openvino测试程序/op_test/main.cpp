#include <iostream>
#include <string>
#include <queue>
#include <boost/filesystem.hpp>
#include <inference_engine.hpp>
#include <ie_version.hpp>
#include <ie_core.hpp>
#include <inference_engine.hpp>
#include <memory>
#include <opencv2/core/hal/interface.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <Windows.h>
#include <set>
#include <inference_engine.hpp> 
#include <chrono>  
#include "classification_results.h"
using namespace std;
using namespace cv;
using namespace InferenceEngine;
using namespace chrono;
#define ClassificationResult_t ClassificationResultW
#define tcout std::wcout
string label[] = {
	"invert",
	"miss",
	"normal"
};

struct InferRequestInfo {
	struct InferRequestImage {
		cv::Mat mat;
		unsigned correctClass;
		std::chrono::time_point<std::chrono::steady_clock> startTime;
	};
	InferRequest& inferRequest;
	std::vector<InferRequestImage> images;
};

struct InferRequestT {
	cv::Mat mat;
	unsigned correctClass;
	string label2;
	float maxP;
};

vector<string> GetFilePaths(string rootPath)
{
	namespace fs = boost::filesystem;
	string strFilePath;
	vector<string> m_childFilePathList;
	fs::path path(rootPath);
	if (!fs::exists(path))
		return m_childFilePathList;
	fs::directory_iterator end_iter; // 尾指针
	fs::directory_iterator iter(path);
	while (iter != end_iter)
	{
		if (fs::is_directory(iter->status()))
		{
			m_childFilePathList.emplace_back(iter->path().string());
		}
		iter++;
	}
	return m_childFilePathList;
}

vector<cv::Mat> readImgFromPath(string _imgpath)
{
	//读取文件夹中的图片；
	vector<cv::Mat> m_picList;
	namespace fs = boost::filesystem;
	if (!fs::exists(_imgpath) && !fs::is_directory(_imgpath))
	{
		throw std::runtime_error("ERROR: Cannot find input directory");
	}
	fs::path inputFilePath(_imgpath);
	fs::recursive_directory_iterator iter(inputFilePath);
	fs::recursive_directory_iterator end_iter;

	while (iter != end_iter)
	{
		if (fs::is_regular_file(*iter) && iter->path().extension() == ".png")
		{
			string filePath = iter->path().string();
			cv::Mat  im = cv::imread(filePath, IMREAD_GRAYSCALE);
			if (im.rows)
			{
				m_picList.push_back(im);
			}
		}
		iter++;
	}
	return m_picList;
}

InferenceEngine::Blob::Ptr ReshapeMat2Blob(cv::Mat& mat)
{
	std::chrono::system_clock::time_point start;
	std::chrono::system_clock::time_point end;
	mat /= 255.0;
	cvtColor(mat, mat, COLOR_GRAY2RGB);
	start = system_clock::now();
	//cv::resize(mat, mat, Size(224, 224), 0, 0, INTER_NEAREST);
	end = system_clock::now();
	std::chrono::microseconds duration = duration_cast<microseconds>(end - start);
	double p = double(duration.count()) * microseconds::period::num / microseconds::period::den;
	cout << "reshape time:" << p * 1000 << "ms" << endl;
	
	size_t channels = mat.channels();
	size_t height = mat.size().height;
	size_t width = mat.size().width;

	size_t strideH = mat.step.buf[0];
	size_t strideW = mat.step.buf[1];

	bool is_dense = strideW == channels &&	strideH == channels * width;

	if (!is_dense) THROW_IE_EXCEPTION
		<< "Doesn't support conversion from not dense cv::Mat";
	
	////C
	//InferenceEngine::Layout a = InferenceEngine::TensorDesc::getLayoutByDims({ 1 });
	////NC
	//InferenceEngine::Layout b = InferenceEngine::TensorDesc::getLayoutByDims({ 1, 2 });
	////NCHW
	//InferenceEngine::Layout c = InferenceEngine::TensorDesc::getLayoutByDims({ 1, 2, 3, 4 });
	////CHW
	//InferenceEngine::Layout d = InferenceEngine::TensorDesc::getLayoutByDims({ 1, 2, 3 });
	////NCDHW
	//InferenceEngine::Layout e = InferenceEngine::TensorDesc::getLayoutByDims({ 1, 2, 3, 4, 5 });
	// Layout N-BatchSize, H-Height, W-Width, C-Channel
	// Layout NCHW ---- channel first
	
	InferenceEngine::TensorDesc tDesc(InferenceEngine::Precision::U8,
		{ 1, channels, height, width },
		InferenceEngine::Layout::NHWC);
	/*return InferenceEngine::make_shared_blob<unsigned char, InferenceEngine::SizeVector>(InferenceEngine::Precision:U8, input_data->getDims());*/

	return InferenceEngine::make_shared_blob<uint8_t>(tDesc, mat.data);
}

void CreateMyWindow(string window_name = "test")
{
	//--------------Create Window------------------------------
	cv::namedWindow(window_name, cv::WINDOW_NORMAL);
	cv::resizeWindow(window_name, 700, 500);
	cv::moveWindow(window_name, 0, 0);
}

void procOnce()
{

}

void putTextForSequence(Mat& image, float *text, double time, Point p = Point(10, 20), int font_face = cv::FONT_HERSHEY_COMPLEX, double font_scale = 0.5, int thicness = 1, Scalar color = cv::Scalar(0, 255, 0))
{
	cvtColor(image, image, COLOR_GRAY2BGR);
	string t1 = label[0] + ":" + to_string(text[0]);
	cv::putText(image, t1, p, font_face, font_scale, color, thicness, 4, 0);
	string t2 = label[1] + ":" + to_string(text[1]);
	cv::putText(image, t2, Point(10, 40), font_face, font_scale, color, thicness, 4, 0);
	string t3 = label[2] + ":" + to_string(text[2]);
	cv::putText(image, t3, Point(10, 60), font_face, font_scale, color, thicness, 4, 0);
	string t4 = "spend:" + to_string(time * 1000) + "ms";
	cv::putText(image, t4, Point(10, 80), font_face, font_scale, Scalar(0, 0, 255), thicness, 4, 0);
	return;
}

void getMyDeviceInfo(Core& ie)
{
	vector<std::string> availableDevices = ie.GetAvailableDevices();
	cout << "Device List:" << endl;
	for (auto i : availableDevices)
		cout << i << endl;
	// 网格化信息
	std::string cpuDeviceName = ie.GetMetric("CPU", METRIC_KEY(FULL_DEVICE_NAME)).as<std::string>();
	cout << cpuDeviceName << endl;
}

/*
	step1: create Core 
	step2: LoadNetWork Structure ---> need (.xml and .bin)
	step3: set input_Layout and output_Layout ---> Info and name
	step4: Instance NetWork
	step5: CreateInfer Request--------> Infer
*/
class NetWorkOpt
{
private:
	Core ie;														//推断引擎
	CNNNetwork network;												//网络结构
	ExecutableNetwork executableNetwork;							//执行者
	//InferenceEngine::InputInfo::Ptr input_info;
	InferenceEngine::InputInfo::Ptr input_data;
	InferenceEngine::DataPtr output_data;
	//InferenceEngine::DataPtr output_info;
	InferenceEngine::InferRequest infer_request;
	string input_name;
	string output_name;
	string net;
	string weight;
	Blob::Ptr imgBlob;
	Blob::Ptr output;
	int batchSize;
public:
	NetWorkOpt(string xml_path= "C:\\Program Files (x86)\\Intel\\openvino_2021.1.110\\deployment_tools\\model_optimizer\\log\\saved_model.xml", string weight_path = "C:\\Program Files (x86)\\Intel\\openvino_2021.1.110\\deployment_tools\\model_optimizer\\log\\saved_model.bin", int batchSize=1): net(xml_path), weight(weight)
	{
		getMyDeviceInfo(ie);
		network = ie.ReadNetwork(net, weight);
		network.setBatchSize(1);
	}
	
	void NetWorkConfigure()
	{
		auto inputShapes = network.getInputShapes();
		//cout << inputShapes.size() << endl;
		//auto inputMap = network.getInputsInfo();
		////------------------configure input layer ---------------------------
		//input_info = network.getInputsInfo().begin()->second;
		//input_name = network.getInputsInfo().begin()->first;
		//input_info->getPreProcess().setResizeAlgorithm(RESIZE_AREA);
		////input_info->getPreProcess().setColorFormat(ColorFormat::RGB);
		//input_info->setLayout(Layout::NHWC);
		//input_info->setPrecision(Precision::U8);

		InferenceEngine::InputsDataMap inputInfo = network.getInputsInfo();
		for (auto& item : inputInfo)
		{
			input_name = item.first;
			input_data = item.second;
			input_data->setLayout(Layout::NHWC);
			input_data->setPrecision(Precision::U8);
			input_data->getPreProcess().setResizeAlgorithm(RESIZE_AREA);
		}

		InferenceEngine::OutputsDataMap outputInfo = network.getOutputsInfo();
		for (auto& item : outputInfo)
		{
			output_name = item.first;
			output_data = item.second;
			output_data->setLayout(Layout::NC);
			output_data->setPrecision(Precision::FP32);
		}
		//-----------------configure output layer-----------------------------
		//output_info = network.getOutputsInfo().begin()->second;
		//output_name = network.getOutputsInfo().begin()->first;
		//auto layerDataDims = output_info->getTensorDesc().getDims();
		////cout << layerDataDims.size() << endl;
		////验证是否和类别一致 labelSize + 1
		////cout << layerDataDims[1] << endl;
		//output_info->setPrecision(Precision::FP32);
	}

	void deviceSet()
	{
		// 使用所有内核
		ie.SetConfig({ { CONFIG_KEY(CPU_THREADS_NUM), std::to_string(0) } }, "CPU");
		// 并行流处理内核的数量
		ie.SetConfig({ { CONFIG_KEY(CPU_THROUGHPUT_STREAMS), (CONFIG_VALUE(CPU_THROUGHPUT_AUTO)) } }, "CPU");

		//ie.SetConfig({ { CONFIG_KEY(CPU_THROUGHPUT_STREAMS), (4) } }, "CPU");
		//std::set<std::string> devices;
		//for (std::string& device : deviceList)
		//	devices.insert(device);
		//if (*devices.begin() == "CPU")
		//{
		//	// cpu设置优化？
		//	//// 设置多线程
		//	//ie.SetConfig({ { CONFIG_KEY(CPU_THREADS_NUM), std::to_string(4) } }, "CPU");
		//	//// for CPU execution, more throughput-oriented execution via streams
		//	//ie.SetConfig({ { CONFIG_KEY(CPU_THROUGHPUT_STREAMS),
		//	//				(deviceNstreams.count(device) > 0 ? std::to_string(deviceNstreams.at(device))
		//	//												  : CONFIG_VALUE(CPU_THROUGHPUT_AUTO)) } }, device);
		//	//deviceNstreams[device] = std::stoi(
		//	//	ie.GetConfig(device, CONFIG_KEY(CPU_THROUGHPUT_STREAMS)).as<std::string>());
		//}
	}

	void InitNetWork()
	{
		NetWorkConfigure();
		std::map<std::string, std::string> config = { { PluginConfigParams::KEY_PERF_COUNT, PluginConfigParams::YES } };
		// 启用计数器
		executableNetwork = ie.LoadNetwork(network, "CPU", config);
		auto ncores = executableNetwork.GetConfig(PluginConfigParams::KEY_CPU_THREADS_NUM).as<std::string>();
		cout << "KEY_CPU_THREADS_NUM: " << ncores << endl;
		auto nireq = executableNetwork.GetMetric(METRIC_KEY(OPTIMAL_NUMBER_OF_INFER_REQUESTS)).as<unsigned int>();
		cout << "Infer num" << nireq << endl;
		deviceSet();
		infer_request = executableNetwork.CreateInferRequest();
	}

	float* Infer(Mat image)
	{
		string text;
		std::chrono::system_clock::time_point start;
		std::chrono::system_clock::time_point end;
		Mat in = image.clone();
		imgBlob = ReshapeMat2Blob(in);
		//output = infer_request.GetBlob(output_name);
		start = system_clock::now();
		//  infer_request.GetBlob(input_name);
		//  Accept any size image, Input resize will be invoked automatically using resize algorithm


		//  可以eager模式 or fill before
		infer_request.SetBlob(input_name, imgBlob);
		// 前向Forward propagation
		// 同步请求
		infer_request.Infer();

		// 异步处理
		/*infer_request.StartAsync();
		infer_request.Wait(IInferRequest::WaitMode::RESULT_READY);*/
		// 获得结果, SetBlob 和 GetBlob不能够颠倒使用
		Blob::Ptr output = infer_request.GetBlob(output_name);


		end = system_clock::now();
		std::chrono::microseconds duration = duration_cast<microseconds>(end - start);
		double p = double(duration.count()) * microseconds::period::num / microseconds::period::den;
		cout << "Infer time:" << p * 1000 << "ms" << endl;

		float* logits = output->buffer().as<InferenceEngine::PrecisionTrait<InferenceEngine::Precision::FP32>::value_type*>();
		/*int maxIdx = 0;
		float maxP = 0;
		float sum = 1.0;
		for (int i = 0; i < 3; i++)
		{
			if (logits[i] > maxP)
			{
				maxP = logits[i];
				maxIdx = i;
			}
		}*/
		
		return logits;
	}
};

int main()
{

	//auto inputInfoItem = *inputInfo.begin();
	//inputInfoItem.second->setPrecision(Precision::U8);
	//inputInfoItem.second->setLayout(Layout::NCHW);
	//std::vector<std::shared_ptr<unsigned char>> imagesData = {};
	//std::vector<std::string> validImageNames = {};
	//for (const auto& i : imageNames) {
	//	FormatReader::ReaderPtr reader(i.c_str());
	//	if (reader.get() == nullptr) {
	//		slog::warn << "Image " + i + " cannot be read!" << slog::endl;
	//		continue;
	//	}
	//	/** Store image data **/
	//	std::shared_ptr<unsigned char> data(
	//		reader->getData(inputInfoItem.second->getTensorDesc().getDims()[3],
	//			inputInfoItem.second->getTensorDesc().getDims()[2]));
	//	if (data != nullptr) {
	//		imagesData.push_back(data);
	//		validImageNames.push_back(i);
	//	}
	//}
	//if (imagesData.empty()) throw std::logic_error("Valid input images were not found!");
	//network.setBatchSize(imagesData.size());
	//size_t batchSize = network.getBatchSize();
	//slog::info << "Batch size is " << std::to_string(batchSize) << slog::endl;
	//cout << "InferenceEngine: " << GetInferenceEngineVersion() << endl;
	vector<InferRequestT> result;
	string sequencePath;
	cout << "Enter path:" << endl;
	cin >> sequencePath;
	CreateMyWindow();
	NetWorkOpt opt;
	opt.InitNetWork();
	std::chrono::system_clock::time_point start;
	std::chrono::system_clock::time_point end;
	vector<double> ptime;
	// -----------------------------Read InputImages---------------------------------------------------------
	//vector<std::string> imageNames{ GetFilePaths("G:\\tmp\\shuizhen\\test\\dataSource\\dataSource_normal") };
	vector<std::string> imageNames{ GetFilePaths(sequencePath) };
	std::vector<cv::Mat> inputImages;
	if (imageNames.empty())
		throw std::runtime_error("No images provided");
	std::sort(imageNames.begin(), imageNames.end());
	for (int j = 0; j < imageNames.size(); j++)
	{
		inputImages = readImgFromPath(imageNames[j]);
		for (int i = 0; i < 4; i++)
			inputImages.pop_back();
		
		for (int i = 0; i < inputImages.size(); i++)
		{
			start = system_clock::now();
			float* logit = opt.Infer(inputImages[i]);
			end = system_clock::now();
			std::chrono::microseconds duration = duration_cast<microseconds>(end - start);
			double p = double(duration.count()) * microseconds::period::num / microseconds::period::den;
			ptime.push_back(double(duration.count()) * microseconds::period::num / microseconds::period::den);

			putTextForSequence(inputImages[i], logit, p);
			cv::imshow("test", inputImages[i]);
			cv::waitKey(50);
		}
		// 序列结束判断结果
		float sum = 0;
		for (auto i : ptime)
			sum += i;
		cout<<"average spend time:" << (sum / ptime.size()) * 1000<<"ms" << endl;
		ptime.clear();
		waitKey(100);
		char key = cv::waitKey(0);
		if (key == 'j' || key == 'J')
			continue;
		else if (key == 'k' || key == 'K')
			return 0;
		else
			return 0;
	}

	system("pause");
	return 0;
}