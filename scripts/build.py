#!/usr/bin/python3
import os
import subprocess


print("Building BarinkOS")


# list and run build scripts
print("Running build-scripts")
os.chdir("scripts")

scripts=os.listdir()
currentScript=os.path.basename(__file__)

if currentScript in scripts:
    scripts.remove(currentScript)


for script in scripts:
    print(os.getcwd())
    print("Running:" + script)
    subprocess.call(script, cwd=os.getcwd())

os.chdir("..")