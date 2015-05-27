Sample codes for Boost.Log
===========================

Tutorials
---------

- [概略](doc/info.md)
- [cmakeによるコンパイル](doc/cmake.md)
- [For librarians](doc/logger.md)
- [For users](doc/user.md)

Build sample codes
-------------------

```shell
mkdir build
cd build
cmake ../src/
make
```

状況設定
--------

ログを取るためにライブラリを使用する必要があるのは、複数のモジュールがあるときに
特定のモジュールからのログを選り分けたりする必要がある場合です。
最小の状況設定として`src/`にあるように、以下の場合を考えます。

```
CMakeLists.txt
mod1/CMakeLists.txt
     func.hpp
     func1.cpp
     func2.cpp
     logger.cpp
client.cpp
```

あるモジュール`mod1`を`client.cpp`から使う場合を考えます。
また全てcmakeを使用してコンパイルします。

尚文章中に出現する略したnamespaceは以下の通りです：

```cpp
namespace logging = boost::log;
namespace keywords = logging::keywords;
namespace sinks = logging::sinks;
namespace src = logging::sources;
namespace attrs = logging::attributes;
namespace expr = logging::expressions;
```
