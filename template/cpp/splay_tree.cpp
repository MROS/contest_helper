struct Node {
    int idx;
    int value;
    int size;  // 子樹大小
    int inverse;  // inverse flag，僅為了 flip 操作而生
    struct Node* ch[2];
    int compare(int k) {  // 0 左邊, 1 右邊
        if (ch[0]->size + 1 == k)  return -1;
        return k <= ch[0]->size ? 0 : 1;
    }
    void maintain() {  // 重新計算子樹大小
        size = ch[0]->size + 1 + ch[1]->size;
    }
    void pushdown() {  // 將 inverse 標記往下傳
        if (!inverse)  return;
        swap(ch[0], ch[1]);
        if (ch[0]->idx != -1)  ch[0]->inverse = 1 - ch[0]->inverse;
        if (ch[1]->idx != -1)  ch[1]->inverse = 1 - ch[1]->inverse;
        inverse = 0;
    }
};

class SplayTree {
public:
    SplayTree(int n);
    void insert(int v, int k);  // 插入 v 成為左邊數來第 k-th 節點
    void ordered_insert(int v);  // 插入 v 於有序序列中的位置，需原先序列為排序狀態下才有意義
    int rank(int v);  // 查詢數值 v 在此序列中的最小排名，需原先序列為排序狀態下才有意義
    void remove_by_rank(int k);  // 移除一個數值為 v 的節點
    void remove_by_value(int k);  // 移除左邊數來第 k-th 節點
    void flip(int k1, int k2);  // 翻轉區間 [k1, k2]，可參考 LibreOJ #105
    int get(int k);  // 獲取左邊數來第 k-th 節點的數值
    void print();
private:
    Node* get_node(int v);
    void splay(Node* &tree, int k);  // 把左邊數來第 k-th 節點旋轉到樹根
    void rotate(Node* &tree, int d);  // 0 左旋, 1 右旋

    vector<Node> nodes;
    unordered_set<int> nodes_available_index;
    Node null_node;
    Node* root;
};

SplayTree::SplayTree(int n) {
    nodes.resize(n);
    for (int i = 0; i < n; ++i)  nodes_available_index.insert(i);
    null_node.idx = -1;
    null_node.size = 0;
    null_node.inverse = 0;
    null_node.ch[0] = null_node.ch[1] = &null_node;
    root = &null_node;
}

Node* SplayTree::get_node(int v) {
    int idx = *nodes_available_index.begin();
    nodes_available_index.erase(idx);
    nodes[idx].idx = idx;
    nodes[idx].value = v;
    nodes[idx].size = 1;
    nodes[idx].inverse = 0;
    nodes[idx].ch[0] = nodes[idx].ch[1] = &null_node;
    return &(nodes[idx]);
}

void SplayTree::splay(Node* &tree, int k) {
    tree->pushdown();
    int d = tree->compare(k);
    if (d == -1)  return;
    if (d == 1) {
        k -= tree->ch[0]->size + 1;
    }
    Node* p = tree->ch[d];
    p->pushdown();
    int d2 = p->compare(k);
    int k2 = (d2 == 0 ? k : k - p->ch[0]->size - 1);
    if (d2 != -1) {
        splay(p->ch[d2], k2);
        if (d == d2) {
            rotate(tree, d ^ 1);
        } else {
            rotate(tree->ch[d], d2 ^ 1);
        }
    }
    rotate(tree, d ^ 1); 
}

void SplayTree::rotate(Node* &tree, int d) {
    Node* k = tree->ch[d ^ 1]; 
    if (k == &null_node)  return;
    tree->ch[d ^ 1] = k->ch[d];
    k->ch[d] = tree;
    tree->maintain();
    k->maintain();
    tree = k;
}

void SplayTree::insert(int v, int k) {
    Node* node = get_node(v);
    if (k == root->size + 1) {
        node->ch[0] = root;
    } else {
        splay(root, k);
        node->ch[0] = root->ch[0];
        root->ch[0] = &null_node;
        root->maintain();
        node->ch[1] = root;
    }
    node->maintain();
    root = node;
}

void SplayTree::ordered_insert(int v) {
    int r = rank(v);
    insert(v, r);
}

int SplayTree::rank(int v) {
    int size = root->size;
    if (size == 0)  return 1;
    int mi = get(1);
    int ma = get(size);
    if (v <= mi)  return 1;
    if (v > ma)  return size + 1;
    int L = 1, R = size;
    while (R - L > 1) {
        int mid = (L + R) / 2;
        if (get(mid) >= v)  R = mid;
        else  L = mid;
    }
    return R;
}

void SplayTree::remove_by_value(int v) {
    remove_by_rank(rank(v));
}

void SplayTree::remove_by_rank(int k) {
    splay(root, k);
    nodes_available_index.insert(root->idx);
    if (root->ch[0]->size) {
        splay(root->ch[0], k - 1);
        root->ch[0]->ch[1] = root->ch[1];
        root->ch[0]->maintain();
        root = root->ch[0]; 
    } else {
        root = root->ch[1];
    }
}

void SplayTree::print() {
    for (int i = 0; i < nodes.size(); ++i) {
        if (nodes_available_index.find(i) != nodes_available_index.end())  continue;
        cout << "============idx " << i;
        cout << "value " << nodes[i].value << " size " << nodes[i].size << endl;
        cout << "ch[0] " << nodes[i].ch[0]->idx << " ch[1] " << nodes[i].ch[1]->idx << endl;
        cout << "inverse " << (nodes[i].inverse ? "true" : "false") << endl;
    }
}

void SplayTree::flip(int k1, int k2) {
    splay(root, k1);
    Node* left = root->ch[0];
    root->ch[0] = &null_node;
    root->maintain();    
    splay(root, k2 - k1 + 1);
    Node* right = root->ch[1];
    root->ch[1] = &null_node;
    root->maintain();

    root->inverse = 1 - root->inverse;

    splay(root, 1);
    root->ch[0] = left;
    root->maintain();
    splay(root, root->size);
    root->ch[1] = right;
    root->maintain();
}

int SplayTree::get(int k) {
    splay(root, k);
    return root->value;
}
