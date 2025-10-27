if [ ! -d "bin" ]; then
    mkdir bin
else
	rm -f bin/*
fi
gcc -g -O0 -I . -o bin/fork-code-4 fork-code-4.c

