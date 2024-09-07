// Copyright (c) 2024 l2xl
// Distributed under the MIT software license, see the accompanying
// file LICENSE or https://opensource.org/license/mit

#include "wallet.hpp"
#include "secret_store.hpp"
#include "mnemonic.hpp"
#include "words.hpp"

#include "smartinserter.hpp"

#include "secp256k1.h"

#include <algorithm>

namespace {
    const char* const s_rootkey_filename = "data.bin";
}


namespace bscratcher {
l15::sensitive_bytevector Wallet::SerializeKeyFile(const std::list<l15::sensitive_bytevector> &keys)
{
    size_t len = std::accumulate(keys.begin(), keys.end(), keys.size(), [](size_t a, const auto& k) { return a + k.size();});
    l15::sensitive_bytevector res;
    res.reserve(len);
    for (const auto k: keys) {
        res.push_back((uint8_t)k.size());
        std::copy(k.begin(), k.end(), cex::smartinserter(res, res.end()));
    }
    return res;
}

std::list<l15::sensitive_bytevector> Wallet::ParseKeyFile(const l15::sensitive_bytevector &blob)
{
    std::list<l15::sensitive_bytevector> keys;
    for (auto it = blob.begin(); it != blob.end(); ) {
        const size_t klen = *it++;

        if (blob.end() - it < klen) throw InconsistentDataError("keystorage");

        l15::sensitive_bytevector k;
        k.reserve(klen);
        std::copy_n(it, klen, cex::smartinserter(k, k.end()));

        keys.emplace_back(move(k));

        std::advance(it, klen);
    }
    return keys;
}

Wallet::Wallet(std::shared_ptr<Config> config, l15::sensitive_string wallet_password)
    : mConfig(config)
{
    m_ctx = secp256k1_context_create(SECP256K1_CONTEXT_SIGN | SECP256K1_CONTEXT_VERIFY);
    std::vector<unsigned char, secure_allocator<unsigned char>> vseed(32);
    RandomInit();
    GetRandBytes(vseed);
    int ret = secp256k1_context_randomize(m_ctx, vseed.data());
    assert(ret);

    cur_key_path = std::filesystem::path(config->DataDir()) / s_rootkey_filename;

    const l15::core::MnemonicParser<const l15::stringvector&> mnemonicParser(words::words_en);

    SecureBlobStore keyStore(cur_key_path, move(wallet_password));
    for (const auto& root_entropy: ParseKeyFile(keyStore.Data())) {
        l15::sensitive_stringvector phrase = mnemonicParser.EncodeEntropy(root_entropy);

        //TODO: implement passphrase for mnemonic
        l15::sensitive_bytevector seed = mnemonicParser.MakeSeed(phrase, "");

        m_key_registry.emplace_back(m_ctx, l15::Bech32(), seed);
    }

}

void Wallet::AddKeyRoot(l15::sensitive_bytevector secret, l15::sensitive_string wallet_password)
{
    const l15::core::MnemonicParser<const l15::stringvector&> mnemonicParser(words::words_en);
    l15::sensitive_stringvector phrase = mnemonicParser.EncodeEntropy(secret);

    //TODO: implement passphrase for mnemonic
    l15::sensitive_bytevector seed = mnemonicParser.MakeSeed(phrase, "");

    m_key_registry.emplace_back(m_ctx, l15::Bech32(), seed);

    cur_key_path = std::filesystem::path(mConfig->DataDir()) / s_rootkey_filename;
    SecureBlobStore keyStore(cur_key_path, move(wallet_password));

    auto keys = ParseKeyFile(keyStore.Data());
    keys.emplace_back(secret);
    keyStore.Update(SerializeKeyFile(keys), wallet_password);

}
} // bscratcher