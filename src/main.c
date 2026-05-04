#include "tree.h"
#include "io.h"

int main(int argc, char* argv[]) {
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
        int result = play_round(root, io);
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
