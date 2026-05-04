#include "tree.h"
#include <stdlib.h>


// Создать лист-объект
TreeNode* create_object_node(const char* object_name) {
    (void)object_name;
    return NULL;
}

// Создать узел-вопрос
TreeNode* create_question_node(const char* question_text,
    TreeNode* no_branch, TreeNode* yes_branch) {
    (void)question_text;
    (void)no_branch;
    (void)yes_branch;
    return NULL;
}

// Загрузить дерево из файла (возвращает корень)
TreeNode* load_tree_from_file(const char* filename) {
    (void)filename;
    return NULL;
}

// Сохранить дерево в файл
int save_tree_to_file(TreeNode* root, const char* filename) {
    (void)root;
    (void)filename;
    return 0;
}

// Очистить всё дерево
void free_tree(TreeNode* root) {
    (void)root;
}

// Запуск угадывания: возвращает 1, если объект угадан, 0 - если дообучение
int play_round(TreeNode* root, IO_interface* io) {
    (void)root;
    (void)io;
    return 0;
}

// Добавить новый объект и вопрос на место листа
void add_new_object(TreeNode* wrong_leaf, const char* new_object,
    const char* new_question, int answer_for_new) {
    (void)wrong_leaf;
    (void)new_object;
    (void)new_question;
    (void)answer_for_new;
}
