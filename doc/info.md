# Boost.Logの全体像

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


# 特徴
- macroを多用する
- placeholderを多用する

特にplaceholderは慣れると便利なんですが、
Boost.Lambdaに慣れ親したしんだ諸兄にはともかく、
C++11以降の我々ゆとりC++erには少しハードルが高いかもしれません。

## 利点
- 全体の設計が一貫しているので大規模化しやすい（？）

## 難点
- compile遅い
- macroなにしてるかわかりにくい
- placeholderきもい (個人の感想です)

# references

- [Boost.Log(本家)](http://www.boost.org/doc/libs/1_57_0/libs/log/doc/html/index.html)
  個々の説明はそこそこわかりやすいが長い
- [boost.Logの使い方を勉強した (｡･ｘ･)ゝ](http://d.hatena.ne.jp/yamada28go/20140215/1392470561)
  詳しい所まで調べてあるが、結構情報量があるので読むの大変
