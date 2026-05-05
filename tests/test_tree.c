#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"

static void ok(const char *msg) {
    printf("[ OK ] %s\n", msg);
}

static void fail(const char *msg) {
    printf("[FAIL] %s\n", msg);
    exit(1);
}

static void assert_true(int cond, const char *msg) {
    if (!cond) fail(msg);
    ok(msg);
}

static void assert_str(const char *a, const char *b, const char *msg) {
    if (!a || !b || strcmp(a, b) != 0) fail(msg);
    ok(msg);
}

int main() {
    printf("UNIT TESTS START\n");

    // 1. create object
    TreeNode *obj = create_object_node("Cat");
    assert_true(obj != NULL, "Object created");
    assert_true(obj->is_question == 0, "Object is_question=0");
    assert_str(obj->data, "Cat", "Object data correct");

    // 2. create question
    TreeNode *q = create_question_node("Is it alive?", NULL, NULL);
    assert_true(q != NULL, "Question created");
    assert_true(q->is_question == 1, "Question is_question=1");
    assert_str(q->data, "Is it alive?", "Question data correct");

    // 3. add new object
    TreeNode *leaf = create_object_node("Dog");
    add_new_object(leaf, "Cat", "Does it meow?", 1);

    assert_true(leaf->is_question == 1, "Leaf converted to question");
    assert_str(leaf->data, "Does it meow?", "New question inserted");
    assert_str(leaf->right->data, "Cat", "YES branch correct");
    assert_str(leaf->left->data, "Dog", "NO branch correct");

    // 4. save and download
    TreeNode *root =
        create_question_node("Is it animal?",
            create_object_node("Rock"),
            create_object_node("Dog"));

    assert_true(save_tree_to_file(root, "test_tree.json") == 1, "Tree saved");

    TreeNode *loaded = load_tree_from_file("test_tree.json");
    assert_true(loaded != NULL, "Tree loaded");
    assert_str(loaded->data, "Is it animal?", "Loaded root question correct");
    assert_str(loaded->left->data, "Rock", "Loaded left leaf correct");
    assert_str(loaded->right->data, "Dog", "Loaded right leaf correct");

    free_tree(obj);
    free_tree(q);
    free_tree(leaf);
    free_tree(root);
    free_tree(loaded);

    printf("\n ALL UNIT TESTS PASSED \n");
    return 0;
}
