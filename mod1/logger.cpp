
#include "func.hpp"

namespace logging = boost::log;
namespace attrs = logging::attributes;

namespace mod1 {

void init() {
  auto &lg = logger::get();
  lg.add_attribute("Tag", attrs::constant<std::string>("mod1"));
  lg.add_attribute("Scope", attrs::named_scope());
}

std::ostream &operator<<(std::ostream &ost, severity_level l) {
  static const char *names[] = {
    "debug", "info", "warning", "error", "critical"
  };
  ost << names[l];
  return ost;
}

} // namespace mod1
