EXEC:=mnoter
TARGETS:=msl ${EXEC} # Any additional library should be add before executable
DEFS:=
ARGS:=r 2

export SHELL:=/bin/bash
DEBUGFLAG:=-g
CMAKEDEBUGFLAG:=--config Debug
export CFLAGS:=-Wall -std=c++20 ${DEBUGFLAG} $(shell for i in ${DEFS}; do echo "-D$$i"; done)
export OFLAG:=-o
export RM:=rm -rf
export AR:=ar rcs

export BUILDDIR:=build
export OBJDIR:=${BUILDDIR}/obj
export BINDIR:=${BUILDDIR}/bin

MAKEFLAGS+=--no-print-directory -j -s

export BOLDGREEN:=\x1b[1;32m
export BOLDCYAN:=\x1b[1;36m
export GREEN:=\x1b[0;32m
export MAGENTA:=\x1b[0;35m
export RESET:=\x1b[0m

BEAUTIFYCOLOR:=${MAGENTA}

export beautify=@echo -en "${BEAUTIFYCOLOR}"
printStatus=@echo -e "${BOLDGREEN}[$1]:${BOLDCYAN}$2${RESET}"

.PHONY: all run clean
all:
	@for i in ${TARGETS}; do\
		if [ -f "$$i/CMakeLists.txt" ]; then\
			if [ ! -d "${BINDIR}/$$i" ]; then ${BINDIR}/$$i; fi;\
			cmake -S $$i/ -B ${BINDIR}/$$i/ -G Ninja;\
			cmake --build ${BINDIR}/$$i ${CMAKEDEBUGFLAG};\
			echo -e "\x1b[37;1m--------------------------------------------------\x1b[0m";\
		else\
			$(MAKE) -C $$i/;\
		fi;\
	done
	$(call, printStatus,COMPLETED, everything)
	echo -e "${RESET}"

run: all
	$(call, printStatus,RUNNING, "${EXEC} with args ${ARGS}")
	@${BINDIR}/${EXEC}/${EXEC} ${ARGS}
	@${BINDIR}/${EXEC}/${EXEC} show

clean:
	@for i in ${TARGETS}; do\
		if [ -f "$$i/CMakeLists.txt" ]; then\
			cmake --build ${BINDIR}/$$i/ --target clean >> /dev/null;\
			echo -e "${BOLDGREEN}[CLEANED]:${BOLDCYAN} $$i";\
		else\
			$(MAKE) -C $$i/ clean;\
		fi;\
	done

install: all
	sudo cp ./build/bin/mnoter/mnoter /usr/bin/
	echo -e "${BOLDGREEN}[INSTALLED]: ${BOLDCYAN}MNoter to /usr/bin/${RESET}"
