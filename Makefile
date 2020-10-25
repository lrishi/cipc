
ARCH=$(shell uname -m)
SRC_DIR=src
INCLUDE_DIR=include
BUILD_DIR=build
LIB_DIR=$(BUILD_DIR)/lib
OBJ_DIR=$(BUILD_DIR)/obj-$(ARCH)
BUILD_HDR_DIR=$(BUILD_DIR)/include

CFLAGS := -Wall -g -I$(INCLUDE_DIR)/
LDFLAGS := -lpthread

HEADERS := $(wildcard $(INCLUDE_DIR)/*.h)
SOURCES := $(wildcard $(SRC_DIR)/*.c)
OBJECTS_1 := $(SOURCES:.c=.o)
OBJECTS := $(subst $(SRC_DIR), $(OBJ_DIR), $(OBJECTS_1))


$(LIB_DIR)/libcipc.so: $(LIB_DIR) $(OBJ_DIR) $(OBJECTS) $(BUILD_DIR)/include/cipc.h
	$(CC) $(CFLAGS) -fPIC -shared $(OBJECTS) -o $@ $(LDFLAGS)
	@echo "\n**** Build Successful ****"
	@echo "Run 'sudo make install' to install system-wide"
	@echo "Artifacts available in ./build/ directory"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(BUILD_HDR_DIR)/cipc.h: $(BUILD_HDR_DIR)
	cp $(INCLUDE_DIR)/cipc.h $(BUILD_HDR_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)
$(LIB_DIR):
	mkdir -p $(LIB_DIR)

$(BUILD_HDR_DIR):
	mkdir -p $(BUILD_HDR_DIR)

all: $(LIB_DIR)/libcipc.so

install:
	mkdir -p /usr/include/cipc
	mkdir -p /usr/lib/cipc
	cp $(BUILD_HDR_DIR)/* /usr/include/cipc/
	cp $(LIB_DIR)/* /usr/lib/cipc/
	@echo Verifying Install...
	$(CC) etc/cipc_test_install.c -o /dev/null -lcipc
	@echo Installation completed successfully

clean:
	@echo Cleaning unconditionally...
	rm -rf build
	@echo "*** Cleanup successful ***"
