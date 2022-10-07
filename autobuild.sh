#!/bin/bash

set -ueo pipefail

install=0
run=0
clean=0

config_file=build/.xconfig

for var in $@; do
	if [ $var == run ]; then
		run=1
	fi
	if [ $var == install ]; then
		install=1
	fi
	if [ $var == clean ]; then
		clean=1
	fi
done

if [ $clean == 1 ]; then
	rm -r build
fi

if [ -d xtodo/ ]; then
	cp resources/icon.ico xtodo/icon.ico
	if [ -d build ]; then
		echo "Existing build dir, skipping initialisation."
	else 
		mkdir build
		echo "First task"  > build/todo.txt
		echo "#script config after init" > $config_file
		echo "CLIENT=GUI" > $config_file
		echo "FILE=todo.txt" > $config_file
	fi
	cmake -S xtodo/ -B build/
	cp resources/icon.ico build/icon.ico
	cd build/
	make
	if [ $install == 1 ]; then
		sudo mkdir -p /opt/xtodo
		sudo cp -r . /opt/xtodo/
	fi
	if [ $run == 1 ]; then
		./xtodo
	fi
fi

# Win-Build
# x86_64-w64-mingw32-cmake -S x++todo/ -B win-build/
