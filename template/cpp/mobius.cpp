class Mobius {
public:
	vector<bool> is_prime;
	vector<int> primes;
	vector<int> mobius;
	// 計算 1 ~ n 的莫比烏斯函數
	Mobius(int n) {
		is_prime = vector<bool>(n + 1, true);
		mobius = vector<int>(n + 1, 0);
		mobius[1] = 1;
		for (int i = 2; i <= n; i++) {
			if (is_prime[i]) {
				primes.push_back(i);
				mobius[i] = -1; // 質數爲 -1
			}
			for (int p: primes) {
				if (p * i > n) {
					break;
				}
				is_prime[p * i] = false;
				if (i % p == 0) {
					mobius[p * i] = 0; // 有平方因數爲 0
					break;
				} else {
					mobius[p * i] = mobius[p] * mobius[i];
				}
			}
		}
	}
	int operator[](int i) {
		return mobius[i];
	}
};
