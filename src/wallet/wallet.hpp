// Copyright (c) 2024 l2xl
// Distributed under the MIT software license, see the accompanying
// file LICENSE or https://opensource.org/license/mit

#ifndef WALLET_HPP
#define WALLET_HPP

#include <memory>
#include <filesystem>
#include "common.hpp"
#include "keyregistry.hpp"
#include "config.hpp"


namespace bscratcher {

class InconsistentDataError : public std::logic_error
{
public:
    explicit InconsistentDataError(const string& m) : logic_error(m) {}
    explicit InconsistentDataError(string&& m) : logic_error(move(m)) {}
    explicit InconsistentDataError(const char* m) : logic_error(m) {}
};

class Wallet {
    std::shared_ptr<Config> mConfig;

    secp256k1_context* m_ctx;

    std::filesystem::path cur_key_path;
    std::list<l15::core::KeyRegistry> m_key_registry;

    static l15::sensitive_bytevector SerializeKeyFile(const std::list<l15::sensitive_bytevector>& keys);
    static std::list<l15::sensitive_bytevector> ParseKeyFile(const l15::sensitive_bytevector& blob);

public:
    explicit Wallet(std::shared_ptr<Config> config, l15::sensitive_string wallet_password);
    void AddKeyRoot(l15::sensitive_bytevector secret, l15::sensitive_string wallet_password);
};

} // bscratcher

#endif //WALLET_HPP
