
#include <boost/shared_ptr.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/file.hpp> // add_file_log
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/attributes/attribute.hpp>

namespace logging = boost::log;
namespace keywords = logging::keywords;
namespace sinks = logging::sinks;
namespace src = logging::sources;
namespace attrs = logging::attributes;
namespace expr = boost::log::expressions;

enum severity_level {
  info,
  warning,
  error,
};

BOOST_LOG_INLINE_GLOBAL_LOGGER_DEFAULT(my_logger,
                                       src::severity_logger_mt<severity_level>);

BOOST_LOG_ATTRIBUTE_KEYWORD(scope, "Scope", attrs::named_scope::value_type)
BOOST_LOG_ATTRIBUTE_KEYWORD(severity, "Severity", severity_level)

int main(int argc, char const *argv[]) {
  logging::add_common_attributes();
  boost::shared_ptr<logging::core> core = logging::core::get();
  core->add_global_attribute("Scope", attrs::named_scope());
  core->add_global_attribute("Severity", attrs::named_scope());

  logging::add_file_log(keywords::file_name = "logger.log",
                        keywords::format = (expr::stream << scope << severity));

  auto &lg = my_logger::get();
  BOOST_LOG_NAMED_SCOPE("madoka");
  {
    BOOST_LOG_NAMED_SCOPE("mami");
    BOOST_LOG_SEV(lg, info) << "mami mami";
  }
  {
    BOOST_LOG_NAMED_SCOPE("hom");
    BOOST_LOG_SEV(lg, error) << "hom hom";
  }

  return 0;
}
