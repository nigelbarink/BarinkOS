#!/usr/bin/python3
import os
import subprocess

print("Give BarinkOS A Test Run")

# list and run build scripts
print("Running build-scripts")
os.chdir("scripts")
scripts=os.listdir()
currentScript=os.path.basename(__file__)

os.chdir("../CoreLib")
print("Building CoreLib")
subprocess.call("make")

os.chdir("../kernel")
print("Building kernel")
subprocess.call("make")

os.chdir("..")
subprocess.call("scripts/update_harddrive.sh", cwd=os.getcwd())
subprocess.call("scripts/create_symbol_lookup.sh", cwd=os.getcwd())
subprocess.call("scripts/run_qemu.sh", cwd=os.getcwd())