class AC {
public:
    struct Node {
        int child[26];   // child pointer
        int parent;       // parent pointer
        int fail;        // failure pointer
        int val;
        int depth;
        unordered_set<int> word_ids;  // 狀態機中從 root 到該節點爲結尾對應的的 word 編號 (因允許有相同的 word 被加入，此處選用 set)
                             // for example, words = ["a", "aa", "aaa", "aa"], the second node 'a' have word_ids = {1, 3}
        int word_end_count;  // 和 word_ids 不同，這邊統計所有 (不限定起源於 root) 可以以此 node 作為最後一個 node 的數量
                             // for example, words = ["a", "aa", "aaa", "aa"], the second node 'a' have word_end_count = 3 (對應 word_ids 0, 1, 3)
    };

    AC(vector<string> words) {
        this->words = words;
        build_tree();
        build_failure();
        build_word_end_count();
    }

    unordered_map<int, int> get_matched_words(const string& sentence) {  // mapping of wordIdx <-> counts
        vector<int> cnt(nodes.size(), 0);
        unordered_map<int, int> ret;
        int ptr = 0;
        for (int i = 0; i < sentence.size(); ++i) {
            auto ch = sentence[i];
            while(ptr != 0 && nodes[ptr].child[ch - 'a'] == -1)  ptr = nodes[ptr].fail;
            if (nodes[ptr].child[ch - 'a'] != -1)  ptr = nodes[ptr].child[ch - 'a'];
            cnt[ptr] += 1;
        }

        priority_queue<pair<int, int>> pq;  // (depth, idx)
        for (int i = 0; i < nodes.size(); ++i)  pq.push({nodes[i].depth, i});
        while (!pq.empty()) {
            auto x = pq.top(); pq.pop();
            int idx = x.second;
            for (auto word_idx : nodes[idx].word_ids)  ret[word_idx] = cnt[idx];
            cnt[nodes[idx].fail] += cnt[idx];
        }

        return ret;
    }

    void print_trie() {
        for (int i = 0; i < nodes.size(); ++i) {
            cout << "====== idx " << i << " ======" << endl;
            cout << "val " << nodes[i].val << endl;
            cout << "parent " << nodes[i].parent << endl;
            cout << "child";
            for (int j = 0; j < 26; ++j)  if (nodes[i].child[j] != -1)  cout << " (" << j << "," << nodes[i].child[j] << ")";
            cout << endl;
            cout << "fail " << nodes[i].fail << endl;
            cout << "word_ids";
            for (auto id : nodes[i].word_ids)  cout << " " << id << ",";
            cout << endl;
            cout << "word_end_count " << nodes[i].word_end_count << endl;
        }
    }

    string get_word(int idx) {
        return words[idx];
    }

private:
    int get_new_node(int parent, int val, int depth) {
        nodes.push_back(Node());
        Node *node = &nodes[nodes.size() - 1];
        memset(node->child, -1, sizeof(node->child));
        node->parent = parent;
        node->val = val;
        node->depth = depth;
        node->word_end_count = 0;
        return nodes.size() - 1;
    }

    void build_tree() {
        get_new_node(0, -1, 0);  // root
        for (int i = 0; i < words.size(); ++i) {
            add_word_to_trie(words[i], i);
        }
    }

    void add_word_to_trie(const string& word, const int wordIdx) {
        int ptr = 0;  // root
        int depth = 0;
        for (auto ch : word) {
            ++depth;
            if (nodes[ptr].child[ch - 'a'] == -1) {
                int child_ptr = get_new_node(ptr, ch - 'a', depth);
                nodes[ptr].child[ch - 'a'] = child_ptr;
            }
            ptr = nodes[ptr].child[ch - 'a'];
        }
        nodes[ptr].word_ids.insert(wordIdx);
    }

    void build_failure() {
        queue<int> q;
        q.push(0);
        while (q.size()) {
            int idx = q.front(); q.pop();

            if (idx == 0 || nodes[idx].parent == 0)  nodes[idx].fail = 0;
            else {
                int ptr = nodes[nodes[idx].parent].fail;
                while (ptr != 0 && nodes[ptr].child[nodes[idx].val] == -1)  ptr = nodes[ptr].fail;
                nodes[idx].fail = nodes[ptr].child[nodes[idx].val] == -1 ? 0 : nodes[ptr].child[nodes[idx].val];
            }

            for (int i = 0; i < 26; ++i) {
                if (nodes[idx].child[i] == -1)  continue;
                q.push(nodes[idx].child[i]);
            }
        }
    }

    void build_word_end_count() {
        queue<int> q;
        q.push(0);
        while (q.size()) {
            int idx = q.front(); q.pop();

            nodes[idx].word_end_count = nodes[idx].word_ids.size();

            int idx_fail = nodes[idx].fail;
            nodes[idx].word_end_count += nodes[idx_fail].word_end_count;

            for (int i = 0; i < 26; ++i)  if (nodes[idx].child[i] != -1)  q.push(nodes[idx].child[i]);
        }
    }

    vector<Node> nodes;
    vector<string> words;
};