// Copyright (c) 2024 l2xl
// Distributed under the MIT software license, see the accompanying
// file LICENSE or https://opensource.org/license/mit

#include "secret_store.hpp"

#include "aes.h"
#include "random.h"

#include <filesystem>
#include <fstream>
#include <thread>

namespace bscratcher {

void SecureBlobStore::LoadFromDisk(l15::sensitive_bytevector password_hash)
{
    if (!std::filesystem::is_regular_file(m_path)) throw StoreFileError("File does not exist: " + m_path);

    l15::bytevector data(AES_BLOCKSIZE);

    std::ifstream f(m_path, std::ios::in | std::ios::binary);
    for (l15::bytevector buf(1024); f.good();) {
        auto read = f.readsome(reinterpret_cast<std::ifstream::char_type*>(buf.data()), 1024);
        if (read > 0) {
            data.reserve(data.size() + read);
            std::copy_n(buf.data(), read, cex::smartinserter(data, data.end()));
        }
        else if (f.good()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
    if (!f.eof()) throw StoreFileError("File read error: " + m_path);

    uint8_t fake_iv[AES_BLOCKSIZE] = {};
    GetRandBytes(fake_iv);

    AES256CBCDecrypt aes(password_hash.data(), fake_iv, true);

    l15::sensitive_bytevector out(data.size());

    auto len = aes.Decrypt(data.data(), data.size(), out.data());
    if (len != data.size()) throw StoreFileError("Decrypt");

    out.erase(out.begin(), out.begin() + AES_BLOCKSIZE); // Always greater AES_BLOCKSIZE
    m_blob = move(out);
}

void SecureBlobStore::SaveToDisk(l15::sensitive_bytevector password_hash)
{
    uint8_t iv[AES_BLOCKSIZE] = {};
    GetStrongRandBytes(iv);

    l15::sensitive_bytevector input;
    input.reserve(m_blob.size() + AES_BLOCKSIZE);
    input.resize(AES_BLOCKSIZE);

    std::copy_n(m_blob.begin(), m_blob.size(), cex::smartinserter(input, input.end()));

    AES256CBCEncrypt aes(password_hash.data(), iv, true);

    size_t out_size = input.size();
    size_t block_rem = out_size % AES_BLOCKSIZE;
    if (block_rem > 0) {
        out_size += (AES_BLOCKSIZE - block_rem);
    }
    l15::bytevector out(out_size);

    auto len = aes.Encrypt(input.data(), input.size(), out.data());
    if (len != out_size) throw StoreFileError("Encrypt");

    std::ofstream f(m_path, std::ios::out | std::ios::binary);
    if (!f.good()) throw StoreFileError("File open error: " + m_path);
    f.write(reinterpret_cast<std::ofstream::char_type*>(out.data()), out_size);
    if (!f.good()) throw StoreFileError("File write error: " + m_path);
    f.close();
}

}
