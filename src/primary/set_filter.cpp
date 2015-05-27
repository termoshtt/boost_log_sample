
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/file.hpp> // add_file_log
#include <boost/log/utility/setup/common_attributes.hpp>

namespace logging = boost::log;
namespace keywords = logging::keywords;
namespace sinks = logging::sinks;

void init() {
  logging::add_file_log(keywords::file_name = "sample_%N.log",
                        keywords::format = "[%TimeStamp%]: %Message%");
  logging::core::get()->set_filter(logging::trivial::severity >=
                                   logging::trivial::info);
  logging::add_common_attributes(); // keywords::format (%TimeStamp%) の初期化
}

int main(int argc, char const *argv[]) {

  init();

  BOOST_LOG_TRIVIAL(trace) << "A trace severity message";
  BOOST_LOG_TRIVIAL(debug) << "A debug severity message";
  BOOST_LOG_TRIVIAL(info) << "An informational severity message";
  BOOST_LOG_TRIVIAL(warning) << "A warning severity message";
  BOOST_LOG_TRIVIAL(error) << "An error severity message";
  BOOST_LOG_TRIVIAL(fatal) << "A fatal severity message";

  return 0;
}
