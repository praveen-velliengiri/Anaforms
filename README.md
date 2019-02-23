# Anaforms
Simple transformation &amp; analysis passes for manipulating llvm IR

Contains simple passes. 

To build, just run cmake and make.

To old passes just follow the llvm docs, but to run new pass (FunctionPass.cpp) use : opt -load-pass-plugin=/path/to/librun.so -passes="hello-new-pm-pass" <sampleIr.ll> >/dev/null
