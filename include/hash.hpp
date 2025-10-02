namespace scbe {

inline void hashCombine(std::size_t& seed, std::size_t value) {
    seed ^= value + 0x9e3779b97f4a7c15ULL + (seed << 6) + (seed >> 2);
}

template <typename... Ts>
inline std::size_t hashValues(const Ts&... values) {
    std::size_t seed = 0;
    (hashCombine(seed, std::hash<Ts>{}(values)), ...);
    return seed;
}

inline size_t hashTypes(const std::vector<Type*>& types) {
    size_t hash = 0;
    for(auto type : types) {
        size_t hashCur = std::hash<Type*>{}(type);
        hash ^= hashCur + 0x9e3779b9 + (hash << 6) + (hash >> 2);
    }
    return hash;
}

}