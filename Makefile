INCL_DIR=include
SRC_DIR=src/npshell
SRCS=${wildcard *.c ./${SRC_DIR}/*.cpp}
INCLS=${wildcard *.h ./${INCL_DIR}/*.h}
EXEC=npshell

all: npshell

npshell: ${SRCS} ${INCLS}
	g++ -g ${SRCS} -I${INCL_DIR} -o ${EXEC} -std=c++11

build:
	g++ -c ${SRCS}

clean:
	@rm -f npshell