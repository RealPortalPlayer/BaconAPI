# Configuration

CC=gcc
LD=ld
AR=ar
BUILD_ROOT=build_make
OUT=${BUILD_ROOT}/out
OBJECTS=${BUILD_ROOT}/objects

.PHONY: all baconapi test variables createbuild help
.PRECIOUS: ${OBJECTS}/test/%Test.o

# Globbed files

BACONAPI_SOURCE_FILES:=$(filter-out ./source/Hacks/Windows.c,$(shell find ./source -type f -name "*.c"))
BACONAPI_OBJECT_FILES:=$(patsubst ./source/%.c,${OBJECTS}/BaconAPI/%.o,${BACONAPI_SOURCE_FILES})
BACONAPI_OBJECT_DIRECTORIES:=$(sort $(filter-out $(OBJECTS)/BaconAPI/,$(dir ${BACONAPI_OBJECT_FILES})))
BACONAPI_DEPENDENCIES:=$(wildcard $(BACONAPI_OBJECT_FILES:%.o=%.d))

TESTS_SOURCE_FILES:=$(shell find ./test -type f -name "*.c")
TESTS_OBJECT_FILES:=$(patsubst ./test/%.c,${OBJECTS}/test/%Test.o,${TESTS_SOURCE_FILES})
TESTS_OUTPUT_FILES:=$(patsubst ${OBJECTS}/test/%Test.o,${OUT}/%Test,${TESTS_OBJECT_FILES})
TESTS_DEPENDENCIES:=$(wildcard $(TESTS_OBJECT_FILES:%.o=%.d))

# Targets

all: baconapi test

baconapi: createbuild $(BACONAPI_OBJECT_FILES) $(OUT)/libBaconAPI.a

test: createbuild baconapi $(TESTS_OUTPUT_FILES)

variables:
	@echo "CC=${CC}"
	@echo "LD=${LD}"
	@echo "AR=${AR}"
	@echo "BUILD_ROOT=${BUILD_ROOT}"
	@echo "OUT=${OUT}"
	@echo "OBJECTS=${OBJECTS}"

clean:
	$(RM) -r ${BUILD_ROOT}

help:
	@echo "all: Build all the projects"
	@echo "baconapi: Build BaconAPI"
	@echo "test: Build tests"
	@echp "variables: Show the set variables"
	@echo "help: Information about each target"

# Internal

createbuild: ${OUT} ${OBJECTS} ${BACONAPI_OBJECT_DIRECTORIES} ${OBJECTS}/test

${OUT} ${OBJECTS} ${BACONAPI_OBJECT_DIRECTORIES} ${OBJECTS}/test:
	mkdir -p $@

${OBJECTS}/BaconAPI/%.o: ./source/%.c
	$(CC) -MMD ${CFLAGS} -c $< -o $@ -I./include

$(OUT)/libBaconAPI.a:
	$(AR) -rcs $@ ${BACONAPI_OBJECT_FILES}

${OBJECTS}/test/%Test.o: ./test/%.c
	$(CC) -MMD ${CFLAGS} -c $< -o $@ -I./include

${OUT}/%Test: ${OBJECTS}/test/%Test.o
	$(CC) ${LDFLAGS} $< -lBaconAPI -o $@ -L${OUT}

include $(BACONAPI_DEPENDENCIES)
include $(TESTS_DEPENDENCIES)
