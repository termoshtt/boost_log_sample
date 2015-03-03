
#include "func.hpp"

namespace mod1 {

void func1() {
  BOOST_LOG_FUNCTION();
  auto &lg = logger::get();
  BOOST_LOG_SEV(lg, info) << "info in mod1::func1";
}

} // namespace mod1
