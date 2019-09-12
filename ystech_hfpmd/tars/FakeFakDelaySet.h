////////////////////////////////////////////////////////////////////////////////
//@brief Fake FAK, receive order response, delay to cancel order for a while
// AddFakOrder() ==> [fake_fak_set] ==> 
// RspFakOrder() ==> [cancel_order_queue] ==>
// CancleExecute ==> [trader_api]
////////////////////////////////////////////////////////////////////////////////
#pragma once
#include <mutex>
#include <string>
#include <functional>
#include <unordered_set>
#include "../misc/nano_executor.h"
using namespace nano;
class CFakeFakDelaySet
{
public:
	CFakeFakDelaySet();

	using CancelFunction = std::function<void(int, std::string, std::string)>;
	void Init(size_t lDelayMicroSecs, CancelFunction cancelFunction);

	void AddFakOrder(int iOrderRef);
	void RspFakOrder(int iOrderRef, const char* szExchangeId, const char* szOrderSysId);

private:
	size_t delay_nano_secs_;
	CancelFunction cancel_function_;

	mutable std::mutex fak_faks_mtx_;
	std::unordered_set<int> fake_faks_;

	single_thread_executor<512, false> cancel_executor_;
};