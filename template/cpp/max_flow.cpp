struct Edge {
    int to;
    int cap; // 容量
    int rev; // 反向邊編號
};

struct Node {
    int level; // 分層圖中的所處層級
    int cur; // 當前弧
};

struct Graph {
    vector<vector<Edge> > edges;     // edges[x][i] 代表第 x 點的第 i 個出邊
    vector<Node> nodes;

private:
    // 構造分層圖，若源點匯點之間仍有增廣路，回傳真；無增廣路則回傳假
    bool bfs(int s, int t) {
        // 先將層級清爲 -1
        for (int i = 0; i < nodes.size(); i++) {
            nodes[i].level = -1;
            nodes[i].cur = 0;
        }
        // 開始 bfs
        nodes[s].level = 0;
        queue<int> q;
        q.push(s);
        while (!q.empty()) {
            int x = q.front(); q.pop();
            for (int i = 0; i < edges[x].size(); i++) {
                Edge edge = edges[x][i];
                int next = edge.to;
                if (edge.cap > 0 && nodes[next].level == -1) {
                    nodes[next].level = nodes[x].level + 1;
                    q.push(next);
                }
            }
        }
        return nodes[t].level != -1;
    }

    // 在分層圖上找增廣路
    // 目前在 v ，匯點在 t ，到此的流量爲 f
    int dfs(int v, int t, int f) {
        // 已到匯點，直接返回 f
        if (v == t) { return f; }

        // 未到匯點，嘗試繼續 dfs
        for (int &i = nodes[v].cur; i < edges[v].size(); i++) {
            Edge &e = edges[v][i];
            if (e.cap > 0 && nodes[e.to].level > nodes[v].level) {
                int r = dfs(e.to, t, min(f, e.cap));
                if (r > 0) {
                    e.cap -= r;
                    edges[e.to][e.rev].cap += r;
                    return r;
                }
            }
        }
        return 0;
    }

public:
    Graph(int number) {
        edges = vector<vector<Edge> >(number + 1);   // +1 以適應索引從 1 起算的狀況
        nodes = vector<Node>(number + 1);   // +1 以適應索引從 1 起算的狀況
    }
    void add_edges(int from, int to, int cap) {
        edges[from].push_back(Edge {to, cap, (int)edges[to].size()});
        edges[to].push_back(Edge {from, 0, (int)edges[from].size() - 1});
    }
    // 求解 s 到 t 的最大流
    int max_flow(int s, int t) {
        int ans = 0;
        while (bfs(s, t)) {
            int f;
            while ((f = dfs(s, t, 2147483647)) > 0) {
                ans += f;
            }
        }
        return ans;
    }
};
