int invoke0(void) {
    return 0;
}

int invoke1(int a) {
    return a;
}

int invoke2(int a, int b) {
    return a + b;
}

int invoke3(int a, int b, int c) {
    return a + b + c;
}

int invoke4(int a, int b, int c, int d) {
    return a + b + c + d;
}

int invoke16(int a, int b, int c, int d,
             int e, int f, int g, int h,
             int i, int j, int k, int l,
             int m, int n, int o, int p) {
    return a + b + c + d +
        e + f + g + h +
        i + j + k + l +
        m + n + o + p;
}

int invoke1var(int a, ...) {
    return a;
}

int main() {
    invoke0();
    invoke1(1);
    invoke2(1, 2);
    invoke3(1, 2, 3);
    invoke4(1, 2, 3, 4);
    invoke16(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
    invoke1var(1);
    invoke1var(1, 99);
    return 0;
}
