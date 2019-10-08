if [ "$#" -eq "0" ]
then
	cmake . -B ./build/bin 
else
	cmake . -B ./build/bin -DCMAKE_BUILD_TYPE=$1
fi
cmake --build ./build/bin
