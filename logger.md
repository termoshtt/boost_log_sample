non-trivialなBoost.Logの使い方(1. 自作logger編)
===============================================

TrivialなBoost.Logの使い方を超える使い方を説明していきます。
長くなるので複数回に分けます。

1. loggerの作成
2. cmakeによるコンパイル
3. channelの扱い

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

またサンプルコードは[termoshtt/boost_log_sample](https://github.com/termoshtt/boost_log_sample)にあります。

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

### Tag, Scopeの追加

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
これで`mod1::logger`を通して収集されたログにはタグ"mod1"が付きます。

またログがどの関数で収集されたか、さらに言えばどのif文等のどのスコープで出力されたか、
というのは重要な情報である場合が多いです。
これを保持するのが`Scope`です。
これは常に値が一定("mod1")のタグと異なり、収集されるログ毎に値が変化します。

```cpp:mod1/func1.cpp
namespace mod1 {

void func1(bool flag) {
  BOOST_LOG_FUNCTION(); // Scopeに関数名(void func1(boo))を登録する
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
```

出力やフィルターの設定(ユーザー側)
----------------------

上述の方法で収集したログを出力しましょう。

```cpp:client.cpp
void init() {
  mod1::init();
  logging::add_common_attributes();
  logging::add_file_log(
      keywords::file_name = "client.log", // logを出力するファイル名
      keywords::format =
          "%Tag%: [%TimeStamp%] [%Scope%] %Message%" // logのフォーマット
      );
}

int main(int argc, char const *argv[]) {
  init();
  mod1::func1(true);
  mod1::func1(false);
  return 0;
}
```

`add_common_attributes()`はコアに標準の属性
(TimeStamp, LineID, ThreadID, ProcessID)を記録する様に命令します。
上でloggerに対して"Tag"や"Scope"を記録するように命令したのと同じイメージですが、
こちらは全てのログに対して有効になります。

`add_file_log()`でファイルへ出力するsinkを作成します。
上の図のように収集されたすべてのログがこのsinkに流れこみます。
ファイルへの出力の形式は`keywords::format`で調整されます。
`%...%`が展開される事で、出力は

```console:client.log
mod1: [2015-Mar-04 01:38:57.711251] [void mod1::func1(bool)->true case] flag is true
mod1: [2015-Mar-04 01:38:57.711620] [void mod1::func1(bool)] Some error occurs!!
mod1: [2015-Mar-04 01:38:57.711667] [void mod1::func1(bool)->false case] flag is false
mod1: [2015-Mar-04 01:38:57.711698] [void mod1::func1(bool)] Some error occurs!!
```

のようになります。

### placeholderの設定

上ではフォーマットを`%...%`のテンプレートで生成しましたが、
C++的なstreamを使用する事もできます。
文字列テンプレートの場合の"Tag", "Scope"等の名前は定義時に指定されました

```cpp
lg.add_attribute("Tag", attrs::constant<std::string>("mod1"));
lg.add_attribute("Scope", attrs::named_scope());
```

streamを使用する場合にはまず、モジュール側でplaceholderを設定しておく必要があります。

```cpp:mod1/func.hpp
namespace mod1 {
BOOST_LOG_ATTRIBUTE_KEYWORD(line_id, "LineID", unsigned int)
BOOST_LOG_ATTRIBUTE_KEYWORD(severity, "Severity", severity_level)
BOOST_LOG_ATTRIBUTE_KEYWORD(tag_attr, "Tag", std::string)
BOOST_LOG_ATTRIBUTE_KEYWORD(scope, "Scope", attrs::named_scope::value_type)
} // namespace mod1
```

`tag_attr`, `scope`等がplaceholderになります。
ユーザー側では以下のように使います:

```cpp:client.cpp
void init() {
  mod1::init();
  logging::add_common_attributes();
  logging::add_file_log(
      keywords::file_name = "client.log",
      keywords::format = "%Tag%: [%TimeStamp%] [%Scope%] %Message%"
      );
  logging::add_file_log(
    keywords::file_name = "stream_format.log",
    keywords::format = (expr::stream << mod1::tag_attr << mod1::line_id
                                     << ": <" << mod1::severity << "> ["
                                     << mod1::scope << "] "
                                     << expr::smessage));
}
```

この場合の`keywords::format`にはplaceholderにより生成されたラムダ式が入りますが、
見た目的に挙動は理解して頂けるでしょう。
この場合の`<<`は`operator<<(std::ostream&, T)`が呼ばれますので、

```cpp:mod1/func.hpp
namespace mod1 {
enum severity_level { debug, info, warning, error, critical };
std::ostream &operator<<(std::ostream &ost, severity_level l);
} // namespace mod1
```

のようにあらかじめ出力を調整しておく事が可能です。

Boost.Logは多数の出力の設定方法を提供しています。

- [Log record formatting](http://www.boost.org/doc/libs/1_57_0/libs/log/doc/html/log/tutorial/formatters.html)

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
