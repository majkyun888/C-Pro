//#include <iostream>
//#include<string>
//using namespace std;
//class ICar
//{
//public:
//	virtual string Name() = 0;
//};
//class AFactory
//{
//public:
//	virtual ICar * CreateCar() = 0;
//};
//class BenzCar : public ICar
//{
//public:
//	string Name()
//	{
//		return "Benz Car";
//	}
//};
//class BmwCar : public ICar
//{
//public:
//	string Name()
//	{
//		return "Bmw Car";
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
//class BenzFactory : public AFactory
//{
//public:
//	ICar * CreateCar()
//	{
//		return new BenzCar;
//	}
//};
//class BmwFactory : public AFactory
//{
//public:
//	ICar * CreateCar()
//	{
//		return new BmwCar;
//	}
//};
//class AudiFactory : public AFactory
//{
//public:
//	ICar * CreateCar()
//	{
//		return new AudiCar;
//	}
//};
//int main()
//{
//	{
//		AFactory * pFactory = new BenzFactory;
//		ICar * pCar = pFactory->CreateCar();
//		cout << pCar->Name() << endl;
//		delete pFactory;
//		delete pCar;
//	}
//	{
//		AFactory * pFactory = new BmwFactory;
//		ICar * pCar = pFactory->CreateCar();
//		cout << pCar->Name() << endl;
//		delete pFactory;
//		delete pCar;
//	}
//	{
//		AFactory * pFactory = new AudiFactory;
//		ICar * pCar = pFactory->CreateCar();
//		cout << pCar->Name() << endl;
//		delete pFactory;
//		delete pCar;
//	}
//	getchar();
//	return 0;
//}

