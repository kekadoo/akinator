#include "tree.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
// TODO удалить
#include <windows.h>

TreeNode* create_object_node(const char *object_name) {
    TreeNode* node = malloc(sizeof(TreeNode));

    if (!node) return NULL;

    node->is_question = 0;
    node->data = strdup(object_name);
    node->left = NULL;
    node->right = NULL;

    return node;
}

TreeNode* create_question_node(const char *question_text,
                               TreeNode *no_branch, TreeNode *yes_branch) {
    TreeNode* node = malloc(sizeof(TreeNode));

    if (!node) return NULL;

    node->is_question = 1;
    node->data = strdup(question_text);
    node->left = no_branch;
    node->right = yes_branch;

    return node;
}

void free_tree(TreeNode *root) {
    if (root == NULL) return;

    free_tree(root->left);
    free_tree(root->right);

    free(root->data);
    free(root);
}

void skip_whitespace(JSONParser *parser) {
    while (parser->json[parser->position] && isspace(parser->json[parser->position])) {
        parser->position++;
    }
}

char peek_char(JSONParser *parser) {
    skip_whitespace(parser);
    return parser->json[parser->position];
}

char get_char(JSONParser *parser) {
    skip_whitespace(parser);
    if (parser->json[parser->position]) {
        return parser->json[parser->position++];
    }
    return '\0';
}

// Парсинг строки JSON
char* parse_string(JSONParser *parser) {
    skip_whitespace(parser);

    if (get_char(parser) != '"') {
        return NULL;
    }

    int start = parser->position;

    while (parser->json[parser->position] && parser->json[parser->position] != '"') {
        if (parser->json[parser->position] == '\\') {
            parser->position++;
            if (parser->json[parser->position]) {
                parser->position++;
            }
        } else {
            parser->position++;
        }
    }

    if (parser->json[parser->position] != '"') {
        return NULL;
    }

    int length = parser->position - start;
    char *str = malloc(length + 1);

    if (!str) {
        return NULL;
    }

    strncpy(str, parser->json + start, length);
    str[length] = '\0';

    parser->position++;

    return str;
}

int parse_integer(JSONParser *parser) {
    skip_whitespace(parser);

    int value = 0;
    int sign = 1;

    if (parser->json[parser->position] == '-') {
        sign = -1;
        parser->position++;
    }

    if (!isdigit(parser->json[parser->position])) {
        return 0;
    }

    while (isdigit(parser->json[parser->position])) {
        value = value * 10 + (parser->json[parser->position] - '0');
        parser->position++;
    }

    return value * sign;
}

int parse_null(JSONParser *parser) {
    skip_whitespace(parser);

    const char *null_str = "null";
    int i = 0;

    for (i = 0; null_str[i]; i++) {
        if (parser->json[parser->position + i] != null_str[i]) {
            return 0;
        }
    }

    parser->position += i;
    return 1;
}

TreeNode* parse_value(JSONParser *parser) {
    char ch = peek_char(parser);

    if (ch == '{') {
        return parse_node(parser);
    } else if (ch == '"') {
        char *str = parse_string(parser);
        if (str) {
            TreeNode *node = malloc(sizeof(TreeNode));
            if (node) {
                node->is_question = 0;
                node->data = str;
                node->left = NULL;
                node->right = NULL;
            }
            return node;
        }
    } else if (isdigit(ch) || ch == '-') {
        int value = parse_integer(parser);
        TreeNode *node = malloc(sizeof(TreeNode));
        if (node) {
            node->is_question = value;
            node->data = NULL;
            node->left = NULL;
            node->right = NULL;
        }
        return node;
    } else if (ch == 'n') {
        if (parse_null(parser)) {
            return NULL;
        }
    }

    return NULL;
}

TreeNode* parse_node(JSONParser *parser) {
    skip_whitespace(parser);

    if (get_char(parser) != '{') {
        return NULL;
    }

    TreeNode *node = malloc(sizeof(TreeNode));
    if (!node) {
        return NULL;
    }

    node->is_question = 0;
    node->data = NULL;
    node->left = NULL;
    node->right = NULL;

    while (1) {
        skip_whitespace(parser);

        if (peek_char(parser) == '}') {
            get_char(parser);
            break;
        }

        char *key = parse_string(parser);
        if (!key) {
            free(node->data);
            free(node);
            return NULL;
        }

        if (get_char(parser) != ':') {
            free(key);
            free(node->data);
            free(node);
            return NULL;
        }

        if (strcmp(key, "is_question") == 0) {
            TreeNode *value_node = parse_value(parser);
            if (value_node) {
                node->is_question = value_node->is_question;
                free(value_node->data);
                free(value_node);
            }
        }
        else if (strcmp(key, "data") == 0) {
            char *str = parse_string(parser);
            if (str) {
                node->data = str;
            }
        }
        else if (strcmp(key, "left") == 0) {
            node->left = parse_value(parser);
        }
        else if (strcmp(key, "right") == 0) {
            node->right = parse_value(parser);
        }
        else {
            parse_value(parser);
        }

        free(key);

        skip_whitespace(parser);
        if (peek_char(parser) == ',') {
            get_char(parser);
            continue;
        }
    }

    return node;
}

TreeNode* load_tree_from_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Ошибка: не удалось открыть файл %s\n", filename);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *json_string = malloc(file_size + 1);
    if (!json_string) {
        fclose(file);
        fprintf(stderr, "Ошибка: не удалось выделить память\n");
        return NULL;
    }

    size_t read_size = fread(json_string, 1, file_size, file);
    json_string[read_size] = '\0';
    fclose(file);

    JSONParser parser;
    parser.json = json_string;
    parser.position = 0;

    TreeNode *tree = parse_node(&parser);

    free(json_string);
    return tree;
}

char* escape_json_string(const char *str) {
    if (!str) return strdup("");

    int escaped_len = 0;
    for (int i = 0; str[i]; i++) {
        if (str[i] == '"' || str[i] == '\\') {
            escaped_len += 2;
        } else {
            escaped_len += 1;
        }
    }

    char *escaped = malloc(escaped_len + 1);
    if (!escaped) return NULL;

    int pos = 0;
    for (int i = 0; str[i]; i++) {
        if (str[i] == '"') {
            escaped[pos++] = '\\';
            escaped[pos++] = '"';
        } else if (str[i] == '\\') {
            escaped[pos++] = '\\';
            escaped[pos++] = '\\';
        } else {
            escaped[pos++] = str[i];
        }
    }
    escaped[pos] = '\0';

    return escaped;
}

void save_node_to_file(TreeNode *node, FILE *file, int indent_level) {
    if (!node) {
        fprintf(file, "null");
        return;
    }

    char indent[100] = "";
    for (int i = 0; i < indent_level; i++) {
        strcat(indent, "  ");
    }

    fprintf(file, "{\n");

    fprintf(file, "%s  \"is_question\": %d,\n", indent, node->is_question);

    char *escaped_data = escape_json_string(node->data);
    fprintf(file, "%s  \"data\": \"%s\",\n", indent, escaped_data);
    free(escaped_data);

    fprintf(file, "%s  \"left\": ", indent);
    save_node_to_file(node->left, file, indent_level + 1);
    fprintf(file, ",\n");

    fprintf(file, "%s  \"right\": ", indent);
    save_node_to_file(node->right, file, indent_level + 1);
    fprintf(file, "\n");

    fprintf(file, "%s}", indent);
}

int save_tree_to_file(TreeNode *root, const char *filename) {
    if (!root) {
        fprintf(stderr, "Ошибка: дерево пустое\n");
        return -1;
    }

    FILE *file = fopen(filename, "w");
    if (!file) {
        fprintf(stderr, "Ошибка: не удалось открыть файл %s для записи\n", filename);
        return -1;
    }

    save_node_to_file(root, file, 0);

    fprintf(file, "\n");

    fclose(file);
    return 0;
}
void add_new_object(TreeNode *wrong_leaf, const char *new_object,
                    const char *new_question, int answer_for_new){
    TreeNode* old_obj = create_object_node(strdup(wrong_leaf->data));
    TreeNode* new_obj = create_object_node(strdup(new_object));

    wrong_leaf->is_question = 1;
    wrong_leaf->data = strdup(new_question);
    if (answer_for_new) {
        wrong_leaf->right = new_obj;
        wrong_leaf->left = old_obj;
    }
    else{
        wrong_leaf->right = old_obj;
        wrong_leaf->left = new_obj;
    }


}
int play_round(TreeNode *root, IO_interface *io) {
    if (!root || !io) {
        io_error(io, "Невозможно начать игру: дерево пустое.");
        return -1;
    }

    TreeNode *current = root;

    while (current->is_question) {
        int answer = ask_question(io, current->data);

        if (answer == -1) {
            return -1;
        }

        if (answer) {
            current = current->right;
        } else {
            current = current->left;
        }

        if (!current) {
            io_error(io, "Ошибка структуры дерева: отсутствует ветка.");
            return -1;
        }
    }

    int confirmed = confirm_object(io, current->data);

    if (confirmed) {
        return 1;
    }

    char *real_object = ask_real_object(io, current->data);
    if (!real_object) {
        return -1;
    }

    char *new_question = ask_new_question(io, real_object, current->data);
    if (!new_question) {
        free(real_object);
        return -1;
    }

    int answer_for_new = ask_answer_for_new(io, new_question, real_object);

    add_new_object(current, real_object, new_question, answer_for_new);

    free(real_object);
    free(new_question);

    return 0;
}
