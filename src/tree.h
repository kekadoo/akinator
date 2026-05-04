#ifndef TREE_H
#define TREE_H

typedef struct TreeNode {
    int is_question;        // 1 - вопрос, 0 - объект
    char *data;             // текст вопроса или объекта
    struct TreeNode *left;  // ветка "нет"
    struct TreeNode *right; // ветка "да"
} TreeNode;

// Создать лист-объект
TreeNode* create_object_node(const char *object_name);

// Создать узел-вопрос
TreeNode* create_question_node(const char *question_text,
                               TreeNode *no_branch, TreeNode *yes_branch);

// Загрузить дерево из файла (возвращает корень)
TreeNode* load_tree_from_file(const char *filename);

// Сохранить дерево в файл
int save_tree_to_file(TreeNode *root, const char *filename);

// Очистить всё дерево
void free_tree(TreeNode *root);

// Запуск угадывания: возвращает 1, если объект угадан, 0 - если дообучение
//int play_round(TreeNode *root, IO_interface *io);

// Добавить новый объект и вопрос на место листа
void add_new_object(TreeNode *wrong_leaf, const char *new_object,
                    const char *new_question, int answer_for_new);

// Структура для парсера
typedef struct {
  const char *json;
  int position;
} JSONParser;

// Прототипы функций
TreeNode* parse_node(JSONParser *parser);
char* parse_string(JSONParser *parser);
int parse_integer(JSONParser *parser);
void skip_whitespace(JSONParser *parser);
char peek_char(JSONParser *parser);
char get_char(JSONParser *parser);

#endif