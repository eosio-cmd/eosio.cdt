#!/bin/bash
cleos wallet unlock --password < ~/eosio-wallet/wallet.password
cleos create account eosio kvtest EOS6wKpe8WFqfUnvTmMLy8Es5XjpbQSLTEaqSvC1c86viMp6Lmb8w
cleos create account eosio testkv EOS6wKpe8WFqfUnvTmMLy8Es5XjpbQSLTEaqSvC1c86viMp6Lmb8w

curl -X POST http://127.0.0.1:8888/v1/producer/schedule_protocol_feature_activations -d '{"protocol_features_to_activate": ["0ec7e080177b2c02b278d5088611686b49d739925a92d9bfcacd7fc6b74053bd"]}'

sleep 10

pushd /Users/jsmith/workspace/block.one/EOSIO/eosio.contracts/build/contracts/eosio.system || exit
cleos set abi eosio eosio.system.abi
cleos set code eosio eosio.system.wasm
popd || exit

sleep 10

cleos push action eosio activate '["6a3c774b32cd81ef1d2fb76c609fd227406115c511b6d34a2a9453d478a94b8f"]' -p eosio
