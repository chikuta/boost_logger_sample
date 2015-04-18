#include "logger.h"

#include <fstream>
#include <boost/log/trivial.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/utility/setup/from_stream.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>


using namespace utility::logging;


bool utility::logging::init( const std::string& file_name )
{
	Logger::Ptr logger = Logger::GetInstance();
	bool ret = false;

	try {
		ret = logger->Init(file_name);
	} catch ( std::exception& e ) {
		std::cerr << "[logging] " << e.what() << std::endl;	
	}

	return ret;
}


void utility::logging::shutdown()
{
	Logger::Ptr logger = Logger::GetInstance();
	if (logger->IsInit()) {
		boost::log::core::get()->flush();
		boost::log::core::get()->remove_all_sinks();
	}
}


Logger::Ptr Logger::instance_ = NULL;


Logger::Logger()
	: init_(false)
	, lg_channel_()
{}


Logger::~Logger()
{}


Logger::Ptr Logger::GetInstance()
{
	if ( instance_ == NULL ) {
		instance_ = Ptr(new Logger());
	}
	return instance_;
}


boost::log::sources::severity_channel_logger_mt<severity_level, std::string>& Logger::GetLogger()
{ 
	return lg_channel_;
}


bool Logger::IsInit() const
{
	return init_;
}


bool Logger::Init(const std::string& logfile)
{
	// 初期化
	if (IsInit()) {
		std::cerr << "Logger is aleady initialized." << std::endl;
		return true;
	}

	std::fstream file(logfile.c_str());

	if ( file.is_open() ) {
		// ファイルが存在するならば、初期化を行う
		boost::log::register_simple_formatter_factory<severity_level, char>("Severity");
		boost::log::register_simple_filter_factory<severity_level, char>("Severity");

		boost::log::register_simple_formatter_factory<std::string, char>("Channel");
		boost::log::register_simple_filter_factory<std::string, char>("Channel");

		boost::log::register_simple_formatter_factory<std::string, char>("Target");
		boost::log::register_simple_filter_factory<std::string, char>("Target");

		boost::log::init_from_stream(file);
		boost::log::add_common_attributes();

	} else {
		// 例外を出力する
		std::cerr << "Logger create failed." << std::endl;
		return false;
	}

	init_ = true;
	return true;
}

