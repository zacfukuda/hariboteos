# 30日でできる!OS自作入門 on macOS
同書をmacOSにて開発した際のソースコード。

必要ソフトウェアのインストールおよび8日目までの開発は、Qittaに投稿されている[『30日でできる！OS自作入門』を macOS Catalina で実行する](https://qiita.com/noanoa07/items/8828c37c2e286522c7ee)を参照。9日目以降は著者ソースコードを参照。

基本的にターミナルから利用する実行コマンドは`make run`のみ。プログラムに問題があっても、Haribote OSが壊れる、開発PCがフリーズするということはなく、コンパイラーが親切にエラーを出力してくれる。

2020年11月時点で既にCatalinaのアップデート`macOS Big Sur 11`が公開されている。Big Sur上の動作保証はできないが、開発を進めている感じ、最初の必要ソフトウェア(NASM, QEMU, etc.)のインストールさえできれば、問題なくコンパイル・動作しそうである。

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

### harib06b, harib06c
**問題**<br>`memory 128MB`と表示される。

**対応**<br>エミュレーターが自動で割り当てるメモリー量だと解釈し、プログラム自体に問題はないと判断。無視して開発を継続。

### harib10c~harib11e
**問題**<br>`HariMain()`内`for(;;)`直下にシートを更新するプログラムが無いと、動作が遅くなる。PIC/PITもしくはCPUクロック数に原因があると推測される。

**対応**<br>10秒後ではなく、カウンター数を常に表示。動作上の意味は無いが10秒後にカウンター数を表示するプログラムは残してある。なお、この問題は`harib11f`で高速カウンターを消去し、if文内の`io_sti()`を`io_stihlt()`に戻した段階で自然解決される。

```c
void HariMain(void) {
  …
  for (;;) {
    count++；
    // このあたり
  }
}
```

### harib11d
**問題**<br>`asmhead.nas`に指定の画面モードをするも、黒い画面が表示されるのみ。コンパイル時のエラーはなし。

**対応**<br>harib11eのVBE設定プログラム導入で自然解決される。特に意味はないが、解像度は`0x101`を利用して開発を進めた。

### harib12e~harib13b
**問題**<br>`task_b_main`内`for(;;)`直下にシートを更新するプログラムが無いと、動作が遅くなる。harib10c~harib11eと類似した問題。

**対応**<br>前回同様、カウンターを常に表示。(`if (fifo32_status(&fifo) == 0)`)内の`io_sti()`を`io_stihlt()`としてもスムーズな動作はするが、前者と比べてカウントスピードは約1/50になる。

### harib13c~harib13g
**問題**<br>`task_b_main`内`for(;;)`直下にシートを更新するプログラムが無いと、動作が遅くなる。前項同様の問題。

**対応**<br>前項同様、カウンターを常に表示。この問題も`harib14b`でカウンターを削除した段階で自動解決される。

### harib15f
**問題**<br>`<string.h>`がincludeできず、`strcmp`が使えない。

**対応**<br>`sprintf`に倣い、関数を自作。新たに`myfunction.c`を作成し、同ファイルに関数を記述。新規関数・ファイル追加に伴い`bootpack.h`と`Makefile`に追加記述を施す。`strcmp`のプログラムは[Appleのオープンソースサイト](https://opensource.apple.com/source/Libc/Libc-262/ppc/gen/strcmp.c.auto.html)で公開されている。

### harib16b
**問題**<br>`strncmp`が使えない。

**対応**<br>`strcmp`同様、`myfunction.c`に関数自作。`bootpack.h`に関数定義も追加。参照: [Appleのオープンソースサイト](https://opensource.apple.com/source/Libc/Libc-167/gen.subproj/i386.subproj/strncmp.c.auto.html)。

### harib17b~harib17d
**問題**<br>`asm_cons_putchar`の番地が本記載のものと異なる。

**対応**<br>`Makefile`内`bootpack.hrb`を生成するコマンドに`-Xlinker -Map=bootpack.map`オプションを追加し、マップファイルを生成。`bootpack.map`に記載されている`asm_cons_putchar`の番地を調べ、その値を`hlt.nas`内`be3`値と入れ替える。ちなみに自分が実行した際の番地は、`c64`であった。この値を使用して`harib17c`を実行した際、正常に「A」が表示された。よくわかない場合、`harib17e`までスキップすれば、番地を調べることなく、プログラムを実行できるようになる。

### harib18b
**問題**<br>`a.hrb`と`hello3.hrb`の生成。

**対応**<br>[『30日でできる！OS自作入門』のメモ](https://vanya.jp.net/os/haribote.html#hrb)を参照。「アプリケーション用リンカスクリプト」を`app.ld`として保存し、`a.hrb`、`hello3.hrb`生成時に、これをリンカスクリプトとして利用。リンカスクリプトについては自分も未理解。

### harib19c
**問題**<br>`Shift + F1`を押しただけでは強制終了しない。(`0x3d`が入力されない。)

**対応**<br>Macのキーボード設計による問題。`Shift + fn + F1`で強制終了できる。