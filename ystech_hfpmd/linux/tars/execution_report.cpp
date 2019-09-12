#include "../misc/common.h"

#include "execution_report.h"

namespace tars
{

void execution_report::make(
	id_type id, cstr_t order_sys_id, cstr_t trade_id, cstr_t instrument,
	OrdType::Value ord_type, Side::Value side, int volume, double price,
	ExecType::Value exec_type, cstr_t text, long long trans_millis,
	TimeInForce::Value tif, OpenClose::Value openClose)
{
	report_.wrapForEncode(buf_, 0, ExecutionReport::sbeBlockLength());

	// local ID
	report_.clOrdID().highBits(id.first);
	report_.clOrdID().lowBits(id.second);

	// system ID
	if (order_sys_id == nullptr)
		report_.orderID(0, 0);
	else
		report_.putOrderID(order_sys_id);

	// trade ID
	if (trade_id == nullptr)
		report_.tradeID(0, 0);
	else
		report_.putTradeID(trade_id);

	// instrument
	if (instrument == nullptr)
		report_.securityID(0, 0);
	else
		report_.putSecurityID(instrument);

	// order info
	report_.ordType(ord_type);
	report_.side(side);
	report_.lastShares(volume);
	if (price < 10) {
		report_.lastPx()
    		.mantissa(int(price*10000))
    		.exponent(4);
	} else {
		report_.lastPx()
    		.mantissa(int(price*1000))
    		.exponent(3);
	}

	// type
	report_.ordStatus(exec_type);
	report_.execType(exec_type);
	report_.timeInForce(tif);
	report_.openClose(openClose);

	// Additional text/errorMsg
	if (text == nullptr)
		report_.text(0, 0);
	else
		report_.putText(text);

	// Timing
	report_.transactTime(trans_millis);
}

void execution_report::makeNew(
	id_type id, cstr_t order_sys_id, cstr_t instrument,
	OrdType::Value ord_type, Side::Value side, int volume, double price,
	TimeInForce::Value tif, OpenClose::Value openClose)
{
	make(id, order_sys_id, nullptr, instrument,
		ord_type, side, volume, price,
		ExecType::New, nullptr, -1, tif, openClose);
}

void execution_report::makeNew_ms(
	id_type id, cstr_t order_sys_id, cstr_t instrument,
	OrdType::Value ord_type, Side::Value side, int volume, double price,
	TimeInForce::Value tif, OpenClose::Value openClose,
	long long trans_millis)
{
	make(id, order_sys_id, nullptr, instrument,
		ord_type, side, volume, price,
		ExecType::New, nullptr, trans_millis, tif, openClose);
}

void execution_report::makePartiallyFilled(
	id_type id, cstr_t order_sys_id, cstr_t trade_id, cstr_t instrument,
	Side::Value side, int volume, double price,
	long long trans_millis,
	TimeInForce::Value tif, OpenClose::Value openClose)
{
	make(id, order_sys_id, trade_id, instrument,
		OrdType::NULL_VALUE, side, volume, price,
		ExecType::PartiallyFilled, nullptr, trans_millis, tif, openClose);
}

void execution_report::makeCancelled(
	id_type id, cstr_t order_sys_id, cstr_t instrument,
	cstr_t statusMsg, long long trans_millis,
	TimeInForce::Value tif, OpenClose::Value openClose)
{
	make(id, order_sys_id, nullptr, instrument,
		OrdType::NULL_VALUE, Side::NULL_VALUE, -1, -1,
		ExecType::Cancelled, statusMsg, trans_millis, tif, openClose);
}

void execution_report::makeRejected(
	id_type id, cstr_t instrument, cstr_t errorMsg,
	TimeInForce::Value tif, OpenClose::Value openClose)
{
	make(id, nullptr, nullptr, instrument,
		OrdType::NULL_VALUE, Side::NULL_VALUE, -1, -1,
		ExecType::Rejected, errorMsg, -1, tif, openClose);
}

void execution_report::makeCancelRejected(
	id_type id, cstr_t instrument, cstr_t errorMsg,
	TimeInForce::Value tif, OpenClose::Value openClose)
{
	make(id, nullptr, nullptr, instrument,
		OrdType::NULL_VALUE, Side::NULL_VALUE, -1, -1,
		ExecType::CancelRejected, errorMsg, -1, tif, openClose);
}

} // namespace tars
