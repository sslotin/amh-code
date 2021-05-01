float *alloc(int n) {
    return static_cast<float*>(std::aligned_alloc(32, n));
}
