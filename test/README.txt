Cutlibテストプログラム
======================

内容
----

  test1  セル中心版/ノード間版テストプログラム

  test2  Octree版テストプログラム

  test3  配列インタフェース(セル中心版/ノード間版)テストプログラム


コンパイル・リンク方法
----------------------

  Makefileのコメントを参照ください．


ファイル・ディレクトリ構成
--------------------------

  test1.cpp, test1Config.h  test1プログラム

  test2.cpp, test2Config.h  test2プログラム 

  test3.cpp, test3Config.h, test3Check.h  test3プログラム 

  outputVtk.h, outputVtk.cpp  結果出力関数(全プログラム共通)
 
  Config.h, Config.cpp  入力パラメータクラス(全プログラム共通)

  test*.conf  設定ファイルサンプル

  *.xml  Polilib初期設定ファイルサンプル

  Makefile  メイクファイル

  deta/  検証計算入力データ

