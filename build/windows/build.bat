cmake . -B ./build/bin
IF "%1"=="" (
    cmake --build ./build/bin
) ELSE (
    cmake --build ./build/bin --config %1
)
