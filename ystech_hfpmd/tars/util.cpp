#include "../misc/common.h"

#include "util.h"

namespace tars
{

namespace util
{

long long parse_millis(cstr_t hhmmss)
{
	return atoi(hhmmss)*3600000LL
		+atoi(hhmmss+3)*60000LL+atoi(hhmmss+6)*1000LL + 16*3600000LL;
}

} // namespace util

} // namespace tars
