

OBJ = libdsaa.o
SRC = src/libdsaa.c
CFLAGS = -fPIC -Wall
LDFLAGS = -shared
TARGET = libdsaa.so
BUILD_DIR = build
PREFIX=/usr/local

OBJ_ = $(patsubst %,${BUILD_DIR}/%,$(OBJ))
TARGET_ = $(patsubst %,${BUILD_DIR}/%,$(TARGET))

all: CFLAGS += -O2
all: ${TARGET}

${TARGET}: $(OBJ_)
	$(CC) ${LDFLAGS} -o ${TARGET_} $<

$(OBJ_): ${SRC}
	mkdir -p ${BUILD_DIR}
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -fr ${BUILD_DIR}

debug: CFLAGS += -DDEBUG -ggdb -O0
debug: ${TARGET}

.PHONY: $(OBJ_)

install:
	install -m 644 ${TARGET_} ${PREFIX}/lib
	install -m 644 include/libdsaa.h ${PREFIX}/include
