# loggerの定義

自作loggerを作る事によって、そのloggerによって収集されるログに情報を付加できます。
以下で深刻度付きのloggerを生成します

```cpp
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

```cpp
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

## Tag, Scopeの追加

Boost.Logでは別のloggerで収集されたログは全て同じようにsinkに送られます。
つまりデフォルトではどのloggerから出力されたのかわかりません。
そこでloggerに、そのloggerで収集されたログに情報を付加する設定をします。

```cpp
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

```cpp
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

