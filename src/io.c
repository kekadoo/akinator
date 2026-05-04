#include "io.h"
#include <stdlib.h>


// Создать объект ввода/вывода (можно хранить файл лога и т.п.)
IO_interface* io_create() {
	return NULL;
}

// Задать вопрос и получить ответ (1 - да, 0 - нет, -1 - ошибка/выход)
int ask_question(IO_interface* io, const char* question) {
	(void)io;
	(void)question;
	return 0;
}

// Вывести сообщение о догадке, вернуть подтверждение (1 - да, 0 - нет)
int confirm_object(IO_interface* io, const char* object_name) {
	(void)io;
	(void)object_name;
	return 0;
}

// Запросить правильный объект (строка выделяется malloc, caller free)
char* ask_real_object(IO_interface* io, const char* wrong_object) {
	(void)io;
	(void)wrong_object;
	return NULL;
}

// Запросить новый вопрос
char* ask_new_question(IO_interface* io, const char* correct_object, const char* wrong_object) {
	(void)io;
	(void)correct_object;
	(void)wrong_object;
	return NULL;
}

// Запросить ответ на новый вопрос для правильного объекта
int ask_answer_for_new(IO_interface* io, const char* question, const char* correct_object) {
	(void)io;
	(void)question;
	(void)correct_object;
	return 0;
}

// Вывести сообщение об ошибке
void io_error(IO_interface* io, const char* msg) {
	(void)io;
	(void)msg;
}

// Освободить IO_interface
void io_free(IO_interface* io) {
	(void)io;
}
