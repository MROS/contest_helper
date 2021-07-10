namespace Kmp {

    // 計算 s 的 fail function
    // 欲求 boder 可用 border[i] = fail[i + 1]
    vector<int> get_fail(const char *s) {
        int len = strlen(s);
        if (len == 0) { return {-1}; } // 無意義

        vector<int> fail(len + 1);
        fail[0] = -1;

        int i = 1, j = 0;

        while (i < len) {
            if (j == -1 || s[i] == s[j]) {
                i++;
                j++;
                fail[i] = j;
            } else {
                j = fail[j];
            }
        }

        return fail;
    }

    // 返回所有 p 在 s 出現的索引位置
    vector<int> search(const char *s, const char *p) {
        int i = 0, j = 0;
        int s_len = strlen(s);
        int p_len = strlen(p);

        vector<int> fail = get_fail(p);
        vector<int> ret;

        while (i < s_len) {
            if (j == -1 || s[i] == p[j]) {
                i++; j++;
            } else {
                j = fail[j];
            }
            if (j == p_len) {
                ret.push_back(i - p_len);
            }
        }
        return ret;
    }

}