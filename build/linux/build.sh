cmake . -B ./build/bin
if [ "$#" -eq "0" ]
then
    cmake --build ./build/bin
else
    cmake --build ./build/bin --config "$1"
fi
