#!/usr/bin/env python3

import os
import subprocess
import sys

def main():
    run(sys.argv[1], sys.argv[2])

def run(contract_name, action_name):
    args = (
        "cleos", "push", "action", contract_name, action_name, "[]", "-p", contract_name, "-j"
    )
    cleos = subprocess.Popen(args, stdout=subprocess.PIPE)
    jq = subprocess.check_output(("jq", ".processed.action_traces[0].console"), stdin=cleos.stdout)
    cleos.wait()

    out = jq.decode("utf-8").strip().strip('"').split("\\n")

    print("\n\n====Output====\n")
    print('\n'.join(out))



if __name__ == "__main__":
    main()
