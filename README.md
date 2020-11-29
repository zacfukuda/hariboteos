# 30日でできる!OS自作入門 on macOS
著書をmacOSにて開発した際のソースコード。

必要ソフトウェアのインストールおよび8日目までの開発は、Qittaに投稿されている[『30日でできる！OS自作入門』を macOS Catalina で実行する](https://qiita.com/noanoa07/items/8828c37c2e286522c7ee)を参照。9日目以降は著者ソースコードを参照。

基本的にターミナルから利用する実行コマンドは`make run`のみ。プログラムに問題があっても、Haribote OSが壊れる、開発PCがフリーズするということはなく、コンパイラーが親切にエラーを出力してくれる。

### 参考文献
- [マイナビ公式ページ](https://book.mynavi.jp/ec/products/detail/id=22078)
- [著者ソースコード(HariboteOS.zip)](https://book.mynavi.jp/files/user/support/4839919844/HariboteOS.zip)
- [『30日でできる！OS自作入門』を macOS Catalina で実行する](https://qiita.com/noanoa07/items/8828c37c2e286522c7ee)
- [@noanoa07氏ソースコード](https://github.com/noanoa07/myHariboteOS)
- [アマゾン購入ページ](https://www.amazon.co.jp/dp/4839919844)

### 本レポジトリー参照にあたり
- あくまで私的利用目的、教材提供目的ではない。誰かしらの役に立てばと願っている
- 2セクション分を1つのプロジェクトとして一括作業している部分あり。
- 該当セクション終了後ミスを発見・訂正している部分あり。該当セクション時点での簡単なOS動作確認上の不具合は確認されなかった。(下記除く)
- 多くのコメントを割愛。残したコメントは想定外の文字化けを避けるため日本語から英語に
- コメントは`//`を優先利用
- 途中で細かな記述方法の変更あり

## 開発環境
- PC: MacBook Pro 13-inch 2017
- OS: macOS Catalina 10.15.7
- コンパイラー: GCC 12.0.0, NASM 2.15.05
- エミュレーター: QEMU 5.1.0
- エディター: Visual Studio Code 1.51 (何でも良いと思うが、UTF-8標準設定のものが無難)

Homebrewからソフトウェアをインストール際、最新のXcodeが必要。最新のXcodeでは、ターミナルからXcode Command Line Toolのインストールができなくなっており、Apple開発者サイト経由ですることになる。

## 不具合・未動作

### harib10c~harib11e
**問題**<br>`HariMain()`内`for(;;)`直下にシートを更新するプログラムが無いと、動作が遅くなる。PIC/PITもしくはCPUクロック数に原因があると推測される。

**対応**<br>10秒後ではなく、カウンター数を常に表示。動作上の意味は無いが10秒後にカウンター数を表示するプログラムは残してある。なお、この問題は`harib11f`で高速カウンターを消去し、if文内の`io_sti()`を`io_stihlt()`に戻した段階で自然解決される。

```c
void HariMain(void) {
  …
  for (;;) {
    count++:
    // このあたり
  }
}
```

### harib11d
**問題**<br>`asmhead.nas`に指定の画面モードをするも、黒い画面が表示されるのみ。コンパイル時のエラーはなし。

**対応**<br>harib11eのVBE設定プログラム導入で自然解決される。特に意味はないが、解像度は`0x101`を利用して開発を進めた。