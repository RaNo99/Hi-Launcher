PATH=/bin
make -s -f compile.mak #2>&1 | grep -v -fgrep_skip_make.txt
echo 
