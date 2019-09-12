/*
 * mthelp.h - multithread helper interface
 *
 * Authors: Jia Wang
 */

#ifndef MTHELP_H
#define MTHELP_H

#include "common.h"

namespace os
{

// affinity with processor
bool thread_set_affinity(int processor);

} // namespace os

#endif // MTHELP_H
