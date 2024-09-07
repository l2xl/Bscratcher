// Copyright (c) 2024 l2xl
// Distributed under the MIT software license, see the accompanying
// file LICENSE or https://opensource.org/license/mit

#ifndef SECRET_STORE_HPP
#define SECRET_STORE_HPP

#include <common.hpp>
#include <memory>

#include "hash.h"
#include "utils.hpp"

namespace bscratcher {

class StoreFileError : public std::logic_error
{
public:
    explicit StoreFileError(const string& m) : logic_error(m) {}
    explicit StoreFileError(string&& m) : logic_error(move(m)) {}
    explicit StoreFileError(const char* m) : logic_error(m) {}
};

class SecureBlobStore {
    std::string m_path;
    l15::sensitive_bytevector m_blob;

    void LoadFromDisk(l15::sensitive_bytevector password_hash);
    void SaveToDisk(l15::sensitive_bytevector password_hash);
public:
    explicit SecureBlobStore(std::string path, const l15::sensitive_string& password) : m_path(move(path))
    {
        LoadFromDisk(l15::cryptohash<l15::sensitive_bytevector>(l15::sensitive_bytevector(password.begin(), password.end()), CHash256()));
    }
    void Update(l15::sensitive_bytevector data, const l15::sensitive_string& password)
    {
        m_blob = move(data);
        SaveToDisk(l15::cryptohash<l15::sensitive_bytevector>(l15::sensitive_bytevector(password.begin(), password.end()), CHash256()));
    }

    [[nodiscard]]
    const l15::sensitive_bytevector& Data() const
    { return m_blob; }
};
}

#endif //SECRET_STORE_HPP
