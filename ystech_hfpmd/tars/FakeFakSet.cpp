#include "FakeFakSet.h"

CFakeFakSet::CFakeFakSet()
{}

CFakeFakSet::~CFakeFakSet()
{}

void CFakeFakSet::AddFakOrder(int iOrderRef)
{
	std::lock_guard<std::mutex> lock(m_mtx);
	m_fake_faks.insert(iOrderRef);
}

bool CFakeFakSet::DelFakOrder(int iOrderRef)
{
	std::lock_guard<std::mutex> lock(m_mtx);
	return ( 0 < m_fake_faks.erase(iOrderRef));
}
