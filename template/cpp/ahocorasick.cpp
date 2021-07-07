class AhoCorasick {
public:
    struct node {
        int ch[26];   // child pointer
        int pa;       // parent pointer
        int f;        // failure pointer
        int val;
        int depth;
        bool isWord;
    }typedef Node;

    AhoCorasick(int sz) {
        nodes_.resize(sz);
    }

    void AddWordToDictionary(const string& word) {
        words_.push_back(word);
    }

    void BuildAll() {
        BuildTrie();
        BuildFailure();
    }

    unordered_map<int, int> GetMatchedWords(const string& sentence) {  // mapping of wordIdx <-> counts
        vector<int> cnt(sz_, 0);
        unordered_map<int, int> ret;
        int ptr = 0;
        for (int i = 0; i < sentence.size(); ++i) {
            auto ch = sentence[i];
            while(ptr != 0 && nodes_[ptr].ch[ch - 'a'] == -1)  ptr = nodes_[ptr].f;
            if (nodes_[ptr].ch[ch - 'a'] != -1)  ptr = nodes_[ptr].ch[ch - 'a'];
            cnt[ptr] += 1;
        }

        priority_queue<pair<int, int>> pq;  // (depth, idx)
        for (int i = 0; i < sz_; ++i)  pq.push({nodes_[i].depth, i});
        while (!pq.empty()) {
            auto x = pq.top(); pq.pop();
            if (nodes_[x.second].isWord)  for (auto idx : m_[x.second])  ret[idx] = cnt[x.second];
            cnt[nodes_[x.second].f] += cnt[x.second];
        }

        return ret;
    }

    void PrintTrie() {
        for (int i = 0; i < sz_; ++i) {
            cout << "====== idx " << i << " ======" << endl;
            cout << "val " << nodes_[i].val << endl;
            cout << "pa " << nodes_[i].pa << endl;
            cout << "ch";
            for (int j = 0; j < 26; ++j)  if (nodes_[i].ch[j] != -1)  cout << " (" << j << "," << nodes_[i].ch[j] << ")";
            cout << endl;
            cout << "f " << nodes_[i].f << endl;
            cout << "isWord " << (nodes_[i].isWord ? "true" : "false") << endl;
        }
    }

    void Clear() {
        words_.clear();
        m_.clear();
        sz_ = 0;
    }

    string GetWord(int idx) {
        return words_[idx];
    }

private:
    int GetNewNode(int pa, int val, int depth) {
        memset(nodes_[sz_].ch, -1, sizeof(nodes_[sz_].ch));
        nodes_[sz_].pa = pa;
        nodes_[sz_].val = val;
        nodes_[sz_].depth = depth;
        nodes_[sz_].isWord = false;
        return sz_++;
    }

    void BuildTrie() {
        sz_ = 0;
        GetNewNode(0, -1, 0);  // root
        for (int i = 0; i < words_.size(); ++i) {
            AddWordToTrie(words_[i], i);
        }
    }

    void AddWordToTrie(const string& word, const int wordIdx) {
        int ptr = 0;  // root
        int d = 0;
        for (auto ch : word) {
            ++d;
            if (nodes_[ptr].ch[ch - 'a'] == -1) {
                nodes_[ptr].ch[ch - 'a'] = GetNewNode(ptr, ch - 'a', d);
            }
            ptr = nodes_[ptr].ch[ch - 'a'];
        }
        nodes_[ptr].isWord = true;
        m_[ptr].insert(wordIdx);
    }

    void BuildFailure() {
        queue<int> q;
        q.push(0);
        while (q.size()) {
            int idx = q.front(); q.pop();

            if (idx == 0 || nodes_[idx].pa == 0)  nodes_[idx].f = 0;
            else {
                int ptr = nodes_[nodes_[idx].pa].f;
                while (ptr != 0 && nodes_[ptr].ch[nodes_[idx].val] == -1)  ptr = nodes_[ptr].f;
                nodes_[idx].f = nodes_[ptr].ch[nodes_[idx].val] == -1 ? 0 : nodes_[ptr].ch[nodes_[idx].val];
            }

            for (int i = 0; i < 26; ++i) {
                if (nodes_[idx].ch[i] == -1)  continue;
                q.push(nodes_[idx].ch[i]);
            }
        }
    }

    vector<Node> nodes_;
    int sz_;
    vector<string> words_;
    unordered_map<int, unordered_set<int>> m_;
};
