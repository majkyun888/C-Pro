//#include <iostream>
//#include<string>
//#ifndef SAFE_DELETE
//#define SAFE_DELETE(p) {if(p){delete(p);(p)=NULL;}}
//#endif
//using namespace std;
//class ICar
//{
//public:
//	virtual string Name() = 0;
//};
//class BenzCar : public ICar
//{
//public:
//	string Name()
//	{
//		return "Benz Car";
//	}
//};
//class BmwCar :public ICar
//{
//public:
//	string Name()
//	{
//		return "BMW Car";
//	}
//};
//class AudiCar : public ICar
//{
//public:
//	string Name()
//	{
//		return "Audi Car";
//	}
//};
//class Factory	//简单工厂
//{
//public:
//	enum CAR_TYPE {
//		BENZ_CAR,
//		BMW_CAR,
//		AUDI_CAR
//	};
//	ICar * CreateCar(CAR_TYPE type)
//	{
//		ICar * pCar = NULL;
//		switch (type)
//		{
//			case CAR_TYPE::BENZ_CAR:
//				pCar = new BenzCar;
//				break;
//			case CAR_TYPE::BMW_CAR:
//				pCar = new  BmwCar;
//				break;
//			case CAR_TYPE::AUDI_CAR:
//				pCar = new AudiCar;
//				break;
//			default:
//				break;
//		}
//		return pCar;
//	}
//};
//int main()
//{
//	Factory *pFactory = new Factory;
//	ICar *pCar = pFactory->CreateCar(Factory::BENZ_CAR);
//	cout << pCar->Name() << endl;
//	SAFE_DELETE(pCar);
//
//	pCar = pFactory->CreateCar(Factory::BMW_CAR);
//	cout << pCar->Name() << endl;
//	SAFE_DELETE(pCar);
//
//	pCar = pFactory->CreateCar(Factory::AUDI_CAR);
//	cout << pCar->Name() << endl;
//	SAFE_DELETE(pCar);
//
//	getchar();
//
//
//	return 0;
//}
//
