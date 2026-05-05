#include "io.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define IO_MAX_INPUT 512

struct IO_interface {
    FILE* in;
    FILE* out;
    FILE* err;
};

static void trim_newline(char* s) {
    if (!s) return;

    size_t len = strlen(s);
    while (len > 0 && (s[len - 1] == '\n' || s[len - 1] == '\r')) {
        s[len - 1] = '\0';
        len--;
    }
}

static int read_line(IO_interface* io, char* buffer, size_t size) {
    if (!io || !buffer || size == 0) {
        return 0;
    }

    if (!fgets(buffer, (int)size, io->in)) {
        return 0;
    }

    trim_newline(buffer);
    return 1;
}

static char* read_allocated_line(IO_interface* io) {
    char buffer[IO_MAX_INPUT];

    while (1) {
        if (!read_line(io, buffer, sizeof(buffer))) {
            return NULL;
        }

        if (buffer[0] == '\0') {
            fprintf(io->out, "Ввод не должен быть пустым. Повторите: ");
            continue;
        }

        char* result = malloc(strlen(buffer) + 1);
        if (!result) {
            fprintf(io->err, "Ошибка: не удалось выделить память.\n");
            return NULL;
        }

        strcpy(result, buffer);
        return result;
    }
}

static int parse_yes_no(const char* input) {
    if (!input) return -1;

    if (
        strcmp(input, "д") == 0 ||
        strcmp(input, "Д") == 0 ||
        strcmp(input, "да") == 0 ||
        strcmp(input, "Да") == 0 ||
        strcmp(input, "ДА") == 0 ||
        strcmp(input, "y") == 0 ||
        strcmp(input, "Y") == 0 ||
        strcmp(input, "yes") == 0 ||
        strcmp(input, "Yes") == 0 ||
        strcmp(input, "YES") == 0
    ) {
        return 1;
    }

    if (
        strcmp(input, "н") == 0 ||
        strcmp(input, "Н") == 0 ||
        strcmp(input, "нет") == 0 ||
        strcmp(input, "Нет") == 0 ||
        strcmp(input, "НЕТ") == 0 ||
        strcmp(input, "n") == 0 ||
        strcmp(input, "N") == 0 ||
        strcmp(input, "no") == 0 ||
        strcmp(input, "No") == 0 ||
        strcmp(input, "NO") == 0
    ) {
        return 0;
    }

    if (
        strcmp(input, "q") == 0 ||
        strcmp(input, "Q") == 0 ||
        strcmp(input, "exit") == 0 ||
        strcmp(input, "выход") == 0 ||
        strcmp(input, "Выход") == 0
    ) {
        return -2;
    }

    return -1;
}

static int ask_yes_no(IO_interface* io, const char* prompt, int allow_exit) {
    char buffer[IO_MAX_INPUT];

    while (1) {
        fprintf(io->out, "%s", prompt);

        if (!read_line(io, buffer, sizeof(buffer))) {
            fprintf(io->err, "\nОшибка чтения ввода.\n");
            return allow_exit ? -1 : 0;
        }

        int answer = parse_yes_no(buffer);

        if (answer == 1 || answer == 0) {
            return answer;
        }

        if (answer == -2 && allow_exit) {
            return -1;
        }

        fprintf(io->out, "Некорректный ввод. Введите д/н, да/нет или y/n.\n");
    }
}

// Создать объект ввода/вывода
IO_interface* io_create() {
    setlocale(LC_ALL, "");

    IO_interface* io = malloc(sizeof(IO_interface));
    if (!io) {
        fprintf(stderr, "Ошибка: не удалось создать интерфейс ввода/вывода.\n");
        return NULL;
    }

    io->in = stdin;
    io->out = stdout;
    io->err = stderr;

    return io;
}

// Задать вопрос и получить ответ: 1 - да, 0 - нет, -1 - ошибка/выход
int ask_question(IO_interface* io, const char* question) {
    if (!io || !question) {
        return -1;
    }

    fprintf(io->out, "\n%s\n", question);
    return ask_yes_no(io, "Ваш ответ (д/н): ", 1);
}

// Вывести сообщение о догадке, вернуть подтверждение: 1 - да, 0 - нет
int confirm_object(IO_interface* io, const char* object_name) {
    if (!io || !object_name) {
        return 0;
    }

    char prompt[IO_MAX_INPUT];

    snprintf(
        prompt,
        sizeof(prompt),
        "\nЭто %s? (д/н): ",
        object_name
    );

    return ask_yes_no(io, prompt, 0);
}

// Запросить правильный объект
char* ask_real_object(IO_interface* io, const char* wrong_object) {
    if (!io) {
        return NULL;
    }

    fprintf(io->out, "\nЯ ошибся.\n");

    if (wrong_object) {
        fprintf(io->out, "Я думал, что это: %s\n", wrong_object);
    }

    fprintf(io->out, "Кто или что это было на самом деле? ");

    return read_allocated_line(io);
}

// Запросить новый вопрос
char* ask_new_question(
    IO_interface* io,
    const char* correct_object,
    const char* wrong_object
) {
    if (!io) {
        return NULL;
    }

    fprintf(io->out, "\nПридумайте вопрос, который отличает");

    if (correct_object && wrong_object) {
        fprintf(io->out, " \"%s\" от \"%s\"", correct_object, wrong_object);
    }

    fprintf(io->out, ".\n");
    fprintf(io->out, "Новый вопрос: ");

    return read_allocated_line(io);
}

// Запросить ответ на новый вопрос для правильного объекта
int ask_answer_for_new(
    IO_interface* io,
    const char* question,
    const char* correct_object
) {
    if (!io || !question || !correct_object) {
        return 0;
    }

    fprintf(io->out, "\nДля объекта \"%s\" на вопрос:\n", correct_object);
    fprintf(io->out, "\"%s\"\n", question);

    return ask_yes_no(io, "Как нужно отвечать? (д/н): ", 0);
}

// Вывести сообщение об ошибке
void io_error(IO_interface* io, const char* msg) {
    if (!msg) {
        msg = "Неизвестная ошибка.";
    }

    if (io && io->err) {
        fprintf(io->err, "Ошибка: %s\n", msg);
    } else {
        fprintf(stderr, "Ошибка: %s\n", msg);
    }
}

// Освободить IO_interface
void io_free(IO_interface* io) {
    if (!io) {
        return;
    }

    free(io);
}