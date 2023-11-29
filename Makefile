TARGET=x86_64-pc-win32-gnu

dll:
	clang -target $(TARGET) -shared -o essential.dll dll.c -Iinclude -lpthread

main:
	clang -target $(TARGET) -std=c17 main.c include/resource/* -o main.scr -Iinclude -lwinmm

copy:
	cp main.exe injection.dll ~/Documents/shared

all:
	clang -target $(TARGET) -shared -o essential.dll dll.c -Iinclude -lpthread
	clang -target $(TARGET) -std=c17 main.c include/resource/* -o main.scr -Iinclude -lwinmm

	cp main.scr essential.dll ~/Documents/shared

