-- premake5.lua

workspace "semicolon"
    -- todo: add configurations for linux
    -- use cmake for non-windows systems
    configurations { "Debug", "Release" }
    platforms { "x64" }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "semicolon"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    targetdir ("build/bin/" .. outputdir .. "/%{prj.name}")
    objdir("build/bin-int/" .. outputdir .. "/%{prj.name}")

    -- ! remove unnecessary libs
local lib_names = {
    'LLVMCore',
    'LLVMSupport',
    'LLVMRemarks',
    'LLVMBitstreamReader',
    'LLVMBinaryFormat',
    'LLVMTargetParser',
    'LLVMDemangle',
    'LLVMObject',
    'LLVMIRReader',
    'LLVMAnalysis',
    'LLVMOption',
    'LLVMOrcJIT',
    'LLVMCodeGen',
    'LLVMScalarOpts',
    'LLVMRuntimeDyld',
    'LLVMExecutionEngine',
    'LLVMInstCombine',
    'LLVMTransformUtils',
    'LLVMProfileData',
    'LLVMBitReader',
    'LLVMJITLink',
    'LLVMOrcTargetProcess',
    'LLVMOrcShared',
    'LLVMBitWriter',
    'LLVMDebugInfoDWARF',
    'LLVMTextAPI',
    'LLVMMCParser',
    'LLVMAsmParser',
    'LLVMMC',
    'LLVMCodeGenTypes',
    'LLVMDebugInfoCodeView',
    'LLVMX86CodeGen',
    'LLVMX86AsmParser',
    'LLVMX86Desc',
    'LLVMX86Info',
    'LLVMMCDisassembler',
    'LLVMCFGuard',
    'LLVMLinker',
    'LLVMInstrumentation',
    'LLVMGlobalISel',
    'LLVMTarget',
    'LLVMAsmPrinter',
    'LLVMSelectionDAG',
    'LLVMObjCARCOpts',
    -- 'LLVMIRPrinter',
    -- 'LLVMInterfaceStub',
    -- 'LLVMFileCheck',
    -- 'LLVMFuzzMutate',
    -- 'LLVMInstCombine',
    -- 'LLVMAggressiveInstCombine',
    -- 'LLVMSymbolize',
    -- 'LLVMDebugInfoBTF',
    -- 'LLVMDebugInfoPDB',
    -- 'LLVMDebugInfoMSF',
    -- 'LLVMMCJIT',
    -- 'LLVMWebAssemblyDisassembler',
    -- 'LLVMWebAssemblyAsmParser',
    -- 'LLVMWebAssemblyCodeGen',
    -- 'LLVMWebAssemblyUtils',
    -- 'LLVMWebAssemblyDesc',
    -- 'LLVMWebAssemblyInfo',
    -- 'LLVMSystemZDisassembler',
    -- 'LLVMSystemZAsmParser',
    -- 'LLVMSystemZCodeGen',
    -- 'LLVMSystemZDesc',
    -- 'LLVMSystemZInfo',
    -- 'LLVMSparcDisassembler',
    -- 'LLVMSparcAsmParser',
    -- 'LLVMSparcCodeGen',
    -- 'LLVMSparcDesc',
    -- 'LLVMSparcInfo',
    -- 'LLVMRISCVTargetMCA',
    -- 'LLVMRISCVDisassembler',
    -- 'LLVMRISCVAsmParser',
    -- 'LLVMRISCVCodeGen',
    -- 'LLVMRISCVDesc',
    -- 'LLVMRISCVInfo',
    -- 'LLVMPowerPCDisassembler',
    -- 'LLVMPowerPCAsmParser',
    -- 'LLVMPowerPCCodeGen',
    -- 'LLVMPowerPCDesc',
    -- 'LLVMPowerPCInfo',
    -- 'LLVMMSP430Disassembler',
    -- 'LLVMMSP430AsmParser',
    -- 'LLVMMSP430CodeGen',
    -- 'LLVMMSP430Desc',
    -- 'LLVMMSP430Info',
    -- 'LLVMNVPTXCodeGen',
    -- 'LLVMNVPTXDesc',
    -- 'LLVMNVPTXInfo',
    -- 'LLVMAVRDisassembler',
    -- 'LLVMAVRAsmParser',
    -- 'LLVMAVRCodeGen',
    -- 'LLVMAVRDesc',
    -- 'LLVMAVRInfo',
    -- 'LLVMARMDisassembler',
    -- 'LLVMARMAsmParser',
    -- 'LLVMARMCodeGen',
    -- 'LLVMARMDesc',
    -- 'LLVMARMUtils',
    -- 'LLVMARMInfo',
    -- 'LLVMVEDisassembler',
    -- 'LLVMVEAsmParser',
    -- 'LLVMVECodeGen',
    -- 'LLVMVEDesc',
    -- 'LLVMVEInfo',
    -- 'LLVMXCoreDisassembler',
    -- 'LLVMXCoreCodeGen',
    -- 'LLVMXCoreDesc',
    -- 'LLVMXCoreInfo',
    -- 'LLVMXRay',
    -- 'LLVMLibDriver',
    -- 'LLVMDlltoolDriver',
    -- 'LLVMCoverage',
    -- 'LLVMLineEditor',
    -- 'LLVMAMDGPUTargetMCA',
    -- 'LLVMAMDGPUDisassembler',
    -- 'LLVMAMDGPUAsmParser',
    -- 'LLVMAMDGPUCodeGen',
    -- 'LLVMAMDGPUDesc',
    -- 'LLVMAMDGPUUtils',
    -- 'LLVMAMDGPUInfo',
    -- 'LLVMFuzzerCLI',
    -- 'LLVMTableGen',
    -- 'LLVMX86TargetMCA',
    -- 'LLVMX86Disassembler',
    -- 'LLVMWindowsManifest',
    -- 'LLVMMipsDisassembler',
    -- 'LLVMMipsAsmParser',
    -- 'LLVMMipsCodeGen',
    -- 'LLVMMipsDesc',
    -- 'LLVMMipsInfo',
    -- 'LLVMFrontendOpenMP',
    -- 'LLVMFrontendOpenACC',
    -- 'LLVMFrontendHLSL',
    -- 'LLVMLoongArchDisassembler',
    -- 'LLVMLoongArchAsmParser',
    -- 'LLVMLoongArchCodeGen',
    -- 'LLVMLoongArchDesc',
    -- 'LLVMLoongArchInfo',
    -- 'LLVMLanaiDisassembler',
    -- 'LLVMLanaiCodeGen',
    -- 'LLVMLanaiAsmParser',
    -- 'LLVMLanaiDesc',
    -- 'LLVMLanaiInfo',
    -- 'LLVMHexagonDisassembler',
    -- 'LLVMHexagonCodeGen',
    -- 'LLVMHexagonAsmParser',
    -- 'LLVMHexagonDesc',
    -- 'LLVMHexagonInfo',
    -- 'LLVMBPFDisassembler',
    -- 'LLVMBPFAsmParser',
    -- 'LLVMBPFCodeGen',
    -- 'LLVMBPFDesc',
    -- 'LLVMBPFInfo',
    -- 'LLVMAArch64Disassembler',
    -- 'LLVMAArch64AsmParser',
    -- 'LLVMAArch64CodeGen',
    -- 'LLVMAArch64Desc',
    -- 'LLVMAArch64Utils',
    -- 'LLVMAArch64Info',
    -- 'LLVMDWARFLinkerParallel',
    -- 'LLVMDWARFLinker',
    -- 'LLVMMCA',
    -- 'LLVMWindowsDriver',
    -- 'LLVMInterpreter',
    -- 'LLVMDWP',
    -- 'LLVMDebugInfoLogicalView',
    -- 'LLVMDebugInfoGSYM',
    -- 'LLVMObjectYAML',
    -- 'LLVMObjCopy',
    -- 'LLVMExtensions',
    -- 'LLVMLTO',
    -- 'LLVMPasses',
    -- 'LLVMCoroutines',
    -- 'LLVMipo',
    -- 'LLVMVectorize',
    -- 'LLVMMIRParser',
}

    files { "src/**.h", "src/**.cpp", "README.md", "LICENSE", ".clang-tidy", ".clang-format", ".gitignore" }
    filter "configurations:Debug"
        defines { "DEBUG" }
        llvmDir = "./vendor/llvm-17.x"
        includedirs { llvmDir .. "/include", "./src" }
        libdirs { llvmDir .. "/debug/lib" }
        symbols "on"
        runtime "Debug"
        -- make links use local variable lib_names
        -- ! remove unnecessary libs
        links {lib_names}

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "on"
        llvmDir = "./vendor/llvm-17.x"
        includedirs { llvmDir .. "/include", "./src" }
        libdirs { llvmDir .. "/release/lib" }
        runtime "Release"
        -- make links use local variable lib_names
        -- ! remove unnecessary libs
        links {lib_names}


