/*
 *----------------------------------------------------------------------
 *    UCT T2AS (T-Kernel 2.0 AeroSpace) DevKit tuned for ASP-LEON5
 *
 *    © 2021 Ubiquitous Computing Technology Corporation.
 *----------------------------------------------------------------------
 *
 *    Version:   1.00.01a
 *    Released by Ubiquitous Computing Technology Corporation (https://www.uctec.com/) on 2023/07/25.
 *
 *----------------------------------------------------------------------
 */

==============================================================================
        T2AS (T-Kernel 2.0 AeroSpace) ( ASP-LEON5 ) 取扱説明書
==============================================================================
                                                              Version 1.00.01a

------------------------------------------------------------------------------
1. はじめに
------------------------------------------------------------------------------

本書は、LEON-XCKU (CPU LEON5) (以降、ASP-LEON5 と記述) に対応した T2AS
(T-Kernel 2.0 AeroSpace) システムの取り扱いに関する説明書です。

本 T2AS (T-Kernel 2.0 AeroSpace) システムは、以下の開発環境の上に構築されて
います。

・BCC2: Bare-C Cross-Compiler System for LEON2/3/4/5 GCC 10.2.0
・GRMON 3.3
・Eclipse IDE for C/C++ Developers
　* Version: 2023-03 (4.27.0)
　* Build id: 20230309-1520

これらの開発ツールの入手方法などは、本書の最後をご参照ください。
本書では、開発ツールのインストール方法や使用方法などについては記載しておりま
せんので、各開発ツールの説明書をご参照ください。


------------------------------------------------------------------------------
2. リリース内容
------------------------------------------------------------------------------

    ReadMe.txt                  取扱説明書 (本書)
    impl-asp-leon5.txt          T2AS (ASP-LEON5) 実装仕様書
    impl-serial-leon5.txt       シリアルドライバ実装仕様書
    impl-t2ex.txt               T2EX実装仕様書


------------------------------------------------------------------------------
3. ビルド方法
------------------------------------------------------------------------------

(1) 本リポジトリを任意のディレクトリにクローンします。
(2) Eclipse を起動し、Workspace としてクローンしたフォルダを指定します。
(3) メニューの File - Open Projects from File System... を選択します。
(4) Import source の Directory... を押し、ASP_LEON5 フォルダを選択します。
(5) Folder にプロジェクトの一覧が表示されているのを確認して、Finish をクリックします。
(6) Project Explorer に以下のようなプロジェクトの一覧が表示されていることを確認
　　してください。

    config
    console
    datetime
    kernel
    libdrvif
    libstr
    libsvc
    libtk
    libtm
    monitor
    netdrv
    network
    rs
    tkse
    usermain
    usermain_t2ex

(7) これらのプロジェクトを全てビルドします。
　　ただし、usermain を一番最後にビルドする必要があります。
　　それ以外は順不同で構いません。

　　Project Explorer でプロジェクトを選択し、メニューの Project - Build Project
　　でビルドを実行します。

(8) datetime プロジェクトと network プロジェクトでは、オブジェクト本体と SVC 
　　ライブラリの２つをビルドするため、アクティブプロジェクトを切り替えてビルド
　　を行います。

　　オブジェクト本体をビルドするには、Project Explorer でプロジェクトを選択し、
　　メニューの Project - Build Configurations - Set Active - Debug でアクティブ
　　プロジェクトを Debug に変更してからビルドを実行してください。
　　ライブラリをビルドするには、同様にアクティブプロジェクトを Lib に変更して
　　からビルドを実行してください。

(9) 最後に usermain プロジェクトをビルドして生成される usermain が実行プログラム
　　になります。
　　T2EX を使用する場合は、usermain の代わりに usermain_t2ex をビルドしてください。


------------------------------------------------------------------------------
4. プロジェクトの設定
------------------------------------------------------------------------------

各プロジェクトには基本的に同じ設定をしています。

コンパイルオプション    -mcpu=leon3 -D_ASP_LEON5_
アセンブルオプション    -mcpu=leon3 -D_ASP_LEON5_
リンクオプション        -mcpu=leon3

インクルードパス        各プロジェクトごとに異なります。

インクルードパスは、基本的には ASP_LEON5\include を指定しますが、プロジェク
トによってはそれ以外も指定されています。実際の設定内容は、Eclipse で各プロジ
ェクトの設定をご確認ください。

以下のライブラリ関数は、BCC に付属のライブラリを使用するため、ソースからは
#if 0 により省かれています。

    libtk:
        malloc() calloc() realloc() free()
    libstr:
        memset() memcmp() memcpy() memmove()
        strlen() strcmp() strncmp() strcpy() strncpy() strcat() strncat()
        strtol()


------------------------------------------------------------------------------
5. 実行方法
------------------------------------------------------------------------------

------------------------------------------------------------------------------
5.1 KCU105 Evalution Kit
------------------------------------------------------------------------------

プログラムの実行手順は、「LEON-XCKU-EX Quick Start Guide」をご参照ください。

ボードと PC は、２本の USB ケーブルで接続します。
ボードの USB UART コネクタと USB JTAG コネクタの２つを PC と接続します。

PC 上でターミナルソフトを立ち上げます。
Windows のデバイスマネージャーで、以下の名称のシリアルポートを使用します。

    Silicon Labs Dual CP2105 USB to UART Bridge: Standard COM Port

USB シリアルポートの設定は、以下の通りとしてください。

    通信速度    19200 bps
    データ長    8 bits
    パリティ    なし
    ストップ    1 bit
    フロー制御  なし

このあと、「LEON-XCKU-EX Quick Start Guide」を参照頂き、同様の手順でプログラ
ムを実行してください。
操作するコマンドのみを、入力する順に以下に示します。

    vivado -mode tcl -notrace -source doprog.tcl
    doprog EX1          ← EX1 を使用してください。
    grmon -digilent
    load usermain       ← 実行するプログラムを指定します。
    run

「LEON-XCKU-EX Quick Start Guide」の説明では、forward enable uart0 を行って
いますが、これは行わないでください。シリアルポートが使用できなくなります。

実行すると、最初に T-Monitor が起動したのち、T-Kernel が起動し、ターミナル上
に以下のメッセージが表示されます。

      ** ASP-LEON5 - Monitor **

      T-Kernel Version 2.01.00

      T2AS (T-Kernel 2.0 AeroSpace) for ASP-LEON5

      ConsoleIO - OK
      RsDrv - OK
      T2 >>

シリアルドライバ(ConsoleIO, RsDrv) の起動後、簡単なコマンド入力のサンプルプ
ログラムが動作して、ターミナルからのコマンド入力待ちになります。

      T2 >> (enter)
      q[uit]      quit
      # [cmd]     exec t-monitor command
      ?           command help
      <command>   misc. command

"#" を入力すると、T-Monitor に入り、メモリダンプ/メモリ変更のコマンドを実行す
ることができます。

      T2 >> #
      TM> ?
      d{b|h|w} [adr[ cnt]] - dump
      m{b|h|w} [adr[ dat]] - modify
      g [adr] - go/return
      e - halt

      TM> dw 40000000 10
      40000000: 88100000 091000FA 81C12094 01000000
      40000010: 91D02000 01000000 01000000 01000000
      40000020: 91D02000 01000000 01000000 01000000
      40000030: 91D02000 01000000 01000000 01000000

"g" で T-Monitor から戻ります。

      TM> g
      T2 >> 

"#" に続けて T-Monitor のコマンドを直接入力することもできます。

      T2 >> # dw 40000000 10
      40000000: 88100000 091000FA 81C12094 01000000
      40000010: 91D02000 01000000 01000000 01000000
      40000020: 91D02000 01000000 01000000 01000000
      40000030: 91D02000 01000000 01000000 01000000
      T2 >> 

"q" を入力すると、コマンド入力のサンプルプログラムは終了します。

      T2 >> q
      Push any key to shutdown the T-Kernel.

ターミナルから何か入力すると、システムは終了します。

      << SYSTEM SHUTDOWN >>
      -- tm_exit() --
      -- system halt --

システムの終了では、T-Monitor の tm_exit() 関数を呼び出します。
tm_exit() 関数では、特に何もせずに内部で無限ループしているだけです。

コマンド入力のサンプルプログラムに関しては、以下のソースプログラムを参照してく
ださい。

    usrmain\
      command.c           コマンド入力処理メイン
      ref_command.c_inc   T-Kernel のリソース状態を表示する ref コマンドの処理
      test_prog.c_inc     タスクとシリアルドライバのテストプログラム

usrmain\usermain.c にアプリケーションタスクを追加して、再ビルドして、ロード/
実行することで、アプリケーションを実行することができます。

以下にコマンド入力のサンプルプログラムを実行したときのログを示します。

** ASP-LEON5 - Monitor **

T-Kernel Version 2.01.00

T2AS (T-Kernel 2.0 AeroSpace) for ASP-LEON5

ConsoleIO - OK
RsDrv - OK
T2 >> 
q[uit]      quit
# [cmd]     exec t-monitor command
?           command help
<command>   misc. command
T2 >> ?
ref  [item]
call addr [p1 p2 p3]
test-task
test-serial
T2 >> ref
ref  item [num]   dump kernel resources
  item: mem,tsk,sem,flg,mbx,mbf,por,mtx,mpl,mpf,cyc,alm
ref  reg tskid    dump task registers
T2 >> ref mem
MEMORY: Blksz=1024 Total=1040122 (1040122 KB) Free=1040024 (1040024 KB) [1 % used]
T2 >> ref tsk
TSK STATE (MAX:150)
TID PRI:BPR SLT WUP SUS STS(*:NODISWAI)  ST+UT(x10) RID EXINF
  1 138:138   0   0   0 RUN                22+0       1 
  2  25: 25   0   0   0 WAI-SLP            0+0       1 4004787c 
  3  25: 25   0   0   0 WAI-SLP            0+0       1 4004787c 
  4  25: 25   0   0   0 WAI-FLG  [  7]     0+0       1 4004787c 
T2 >> ref sem
SEM STATE (MAX:100):
 ID WID CNT EXINF
  1   0   0 4d656d00 Mem
  2   0   0 444d5379 DMSy
  3   0   0 636f6e73 cons
T2 >> ref flg
FLG STATE (MAX:100):
 ID WID PTN      EXINF
  1   0 00000000 444d4c6b DMLk
  2   0 00000000 6c6c746b lltk
  3   0 00000004 73696f30 sio0
  4   0 00000000 636f6e31 con1
  5   0 00000000 636f6e32 con2
  6   0 00000000 72736100 rsa
  7   4 00000000 72736100 rsa
  8   0 00000000 72736100 rsa
T2 >> ref mbx
MBX STATE (MAX:20):
 ID WID MSG      EXINF
T2 >> ref mbf
MBF STATE (MAX:20):
 ID WID SID  MSGSZ   FREE    MAX EXINF
  1   0   0      0   1024     64 44457674 DEvt
T2 >> ref por
POR STATE (MAX:50):
 ID WID AID MAXCSZ MAXRSZ EXINF
T2 >> ref mtx
MTX STATE (MAX:100):
 ID HID WID EXINF
T2 >> ref mpl
MPL STATE (MAX:10):
 ID WID   FREE    MAX EXINF
T2 >> ref mpf
MPF STATE (MAX:10):
 ID WID FREE EXINF
T2 >> ref cyc
CYC STATE (MAX:20):
 ID STS   TIME EXINF
T2 >> ref alm
ALM STATE (MAX:40):
 ID STS   TIME EXINF
T2 >> ref reg 4
TASK REGISTER (TID:4)
PC: 40039950 nPC:40014b38 PSR:f59000c3 SP: 4084f4c8
Y:  61881200 g1: 00000020 g2: f59000c4 g3: 00000004
g4: 00000000 g5: 8a124102 g6: 00000000 g7: 00010000
o0: f5900fe4 o1: ffffffff o2: ffffffff o3: 40035d78
o4: ffffffff o5: 00000004              o7: 40014b30
l0: f54000c4 l1: 4003812c l2: 40038130 l3: 00000010
l4: 4003e800 l5: 00000040 l6: 00000000 l7: 00000007
i0: 00000007 i1: 00ff0606 i2: 00000021 i3: 4084f71c
i4: ffffffff i5: ffffffff i6: 4084f5e8 i7: 4001479c
SYSTEM STACK AREA: 0x4084e808 - 0x4084f808
  USER STACK AREA:          ? - 0x00000000
T2 >> test-task
1 - Task test started.
2 - Start sub task (tid=5) and wait.
3 - Sub task started (tid=5).
4 - Delay 2000 msec.
5 - Delay 2000 msec.
6 - Delay 2000 msec.
7 - Wakeup main task.
8 - Wakeup from sub task.
9 - Sub task finished.
10 - Task test finished.
T2 >> test-serial
Serial driver test
>> ?
f [SRcr]      -- get/sey flow control
l             -- loop back test, key-in '!' to finish
s [len] [tmo] -- serial driver in/out test
r [len] [tmo] -- rs driver in/out test
q             -- quit
>> f
serial_ctl: flow:cs=0 rs=0 sx=1 rx=1
>> q
T2 >> q
Push any key to shutdown the T-Kernel.

<< SYSTEM SHUTDOWN >>
-- tm_exit() --
-- system halt --


------------------------------------------------------------------------------
5.2 SPC-MK4
------------------------------------------------------------------------------

プログラムの実行手順は、「LEON-XCKU-EX Quick Start Guide」をご参照ください。

ボードと PC は、1本の USB ケーブルとPlatform Cable USBで接続します。
ボードの MicroB CONSOLE コネクタ (CN12) と FPGA-JTAG コネクタ (CN13) の２つを PC
と接続します。

PC 上でターミナルソフトを立ち上げます。
Windows のデバイスマネージャーで、以下の名称のシリアルポートを使用します。

    USB Serial Port

USB シリアルポートの設定は、以下の通りとしてください。

    通信速度    19200 bps
    データ長    8 bits
    パリティ    なし
    ストップ    1 bit
    フロー制御  なし

SPC-MK4 のディップスイッチ DSW1-4 が OFF になっていることを確認してください。

このあと、「LEON-XCKU-EX Quick Start Guide」を参照頂き、同様の手順でプログラ
ムを実行してください。
操作するコマンドのみを、入力する順に以下に示します。

    vivado -mode tcl -notrace -source doprog.tcl
    doprog EX1               ← EX1 を使用してください。
    grmon -xilusb            ← -xilusb を指定してください。
    load usermain_t2ex       ← 実行するプログラムを指定します。
    run

「LEON-XCKU-EX Quick Start Guide」の説明では、forward enable uart0 を行って
いますが、これは行わないでください。シリアルポートが使用できなくなります。

実行すると、最初に T-Monitor が起動したのち、T-Kernel が起動し、ターミナル上
に以下のメッセージが表示されます。

      ** ASP-LEON5 - Monitor **

      T-Kernel Version 2.01.00

      T2AS (T-Kernel 2.0 AeroSpace) for ASP-LEON5

      ConsoleIO - OK
      RsDrv - OK
      NetDrv - OK
      dt_main(0) - OK
      so_main(0) - OK
      *** T2EX Application program start !!
      T2EX >>

シリアルドライバ(ConsoleIO, RsDrv) の起動後、簡単なコマンド入力のサンプルプ
ログラムが動作して、ターミナルからのコマンド入力待ちになります。

      T2EX >> (enter)
      q[uit]      quit
      # [cmd]     exec t-monitor command
      ?           command help
      <command>   misc. command

"#" を入力すると、T-Monitor に入り、メモリダンプ/メモリ変更のコマンドを実行す
ることができます。

      T2EX >> #
      TM> ?
      d{b|h|w} [adr[ cnt]] - dump
      m{b|h|w} [adr[ dat]] - modify
      g [adr] - go/return
      e - halt

      TM> dw 40000000 10
      40000000: 88100000 09100330 81C12090 01000000
      40000010: 91D02000 01000000 01000000 01000000
      40000020: 91D02000 01000000 01000000 01000000
      40000030: 91D02000 01000000 01000000 01000000

"g" で T-Monitor から戻ります。

      TM> g
      T2EX >> 

"#" に続けて T-Monitor のコマンドを直接入力することもできます。

      T2EX >> # dw 40000000 10
      40000000: 88100000 09100330 81C12090 01000000
      40000010: 91D02000 01000000 01000000 01000000
      40000020: 91D02000 01000000 01000000 01000000
      40000030: 91D02000 01000000 01000000 01000000
      T2EX >> 

"q" を入力すると、コマンド入力のサンプルプログラムは終了します。

      T2EX >> q
      dt_main(-1) - OK
      so_main(-1) - OK
      Push 'g' key to shutdown the T-Kernel.

ターミナルから何か入力すると、システムは終了します。

      << SYSTEM SHUTDOWN >>
      -- tm_exit() --
      -- system halt --

システムの終了では、T-Monitor の tm_exit() 関数を呼び出します。
tm_exit() 関数では、特に何もせずに内部で無限ループしているだけです。

コマンド入力のサンプルプログラムに関しては、以下のソースプログラムを参照してく
ださい。

    usrmain_t2ex\
      command.c           コマンド入力処理メイン
      ref_command.c_inc   T-Kernel のリソース状態を表示する ref コマンドの処理
      test_prog.c_inc     タスクとシリアルドライバのテストプログラム

usrmain_t2ex\usermain.c にアプリケーションタスクを追加して、再ビルドして、ロード/
実行することで、アプリケーションを実行することができます。

以下にコマンド入力のサンプルプログラムを実行したときのログを示します。

** ASP-LEON5 - Monitor **

T-Kernel Version 2.01.00

T2AS (T-Kernel 2.0 AeroSpace) for ASP-LEON5

ConsoleIO - OK
RsDrv - OK
dt_main(0) - OK
so_main(0) - OK
T2EX >> 
q[uit]      quit
# [cmd]     exec t-monitor command
?           command help
<command>   misc. command
T2EX >> ?
ref  [item]
call addr [p1 p2 p3]
test-task
test-serial
T2EX >> ref
ref  item [num]   dump kernel resources
  item: mem,tsk,sem,flg,mbx,mbf,por,mtx,mpl,mpf,cyc,alm
ref  reg tskid    dump task registers
T2EX >> ref mem
MEMORY: Blksz=1024 Total=1040122 (1040122 KB) Free=1040024 (1040024 KB) [1 % used]
T2EX >> ref tsk
TSK STATE (MAX:150)
TID PRI:BPR SLT WUP SUS STS(*:NODISWAI)  ST+UT(x10) RID EXINF
  1 138:138   0   0   0 RUN                22+0       1 
  2  25: 25   0   0   0 WAI-SLP            0+0       1 4004787c 
  3  25: 25   0   0   0 WAI-SLP            0+0       1 4004787c 
  4  25: 25   0   0   0 WAI-FLG  [  7]     0+0       1 4004787c 
T2EX >> ref sem
SEM STATE (MAX:100):
 ID WID CNT EXINF
  1   0   0 4d656d00 Mem
  2   0   0 444d5379 DMSy
  3   0   0 636f6e73 cons
T2EX >> ref flg
FLG STATE (MAX:100):
 ID WID PTN      EXINF
  1   0 00000000 444d4c6b DMLk
  2   0 00000000 6c6c746b lltk
  3   0 00000004 73696f30 sio0
  4   0 00000000 636f6e31 con1
  5   0 00000000 636f6e32 con2
  6   0 00000000 72736100 rsa
  7   4 00000000 72736100 rsa
  8   0 00000000 72736100 rsa
T2EX >> ref mbx
MBX STATE (MAX:20):
 ID WID MSG      EXINF
T2EX >> ref mbf
MBF STATE (MAX:20):
 ID WID SID  MSGSZ   FREE    MAX EXINF
  1   0   0      0   1024     64 44457674 DEvt
T2EX >> ref por
POR STATE (MAX:50):
 ID WID AID MAXCSZ MAXRSZ EXINF
T2EX >> ref mtx
MTX STATE (MAX:100):
 ID HID WID EXINF
T2EX >> ref mpl
MPL STATE (MAX:10):
 ID WID   FREE    MAX EXINF
T2EX >> ref mpf
MPF STATE (MAX:10):
 ID WID FREE EXINF
T2EX >> ref cyc
CYC STATE (MAX:20):
 ID STS   TIME EXINF
T2EX >> ref alm
ALM STATE (MAX:40):
 ID STS   TIME EXINF
T2EX >> ref reg 4
TASK REGISTER (TID:4)
PC: 40039950 nPC:40014b38 PSR:f59000c3 SP: 4084f4c8
Y:  61881200 g1: 00000020 g2: f59000c4 g3: 00000004
g4: 00000000 g5: 8a124102 g6: 00000000 g7: 00010000
o0: f5900fe4 o1: ffffffff o2: ffffffff o3: 40035d78
o4: ffffffff o5: 00000004              o7: 40014b30
l0: f54000c4 l1: 4003812c l2: 40038130 l3: 00000010
l4: 4003e800 l5: 00000040 l6: 00000000 l7: 00000007
i0: 00000007 i1: 00ff0606 i2: 00000021 i3: 4084f71c
i4: ffffffff i5: ffffffff i6: 4084f5e8 i7: 4001479c
SYSTEM STACK AREA: 0x4084e808 - 0x4084f808
  USER STACK AREA:          ? - 0x00000000
T2EX >> test-task
1 - Task test started.
2 - Start sub task (tid=5) and wait.
3 - Sub task started (tid=5).
4 - Delay 2000 msec.
5 - Delay 2000 msec.
6 - Delay 2000 msec.
7 - Wakeup main task.
8 - Wakeup from sub task.
9 - Sub task finished.
10 - Task test finished.
T2EX >> test-serial
Serial driver test
>> ?
f [SRcr]      -- get/sey flow control
l             -- loop back test, key-in '!' to finish
s [len] [tmo] -- serial driver in/out test
r [len] [tmo] -- rs driver in/out test
q             -- quit
>> f
serial_ctl: flow:cs=0 rs=0 sx=1 rx=1
>> q
T2EX >> q
dt_main(-1) - OK
so_main(-1) - OK
Push 'g' key to shutdown the T-Kernel.

<< SYSTEM SHUTDOWN >>
-- tm_exit() --
-- system halt --


------------------------------------------------------------------------------
6. システムの構成
------------------------------------------------------------------------------

6.1 概要

    LEON-XCKU の Example Configuretion EX1 で T-Monitor(簡易版) および
    T-Kernel が動作します。

    ※ 詳細は、T2AS (ASP-LEON5) 実装仕様書を参照してください。

6.2 T-Monitor

    T-Monitor は、必要最小限のブート機能のみの実装ですので、デバッグ機能はデ
    バッグコンソールの入出力と簡単なコマンド処理のみに対応しています。

    システムコールとしては、以下のシリアル入出力に対応しています。シリアル入出
    力は割り込み禁止状態で実行されます。

        tm_getchar( INT wait )
        tm_putchar( INT c )
        tm_getline( UB *buff )
        tm_putstring( const UB *buff )
        tm_printf( const char *format, ... )  (ライブラリ)

    以下のシステムコールは、T-Monitor に入りコンソールからのコマンド入力待ちと
    なります。"g" コマンドで T-Monitor から戻ります。

        tm_monitor( )

    以下のシステムコールは、T-Monitor のコマンドを実行してすぐに戻ります。

        tm_command( const UB *buff )

    拡張システムコールとしては、LED 操作に対応しています。

        tm_extsvc( 0x11, v, 0, 0 )

    また、以下のシステムコールは、T-Monitor 内で無限ループに入りますので、戻っ
    てきません。

        tm_exit( INT mode )

    なお、シリアルポートの通信速度は、ROM 情報に設定しますので、以下の定義を
    変更して、再ビルドすることにより変更可能です。

        config\src\sysdepend\asp_leon5\rominfo_conf.h

        #define RI_DBGPORT_BPS  (UW)19200

6.3 T-Kernel

    T-Engine フォーラムから公開されている「T-Kernel 2.0ソースコードパッケージ
    Ver 2.01.03」を CPU の違い、開発環境の違い、を反映して移植しています。

    Makefile はなく、Eclipse の internal builder を使用しています。
    その関係で、ファイルの配置やファイル名(拡張子)を変更したものがあります。

    リソース数を変更する場合は、以下の定義を変更して、再ビルドしてください。

        config\src\sysdepend\asp_leon5\sysconf.c

------------------------------------------------------------------------------
7. T2AS : T-Kernel 機能制限
------------------------------------------------------------------------------

T2AS では、T-Kernel のシステムコールのうち、実際には使用しないシステムコールの
処理を切り離して、全体のコードサイズを小さくするとともに信頼性を向上することが
できます。

このためには、以下の手順が必要になります。

  (1) 使用するシステムコールを以下の設定ファイルで設定してください。

      システムコール単位での指定
          使用しないシステムコールに対応する USE_FUNC_xxxx の定義を削除して
          ください。

                config\tk_config_func.h

      機能単位での指定
          使用しない機能に対応する NUM_xxxx の定義を削除してください。
          デバッグ機能を使用しない場合は、USE_DBGSPT を 0 に設定してください。

                kernel\tkernel\src\config.h

      ※ 設定ファイルに関しては、以下のファイルを参照してください。
           フル構成 (*.org)
                config\tk_config_func.h.org
                kernel\tkernel\src\config.h.org
           T2AS 標準構成 (*.std)
                config\tk_config_func.h.std
                kernel\tkernel\src\config.h.std

      ※ 本リリースではフル構成の設定ファイルになっています。つまり *.h
         ファイル内容は、*.h.org と同じです。

  (2) 全プロジェクトをクリーンし、その後、全ビルドを行ってください。

      ※ 削除したシステムコールを実行すると、E_RSFN のエラーとなります。
         ただし、ライブラリとして実装されている一部の関数はリンクエラーとなり
         ます。


使用しないシステムコールを完全に削除して、使用した場合にリンクエラーとするため
には、上記 (1)の設定を行った状態で、さらに以下の手順を行ってください。
この場合、使用するシステムコールの機能番号は詰めて割り当てられるため、システム
コールの分岐用テーブルのサイズが小さくなります。

  (A) 以下のヘッダファイル内の使用しないシステムコールの宣言を削除します。

        include\tk\syscall.h
        include\tk\devmgr.h
        include\sys\segment.h

  (B) cygwin または linux 環境で、以下の操作を行います。

        $ cd lib/libsvc/build_src/leon5
        $ make clean_source
        $ make source

     以下のファイルが生成されます。
         lib/libsvc/src/sysdepend/include/*.h, *.inc
         lib/libsvc/src/sysdepend/leon5/*.S, *.c

  (C) 生成した lib/libsvc/src/sysdepend/include/*.h, *.inc をすべて、
      include/sys/svc/*.h, *.inc にコピーします。

        $ cd lib/libsvc/src/sysdepend/include
        $ cp * ../../../../../include/sys/svc/


------------------------------------------------------------------------------
8. T2AS : T-Kernel 2.0 Extension 機能制限
------------------------------------------------------------------------------

T2AS では、T-Kernel 2.0 Extension (T2EX) の機能のうち、以下の機能は使用で
きません。

    ディレクトリ、ファイル            機能
    kernel/extension/memory/t2ex/     T2EX用メモリマネージャ
    t2ex/fs/                          T2EXファイル管理機能
    t2ex/load/                        T2EXプログラムロード機能
    t2ex/lib/libc/                    標準C互換ライブラリ (※)
    t2ex/lib/libtk/                   T-Kernel 関数コールライブラリ（差分）
    t2ex/lib/libusermod/              T2EXプログラムモジュール用ライブラリ
    module/                           T2EXプログラムモジュールサンプル/ビルド用

    ※ libc の構成ファイルうち、inet_ntop.c とinet_pton.c の２つファイルは T2EX
      ネットワーク機能から使用されるため、t2ex/network/net/src_bsd/arpa_inet に
     ファイルをコピーしています。

以下のファイルは、T2AS の元のソースから T2EX 用に更新、もしくは削除しています。

    kernel/sysdepend/cpu/em1d/cpu_conf.h        [*更新*]
    kernel/sysmgr/src/imalloc.c                 [*更新*]

以下のフォルダはT2EX 用に追加されており、T2AS の元のソースからこちらのソースを
ビルドするようにプロジェクトの設定を変更しています。

    config/src_t2ex/sysdepend/asp_leon5/
    usermain_t2ex/


------------------------------------------------------------------------------
(参考) 開発ツール
------------------------------------------------------------------------------

開発ツールの入手およびインストール方法などは、下記 URL を参照してください。
また、ここには基本的な使い方の説明書なども掲載されていますので、合わせてご参
照ください。
本書内で参照している説明書も入手できます。

・BCC2: Bare-C Cross-Compiler System for LEON2/3/4/5 GCC 10.2.0
https://www.gaisler.com/index.php/downloads/compilers

・GRMON 3.3
https://www.gaisler.com/index.php/downloads/debug-tools

・Eclipse IDE for C/C++ Developers
https://www.gaisler.com/index.php/products/compiler?option=com_content&task=view&id=148
https://www.eclipse.org/downloads/packages/release/2021-06/r/eclipse-ide-cc-developers

・LEON-XCKU
https://www.gaisler.com/index.php/products/processors/leon-examples/leon-xcku



------------------------------------------------------------------------------
A.1 ライセンス
------------------------------------------------------------------------------

T-Kernel 2.0は、トロンフォーラムがT-License2.2に従って配布するソースコードです。
T2EX(T-Kernel 2.0 Extension)は、トロンフォーラムがT-License2.2に従って配布する
ソースコードです。
TKSE(T-Kernel Standard Extension)は、トロンフォーラムがT-License2.0に従って配布
するソースコードです。

T-License2.2、T-License2.0の内容は以下をご覧ください。

・T-License2.2
https://www.tron.org/download/index.php?route=information/information&information_id=79

・T-License2.0
https://www.tron.org/download/index.php?route=information/information&information_id=40


------------------------------------------------------------------------------
A.2 ソースコード
------------------------------------------------------------------------------

本リポジトリに含まれるソースコードは、T-License2.2に規定された再配布の条件に
従ってユーシーテクノロジ株式会社(以下、UCT)が無償で再配布しています。

本リポジトリには、UCTが開発した以下のソースコードが含まれます。

- main : T-Kernel 2.0 Aerospace for ASP-SH7750R
- asp_leon5 : T-Kernel 2.0 Aerospace (T2AS) for LEON-XCKU
- asp_leon5-network : T-Kernel 2.0 Aerospace (T2AS) for LEON-XCKU with LAN driver

UCTは、UCTが開発したソースコードを含めて、本リポジトリに含まれるソースコードに
ついてT-License2.2第３条第3項に定める利用を認めます。ただし、UCTが開発したソー
スコードについて、T-License2.2第３条第3項の第４号、第５号における有償での再配布
は認めません。


------------------------------------------------------------------------------
A.3 注意事項
------------------------------------------------------------------------------

・本リポジトリに含まれるソースコード(付随するコメント、ドキュメンテーションを
　含む)を「本ソースコード」といいます。
・本ソースコードにはT-License2.2が適用されることを確認します。
・本ソースコードは全て無保証で提供するものであり、その適用可能性も含めて、
　UCTはいかなる保証も行いません。
・本ソースコードを利用することにより直接的または間接的に生じたいかなる損害に
　関しても、UCTは一切の責任を負いません。
・本ソースコードの仕様、実装にいかなる欠陥が発見されたとしても、UCTには対応
　義務はありません。
・本ソースコードの仕様、実装は予告なく変更されることがあります。
・本ソースコードの仕様、実装の変更に伴って利用者にいかなる損害が発生したとし
　ても、UCTは一切の責任を負いません。
・本ソースコードに対するライセンス料、保守費用、ロイヤリティは不要です。
・本ソースコードに関するお問い合わせは一切お受けできません。お問い合わせいた
　だいても回答することはありません。
以上
