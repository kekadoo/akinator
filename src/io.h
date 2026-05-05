#ifndef IO_H
#define IO_H

#include "tree.h"

typedef struct IO_interface IO_interface; // скрытая структура

// Запуск угадывания: возвращает 1, если объект угадан, 0 - если дообучение
int play_round(TreeNode* root, IO_interface* io);

// Создать объект ввода/вывода (можно хранить файл лога и т.п.)
IO_interface* io_create();

// Задать вопрос и получить ответ (1 - да, 0 - нет, -1 - ошибка/выход)
int ask_question(IO_interface *io, const char *question);

// Вывести сообщение о догадке, вернуть подтверждение (1 - да, 0 - нет)
int confirm_object(IO_interface *io, const char *object_name);

// Запросить правильный объект (строка выделяется malloc, caller free)
char* ask_real_object(IO_interface *io, const char *wrong_object);

// Запросить новый вопрос
char* ask_new_question(IO_interface *io, const char *correct_object, const char *wrong_object);

// Запросить ответ на новый вопрос для правильного объекта
int ask_answer_for_new(IO_interface *io, const char *question, const char *correct_object);

// Вывести сообщение об ошибке
void io_error(IO_interface *io, const char *msg);

// Освободить IO_interface
void io_free(IO_interface *io);




#endif
