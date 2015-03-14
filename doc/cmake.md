compile with cmake
------------------

Boost.Logのコンパイルは面倒との意見が散見されます。
公式はbjamを使用するようにと書いていますが、
ここではcmakeを使用したコンパイルについて説明します。

```cmake
cmake_minimum_required(VERSION 2.8)
find_package(Boost COMPONENTS thread system log log_setup REQUIRED)
find_package(Threads)
include_directories(${Boost_INCLUDE_DIRS})
```

Boost.Logは1.54よりBoost本体に含まれていますので、
上述のようにcmakeで簡単に検出できます。
Boost.Threadをリンクする必要があるので、Threadsも同時にfindしておきます。

注意としてはBoost.Logのコンパイル時に動的リンクを行うためのフラグを指定する必要がある点です：

```cmake
add_definitions("-DBOOST_LOG_DYN_LINK")
```

これであとは`target_link_libraries`で`${Boost_LIBRARIES}`と`${CMAKE_THREAD_LIBS_INIT}`をリンクするか、

```cmake
macro(link_boost_log target)
  target_link_libraries(${target} ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
endmacro(link_boost_log)
```

のようなマクロを組めば完了です。
