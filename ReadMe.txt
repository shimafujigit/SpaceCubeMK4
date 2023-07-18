/*
 *----------------------------------------------------------------------
 *    UCT T-Kernel 2.0 DevKit tuned for ASP-SH7750R
 *
 *    Copyright (c) 2013 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 *
 *    Version:   1.00.00
 *    Released by UC Technology (http://www.uctec.com/) at 2013/01/31.
 *
 *    Changed by UC Technology at 2015/05/15.
 *----------------------------------------------------------------------
 */

本リリースは、T-Kernel 2.0 ( ASP-SH7750R ) 2013/06/14 版に対して、FPU 抑止例外
を使用せずに FPU のコンテキスト切り替えを行うように修正したバージョンである。

実際に FPU 命令を使用したかどうかにかかわらず、TA_FPU 属性を指定して生成された
タスクの切り替えの際に、無条件に FPUレジスタの保存/復帰を行う。

しがって、タスク切り替えの際のオーバーヘッドは増加するが、逆に FPU 抑止例外は
発生しないため、その例外処理のオーバーヘッドは無くなる。

この方式の場合、FPU を使用するタスクにのみ、TA_FPU 属性を指定することが望まれ
る。

今回のバージョンで変更したソースファイルは、以下の通りである。

  ├── include
  │   └── tk
  │       └── sysdepend
  │           └── asp_sh7750r
  │               ├── asm_depend.h
  │               └── asm_depend.inc
  ├── kernel
  │   └── sysdepend
  │       └── cpu
  │           └── sh7750r
  │               └── cpu_support.S
  ├── lib
  │   └── libtk
  │       └── src
  │           └── sysdepend
  │               └── asp_sh7750r
  │                   ├── disint.S
  │                   └── int.c
  └── monitor
       └── src
           └── asp_sh7750r
               └── eitent.S

また、実装仕様書 (impl-asp-sh7750r.txt ) も変更している。

