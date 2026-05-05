# Makefile для Акинатора под nmake (Windows, MSVC)
CC = cl
CFLAGS = /W2 /WX /std:c11 /nologo
LDFLAGS =

BUILD = build
BIN = bin
SRC = src
TESTS = tests

# Списки объектных файлов
GAME_OBJS = $(BUILD)\tree.obj $(BUILD)\io.obj $(BUILD)\utils.obj $(BUILD)\main.obj $(BUILD)\game.obj
TEST_OBJS = $(BUILD)\tree.obj $(BUILD)\io.obj $(BUILD)\utils.obj $(BUILD)\test_tree.obj

# Цели
all: $(BIN)\akinator.exe

test: $(BIN)\test_runner.exe
	$(BIN)\test_runner.exe

clean:
	@if exist $(BUILD) rmdir /s /q $(BUILD)
	@if exist $(BIN) rmdir /s /q $(BIN)
	@echo Очистка завершена.

run: $(BIN)\akinator.exe
	$(BIN)\akinator.exe


# Правила компиляции каждого .c -> .obj
$(BUILD)\tree.obj: $(SRC)\tree.c $(SRC)\tree.h
	@if not exist $(BUILD) mkdir $(BUILD)
	$(CC) /c $(CFLAGS) /Fo$@ $(SRC)\tree.c

$(BUILD)\io.obj: $(SRC)\io.c $(SRC)\io.h
	@if not exist $(BUILD) mkdir $(BUILD)
	$(CC) /c $(CFLAGS) /Fo$@ $(SRC)\io.c

$(BUILD)\utils.obj: $(SRC)\utils.c $(SRC)\utils.h
	@if not exist $(BUILD) mkdir $(BUILD)
	$(CC) /c $(CFLAGS) /Fo$@ $(SRC)\utils.c

$(BUILD)\main.obj: $(SRC)\main.c $(SRC)\tree.h $(SRC)\io.h $(SRC)\utils.h
	@if not exist $(BUILD) mkdir $(BUILD)
	$(CC) /c $(CFLAGS) /Fo$@ $(SRC)\main.c

$(BUILD)\game.obj: $(SRC)\game.c $(SRC)\game.h $(SRC)\tree.h $(SRC)\io.h $(SRC)\utils.h
	@if not exist $(BUILD) mkdir $(BUILD)
	$(CC) /c $(CFLAGS) /Fo$@ $(SRC)\game.c

$(BUILD)\test_tree.obj: $(TESTS)\test_tree.c $(SRC)\tree.h $(SRC)\io.h $(SRC)\utils.h
	@if not exist $(BUILD) mkdir $(BUILD)
	$(CC) /c $(CFLAGS) /Fo$@ $(TESTS)\test_tree.c


# Линковка игры
$(BIN)\akinator.exe: $(GAME_OBJS)
	@if not exist $(BIN) mkdir $(BIN)
	$(CC) $(CFLAGS) $** /Fe$@

# Линковка тестов
$(BIN)\test_runner.exe: $(TEST_OBJS)
	@if not exist $(BIN) mkdir $(BIN)
	$(CC) $(CFLAGS) $** /Fe$@

.PHONY: all clean test run