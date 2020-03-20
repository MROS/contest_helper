namespace Kmp {

    // 計算 s 的 prefix function
    vector<int> get_prefix(string &s) {
        int len = s.size();
        if (len == 0) {
            return vector<int>();
        }

        vector<int> ret(len);
        ret[0] = -1;

        int i = 1, j = 0;

        while (i < len - 1) {
            if (j == -1 || s[i] == s[j]) {
                i++;
                j++;
                ret[i] = j;
            } else {
                j = ret[j];
            }
        }

        return ret;
    }

    // 若 s 中有 p ，則返回第一次出現的索引
    // 否則返回 -1
    int search(string &s, string &p) {
        int i = 0, j = 0;
        int s_len = s.size();
        int p_len = p.size();

        vector<int> prefix = get_prefix(p);

        while (i < s_len && j < p_len) {
            if (j == -1 || s[i] == p[j]) {
                i++; j++;
            } else {
                j = prefix[j];
            }
        }
        if (j == p.size()) {
            return i - p.size();
        }
        return -1;
    }

}
