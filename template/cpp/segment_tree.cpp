template <typename T>
class SegmentTree {
public:
    SegmentTree(int n): n(n) {
        idxMax = 1;
        while (idxMax < n) {
            idxMax <<= 1;
        }
        idxMax <<= 1;
        sum.insert(sum.end(), idxMax, 0);
        mi.insert(mi.end(), idxMax, 0);
        ma.insert(ma.end(), idxMax, 0);
        addVal.insert(addVal.end(), idxMax, 0);
        setVal.insert(setVal.end(), idxMax, -1);  // value -1 means no setVal
        rl.insert(rl.end(), idxMax, 0);  // the range of idx i node is [rl[i], rr[i]]
        rr.insert(rr.end(), idxMax, 0);
        __init(1, 1, n);
    } 
    void update_set(int L, int R, T val) {  __update_set(1, L, R, val); }
    void update_add(int L, int R, T val) {  __update_add(1, L, R, val); }
    T query_sum(int L, int R) {  return __query_sum(1, L, R); }
    T query_min(int L, int R) {  return __query_min(1, L, R); }
    T query_max(int L, int R) {  return __query_max(1, L, R); }
    void trace(int idx);  // (debugger) print the information
private:
    int n, idxMax;
    void __init(int idx, int L, int R);  // idx start from 1
    void __push_down(int idx);
    void __update_set(int idx, int L, int R, T val);  // set [L, R] as value val
    void __update_add(int idx, int L, int R, T val);  // add [L, R] by value val
    T __query_sum(int idx, int L, int R);  // initial idx is 1
    T __query_min(int idx, int L, int R);  // initial idx is 1
    T __query_max(int idx, int L, int R);  // initial idx is 1
    vector<T> sum, mi, ma, addVal, setVal;
    vector<int> rl, rr;
};

template <typename T>
void SegmentTree<T>::__init(int idx, int L, int R) {  // idx start from 1
    rl[idx] = L; rr[idx] = R;
    if (L != R) {
        int mid = (L + R) / 2;
        int lc = (idx << 1), rc = (idx << 1) + 1;
        __init(lc, L, mid);
        __init(rc, mid + 1, R);
    }
}

template <typename T>
void SegmentTree<T>::__push_down(int idx) {
    if (rl[idx] == rr[idx])  return;
    int lc = (idx << 1), rc = (idx << 1) + 1;
    if (setVal[idx] != -1){
        setVal[lc] = setVal[rc] = setVal[idx];
        addVal[lc] = addVal[rc] = 0;
        sum[lc] = setVal[idx] * (rr[lc] - rl[lc] + 1);
        sum[rc] = setVal[idx] * (rr[rc] - rl[rc] + 1);
        mi[lc] = mi[rc] = ma[lc] = ma[rc] = setVal[idx];
        setVal[idx] = -1;
    }
    if (addVal[idx] != 0){
        addVal[lc] += addVal[idx];
        addVal[rc] += addVal[idx];
        sum[lc] += addVal[idx] * (rr[lc] - rl[lc] + 1);
        sum[rc] += addVal[idx] * (rr[rc] - rl[rc] + 1);
        mi[lc] += addVal[idx];
        mi[rc] += addVal[idx];
        ma[lc] += addVal[idx];
        ma[rc] += addVal[idx];
        addVal[idx] = 0;
    }
}

template <typename T>
void SegmentTree<T>::__update_set(int idx, int L, int R, T val) {  // set [L, R] as value val
    if (rl[idx] == L && rr[idx] == R) {
        setVal[idx] = val;
        addVal[idx] = 0;
        sum[idx] = val * (rr[idx] - rl[idx] + 1);
        mi[idx] = ma[idx] = val;
        return;
    }
    __push_down(idx);
    int mid = (rl[idx] + rr[idx]) / 2;
    int lc = (idx << 1), rc = (idx << 1) + 1;
    if (R <= mid) {
        __update_set(lc, L, R, val);
    } else if (L >= mid + 1) {
        __update_set(rc, L, R, val);
    } else {
        __update_set(lc, L, mid, val);
        __update_set(rc, mid + 1, R, val);
    }
    sum[idx] = sum[lc] + sum[rc];
    mi[idx] = min(mi[lc], mi[rc]);
    ma[idx] = max(ma[lc], ma[rc]);
}

template <typename T>
void SegmentTree<T>::__update_add(int idx, int L, int R, T val) {  // add [L, R] by value val
    if (rl[idx] == L && rr[idx] == R) {
        addVal[idx] += val;
        sum[idx] += val * (rr[idx] - rl[idx] + 1);
        mi[idx] += val;
        ma[idx] += val;
        return;
    }
    __push_down(idx);
    int mid = (rl[idx] + rr[idx]) / 2;
    int lc = (idx << 1), rc = (idx << 1) + 1;
    if (R <= mid) {
        __update_add(lc, L, R, val);
    } else if (L >= mid + 1) {
        __update_add(rc, L, R, val);
    } else {
        __update_add(lc, L, mid, val);
        __update_add(rc, mid + 1, R, val);
    }
    sum[idx] = sum[lc] + sum[rc];
    mi[idx] = min(mi[lc], mi[rc]);
    ma[idx] = max(mi[lc], ma[rc]);
}

template <typename T>
T SegmentTree<T>::__query_sum(int idx, int L, int R) {
    if (rl[idx] == L && rr[idx] == R) {
        return sum[idx];
    }
    __push_down(idx);
    int mid = (rl[idx] + rr[idx]) / 2;
    int lc = (idx << 1), rc = (idx << 1) + 1;
    if (R <= mid) {
        return __query_sum(lc, L, R);
    } else if (L >= mid + 1) {
        return __query_sum(rc, L, R);
    } else {
        return __query_sum(lc, L, mid) + __query_sum(rc, mid + 1, R);
    }
}

template <typename T>
T SegmentTree<T>::__query_min(int idx, int L, int R) {
    if (rl[idx] == L && rr[idx] == R) {
        return mi[idx];
    }
    __push_down(idx);
    int mid = (rl[idx] + rr[idx]) / 2;
    int lc = (idx << 1), rc = (idx << 1) + 1;
    if (R <= mid) {
        return __query_min(lc, L, R);
    } else if (L >= mid + 1) {
        return __query_min(rc, L, R);
    } else {
        return min(__query_min(lc, L, mid), __query_min(rc, mid + 1, R));
    }
}

template <typename T>
T SegmentTree<T>::__query_max(int idx, int L, int R) {
    if (rl[idx] == L && rr[idx] == R) {
        return mi[idx];
    }
    __push_down(idx);
    int mid = (rl[idx] + rr[idx]) / 2;
    int lc = (idx << 1), rc = (idx << 1) + 1;
    if (R <= mid) {
        return __query_max(lc, L, R);
    } else if (L >= mid + 1) {
        return __query_max(rc, L, R);
    } else {
        return max(__query_max(lc, L, mid), __query_max(rc, mid + 1, R));
    }
}

template <typename T>
void SegmentTree<T>::trace(int idx) {
    printf("idx %d rl %d rr %d setVal %d addVal %d sum %d min %d max %d\n", idx, rl[idx], rr[idx], setVal[idx], addVal[idx], sum[idx]);  // FIXME how to determine the type T that needs to use in printf?
    if (rl[idx] < rr[idx]) {
        trace(idx << 1);
        trace((idx << 1) + 1);
    }
}
