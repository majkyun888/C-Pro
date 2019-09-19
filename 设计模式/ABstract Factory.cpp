//#include <iostream>
//#include<string>
//using namespace std;
//enum SOAPTYPE {SFJ,XSL,NAS};
//enum TOOTHTYPE {HR,ZH};
//class SoapBase
//{
//public:
//	virtual ~SoapBase() {};
//	virtual void show() = 0;
//};
//class SFJSoap : public SoapBase
//{
//public:
//	void show() { cout << "SFJ Soap!" << endl; }
//};
//class NASSoap : public SoapBase
//{
//public:
//	void show() { cout << "NAS Soap!" << endl; }
//};
//class ToothBase
//{
//public:
//	virtual ~ToothBase() {};
//	virtual void show() = 0;
//};
//class HRTooth : public ToothBase
//{
//public:
//	void show() { cout << "Hei ren Toothpaste!" << endl; }
//};
//class ChinaTooth : public ToothBase
//{
//public:
//	void show() { cout << "China Toothpaste!" << endl; }
//};
//class FactoryBase
//{
//public:
//	virtual SoapBase * creatSoap() = 0;
//	virtual ToothBase * createToothpaste() = 0;
//};
//class FactoryA : public FactoryBase
//{
//public:
//	SoapBase * creatSoap()
//	{
//		return new SFJSoap;
//	}
//	ToothBase * createToothpaste() { return new HRTooth; }
//};
//class FactoryB : public FactoryBase
//{
//public:
//	SoapBase * creatSoap(){return new NASSoap;}
//	ToothBase * createToothpaste() { return new ChinaTooth; }
//};
//int main()
//{
//	FactoryA factory1;
//	SoapBase * pSoap1 = factory1.creatSoap();
//	ToothBase * tooth1 = factory1.createToothpaste();
//	pSoap1->show();
//	tooth1->show();
//    
//
//
//
//
//
//	getchar();
//	return 0;
//}

