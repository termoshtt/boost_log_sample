
#include "mod1/func.hpp"
#include <boost/log/utility/setup/file.hpp> // add_file_log

namespace logging = boost::log;
namespace keywords = logging::keywords;
namespace expr = boost::log::expressions;

int main(int argc, char const *argv[]) {
  mod1::init();

  logging::add_file_log(keywords::file_name = "client.log",
                        keywords::format =
                            (expr::stream << mod1::tag_attr << mod1::line_id
                                          << ": <" << mod1::severity << "> ["
                                          << mod1::scope << "] "
                                          << expr::smessage));

  mod1::func1();
  mod1::func2();
  return 0;
}
