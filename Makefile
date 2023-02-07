SRCS = donut precomputed
BUILD_DIR = build

all: $(SRCS)

CFLAGS := -Wall
LDFLAGS := -lm

$(SRCS):
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $@.c -o $(BUILD_DIR)/$@ $(LDFLAGS) 

clean:
	rm -rf $(BUILD_DIR)