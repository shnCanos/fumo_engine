#pragma once
#define SERIALIZE(...) \
    template<class Archive> \
    void serialize(Archive& archive) { \
        archive(__VA_ARGS__); \
    }
