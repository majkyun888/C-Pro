////////////////////////////////////////////////////////////////////////////////
//@brief the set of Fake FAK Order's OrderRef
////////////////////////////////////////////////////////////////////////////////
#pragma once
#include <mutex>
#include <unordered_set>

class CFakeFakSet
{
public:
	CFakeFakSet();
	~CFakeFakSet();
	void AddFakOrder(int iOrderRef);
	// @return: true = succeed to del orderRef
	bool DelFakOrder(int iOrderRef);

private:
	mutable std::mutex m_mtx;
	std::unordered_set<int> m_fake_faks;
};

