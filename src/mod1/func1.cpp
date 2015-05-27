
#include "func.hpp"

namespace mod1 {

void func1(bool flag) {
  BOOST_LOG_FUNCTION(); // Scopeに関数名を登録する
  auto &lg = logger::get();
  if (flag) {
    BOOST_LOG_NAMED_SCOPE("true case"); // Scopeに入った事を記録する
    BOOST_LOG_SEV(lg, info) << "flag is true";
  } else {
    BOOST_LOG_NAMED_SCOPE("false case"); // Scopeに入った事を記録する
    BOOST_LOG_SEV(lg, info) << "flag is false";
  }
  BOOST_LOG_SEV(lg, error) << "Some error occurs!!";
}

} // namespace mod1
