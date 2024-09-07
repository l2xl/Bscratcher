# Bitcoin Scratcher Wallet

The project intended to provide alternative UTXO-based blockchain infrastructure.

The main design intentions:

1. Use C++ as main language

1. High level of modular structure which must allow to construct arbitrary UTXO-based blockchain software

1. The first phase supposes to create a lightweight BTC wallet with extended functionality which will allow to construct any complex transactions

1. At the end wanna to have modular system allowing to build any blockchain component like a wallet, a node, a miner or whatever may be needed  

1. Initially the project will intensively use bitcoin-core sources for implementing critical functionality; later it's supposed to migrate all the code to the project repos

1. The project repo itself intended for a "front-end" code which uses L15 project repo for core functionality; new features initially prototyped here and moved to L15 when gets a stable and mature status