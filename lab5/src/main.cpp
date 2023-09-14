#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

class Trie {
private:
    struct Vertex {
        int l;
        int r;
        int enter;            /* вхождение суффикса в исходный текст*/
        map<char, int> child; /* индексы вершин, по которым я могу перйти по букве */

        Vertex() {
            l = -1;
            r = -1;
            enter = -1;
        };
    };

    vector<Vertex> storage;
    string text;

    string ReadText() {
        getline(cin, text);
        text += '$';
        storage.push_back(Vertex());
        for (size_t i = 0; i < text.length(); ++i)
            Insert(i, text.length() - 1);
        return text;
    }

    void Insert(int l, int r) {
        int current = 0;
        int old_l = l;
        while (l <= r) {
            if (current == 0) { /* root */
                if (storage[current].child.find(text[l]) != storage[current].child.end())
                    current = storage[current].child[text[l]];
                if (current == 0) {
                    NewVertex(l, r, old_l);
                    storage[current].child[text[l]] = storage.size() - 1;
                    break;
                }
            }
            int start = storage[current].l;
            int finish = storage[current].r;
            bool cut = false;
            for (int i = start; (i <= finish) && (l + i - start <= r); ++i) {
                if (text[i] != text[l + i - start]) {
                    storage[current].r = i - 1;
                    int old_enter = storage[current].enter;
                    storage[current].enter = -1;
                    if (text[finish] == '$')
                        NewVertex(i, finish, old_enter);
                    else
                        NewVertex(i, finish);
                    storage[storage.size() - 1].child = storage[current].child;
                    storage[current].child.clear();
                    storage[current].child[text[i]] = storage.size() - 1;
                    NewVertex(l + i - start, r, old_l);
                    storage[current].child[text[l + i - start]] = storage.size() - 1;
                    cut = true;
                    break;
                }
            }
            if (cut == false) {
                int new_l = l + finish - start + 1;
                if (storage[current].child.find(text[new_l]) != storage[current].child.end()) {
                    current = storage[current].child[text[new_l]];
                } else {
                    NewVertex(new_l, r, old_l);
                    storage[current].child[text[new_l]] = storage.size() - 1;
                    break;
                }
                l = new_l;
            } else
                break;
        }
    }

    void NewVertex(int l, int r) {
        storage.push_back(Vertex());
        storage[storage.size() - 1].l = l;
        storage[storage.size() - 1].r = r;
    }

    void NewVertex(int l, int r, int enter) {
        storage.push_back(Vertex());
        storage[storage.size() - 1].l = l;
        storage[storage.size() - 1].r = r;
        storage[storage.size() - 1].enter = enter;
    }

    void PrintTree(int cur) {
        if (cur < storage.size()) {
            if (cur != 0) {
                printf("\n");
                printf("current = %d\n\t", cur);
                for (int k = storage[cur].l; k <= storage[cur].r; ++k) {
                    printf("%c", text[k]);
                }
                printf("\n");
            }
            for (const auto &[first, second] : storage[cur].child) {
                printf("%d ", storage[second].enter);
                for (int i = storage[second].l; i <= storage[second].r; ++i) {
                    printf("%c", text[i]);
                }
                printf("\t");
            }
            if (storage[cur].child.size() == 0)
                printf("empty\n");
            else
                printf("\n");
            PrintTree(cur + 1);
        }
    }

    void ReadPattern() {
        string pattern;
        int counter = 1;
        while (getline(cin, pattern)) {
            vector<int> res = Search(pattern);
            if (!res.empty()) {
                printf("%d: ", counter);
                sort(res.begin(), res.end());
                for (size_t i = 0; i < res.size(); ++i) {
                    if (i != 0)
                        printf(", ");
                    printf("%d", res[i] + 1);
                }
                printf("\n");
            }
            ++counter;
        }
    }

    vector<int> Search(string &pattern) {
        vector<int> res;
        int current = 0;
        int l = 0;
        int r = pattern.length() - 1;
        bool flag = false;
        while (l <= r) {
            if (current == 0) {
                if (storage[current].child.find(pattern[l]) != storage[current].child.end())
                    current = storage[current].child[pattern[l]];
                else
                    break;
            }
            int start = storage[current].l;
            int finish = storage[current].r;
            for (int i = 0; (start + i <= finish) && (i + l <= r); ++i) {
                if (pattern[i + l] != text[start + i]) {
                    flag = true;
                    break;
                }
            }
            if (flag == false) {
                l = l + finish - start + 1;
                if (l > r)
                    break;
                if (storage[current].child.find(pattern[l]) != storage[current].child.end())
                    current = storage[current].child[pattern[l]];
                else
                    break;
            } else
                break;
        }
        if ((l > r) && (flag == false) && (pattern.length() > 0))
            DFS(res, current);
        return res;
    }

    void DFS(vector<int> &res, int current) {
        if (storage[current].child.empty())
            res.push_back(storage[current].enter);
        for (const auto &[first, second] : storage[current].child)
            DFS(res, second);
    }

public:
    void RunAlgo() {
        ReadText();
        ReadPattern();
    }
};

int main() {
    Trie algo;
    algo.RunAlgo();
    return 0;
}
