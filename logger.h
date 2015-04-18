#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <string>
#include <memory>

#include <boost/log/core.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/severity_channel_logger.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/expressions/keyword.hpp>


#define LOG_CHANNEL(lvl, channel) \
	BOOST_LOG_CHANNEL_SEV(::utility::logging::Logger::GetInstance()->GetLogger(), channel, ::utility::logging::lvl)

#define LOG(lvl) \
	BOOST_LOG_CHANNEL_SEV(::utility::logging::Logger::GetInstance()->GetLogger(), "general", ::utility::logging::lvl)


namespace utility {
namespace logging {

enum severity_level 
{
	trace,
	debug,
	info,
	warn,
	error,
	fatal,
};


BOOST_LOG_ATTRIBUTE_KEYWORD(severity, "Severity", severity_level);
BOOST_LOG_ATTRIBUTE_KEYWORD(channel, "Channel", std::string);

static const char* severity_level_str[] = 
{
	"trace",
	"debug",
	"info",
	"warn",
	"error",
	"fatal"
};


template<typename CharT, typename TraitsT>
inline std::basic_ostream< CharT, TraitsT>& operator<< (std::basic_ostream<CharT, TraitsT>& strm, severity_level lvl) {
	if (static_cast<std::size_t>(lvl) < (sizeof(severity_level_str) / sizeof(*severity_level_str))) {
		strm << severity_level_str[lvl];
	} else {
		strm << static_cast<int>(lvl);
	}
	return strm;
}


template<typename CharT, typename TraitsT>
inline std::basic_istream<CharT, TraitsT>& operator>> (std::basic_istream<CharT, TraitsT>& strm, severity_level& lvl) {
	if (strm.good()) {
		std::string level_str; 
		strm >> level_str; 
		for (unsigned int i = 0; i < sizeof(severity_level_str) / sizeof(*severity_level_str); ++i) { 
			if (level_str == severity_level_str[i]) { 
				lvl = static_cast<severity_level>(i); 
				break;
			} 
		}
	}
	return strm;
}


class Logger
{
public:
	typedef std::shared_ptr<Logger> Ptr;

	/**
	 * @brief デストラクタ
	 */
	virtual ~Logger();

	/**
	 * @brief インスタンスを取得する
	 * @return Loggerインスタンスのポインタ
	 */
	static Ptr GetInstance();


	/**
	 * @brief Loggerクラスを初期化する
	 * 
	 * @param logfile 設定ファイルが記述されているファイル
	 * @retval true 初期化成功
	 * @retval false 初期化失敗
	 */
	bool Init(const std::string& logfile);


	/**
	 * @brief 初期化を確認する
	 * @retval true 初期化完了
	 * @retval true 未初期化
	 */
	bool IsInit() const;


	/**
	 * @brief Loggerを取得する
	 * @return Logger
	 */
	boost::log::sources::severity_channel_logger_mt<severity_level, std::string>& GetLogger();


private:
	/**
	 * @brief コンストラクタ
	 */
	Logger();

	bool init_;
	static Ptr instance_;

	boost::log::sources::severity_channel_logger_mt<severity_level, std::string> lg_channel_;
};


/**
 * @brief init logging
 * 
 * @param file_name file name
 */
bool init( const std::string& file_name );


/**
 * @brief Exit Logging
 */
void shutdown();



}; // end logging
}; // end utils

#endif //_LOGGER_H_
