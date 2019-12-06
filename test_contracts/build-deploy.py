#!/usr/bin/env python3

import os
import subprocess
import sys

#CONTRACT_NAME="kvtest"
CONTRACT_NAME=""

def main():

    contracts = ["testkv"]
    #contracts = ["kvtest", "testkv"]
    for c in contracts:
        global CONTRACT_NAME
        CONTRACT_NAME = c

        if len(sys.argv) < 1:
            build()
            deploy()
            run()
            sys.exit(0)

        command = sys.argv[1]

        if command == 'build':
            build()
        elif command == 'deploy':
            build()
            deploy()
        elif command == 'run':
            build()
            deploy()
            run()

def build():
    subprocess.run(["eosio-cpp", CONTRACT_NAME+".cpp"])

def deploy():
    subprocess.run(["cleos", "set", "abi", CONTRACT_NAME, CONTRACT_NAME+".abi"])
    subprocess.run(["cleos", "set", "code", CONTRACT_NAME, CONTRACT_NAME+".wasm"])

def run():
    args = (
        "cleos", "push", "action", CONTRACT_NAME, "test2", "[]", "-p", CONTRACT_NAME, "-j"
    )
    cleos = subprocess.Popen(args, stdout=subprocess.PIPE)
    jq = subprocess.check_output(("jq", ".processed.action_traces[0].console"), stdin=cleos.stdout)
    cleos.wait()

    out = jq.decode("utf-8").strip().strip('"').split("\\n")

    print("\n\n====Output====\n")
    print('\n'.join(out))



if __name__ == "__main__":
    main()
