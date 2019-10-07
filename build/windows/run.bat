cmake . -B ./build/bin
IF "%1"=="" (
    cmake --build ./build/bin
    build\bin\main\Debug\main.exe
) ELSE (
    cmake --build ./build/bin --config %1
    build\bin\main\%1\main.exe
)
