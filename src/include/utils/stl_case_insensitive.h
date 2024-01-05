//
// Created by Bram Nijenkamp on 05-01-2024.
//

#pragma once
#include <string_view>

// Concept that checks that T is either a std::string or std::string_view.
template<typename T>
concept StringLike = std::is_same_v<std::string, std::remove_cvref_t<T>> || std::is_same_v<std::string_view, std::remove_cvref_t<T>>;

struct CaseInsensitiveHash {
    template<StringLike T>
    size_t operator()(const T& key) const {
        // You might need a better hash function than this
        size_t h = 0;
        for (auto c : key) {
            h += std::tolower(c);
        }
        return h;
    }
};
struct CaseInsensitiveEqual {
    template<StringLike T>
    bool operator()(const T& left, const T& right) const {
        if (left.size() != right.size()) return false;
        for (size_t i = 0; i < left.size(); i++) {
            if (std::tolower(left[i]) != std::tolower(right[i])) {
                return false;
            }
        }
        return true;
    }
};
template<StringLike Key, typename Value>
using idictionary = std::unordered_map<Key, Value, CaseInsensitiveHash, CaseInsensitiveEqual>;