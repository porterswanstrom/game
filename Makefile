BINDIR=/usr/local/bin
BIN=game

all:
	cc -O2 -Wall -pthread ${BIN}.c -o ${BIN}
install:
	mkdir -p ${BINDIR}
	cp ${BIN} ${BINDIR}/
uninstall:
	rm -f ${BINDIR}/${BIN}
deinstall: uninstall
clean:
	rm -f ${BIN}
