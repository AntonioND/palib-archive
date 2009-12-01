DIRZ:= $(shell ls)

.PHONY: all clean
all:
	@for i in $(DIRZ); do if test -e $$i/Makefile ; then $(MAKE) -C $$i || { exit 1;} fi; done;
clean:
	@for i in $(DIRZ); do if test -e $$i/Makefile ; then $(MAKE)  -C $$i clean || { exit 1;} fi; done;
