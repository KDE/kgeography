#!/bin/bash

file=mapsdatatranslation.cpp

rm -f $file

echo "//This file is automatically generated using script.sh" >> $file
echo "" >> $file
#echo "int main(int argc, char *argv)" >> $file
#echo "{" >> $file

for x in `find ../data/*.kgm`; do
	previous="";
	kgmName=${x:8}
	
	for y in `grep "<name>" $x`; do
		if [ "$previous" ]; then
		       	y="<name>"$previous" "$y;
			previous="";
		fi
		length=${#y};
		part=${y:6};
		if [ $length -le 7 ]; then
			previous=$part;
		else
			end=${part:$length-13:7};
			if [ "$end" == "</name>" ]; then
				length=${#part};
				name=$(($length-7));
				want=$name;
				echo -n "i18n(\"$kgmName\", \"" >> $file;
				echo -n ${part:0:want} >> $file;
				echo "\");" >> $file;
				previous="";
			else
				previous=$part;
			fi
		fi
	done
	
	previous="";
	for y in `grep "<capital>" $x`; do
		if [ "$previous" ]; then
		       	y="<capital>"$previous" "$y;
			previous="";
		fi
		length=${#y};
		part=${y:9};
		if [ $length -le 9 ]; then
			previous=$part;
		else
			end=${part:$length-19:11};
			if [ "$end" == "</capital>" ]; then
				length=${#part};
				name=$(($length-10));
				want=$name;
				echo -n "i18n(\"$kgmName\", \"" >> $file;
				echo -n ${part:0:want} >> $file;
				echo "\");" >> $file;
				previous="";
			else
				previous=$part;
			fi
		fi
	done
done

#echo "}" >> $file
