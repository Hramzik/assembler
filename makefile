

cc = gcc

define flags
	-Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal -Winline -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default -Weffc++ -Wmain -Wextra -Wall -g -pipe -fexceptions -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wempty-body -Wformat-security -Wformat=2 -Wignored-qualifiers -Wlogical-op -Wmissing-field-initializers -Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wsign-promo -Wstrict-aliasing -Wstrict-null-sentinel -Wtype-limits -Wwrite-strings -D_DEBUG -D_EJUDGE_CLIENT_SIDE
endef

define compile
	$(sc) $(cc) $1 $2 -o $3 $(flags)
endef

sc = @ #showcommands
default_name = prog



all:
	$(call compile, mainasm.cpp,      asm.cpp,      asm)
	$(call compile, maindisasm.cpp,   disasm.cpp,   disasm)
	$(call compile, mainproc.cpp,     proc.cpp,     proc)
	$(call compile, maingenerate.cpp, generate.cpp, generate)



asm:
	$(call compile, mainasm.cpp,      asm.cpp,      $(default_name) )

disasm:
	$(call compile, maindisasm.cpp,   disasm.cpp,   $(default_name) )

proc:
	$(call compile, mainproc.cpp,     proc.cpp,     $(default_name) )

generate:
	$(call compile, maingenerate.cpp, generate.cpp, $(default_name) )

test:
	$(call compile, test.cpp,                   ,   $(default_name) )