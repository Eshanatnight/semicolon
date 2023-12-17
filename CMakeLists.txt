cmake_minimum_required(VERSION 3.16)

project(Semicolon)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(BUILD_ARCH "-m64")
set(TARGET_64 ON)

# rn it is using the global llvm installation. make it use the local one? or do i integrate vcpkg?
# import llvm
find_package(LLVM REQUIRED CONFIG)
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

file(GLOB_RECURSE sources
    "src/*.cpp"
    "src/*.h"
)

set(SEMICOLON_INCLUDE_DIRS
    "${PROJECT_SOURCE_DIR}/src/"
)
include_directories(${SEMICOLON_INCLUDE_DIRS})

add_executable(Semicolon ${sources})

# remove unnecessary libs
llvm_map_components_to_libnames(llvm_libs
    Core
    Support
    Remarks
    BitstreamReader
    BinaryFormat
    TargetParser
    Demangle
    Object
    McJIT
    IRReader
    Analysis
    Option
    OrcJIT
    CodeGen
    ScalarOpts
    RuntimeDyld
    ExecutionEngine
    InstCombine
    TransformUtils
    ProfileData
    BitReader
    JITLink
    OrcTargetProcess
    OrcShared
    BitWriter
    DebugInfoDWARF
    TextAPI
    MCParser
    AsmParser
    MC
    CodeGenTypes
    IRPrinter
    InterfaceStub
    FileCheck
    FuzzMutate
    InstCombine
    AggressiveInstCombine
    DebugInfoCodeView
    Symbolize
    DebugInfoBTF
    DebugInfoPDB
    DebugInfoMSF
    WebAssemblyDisassembler
    WebAssemblyAsmParser
    WebAssemblyCodeGen
    WebAssemblyUtils
    WebAssemblyDesc
    WebAssemblyInfo
    SystemZDisassembler
    SystemZAsmParser
    SystemZCodeGen
    SystemZDesc
    SystemZInfo
    SparcDisassembler
    SparcAsmParser
    SparcCodeGen
    SparcDesc
    SparcInfo
    RISCVTargetMCA
    RISCVDisassembler
    RISCVAsmParser
    RISCVCodeGen
    RISCVDesc
    RISCVInfo
    PowerPCDisassembler
    PowerPCAsmParser
    PowerPCCodeGen
    PowerPCDesc
    PowerPCInfo
    MSP430Disassembler
    MSP430AsmParser
    MSP430CodeGen
    MSP430Desc
    MSP430Info
    NVPTXCodeGen
    NVPTXDesc
    NVPTXInfo
    AVRDisassembler
    AVRAsmParser
    AVRCodeGen
    AVRDesc
    AVRInfo
    ARMDisassembler
    ARMAsmParser
    ARMCodeGen
    ARMDesc
    ARMUtils
    ARMInfo
    VEDisassembler
    VEAsmParser
    VECodeGen
    VEDesc
    VEInfo
    XCoreDisassembler
    XCoreCodeGen
    XCoreDesc
    XCoreInfo
    XRay
    LibDriver
    DlltoolDriver
    Coverage
    LineEditor
    AMDGPUTargetMCA
    AMDGPUDisassembler
    AMDGPUAsmParser
    AMDGPUCodeGen
    AMDGPUDesc
    AMDGPUUtils
    AMDGPUInfo
    X86TargetMCA
    X86CodeGen
    X86AsmParser
    X86Desc
    X86Disassembler
    X86Info
    FuzzerCLI
    WindowsManifest
    TableGen
    MipsDisassembler
    MipsAsmParser
    MipsCodeGen
    MipsDesc
    MipsInfo
    FrontendOpenMP
    FrontendOpenACC
    FrontendHLSL
    LoongArchDisassembler
    LoongArchAsmParser
    LoongArchCodeGen
    LoongArchDesc
    LoongArchInfo
    LanaiDisassembler
    LanaiCodeGen
    LanaiAsmParser
    LanaiDesc
    LanaiInfo
    HexagonDisassembler
    HexagonCodeGen
    HexagonAsmParser
    HexagonDesc
    HexagonInfo
    BPFDisassembler
    BPFAsmParser
    BPFCodeGen
    BPFDesc
    BPFInfo
    AArch64Disassembler
    AArch64AsmParser
    AArch64CodeGen
    AArch64Desc
    AArch64Utils
    AArch64Info
    DWARFLinkerParallel
    DWARFLinker
    MCA
    MCDisassembler
    WindowsDriver
    Interpreter
    DWP
    DebugInfoLogicalView
    DebugInfoGSYM
    ObjectYAML
    ObjCopy
    Extensions
    LTO
    Passes
    CFGuard
    Coroutines
    ipo
    Vectorize
    Linker
    Instrumentation
    GlobalISel
    MIRParser
    AsmPrinter
    SelectionDAG
    Target
    ScalarOpts
    ObjCARCOpts
)

target_link_libraries(Semicolon PUBLIC ${llvm_libs})