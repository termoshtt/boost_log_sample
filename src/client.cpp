
#include "mod1/func.hpp"
#include "mod2/mod2.hpp"
#include <boost/log/utility/setup/file.hpp> // add_file_log

namespace logging = boost::log;
namespace keywords = logging::keywords;
namespace expr = boost::log::expressions;

void init() {
  mod1::init();
  mod2::init();

  logging::add_common_attributes();
  logging::add_file_log(
      keywords::file_name = "client.log", // logを出力するファイル名
      keywords::format =
          "%Tag%: [%TimeStamp%] [%Scope%] %Message%" // logのフォーマット
      );
  logging::add_file_log(keywords::file_name = "stream_format.log",
                        keywords::format =
                            (expr::stream << mod1::tag_attr << mod1::line_id
                                          << ": <" << mod1::severity << "> ["
                                          << mod1::scope << "] "
                                          << expr::smessage));
}

int main(int argc, char const *argv[]) {
  init();
  mod1::func1(true);
  mod1::func1(false);
  mod2::func();
  // mod1::func2();
  return 0;
}
