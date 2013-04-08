// Copyright (c) 2011-2013, Zeex
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#ifndef AMX_PROFILER_FUNCTION_INFO_H
#define AMX_PROFILER_FUNCTION_INFO_H

#include "duration.h"

namespace amx_profiler {

class Function;

// Various runtime information about a function.
class FunctionStatistics {
public:
	explicit FunctionStatistics(Function *fn);

	Function *function() { return fn_; }
	const Function *function() const { return fn_; }

	long num_calls() const { return num_calls_; }
	void AdjustNumCalls(long delta) { num_calls_ += delta; }

	const Nanoseconds total_time() const { return total_time_; }
	void AdjustTotalTime(Nanoseconds delta) { total_time_ += delta; }

	Nanoseconds child_time() const { return child_time_; }
	void AdjustChildTime(Nanoseconds delta) { child_time_ += delta; }

	Nanoseconds GetSelfTime() const { return total_time() - child_time(); }

	const Nanoseconds average_time() const { return average_time_; }
	void AddToAverageTime(Nanoseconds time) { average_time_ = (average_time_ + time) / (Nanoseconds)2; }

	const Nanoseconds worst_time() const { return worst_time_; }
	void UpdateWorstTime(Nanoseconds time) { worst_time_ = time > worst_time_ ? time : worst_time_; }

	const Nanoseconds best_time() const { return best_time_; }
	void UpdateBestTime(Nanoseconds time) { best_time_ = time < best_time_ ? time : best_time_; }

private:
	Function *fn_;

	long num_calls_;
	Nanoseconds total_time_;
	Nanoseconds child_time_;
	Nanoseconds average_time_;
	Nanoseconds worst_time_;
	Nanoseconds best_time_;
};

} // namespace amx_profiler

#endif // !AMX_PROFILER_FUNCTION_INFO_H
