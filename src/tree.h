#ifndef TREE_H
#define TREE_H

typedef struct TreeNode {
    int is_question;        // 1 - вопрос, 0 - объект
    char *data;             // текст вопроса или объекта
    struct TreeNode *left;  // ветка "нет"
    struct TreeNode *right; // ветка "да"
} TreeNode;

/**
 * @brief Создать лист-объект
 */ 
TreeNode* create_object_node(const char *object_name);

/**
 * @brief Создать узел-вопрос
 */ 
TreeNode* create_question_node(const char *question_text,
                               TreeNode *no_branch, TreeNode *yes_branch);

/**
 * @brief Загрузить дерево из файла (возвращает корень)
 */ 
TreeNode* load_tree_from_file(const char *filename);

/**
 * @brief Сохранить дерево в файл
 */ 
int save_tree_to_file(TreeNode *root, const char *filename);

/**
 * @brief Очистить всё дерево
 */ 
void free_tree(TreeNode *root);

/**
 * @brief Добавить новый объект и вопрос на место листа
 */ 
void add_new_object(TreeNode *wrong_leaf, const char *new_object,
                    const char *new_question, int answer_for_new);

/**
 * @brief Структура для парсера
 */                     
typedef struct {
    const char *json;
    int position;
} JSONParser;

/**
 * @brief  Парсинг узла
 */ 
TreeNode* parse_node(JSONParser *parser);


/**
 * @brief Парсинг строки строки
 */
char* parse_string(JSONParser *parser);

/**
 * @brief Парсинг числа
 */
int parse_integer(JSONParser *parser);

/**
 * @brief Пропуск пробела
 */
void skip_whitespace(JSONParser *parser);
char peek_char(JSONParser *parser);
char get_char(JSONParser *parser);

#endif