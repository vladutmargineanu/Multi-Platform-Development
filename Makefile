CC = cl
CFLAGS = /nologo /MD /W3 /EHsc /Za /D_CRT_SECURE_NO_DEPRECATE
EXE = so-cpp.exe

build: tema01.obj hashTable.obj
        $(CC) $** $(CFLAGS) /Fe$(EXE)

tema01.obj: tema01.c

hashTable.obj: hashTable.c

run: build
	./$(EXE)

clean:
        del *.obj *.exe

