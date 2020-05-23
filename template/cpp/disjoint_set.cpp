class DisjointSet {
public:
    DisjointSet(int sz): sz(sz) {  // 初始化 sz 個元素
        par.resize(sz);
        rank.resize(sz);
        for (int i = 0; i < sz; ++i) {
            par[i] = i;
            rank[i] = 0;
        }
    }
    int find(int x) {  // 查詢樹的根
        // 為了簡單起見，即使樹的高度發生了變化，我們也不修改 rank 的值
        if (par[x] == x)  return x;
        return par[x] = find(par[x]);
    }
    void unite(int x, int y) {  // 合併 x 和 y 所屬的集合
        x = find(x);
        y = find(y);
        if (x == y)  return;
        if (rank[x] < rank[y]) {
            par[x] = y;
        } else {
            par[y] = x;
            if (rank[x] == rank[y])  ++rank[x];
        }
    }
    bool same(int x, int y) {  // 判斷 x 和 y 是否屬於同一集合
        return find(x) == find(y);
    }
private:
    vector<int> par, rank;
    int sz;
};
