# system configuration
CC = gcc -O
INSTALL = /usr/bin/install -m 755

#magic

DEPS = commands.h
SRCS_C = main.c
SHELLNAME = mmshell

myshell: $(SRCS_C) $(DEPS)
	$(CC) -o $(SHELLNAME) $(SRCS_C)

.PHONY: install
install: $(SHELLNAME)
	$(INSTALL) $(SHELLNAME) ~/bin/$(SHELLNAME)

.PHONY: clean
clean:
	-rm $(SHELLNAME)
