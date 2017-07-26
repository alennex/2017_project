#!/bin/sh

i=1
until [ $i -gt 100 ]
do
	echo $i
	s="$i.png"
	cp patch128.png $s
	i=`expr $i + 1`
done
