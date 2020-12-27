# 30日でできる!OS自作入門 on macOS
同書をmacOSにて開発した際のソースコード。

<figure style="text-align:center;">
  <img src="https://raw.githubusercontent.com/zacfukuda/hariboteos/master/screenshot.jpg" alt="30日目の成果" width="480" height="377">
  <figcaption>30日目終了時。<small style="font-size:12px;">vramに少し異常あり</small></figcaption>  
</figure>

[著者ソースコード(HariboteOS.zip)](https://book.mynavi.jp/files/user/support/4839919844/HariboteOS.zip)

8日目までの開発は、Qittaに投稿されている[『30日でできる！OS自作入門』を macOS Catalina で実行する](https://qiita.com/noanoa07/items/8828c37c2e286522c7ee)を参照。9日目以降は著者ソースコードを参照。

(余程のミスをしない限り)コードに問題があっても、コンパイラが親切にエラーを出力してくれる。Haribote OSが壊れる、開発PCがフリーズするということはない。

## 書籍購入
- [マイナビ公式ページ](https://book.mynavi.jp/ec/products/detail/id=22078)
- [アマゾン](https://www.amazon.co.jp/dp/4839919844)
- [楽天](https://item.rakuten.co.jp/booxstore/bk-4839919844/)
- [BookLive](https://booklive.jp/product/index/title_id/253146/vol_no/001)(電子書籍)

## 本レポジトリー参照にあたり
- あくまで私的利用目的
- 2セクション分を1つのプロジェクトとして一括作業している部分あり
- 該当セクション終了後にミスを発見・訂正している部分あり。該当セクション時点での簡単なOS動作確認上の不具合は確認されなかった。(下記除く)
- 多くのコメントを割愛。残したコメントは想定外の文字化けを避けるため英語
- コメントは`//`を優先利用
- 途中で細かな記述方法の変更あり

## 開発環境
- PC: MacBook Pro 13-inch 2017
- OS: macOS Catalina 10.15.7
- コンパイラ: GCC(i386-elf-gcc) 9.2.0, NASM 2.15.05
- イメージ作成: mformat 4.0.25, mcopy 4.0.25
- エミュレータ: QEMU 5.1.0
- エディタ: Visual Studio Code 1.51 (何でも良いと思うが、UTF-8標準設定のものが無難)

Homebrewから各ソフトウェアをインストールする際、最新のXcodeが必要。最新版Xcodeでは、ターミナルからXcode Command Line Toolのインストールができなくなっており、Apple開発者サイト経由ですることになる。

2020年11月時点で既にCatalinaのアップデート`macOS Big Sur 11`が公開されており、加えてアップル独自開発CPU`M1`搭載のPCも流通している。開発を進めている感じ、Big Surでも、(Intel Core ix系搭載のPCであれば、)必要なソフトウェア(GCC, NASM, QEMU, etc.)のインストール・動作ができれば、教材を進められそうである。

## 不具合・未動作

### harib06b, harib06c
**問題**<br>`memory 128MB`と表示される。

**対応**<br>エミュレータが自動で割り当てるメモリー量だと解釈。プログラム・OS自体に問題はないと判断し、無視して開発を継続。

### harib10c~harib11e
**問題**<br>`HariMain()`内`for(;;)`直下にシートを更新するプログラムが無いと、動作が遅くなる。PIC/PITもしくはCPUクロック数に原因があると推測される。

**対応**<br>10秒後ではなく、カウンタ数を常に表示。動作上の意味は無いが10秒後にカウンタ数を表示するプログラムは残してある。尚、この問題は`harib11f`で高速カウンタを消去し、if文内の`io_sti()`を`io_stihlt()`に戻した段階で自然解決される。

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

**対応**<br>前回同様、カウンタを常に表示。(`if (fifo32_status(&fifo) == 0)`)内の`io_sti()`を`io_stihlt()`としてもスムーズな動作はするが、前者と比べてカウントスピードは約1/50になる。

### harib13c~harib13g
**問題**<br>`task_b_main`内`for(;;)`直下にシートを更新するプログラムが無いと、動作が遅くなる。前項同様の問題。

**対応**<br>前項同様、カウンタを常に表示。この問題も`harib14b`でカウンタを削除した段階で自動解決される。

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

### harib20b
**問題**<br>`rand()`が使えない。

**対応**<br>`stars.c`内にStack Overflowを参照して`rand()`を実装。結果的に同書と同じ星が配置された。

### harib21a
**問題**<br>`F11`はmacOSにより既にリザーブされている。

**対応**<br>`F11`の代わりに`F11(0x58)`を使用する。

### harib21g
**問題**<br>`noodle.hrb`生成時、以下のエラーが発生される。

```
/usr/local/Cellar/i386-elf-gcc/9.2.0/lib/gcc/i386-elf/9.2.0/../../../../i386-elf/bin/ld: section .data VMA [0000000000000400,000000000000040f] overlaps section .text VMA [0000000000000030,000000000000048f]
collect2: error: ld returned 1 exit status
make: *** [noodle.hrb] Error 1
```
**対応**<br>良くわからないが、`.data`の一部が`.text`の番地に書き込みされようとしているのが原因。`app.ld`を`app2.ld`として複製し、`0x0400`を`0x0500`と書き換えるとコンパイルに成功した。(`.text`の終了番地が`0x48f`のためそれより大きい値を代入。)

尚、この時自身のソースコード`timer.c:72`に不要なコード(`if (t == 0) { break; }`)があることを発見し、この行を削除。この行を削除しないとタイマーのカウントは1秒で止まる。加えてこの行を削除することにより、アプリ実行後task_aのウィンドウに戻るとカーソルが点滅しなくなる不具合も解決された。

### harib22j
**問題**<br>キーボード入力の条件判定`if (s[0] != 0)`で`Backspace`、`Enter`の入力も`&key_win->task->fifo`へ送信くれるはずだが、うまく送信されない。(`Backspace`および`Enter`が効かない)
**対応**<br>`harib22h`までの`Backspace`、`Enter`用条件判定を残し、その中で`fifo32_put()`を実行する。

### harib24e
**問題**<br>著者開発の`obj2bim`が使えないため、無駄なファイルとのリンクを解けない。

**対応**<br>結果的に`harib24d`よりもファイルサイズが大きくなるが、無視して継続。

### harib24f
**問題**<br>著者開発のライブラリアン`golib00`が使えないため、ライブラリを作れない。

**対応**<br>GNUのアーカイブユーティリティ`ar`を使う。Haribote OSでは、macOS標準の`ar`が使用できないため、elf用`i386-elf-ar`(`386-elf-binutils`の一つ)を使う。`harib24e`と比較し、アプリのファイルサイズが大幅に小さくなった。ライブラリを含めてコンパイルした際、GCCが勝手に必要な関数のみを読み込んでくれたと推測している。

> 余談であるが、アプリファイル生成の際、GCC実行時`-fno-builtin`と`-g`のオプションを外しても正常にコンパイルできた。

### harib24g
**問題**<br>使用ソフトウェアが(10年以上前の)Windowsでの開発と異なるため、Makefileが記述が異なる。

**対応**<br>詳しくは該当ディレクトリの各Makefileを参照。

### harib25a
**問題**<br>`haribote/mysprintf`はアプリ(`noodle.hrb`, `sosu.hrb`, etc.)でも使われる。

**対応**<br>27日目の学習でライブラリに詳しくなったため、`sprintf`, `strcmp`, `strncmp`をライブラリとして書き出す。新規作成、修正したファイルは以下の通り。必要なくなったファイルは削除している。

```bash
# 新規
.
├── lib
│   └── Makefile, sprintf.c, strcmp.c, strncmp.c # `[sprintf.c] => libstdio.a`、`[strcmp.c, strncmp.c] => libstring.a`
└── include
    └── stdio.h, string.h

# 修正
.
├── Makefile
├── app.ld # Change: 0x400 -> 0x500
├── app_make.txt
├── haribote
│   └── Makefile, bootpack.h
└── noodle
    └── noodle.c
```

### harib25b
**問題**<br>`__alloca`が自動で呼び出されない。

**対応**<br>アプリ用リンカスクリプトにアプリ毎スタックサイズを指定できるように`app_make.txt`と`app.ld`を変更。

```Makefile
…
ifndef STACK
	STACK = 0x500
endif
…
$(APP).hrb : …
  … -Wl,'--defsym=__stack=$(STACK)'
…
```

```ld
…
SECTIONS
{
	.head 0x0 : {
    …
    LONG(__stack)
  }
  …
  .data __stack : …
  …
```

`sosu2/Makefile`に`STACK = 0x2800`、`winhelo/Makefile`, `winhelo/Makefile`に`STACK = 0x2000`を追加。(10000=0x2710, 150*50=0x1D4C。)このままでもプログラムは動作するが、指定スタックサイズが必要なメモリサイズより小さい場合にアプリを実行するとOSがフリーズし、強制終了(Shift+fn+F1)もできなくなる。原因は不明だがallocaを使用してバッファを確保すると、アプリプログラムは実行されないものの、強制終了はできるようになった。(`apilib.h`へのalloca関数定義を忘れずに。)

```c
// sosu2.c
char *flag = alloca(MAX);

// winhelo.c, winhelo2
char *buf = alloca(150 * 50);
```

### harib25f
**問題**<br>全てのファイルはShift JISではなくUTF-8でエンコードされているため`、type ipl10.nas`を実行しても文字化け表示される。

**対応**<br>無視。

### harib25g
**問題**<br>`chklang`実行、本記載のバグ(右半分しか表示されない)が発生しない。

**対応**<br>無視。

### harib26b
**問題**<br>(1) `memcmp`が使えない。(2) `setjmp.h`が存在しない、`setjmp`、`longjmp`が読み出せない。

**対応**<br>(1)`lib/memcmp.c`を作成。内容は著者ソースファイル`omake/tolsrc/go_0023s/golibc/memcmp.c`を参照。この関数を`libstring.a`としてライブラリ化。`include/string.h`も併せて更新。

(2) `include/setjmp.h`を作成、`typedef int jmp_buf[3];`を記述し、`tek.c`から読み込む。`tek.c`では、`setjmp`、`longjmp`の代わりに`__builtin_setjmp`、`__builtin_longjmp`を使う。

なおアプリ(.hrb)は著者作成のプログラムを使用してのtek圧縮ができないため、アプリは一切圧縮せず開発継続。

### harib26e
**問題**<br>`sprintf()`で桁数指定(%08d)ができず、(画面表示上での)スコアの上昇がおかしい。

**対応**<br>無視。この問題は`harib27e`で`setdec8`を導入することにより自動解決される。

### harib27a
**問題**<br>(1) `strtol`が使えない。 (2) `|`が入力できない。

**対応**<br>(1) 著者ソースコード(`omake/tolsrc/go_0023s/golibc/strtol.c`)を参照し、`strtol.c`、`strtoul0.c`を作成。`libstdlib.a`として書き出し、コンパイル時にアプリとリンク。`strtol`に必要な`errno.h`、`limits.h`も併せて作成。

(2) 無視。

### harib27c
**問題**<br>音がならない。

**対応**<br>`qemu-system-i386`に`-soundhw pcspk`を加えてエミュレートした結果、正常に音がでた。しかし、`warning: '-soundhw pcspk' is deprecated, please set a backend using '-machine pcspk-audiodev=<name>' instead`の警告を受ける。`-machine pcspk-audiodev`に渡すべき`<name>`がわからなかったため、そのまま`-soundhw pcspk`を使用することに。

音楽ファイルは圧縮したものを著者ソースコードより複製。圧縮前ファイルについては、UTF-8変更したものを保存。

### harib27d
**問題**<br>`bmp.nasm`を流用できない。(`jpeg.c`はほぼそのままで利用できる。)

**対応**<br>`bmp.nasm`を適当なディレクトリへ複製。ファイル内の文字化け部分を全削除。最初の方に文字化けを含む`%if 0…%endif`ブロックも削除。`iconv -f US-ASCII -t UTF-8 bmp.nasm > bmp.nas`を実行してUTF-8形式に変換し、`gview`下へ移動。`[BITS 32]`、関数接頭字`_`を削除。`.bpp4`内の`.do4.1`ラベル宣言でコロンが抜けているので`.do4.1:`と修正。

`jpeg.c`はUTF-8形式としてファイルをした後、著者ソースコードをコピー&ペースト。文字化けは鬱陶しいので、その部分は削除。コンパイル時`jpeg_decode_yuv`が`int`を返さないと警告を受けるので、同関数の`return;`をコメントアウト。

### harib27e
**問題**<br>シリンダ読み込み数。

**対応**<br>各バイナリファイルは、本記載のものよりサイズが大きくなる傾向にあり、且アプリの圧縮も行えない。`setdec8`導入後、バイナリエディタ(Hex Fiend)を使ってharibote.imgの中身が何バイトまで使用されているか調べると`0x02BAA0`であった。結果、シリンダ数: `0x02BAA0 / 18432 = 178848 / 18432 = 9`、余り: `0x02BAA0 % 18432 = 178848 % 18432 = 12960(13k)`となる。圧縮なしで13kを節約するのは無理と判断。10シリンダを読み込む形で`ipl9.nas => ipl10.nas`とした。

### harib27f

割愛。

## 参考文献
- [『30日でできる！OS自作入門』を macOS Catalina で実行する](https://qiita.com/noanoa07/items/8828c37c2e286522c7ee)
