#pragma once

#include <cc_py_tars_fix/ExecutionReport.hpp>

namespace tars
{

using namespace cc_py_tars_fix;
using misc::cstr_t;

class execution_report
{
public:
	typedef std::pair<int64_t, int64_t> id_type;

	void makeNew(
		id_type id, cstr_t order_sys_id, cstr_t instrument,
		OrdType::Value ord_type, Side::Value side, int volume, double price,
		TimeInForce::Value tif = TimeInForce::NULL_VALUE, OpenClose::Value openClose = OpenClose::NULL_VALUE);

	void makeNew_ms(
		id_type id, cstr_t order_sys_id, cstr_t instrument,
		OrdType::Value ord_type, Side::Value side, int volume, double price,
		TimeInForce::Value tif = TimeInForce::NULL_VALUE, OpenClose::Value openClose = OpenClose::NULL_VALUE, 
		long long trans_millis = -1);

	void makePartiallyFilled(
		id_type id, cstr_t order_sys_id, cstr_t trade_id, cstr_t instrument,
		Side::Value side, int volume, double price,
		long long trans_millis,
		TimeInForce::Value tif = TimeInForce::NULL_VALUE, OpenClose::Value openClose = OpenClose::NULL_VALUE);

	void makeCancelled(
		id_type id, cstr_t order_sys_id, cstr_t instrument,
		cstr_t errorMsg, long long trans_millis,
		TimeInForce::Value tif = TimeInForce::NULL_VALUE, OpenClose::Value openClose = OpenClose::NULL_VALUE);

	void makeRejected(
		id_type id, cstr_t instrument, cstr_t errorMsg,
		TimeInForce::Value tif = TimeInForce::NULL_VALUE, OpenClose::Value openClose = OpenClose::NULL_VALUE);

	void makeCancelRejected(
		id_type id, cstr_t instrument, cstr_t errorMsg,
		TimeInForce::Value tif = TimeInForce::NULL_VALUE, OpenClose::Value openClose = OpenClose::NULL_VALUE);

	long long addr() const
	{
		return (long long)buf_;
	}

	int type() const
	{
		return ExecutionReport::sbeTemplateId();
	}

private:
	void make(
		id_type id, cstr_t order_sys_id, cstr_t trade_id, cstr_t instrument,
		OrdType::Value ord_type, Side::Value side, int volumn, double price,
		ExecType::Value exec_type, cstr_t text, long long trans_millis,
		TimeInForce::Value tif, OpenClose::Value openClose);

	ExecutionReport report_;
	char buf_[4096];

}; // class execution_report

} // namespace tars
