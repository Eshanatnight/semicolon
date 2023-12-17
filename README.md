# Semicolon

# Infolog

Writing My Own Scripting Language with #LLVM

## Day 1

1. parser v1 ✅
2. lexer v1 ✅
3. AST nodes v1✅
4. interpreter main loop 🚧
5. #LLVM IR 🚧

Takeaways:

1. I had forgotten how annoying the #MSVC Solutions are when they are built with #cmake* 😑
2. I had completely forgot how to setup #premake5** 😣
3. C++ Errors are so bad 😭

* Yes I know I can just use the #MSYS2 or #Mingw toolchains. But I am set in my ways.

** #premake5 is an absolute god-send for Microsoft #MSVC. If anyone in my connections works with the #MSVC toolchain and has not used #premake5 or is just using #cmake, just check #premake5 out, ⭐ the repo and if possible sponsor them.
<https://lnkd.in/gAMHGK9w>

Looking forward to breaking something in Day 2 😎

## Day 2 (more like Day 1.5)

Progress:

1. interpreter main loop ✅
2. #LLVM IR v1 ✅
3. JIT Optimization 🚧

Hindsight:

1. I don't know why I was screwing up the Main Loop and the IR for functions. That to naive mistakes. 👽
2. I was always apprehensive about always using `smart pointers`, but a lesson that I realize I should have learnt ages ago is a **Fractional** Performance difference is OK 👌 if it prevents you from Mem Leaks 🤡
3. C++ Errors are so hard to read 😭

Come Back tomorrow for the next episode of the show "What is Kells Breaking Now!" See you guys tomorrow.

P.S. Not all the 597 Warnings are from my code Most of them are from the llvm branch I'm using 😌

## Day 3

Yeah I am a day late. I know. I am sorry to the 3 people who are following my LLVM journey. love you guys 💌. I couldn't get out of bed and do anything, curses depression 🥹.

But now I am back taking little steps until I can sprint again.

So what did I do for Day 3. I spent the free time I had fixing #LK2019 Linker Errors.

Make this make sense to me.
When I link a library manually through Microsoft Visual Studio, it's fine it links perfectly, the interpreter opens up, everything is Good.
Now, When I link the same #LLVM libraries through #Cmake I get 16 Linker Errors?
How does this make sense!!!! Send Help 🥹

I started this "build something cool" in October as a part of my #Hacktober. For the past few months everyone just kept on telling me how my story is supposed to be. And this is my way of going rouge, doing my own thing.

See you guys in Day 4!

# HactoberFest #Clang #C++ #LLVM #llvm #gcc #MSVC #GCC #GNU #VisualStudio #CMake

## Day 4

After quite literally getting my butt kicked from mental blocks and "the big sad" I come to you with the progress on Semicolon. After flighting to link everything properly, I was left with exactly 2 unresolved external symbols, and what feels like a lifetime, I finally figured it out I had defined an extern variable in the header file but never instantiated it. Still making Day 1 mistakes.

So Progress Report:

1. JIT ✅
2. Constant Folding ✅
3. LLVM Optimization Passes ✅

Quality of Life Todos:

1. Integrate clang-format
2. Integrate clang-tidy
3. update CMakeLists.txt

I'll start on branching next!

# CMake #C++ #LLVM #CLANG #Compiler

## Day 5

Updates: I came back to Kolkata to spend some time at home and just calming my mind from all the stuff I
have to deal with. Apart from doing some minor refactoring, I found that I could not build the project, with cmake.

## Notes

```lua
    llvmDir = "C:/Program Files (x86)/LLVM"
    includedirs { llvmDir .. "/include", "./src" }
    libdirs { llvmDir .. "/lib" }
    links {
        "LLVMCore",
        "LLVMSupport",
        "LLVMRuntimeDyld",
        "LLVMRemarks",
        "LLVMBitstreamReader",
        "LLVMBinaryFormat",
        "LLVMTargetParser",
        "LLVMDemangle",
        "LLVMObject",
        "LLVMMCJIT",
        "LLVMOption",
        "LLVMOrcJIT",
        "LLVMCodeGen",
        "LLVMAnalysis",
        "LLVMScalarOpts",
        "LLVMIRReader",
        "LLVMExecutionEngine",
        "LLVMInstCombine",
        "LLVMTransformUtils",
        "LLVMProfileData",
        "LLVMBitReader",
        "LLVMJITLink",
        "LLVMOrcTargetProcess",
        "LLVMOrcShared",
        "LLVMBitWriter",
        "LLVMDebugInfoDWARF",
        "LLVMTextAPI",
        "LLVMMCParser",
        "LLVMAsmParser",
        "LLVMMC",
    }
```
