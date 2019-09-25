if ["$1" -eq "arm"]
then
    ./build/premake/premake5_arm gmake2
else   
    ./build/premake/premake5 gmake2
fi
if [ "$#" -eq  "0" ]
then
	make
else
	make config="$1"
fi
