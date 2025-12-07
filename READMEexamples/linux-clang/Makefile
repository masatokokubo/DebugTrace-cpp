PROJECT_ROOT = $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
SRCDIR       = $(PROJECT_ROOT)src

CC  := clang
CXX := clang++

OBJS = readme-example.o

BUILD_MODE = debug

CFLAGS += -I../../../../include
#CFLAGS += --std=c++14
#CFLAGS += --std=c++17
#CFLAGS += --std=c++20
CFLAGS += --std=c++23
CFLAGS += -D DEBUG
CFLAGS += -Wvla-cxx-extension
ifeq ($(BUILD_MODE),debug)
    CFLAGS += -g -O0
else ifeq ($(BUILD_MODE),run)
    CFLAGS += -O2
else ifeq ($(BUILD_MODE),profile)
    CFLAGS += -g -pg --coverage
    LDFLAGS += -pg --coverage
    EXTRA_CLEAN += debugtrace-example.gcda debugtrace-example.gcno $(PROJECT_ROOT)gmon.out
    EXTRA_CMDS = rm -rf debugtrace-example.gcda
else
    $(error Build mode $(BUILD_MODE) not supported by this Makefile)
endif

all: debugtrace-example

debugtrace-example: $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^
	$(EXTRA_CMDS)

%.o: $(SRCDIR)/%.cpp
	$(CXX) -c $(CFLAGS) $(CXXFLAGS) $(CPPFLAGS) -o $@ $<

%.o: $(SRCDIR)/%.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) -o $@ $<

clean:
	rm -fr debugtrace-example $(OBJS) $(EXTRA_CLEAN)
