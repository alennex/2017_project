#!/bin/sh  
  
exe="libqxcb.so" #发布的程序名称  
des="/home/alennex/project/2017_project/qt/test-v0/test-portable" #你的路径  
  
deplist=$(ldd $exe | awk  '{if (match($3,"/")){ printf("%s "),$3 } }')  
cp $deplist $des 
