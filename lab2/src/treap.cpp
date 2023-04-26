#include <iostream>
#include <random>
#include <chrono>
#include <string.h>

using namespace std;

std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());

const unsigned int KEY_LEN = 257;

class TTreap {
    public:
        TTreap() {
            root = nullptr;
        }

        ~TTreap() {
            destroy(root);
        }

        void find(const char *_key) {
            node_ptr leftTree = nullptr;
            node_ptr rightTree = nullptr;
            node_ptr elem = nullptr;
            cut(root, leftTree, elem, rightTree, _key);
            if (elem != nullptr) {
                cout << "OK: " << elem->value << "\n";
            } else {
                cout << "NoSuchWord\n";
            }
            join(leftTree, elem, rightTree);
        }

        void insert(const char *_key, uint64_t _value) {
            node_ptr leftTree = nullptr;
            node_ptr rightTree = nullptr;
            node_ptr elem = nullptr;
            cut(root, leftTree, elem, rightTree, _key);
            if (elem != nullptr) {
                cout << "Exist\n";
            } else {
                elem = new node(_key, _value);
                cout << "OK\n";
            }
            join(leftTree, elem, rightTree);
        }

        void remove(const char *_key) {
            node_ptr leftTree = nullptr;
            node_ptr rightTree = nullptr;
            node_ptr elem = nullptr;
            cut(root, leftTree, elem, rightTree, _key);
            if (elem != nullptr) {
                cout << "OK\n";
                delete elem;
                elem = nullptr;
            } else {
                cout << "NoSuchWord\n";
            }
            join(leftTree, elem, rightTree);
        }
    private:
        struct node {
            node *left;
            node *right;
            char *key;
            uint64_t value;
            int64_t y;
            node(const char *_key, int64_t _value) {
                left = nullptr;
                right = nullptr;
                key = new char[KEY_LEN];
                memcpy(key, _key, KEY_LEN);

                value = _value;
                y = rng();
            }
            ~node() {
                delete[] key;
            }
            
        };

        using node_ptr = node *;

        node_ptr root;
        char tmp[KEY_LEN];

        void destroy (node_ptr r) {
            if (r == nullptr) return;
            destroy(r->left);
            destroy(r->right);
            delete r;
        }

        node_ptr merge(node_ptr t1, node_ptr t2) {
            if (t1 == nullptr) {
                return t2;
            }
            if (t2 == nullptr) {
                return t1;
            }
            if (t1->y > t2->y) {
                t1->right = merge(t1->right, t2);
                return t1;
            } else {
                t2->left = merge(t1, t2->left);
                return t2;
            }
        }

        void split(node_ptr t, const char *key0, node_ptr& t1, node_ptr& t2) {
            if (t == nullptr) {
                t1 = nullptr;
                t2 = nullptr;
                return;
            }
            if (strcmp(t->key, key0) < 0) {  //key0 > t->key
                split(t->right, key0, t->right, t2);
                t1 = t;
            } else {
                split(t->left, key0, t1, t->left);
                t2 = t;
            }
        }

        void cut(node_ptr t, node_ptr& leftTree, node_ptr& middle, node_ptr& rightTree, const char *_key) {
            memcpy(tmp, _key, KEY_LEN);
            ++(tmp[strlen(_key)]);
            node_ptr r = nullptr;
            split(root, _key, leftTree, r);
            split(r, tmp, middle, rightTree);
        }

        void join(node_ptr& leftTree, node_ptr& middle, node_ptr& rightTree) {
            node_ptr r = merge(middle, rightTree);
            root = merge(leftTree, r);
        }

};

void StringToLower(char *str) {
    for (size_t i = 0; str[i] != '\0'; ++i) {
        str[i] = tolower(str[i]);
    }
}

int main() {
    TTreap treap;
    char input[KEY_LEN] = {0};
    uint64_t value;
    while (cin >> input) {
        if (input[0] == '+') {
            cin >> input >> value;
            StringToLower(input);
            treap.insert(input, value);
        } else if (input[0] == '-') {
            cin >> input;
            StringToLower(input);
            treap.remove(input);
        } else {
            StringToLower(input);
            treap.find(input);
        }
    }
    return 0;
}
