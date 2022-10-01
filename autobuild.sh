#!/bin/bash

set -ueo pipefail

if [ -d xtodo/ ]; then
	cp resources/icon.ico xtodo/icon.ico
	if [ -d build ]; then
		echo "Existing build dir"
	else 
		mkdir build
		touch build/todo.txt
		./config.sh todo.txt > build/.x++config
	fi
	cmake -S xtodo/ -B build/
	cp resources/icon.ico build/icon.ico
	cd build/
	make

	for var in $@; do
		if [ $var == run ]; then
			./xtodo
		fi
		if [ $var == install ]; then
			cd ..
			if [ -d /opt/xtodo ]; then
				sudo cp -r build/* /opt/xtodo
			else
				sudo mkdir /opt/xtodo
				sudo cp -r build/* /opt/xtodo
			fi
			cd build
		fi
		if [ $var == clean ]; then
			cd ..
			rm -r build
		fi
	done
fi


# Win-Build
# x86_64-w64-mingw32-cmake -S x++todo/ -B win-build/
