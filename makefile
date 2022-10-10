

cc = gcc

define flags
	-Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef
	-Wfloat-equal -Winline -Wunreachable-code -Wmissing-declarations
	-Wmissing-include-dirs -Wswitch-enum -Wswitch-default -Weffc++ -Wmain
	-Wextra -Wall -g -pipe -fexceptions -Wcast-qual -Wconversion -Wctor-dtor-privacy
	-Wempty-body -Wformat-security -Wformat=2 -Wignored-qualifiers -Wlogical-op
	-Wmissing-field-initializers -Wnon-virtual-dtor -Woverloaded-virtual
	-Wpointer-arith -Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing
	-Wstrict-null-sentinel -Wtype-limits -Wwrite-strings -D_DEBUG -D_EJUDGE_CLIENT_SIDE
endef

define compile
	$(sc) $(cc) $1 $2 -o $3 $(flags)
endef

sc = @ #showcommands
default_name = prog



all: allobjects
	$(call compile, mainasm.o,    asm.o,    asm)
	$(call compile, maindisasm.o, disasm.o, disasm)
	$(call compile, mainproc.o,   proc.o,   proc)



asm: mainasm.o asm.o
	$(call compile, mainasm.o,    asm.o,    default_name)

disasm: maindisasm.o disasm.o
	$(call compile, maindisasm.o, disasm.o, default_name)

proc: mainproc.o proc.o
	$(call compile, mainproc.o,   proc.o,   default_name)



allobjects: asm.o mainasm.o disasm.o maindisasm.o proc.o mainproc.o

asm.o: asm.cpp
	$(sc) $(cc) -c asm.cpp -o asm.o $(flags)

mainasm.o: mainasm.cpp
	$(sc) $(cc) -c mainasm.cpp -o mainasm.o $(flags)

disasm.o: disasm.cpp
	$(sc) $(cc) -c disasm.cpp -o disasm.o $(flags)

maindisasm.o: maindisasm.cpp
	$(sc) $(cc) -c maindisasm.cpp -o maindisasm.o $(flags)

proc.o: proc.cpp
	$(sc) $(cc) -c proc.cpp -o proc.o $(flags)

mainproc.o: mainproc.cpp
	$(sc) $(cc) -c mainproc.cpp -o mainproc.o $(flags)



clean:
	rm -rf *.o prog

