non-trivialなBoost.Logの使い方(1. 自作logger編)
===============================================

TrivialなBoost.Logの使い方を超える使い方を説明していきます。
長くなるので2回に分けます。

1. loggerの作成
2. channelの扱い

今回は自作loggerについてです。

logger is 何？
--------------

![Design.png](http://www.boost.org/doc/libs/1_57_0/libs/log/doc/html/images/log/Design.png)
([Design overview](http://www.boost.org/doc/libs/1_57_0/libs/log/doc/html/log/design.html)より転載)
Boost.Logには3つの階層があり、

- logger : アプリがログを出力するためのインターフェイス
- core : 各loggerから取得したログをまとめて、フィルタリングしたりする
- sink : logをファイルや標準出力したりする(ここでフィルターもできる)

アプリ、あるいはモジュールをユーザーが使用する場合

- core, logger : アプリ、モジュールが使用
- core, sink : ユーザーが使用

のように使う事になります。
loggerはなるべく多くの情報を提供し、ユーザー側がcore/sinkの設定でフィルターします。
この記事ではloggerに情報を追加する方法についてまとめます。

準備、あるいは状況設定の確認
----------------------------

ログを取るためにライブラリを使用する必要があるのは、複数のモジュールがあるときに
特定のモジュールからのログを選り分けたりする必要がある場合です。
最小の状況設定として以下の場合を考えます。

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

自作loggerの作成(モジュール側)
----------------

### loggerの定義

自作loggerを作る事によって、そのloggerによって収集されるログに情報を付加できます。
以下で深刻度付きのloggerを生成します

```cpp:mod1/func.hpp
namespace mod1 {
enum severity_level { debug, info, warning, error, critical };
BOOST_LOG_INLINE_GLOBAL_LOGGER_DEFAULT(
    logger,                                                 // loggerの名称
    boost::log::sources::severity_logger_mt<severity_level> // loggerの型
    );
} // namespace mod1
```

これで`mod1::logger`が定義されます。
これは例えば以下のように使います。

```cpp:mod1/func1.cpp
namespace mod1 {
void func1() {
  auto &lg = logger::get();
  BOOST_LOG_SEV(lg, info) << "info in mod1::func1";
}
} // namespace mod1
```

loggerの実体は`::get()`で取得します。
`BOOST_LOG_SEV`は深刻度と一緒にログを記録するためのマクロです。
`info`は`mod1::info`な事に注意してください。

### Tagの追加

Boost.Logでは別のloggerで収集されたログは全て同じようにsinkに送られます。
つまりデフォルトではどのloggerから出力されたのかわかりません。
そこでloggerに、そのloggerで収集されたログに情報を付加する設定をします。

```cpp:mod1/logger.cpp
namespace mod1 {
void init() {
  auto &lg = logger::get();
  lg.add_attribute("Tag", attrs::constant<std::string>("mod1"));
  lg.add_attribute("Scope", attrs::named_scope());
}
} // namespace mod1
```

モジュール自体の初期化コードでloggerを設定します。

### placeholderの使い方

出力やフィルターの設定(ユーザー側)
----------------------

compile with cmake
------------------

Boost.Logがコンパイルが面倒との意見が散見されます。
公式はbjamを使用するようにと書いていますが、
ここではcmakeを使用したコンパイルについて説明します。

最後に
------

### 特徴
- macroを多用する
- placeholderを多用する

特にplaceholderは慣れると便利なんですが、
Boost.Lambdaに慣れ親したしんだ諸兄にはともかく、
C++11以降の我々ゆとりC++erには少しハードルが高いかもしれません。

### 利点
- 全体の設計が一貫しているので大規模化しやすい（？）

### 難点
- compile遅い
- macroなにしてるかわかりにくい
- placeholderきもい (個人の感想です)

### references

- [Boost.Log(本家)](http://www.boost.org/doc/libs/1_57_0/libs/log/doc/html/index.html)
  個々の説明はそこそこわかりやすいが長い
- [boost.Logの使い方を勉強した (｡･ｘ･)ゝ](http://d.hatena.ne.jp/yamada28go/20140215/1392470561)
  詳しい所まで調べてあるが、結構情報量があるので読むの大変
