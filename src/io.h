#ifndef IO_H
#define IO_H

#include "tree.h"

typedef struct IO_interface IO_interface; // скрытая структура

/**
 * @brief Запуск угадывания: возвращает 1, если объект угадан, 0 - если дообучение
 */ 
int play_round(TreeNode* root, IO_interface* io);

/**
 * @brief Создать объект ввода/вывода (можно хранить файл лога и т.п.)
 */
IO_interface* io_create();

/**
 * @brief Задать вопрос и получить ответ (1 - да, 0 - нет, -1 - ошибка/выход)
 */
int ask_question(IO_interface *io, const char *question);

/**
 * @brief Вывести сообщение о догадке, вернуть подтверждение (1 - да, 0 - нет)
 */
int confirm_object(IO_interface *io, const char *object_name);

/**
 * @brief Запросить правильный объект (строка выделяется malloc, caller free)
 */
char* ask_real_object(IO_interface *io, const char *wrong_object);

/**
 * @brief Запросить новый вопрос
 */
char* ask_new_question(IO_interface *io, const char *correct_object, const char *wrong_object);

/**
 * @brief Запросить ответ на новый вопрос для правильного объекта
 */
int ask_answer_for_new(IO_interface *io, const char *question, const char *correct_object);

/**
 * @brief Вывести сообщение об ошибке
 */
void io_error(IO_interface *io, const char *msg);

/**
 * @brief Освободить IO_interface
 */
void io_free(IO_interface *io);




#endif
