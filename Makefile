#  lang: ridiculous language
#
#  Copyright: (c) 2012 Olivier Guittonneau <OpenMEngine@gmail.com>
#
#  This program is free software; you can redistribute it and/or
#  modify it under the terms of the Do What The Fuck You Want To
#  Public License, Version 2, as published by Sam Hocevar. See
#  http://sam.zoy.org/projects/COPYING.WTFPL for more details.

# I hate makefiles, this is why that one is so crappy

#CC=LC_ALL=en_US.UTF-8 colorgcc
#CC=gcc
CC=clang

# hardcore mode, I save it for later :)
# http://blogs.gnome.org/otte/2008/12/22/warning-options/
# http://mces.blogspot.fr/2008/12/year-end-cleaning-ie-on-warning-options.html
# http://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html
CFLAGS=-Wall -Wextra -fno-common -Wdeclaration-after-statement \
-Wformat=2 -Winit-self -Winline -Wpacked -Wp,-D_FORTIFY_SOURCE=2 \
-Wpointer-arith -Wlarger-than-65500 -Wmissing-declarations \
-Wmissing-format-attribute -Wmissing-noreturn -Wmissing-prototypes \
-Wnested-externs -Wold-style-definition -Wredundant-decls \
-Wsign-compare -Wstrict-aliasing=2 -Wstrict-prototypes -Wswitch \
-Wundef -Wunreachable-code \
#-Wunsafe-loop-optimizations \
-Wwrite-strings #-ansi -pedantic

# easy version
#CFLAGS=-Wall -Wextra

LDFLAGS=

INC=include/

all: bin/zcomp

bin/zcomp: src/*.c include/*.h
	@$(CC) $(CFLAGS) -o bin/zcomp src/*.c -I$(INC) && echo "Compiling zcomp: \033[1;32mOK\033[0m"

tests: bin/zcomp
	@for f in tests/*.z; \
	do \
	echo -n "Testing `basename $$f`: "; \
		if ./bin/zcomp $$f > /dev/null; \
		then echo "\033[1;32mOK\033[0m"; \
		else echo "\033[1;31mKO\033[0m"; \
		fi; \
	done

clean:
	@rm -rf bin/*

.PHONY: all clean tests
