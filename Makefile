#
#  make: 
#     Cutlibライブラリを作成
#
#  make examples:
#     サンプルプログラムをコンパイル・リンク
#
#
# src, examplesの各ディレクトリ内でmakeも可能
#

.PHONY: cutlib examples

cutlib:
	cd src && make

examples:
	cd examples && make

clean:
	cd src && make clean
	cd examples && make clean

dist_clean:
	cd src && make dist_clean
	cd examples && make dist_clean

