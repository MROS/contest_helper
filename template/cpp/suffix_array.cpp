struct Suffix {
    int index; // 從 index 到尾的後綴
    ULL value;
};

class SuffixArray {
private:
    static bool cmp(Suffix x, Suffix y) {
        return x.value < y.value;
    }
public:
    vector<int> sa;   // 後綴數組
    vector<int> rank; // rank[i] 代表第 i 個後綴在後綴數組中的排名
    vector<int> lcp;  // 高度數組

    SuffixArray(string &s) {
        int len = s.size();
        sa = vector<int>(len + 1);
        rank = vector<int>(len + 1);
        lcp = vector<int>(len);

        // 先於 suffixs 上操作，逐步將後綴長度倍增
        // 最後再將結果存到 sa 上
        vector<Suffix> suffixs(len + 1);

        // 開始計算後綴數組
        for (int i = 0; i <= len; i++) {
            suffixs[i].index = i;
            suffixs[i].value = s[i];
        }

        for (int k = 1; k <= len * 2; k *= 2) {
            // 排序
            sort(suffixs.begin(), suffixs.end(), this->cmp);

            // 重新計算 value ，以進行下一趟排序

            int v = 0;
            for (int i = 0; i < len; i++) {
                int index = suffixs[i].index;
                if (suffixs[i].value < suffixs[i + 1].value) { // 小於
                    rank[index] = v++;
                } else { // 相等
                    rank[index] = v;
                }
            }
            rank[suffixs[len].index] = v;

            for (int i = 0; i <= len; i++) {
                int index = suffixs[i].index;
                ULL left = rank[index];
                ULL right = (index + k > len ? 0 : rank[index + k] + 1); // + 1 後至少比 0 來得大
                // 左半部左移 32 位，即可製造出先比左再比右的效果
                suffixs[i].value = (left << 32) + right;
            }
        }

        for (int i = 0; i <= len; i++) {
            sa[i] = suffixs[i].index;
        }

        // 開始計算高度數組
        int h = 0;
        lcp[0] = 0;
        for (int i = 0; i < len; i++) {
            int j = sa[rank[i] - 1];

            if (h > 0) { h--; }
            for (; j + h < len && i + h < len; h++) {
                if (s[j + h] != s[i + h]) { break; }
            }

            lcp[rank[i] - 1] = h;
        }
    }
};
