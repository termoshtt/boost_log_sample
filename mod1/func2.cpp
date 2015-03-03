
#include "func.hpp"

namespace mod1 {

void func2() {
  BOOST_LOG_FUNCTION();
  auto &lg = logger::get();
  BOOST_LOG_SEV(lg, warning) << "info in mod1::func2";
}

} // namespace mod1
