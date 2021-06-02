//===----------------------------------------------------------------------===//
//                         DuckDB
//
// duckdb/common/profiler.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "duckdb/common/helper.hpp"

#include <chrono>

typedef std::chrono::milliseconds ms_type;

namespace duckdb {

//! The profiler can be used to measure elapsed time
class Profiler {
public:
	//! Starts the timer
	void Start() {
		finished = false;
		start = Tick();
	}
	//! Finishes timing
	void End() {
		end = Tick();
		finished = true;
	}

	//! Returns the elapsed time in seconds. If End() has been called, returns
	//! the total elapsed time. Otherwise returns how far along the timer is
	//! right now.
	double Elapsed() const {
		auto _end = finished ? end : Tick();
		return std::chrono::duration_cast<ms_type>(_end - start).count();
	}

private:
	std::chrono::time_point<std::chrono::system_clock, ms_type> Tick() const {
		return std::chrono::time_point_cast<ms_type>(std::chrono::system_clock::now());
	}
	std::chrono::time_point<std::chrono::system_clock, ms_type> start;
	std::chrono::time_point<std::chrono::system_clock, ms_type> end;
	bool finished = false;
};
} // namespace duckdb
