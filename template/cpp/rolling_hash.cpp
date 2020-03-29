namespace RollingHash {
    const ULL B = 1000000007ULL;

    vector<ULL> prefix_hashs(string &s) {
        ULL hash = 0;
        vector<ULL> ret;
        for (int i = 0; i < s.size(); i++) {
            hash = hash * B + s[i];
            ret.push_back(hash);
        }
        return ret;
    }

    vector<ULL> suffix_hashs(string &s) {
        ULL hash = 0;
        ULL t = 1;
        vector<ULL> ret;
        for (int i = 0; i < s.size(); i++) {
            hash = hash + s[s.size() - 1 - i] * t;
            ret.push_back(hash);
            t *= B;
        }
        return ret;
    }

    // 檢查是否 s 包含 p
    bool contain(string &s, string &p) {
        if (s.size() < p.size()) { return false; }

        ULL p_h = prefix_hashs(p)[p.size() - 1];
        string ss = s.substr(0, p.size());
        ULL s_h = prefix_hashs(ss)[ss.size() - 1];

        if (p_h == s_h) { return true; }

        ULL t = 1;
        for (int i = 0; i < p.size(); i++) {
            t *= B;
        }

        for (int i = p.size(); i < s.size(); i++) {
            s_h = s_h * B + s[i] - t * s[i - p.size()];
            if (p_h == s_h) { return true; }
        }
        return false;
    }
}
