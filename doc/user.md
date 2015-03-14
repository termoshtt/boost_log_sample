
# 出力やフィルターの設定

上述の方法で収集したログを出力しましょう。

```cpp
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

```console
mod1: [2015-Mar-04 01:38:57.711251] [void mod1::func1(bool)->true case] flag is true
mod1: [2015-Mar-04 01:38:57.711620] [void mod1::func1(bool)] Some error occurs!!
mod1: [2015-Mar-04 01:38:57.711667] [void mod1::func1(bool)->false case] flag is false
mod1: [2015-Mar-04 01:38:57.711698] [void mod1::func1(bool)] Some error occurs!!
```

のようになります。

# placeholderの設定

上ではフォーマットを`%...%`のテンプレートで生成しましたが、
C++的なstreamを使用する事もできます。
文字列テンプレートの場合の"Tag", "Scope"等の名前は定義時に指定されました

```cpp
lg.add_attribute("Tag", attrs::constant<std::string>("mod1"));
lg.add_attribute("Scope", attrs::named_scope());
```

streamを使用する場合にはまず、モジュール側でplaceholderを設定しておく必要があります。

```cpp
namespace mod1 {
BOOST_LOG_ATTRIBUTE_KEYWORD(line_id, "LineID", unsigned int)
BOOST_LOG_ATTRIBUTE_KEYWORD(severity, "Severity", severity_level)
BOOST_LOG_ATTRIBUTE_KEYWORD(tag_attr, "Tag", std::string)
BOOST_LOG_ATTRIBUTE_KEYWORD(scope, "Scope", attrs::named_scope::value_type)
} // namespace mod1
```

`tag_attr`, `scope`等がplaceholderになります。
ユーザー側では以下のように使います:

```cpp
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

```cpp
namespace mod1 {
enum severity_level { debug, info, warning, error, critical };
std::ostream &operator<<(std::ostream &ost, severity_level l);
} // namespace mod1
```

のようにあらかじめ出力を調整しておく事が可能です。

Boost.Logは多数の出力の設定方法を提供しています。

- [Log record formatting](http://www.boost.org/doc/libs/1_57_0/libs/log/doc/html/log/tutorial/formatters.html)

