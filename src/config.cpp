// Copyright (c) 2024 l2xl
// Distributed under the MIT software license, see the accompanying
// file LICENSE or https://opensource.org/license/mit

#include "exception"
#include "config.hpp"

namespace bscratcher {

const char * const CONF = "--config";
const char * const DATADIR = "--data-dir,-d";
const char * const VERBOSE = "--verbose,-v";
const char * const TRACE = "--debug-trace,-t";
const char * const CHAIN = "--chain-mode,-m";


void Config::parse_chain_mode(const std::string &val)
{
    if (val == "mainnet") {
        m_chain = chainmode::MAINNET;
    }
    else if (val == "testnet") {
        m_chain = chainmode::TESTNET;
    }
    else if (val == "signet") {
        m_chain == chainmode::SIGNET;
    }
    else if (val == "regtest") {
        m_chain = chainmode::REGTEST;
    }
    else {
        throw std::invalid_argument(val);
    }
}

Config::Config(int argc, const char *const argv[])
{
    mApp.set_config(CONF, "bscratcher.conf", "Read the configuration file");
    mApp.set_version_flag("--version", [](){ return std::string("BScratcher Wallet v.0.1"); });
    mApp.set_help_flag("--help,-h", "Display this help information and exit");
    mApp.add_flag(TRACE, mTrace, "Print debug traces to log");
    mApp.add_option(DATADIR, mDataDir, "Directory path to store wallet data");
    mApp.add_option_function(CHAIN, std::function<void(const std::string&)>([&](const auto& val){parse_chain_mode(val);}), "Chain mode (mainnet, testnet, signet, regtest) [default - mainnet]");

    try {
        mApp.parse(argc, argv);
    }
    catch (const CLI::ParseError &e) {
        mApp.exit(e);
        std::rethrow_exception(std::current_exception());
    }
}

}
