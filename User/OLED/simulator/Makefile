# Makefile for OLED Enhanced Simulator
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g -O2
LIBS = -lSDL2 -lm
TARGET = oled_simulator_enhanced
SOURCE = oled_simulator_enhanced.c
SRC_DIR = .
BUILD_DIR = build

# 检查操作系统
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
    LIBS += -lSDL2
endif
ifeq ($(UNAME_S),Darwin)
    LIBS += -lSDL2
endif

.PHONY: all clean run

all: $(BUILD_DIR) $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE) $(LIBS)
	@echo "编译完成: $(TARGET)"

clean:
	rm -f $(TARGET)
	rm -rf $(BUILD_DIR)
	@echo "清理完成"

run: $(TARGET)
	./$(TARGET)

install-deps:
	@echo "安装依赖..."
ifeq ($(UNAME_S),Linux)
	@echo "在Ubuntu/Debian上: sudo apt-get install libsdl2-dev"
	@echo "在CentOS/Fedora上: sudo yum install SDL2-devel"
	@echo "在Arch上: sudo pacman -S sdl2"
endif
ifeq ($(UNAME_S),Darwin)
	@echo "在macOS上: brew install sdl2"
endif

help:
	@echo "可用目标:"
	@echo "  all        - 编译模拟器"
	@echo "  clean      - 清理编译文件"
	@echo "  run        - 编译并运行模拟器"
	@echo "  install-deps - 显示安装依赖的说明"
	@echo "  help       - 显示此帮助信息"