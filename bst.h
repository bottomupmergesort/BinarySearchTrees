#ifndef BST_H
#define BST_H
#include <iostream>
#include <queue>
using namespace std;
template <class Item>
class BST {
    private:
    static const int red = 1;
    static const int black = 0;
    struct node {
        Item key;
        node* left;
        node* right;
        int color;
        int N;
        node(Item x) {
            key = x;
            left = nullptr;
            right = nullptr;
            N = 0;
            color = red;
        }
    };
    typedef node* link;
    link head;
    Item nullitem;
    int N;
    bool isRed(link h)
    {
        if (h == nullptr) return false;
        return (h->color == red);
    }
    Item searchR(link h, Item v)
    {
        if (h == nullptr) return nullitem;
        if (h->key == v)
        {
            return h->key;
        }
        if (v < h->key) return searchR(h->left, v);
        else return searchR(h->right, v);
    }
    void insertR(link& h, int v, int depth)
    {
        if (h == nullptr)
        {
            h = new node(v);
            h->N = depth;
            N++;
            return;
        }
        if (v < h->key)
        {
            insertR(h->left, v, depth++);
        } else {
            insertR(h->right, v, depth++);
        }
    }
    void insertT(link& h, Item v, int depth)
    {
        if (h == nullptr)
        {
            h = new node(v);
            h->N = depth;
            N++;
            return;
        }
        if (v < h->key)
        {
            insertT(h->left, v, depth++);
            rotR(h);
        } else {
            insertT(h->right, v, depth++);
            rotL(h);
        }
    }
    void insertRand(link& h, Item x, int depth)
    {
        if (h == nullptr)
        {
            h = new node(x);
            h->N = depth;
            return;
        }
        if (rand() < RAND_MAX / (h->N+1))
        {
            insertT(h, x, depth++);
            return;
        }
        if (x < h->key)
        {
            insertR(h->left, x, depth++);
        } else {
            insertR(h->right, x, depth++);
        }
    }
    void splay(link& h, Item v)
    {
        if (h == nullptr)
        {
            h = new node(v);
            h->left = h->right = nullptr;
            h->N = 1;
        }
        if (v < h->key)
        {
            link& hl = h->left;
            int n = h->N;
            if (hl == nullptr)
            {
                link t = h;
                h = new node(v);
                h->left = nullptr;
                h->right = t;
                h->N = n+1;
                return;
            }
            if (v < hl->key)
            {
                splay(hl->left, v);
                rotR(h);
            } else {
                splay(hl->right, v);
                rotL(hl);
            }
            rotR(h);
        } else {
            link& hr = h->right;
            int n = h->N;
            if (hr == nullptr)
            {
                link t = h;
                h = new node(v);
                h->left = t;
                h->right = nullptr;
                h->N = n + 1;
                return;
            }
            if (hr->key < v)
            {
                splay(hr->right, v);
                rotL(h);
            } else {
                splay(hr->left, v);
                rotR(hr);
            }
            rotL(h);
        }
    }
    void insertRB(link& h, Item v, int sw)
    {
        if (h == nullptr)
        {
            h = new node(v);
            return;
        }
        //split four node
        if (isRed(h->left) && isRed(h->right))
        {
            h->color = red;
            h->left->color = black;
            h->right->color = black;
        }
        if (v < h->key)
        {
            insertRB(h->left, v, 0);
            if (isRed(h) && isRed(h->left) && sw) rotR(h);
            if (isRed(h->left) && isRed(h->left->left))
            {
                rotR(h);
                h->color = black;
                h->right->color = red;
            }
        } else {
            insertRB(h->right, v, 1);
            if (isRed(h) && isRed(h->right) && !sw) rotL(h);
            if (isRed(h->right) && isRed(h->right->right))
            {
                rotL(h);
                h->color = black;
                h->left->color = red;
            }
        }
    }
    Item selectR(link h, int k)
    {
        if (h == nullptr) 
            return nullitem;
        int t = (h->left == nullptr) ? 0:h->left->N;
        if (t > k) return selectR(h->left, k);
        if (t > k) return selectR(h->right, k - t - 1);
        return h->key;
    }
    void partR(link& h, int k)
    {
        int t = (h->left == nullptr) ? 0:h->left->N;
        if (t > k)
        {
            partR(h->left, k);
            rotR(h);
        }
        if (t < k)
        {
            partR(h->right, k);
            rotL(h);
        }
    }
    link joinR(link a, link b)
    {
        if (b == nullptr) return a;
        if (a == nullptr) return b;
        insertRoot(b, a->key);
        b->left = joinR(a->left, b->left);
        b->right = joinR(a->right, b->right);
        delete a;
        return b;
    }
    link joinLR(link a, link b)
    {
        if (b == nullptr) return a;
        partR(b, 0); 
        b->left = a;
        return b; 
    }
    void removeR(link& h, Item v)
    {
        if (h == nullptr)
            return;
        Item w = h->key;
        if (v < w) removeR(h->left, v);
        if (w < v) removeR(h->right, v);
        if (v == w)
        {
            link t = h;
            h = joinLR(h->left, h->right);
            delete t;
        }
    }
    void balanceR(link& h)
    {
        if (h == nullptr) return;
        partR(h, h->N/2);
        balanceR(h->left);
        balanceR(h->right);
    }
    void rotR(link& h)
    {
        link x = h->left;
        h->left = x->right;
        x->right = h;
        h = x;
    }
    void rotL(link& h)
    {
        link x = h->right;
        h->right = x->left;
        x->left = h;
        h = x;
    }
    void showR(link h)
    {
        if (h == nullptr) return;
        showR(h->left);
        std::cout<<h->key<<" ";
        showR(h->right);
    }
    void bfs(link start)
    {
        int curr_level = 0;
        std::queue<link> fq;
        fq.push(start);
        while (!fq.empty())
        {
            start = fq.front();
            fq.pop();
            if (start != nullptr && start->N != curr_level)
            {
                curr_level = start->N;
                cout<<"\n level: "<<curr_level<<": ";
            }
            if (start != nullptr)
            {
                cout<<start->key<<" ";
            }
            if (start->left != nullptr) fq.push(start->left);
            if (start->right != nullptr) fq.push(start->right);
        }
        cout<<endl;
    }
    public:
    BST()
    {
        head = nullptr;
        N = 0;
    }
    void insert(Item v)
    {
        insertR(head, v, 0);
    }
    void insertRandomized(Item v)
    {
        insertRand(head, v, 0);
    }
    void insertRoot(Item v)
    {
        insertT(head, v, 0);
    }
    void insertSplay(Item v)
    {
        splay(head, v);
    }
    void insertRedBlack(Item v)
    {
        insertRB(head, v, 0);
        head->color = black;
    }
    Item select(int k)
    {
        return selectR(head, k);
    }
    Item search(Item key)
    {
        return searchR(head, key);
    }
    void remove(Item v)
    {
        removeR(head, v);
    }
    void join(BST<Item>& b)
    {
        head = joinR(head, b.head);
    }
    int size()
    {
        return N;
    }
    void sort()
    {
        showR(head);
        std::cout<<"\n";
    }
    void levelorder()
    {
        bfs(head);
    }
};


#endif