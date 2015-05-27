
#include "logger.hpp"

namespace mod2 {

void init() {
  auto &lg = logger::get();
  lg.add_attribute("Tag", log::attrs::make_constant("mod2"));
  lg.add_attribute("Scope", log::attrs::named_scope());
}

} // namespace mod1
