#!/bin/sh

# Usage:
#	Release build:  ./build.sh
#	Debug build:    ./build.sh debug
#	Clean:          ./build.sh clean
#
#	Run the result: ./bin/impromptu

if [ ! -d bin ]; then
	if [ -e bin ]; then
		echo -e "\tError: 'bin' exists and is not a directory."
		exit
	fi

	mkdir bin
fi

cd bin

if [ "$1" = "clean" ]; then
	rm -rf *
	cd ..
	rmdir bin
	exit
elif [ "$1" = "debug" ]; then
	cmake -DCMAKE_BUILD_TYPE=Debug ..
else
	cmake -DCMAKE_BUILD_TYPE=Release ..
fi

make impromptu -j5
