NAME:=mnoter
TYPE:=EXE # This can be either 'EXE' or 'LIB'
LIBS:=msl
ELIBS:=

TARGET:=$(shell if [ ${TYPE} == "LIB" ]; then\
	echo "lib${NAME}.a";\
else\
	echo "${NAME}";\
fi)
SRCDIR:=src
OBJDIR:=../${OBJDIR}
MYOBJDIR:=${OBJDIR}/${NAME}
BINDIR:=../${BINDIR}
MYBINDIR:=${BINDIR}/${NAME}
BINPATH:=${MYBINDIR}/${TARGET}
BUILDDIR:=../${BUILDDIR}

CPPS:=$(shell ls ${SRCDIR}/*.cpp)
OBJS:=$(subst ${SRCDIR},${OBJDIR}/${NAME},${CPPS})
OBJS:=$(subst .cpp,.o,${OBJS})
HEADERS:=$(shell ls ${SRCDIR}/*.hpp)

LIBFLAGS:=$(shell for i in ${LIBS}; do echo "-L${BINDIR}/$$i"; done) $(shell for i in ${LIBS} ${ELIBS}; do echo "-l$$i"; done) $(shell for i in ${LIBS}; do echo "-I../$$i/src"; done)

CFLAGS:=${CFLAGS} ${LIBFLAGS} -I../msl/src/ -I../msl/src/msl -I../msl/src/msl/String -I../msl/src/msl/Timer -I../msl/src/msl/Vec

printStatus=@echo -e "${BOLDGREEN}[$1]:${BOLDCYAN}$2${RESET}"

.PHONY: all clean
all: directoryStruct ${BINPATH}
	$(call printStatus,COMPLETED, ${NAME})
	$(beautify)

${BINPATH}: ${OBJS}
	$(call printStatus,LINKING, ${OBJS})
	$(beautify)
	if [ ${TYPE} = "EXE" ]; then\
		${CXX} ${OBJS} ${CFLAGS} ${OFLAG} $@;\
	else\
		${AR} $@ ${OBJS};\
	fi

${MYOBJDIR}/%.o: ${SRCDIR}/%.cpp ${HEADERS}
	$(call printStatus,COMPILING, $<)
	$(beautify)
	${CXX} $< -c ${CFLAGS} ${OFLAG} $@

directoryStruct:
	@if [ ! -d "${BUILDDIR}" ]; then mkdir -p ${BUILDDIR}; fi
	@if [ ! -d "${OBJDIR}"   ]; then mkdir -p ${OBJDIR}; fi
	@if [ ! -d "${BINDIR}"   ]; then mkdir -p ${BINDIR}; fi
	@if [ ! -d "${MYBINDIR}" ]; then mkdir -p ${MYBINDIR}; fi
	@if [ ! -d "${MYOBJDIR}" ]; then mkdir -p ${MYOBJDIR}; fi

clean:
	@${RM} ${MYOBJDIR}/* ${BINPATH}
	$(call printStatus,CLEANED, ${NAME})
