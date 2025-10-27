if [ ! -d "bin" ]; then
    mkdir bin
else
	rm -f bin/*
fi
gcc -g -O0 -I . -o bin/fork-code-2 fork-code.c

