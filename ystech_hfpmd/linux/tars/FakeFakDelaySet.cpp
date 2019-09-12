#include "FakeFakDelaySet.h"
#include <tars/util.h>

namespace 
{
	constexpr size_t NANO_PER_MICRO = 1000;
}

CFakeFakDelaySet::CFakeFakDelaySet()
	: cancel_executor_(20, 0, tars::util::micro_spin())
	, delay_nano_secs_(500 * NANO_PER_MICRO) // 500 micro secs
	, cancel_function_(nullptr){
}

void CFakeFakDelaySet::Init(size_t lDelayMicroSecs, CancelFunction cancelFunction) {
	delay_nano_secs_ = lDelayMicroSecs * NANO_PER_MICRO;
	cancel_function_ = cancelFunction;
}

void CFakeFakDelaySet::AddFakOrder(int iOrderRef) {
	std::lock_guard<std::mutex> lock(fak_faks_mtx_);
	fake_faks_.insert(iOrderRef);
}

void CFakeFakDelaySet::RspFakOrder(int iOrderRef, const char* szExchangeId, const char* szOrderSysId) {
	{
		std::lock_guard<std::mutex> lock(fak_faks_mtx_);
		if (0 == fake_faks_.erase(iOrderRef)) {
			return;
		}
	}
	if (cancel_function_) {
		cancel_executor_.schedule(delay_nano_secs_, cancel_function_,
			iOrderRef, std::string(szExchangeId), std::string(szOrderSysId));
	}
}
