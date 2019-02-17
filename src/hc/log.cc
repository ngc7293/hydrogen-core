#include "hc/log.hh"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

#include <allegro5/allegro.h>

namespace hc {

Log::Log()
	: logfile_("hc.log")
	, console_level_(INFO)
	, logfile_level_(INFO)
{
	std::ofstream ofs("hc.log", std::ofstream::out | std::ofstream::trunc);
	ofs.close();
}

Log::~Log()
{
}

void Log::log(LogLevel level, std::string module, std::string message)
{
	Log& log = Log::get();

	std::ofstream ofs(log.logfile_, std::ios::out | std::ios::app);
	double timestamp = al_get_time();

	if (level == NONE)
		level = INFO;

	if (level >= log.logfile_level_) {
		ofs << std::fixed << std::setprecision(3) << "[" << timestamp << "][" << level << "][" << module << "]: " << message << std::endl;
	}

	if (level >= log.console_level_) {
		if (level == ERROR)
			std::cout << "\033[31;1m";
		else if (level == WARNING)
			std::cout << "\033[33;1m";

		std::cout << std::fixed << std::setprecision(3) << "[" << timestamp << "][" << level << "][" << module << "]: " << message << std::endl;

		std::cout << "\033[0m";
	}

	ofs.close();
}

} //namespace hc

std::ostream& operator<<(std::ostream& out, const hc::Log::LogLevel& level)
{
	if (level == hc::Log::INFO)
		out << "INFO";
	else if (level == hc::Log::WARNING)
		out << "WARNING";
	else if (level == hc::Log::ERROR)
		out << "ERROR";

	return out;
}