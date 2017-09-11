#ifndef LOG_H_
#define LOG_H_

#include <string>

namespace hc {

class Log {
public:
	enum LogLevel {
		INFO = 1,
		WARNING = 2,
		ERROR = 3,
		NONE = 4
	};

private:
	std::string logfile_;

	LogLevel console_level_;
	LogLevel logfile_level_;

public:
	static void log(LogLevel level, std::string module, std::string message);

	static void set_logfile(std::string logfile) { get().logfile_ = logfile; }
	static void set_console_level(LogLevel level) { get().console_level_ = level; }
	static void set_logfile_level(LogLevel level) { get().logfile_level_ = level; }

public:
	Log();
	~Log();

	static Log& get()
	{
		static Log instance_ = Log();
		return instance_;
	}
};

} //namespace hc

std::ostream& operator<<(std::ostream& out, const hc::Log::LogLevel& level);


#endif