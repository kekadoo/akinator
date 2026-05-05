#include "game.h"
#include "utils.h"

#include <stdlib.h>
#include <stdio.h>


static TreeNode* create_node(int is_question, const char* data) {
    TreeNode* leaf = malloc(sizeof(TreeNode));
    leaf->is_question = is_question;
    leaf->data = my_strdup(data);
    leaf->right = NULL;
    leaf->left = NULL;
    return leaf;
}

TreeNode* create_default_tree() {
    TreeNode* root = create_node(1, "Это А?");
    root->right = create_node(0, "Это А");
    root->left = create_node(0, "Это Б");
    return root;
}

int run_game(int argc, char* argv[]) {
    (void)argc;
    (void)argv;
    const char* tree_file = "akinator_tree.txt";
    TreeNode* root = load_tree_from_file(tree_file);
    if (!root) {
        root = create_default_tree();
    }
    IO_interface* io = io_create();
    int play_again = 1;
    while (play_again) {
        int result = 1;
        if (result == 0) {
            // Было дообучение, дерево изменилось (и уже сохранено внутри play_round)
            // Можно перезагрузить root? Нет, root указывает на новое дерево.
            save_tree_to_file(root, tree_file);
        }
        play_again = ask_question(io, "Сыграем ещё? (д/н)");
    }
    free_tree(root);
    io_free(io);
    return 0;
}



