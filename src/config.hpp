// Copyright (c) 2024 l2xl
// Distributed under the MIT software license, see the accompanying
// file LICENSE or https://opensource.org/license/mit

#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "cli11/CLI11.hpp"

namespace bscratcher {
enum class chainmode {MAINNET, TESTNET, SIGNET, REGTEST};

class Config {
    CLI::App mApp;

    size_t mVerbose;
    bool mTrace;

    std::string mDataDir;
    chainmode m_chain = chainmode::MAINNET;

public:
    Config() = delete;
    Config(int argc, const char *const argv[]);

    size_t Verbose() const { return mVerbose; }
    bool Trace() const {return mTrace; }
    const std::string& DataDir() const { return mDataDir; }
};

}

#endif //CONFIG_HPP
