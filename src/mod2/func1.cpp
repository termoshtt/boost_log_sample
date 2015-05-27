
#include "mod2.hpp"
#include <iostream>

namespace mod2 {

void func() {
  BOOST_LOG_FUNCTION()
  auto &lg = logger::get();

  std::cout << "mod2:func" << std::endl;
  BOOST_LOG_SEV(lg, severity_level::info) << "mod2::func";
}

} // namespace mod2
