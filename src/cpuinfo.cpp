// Version: 1.0.0

// Uses the __cpuid intrinsic to get information
// about CPU extended instruction set support.

#include <array>
#include <bitset>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <chrono>
#include <iomanip> // for std::put_time

#if _WIN32 || WIN32
#include <intrin.h> // __cpuid, __cpuidex
#elif defined(__i386__) || defined(__x86_64__)
#include <cpuid.h>
#include <string.h>
#endif

class InstructionSet
{
    // forward declarations
    class InstructionSet_Internal;

public:
    // getters
    static std::string Vendor(void) { return CPU_Rep.vendor_; }
    static std::string Brand(void)  { return CPU_Rep.brand_;  }

    // EDX Register
    // Intel-defined CPU features, CPUID level 0x00000001 (EDX), word 0

    static bool FPU(void)   { return CPU_Rep.f_1_EDX_[0];  } // Floating-point Unit On-Chip
    static bool VME(void)   { return CPU_Rep.f_1_EDX_[1];  } // Virtual Mode Extension
    static bool DE(void)    { return CPU_Rep.f_1_EDX_[2];  } // Debugging Extension
    static bool PSE(void)   { return CPU_Rep.f_1_EDX_[3];  } // Page Size Extension
    static bool TSC(void)   { return CPU_Rep.f_1_EDX_[4];  } // Time Stamp Counter
    static bool MSR(void)   { return CPU_Rep.f_1_EDX_[5];  } // Model Specific Registers: RDMSR, WRMSR.
    static bool PAE(void)   { return CPU_Rep.f_1_EDX_[6];  } // Physical Address Extension
    static bool MCE(void)   { return CPU_Rep.f_1_EDX_[7];  } // Machine-Check Exception
    static bool CX8(void)   { return CPU_Rep.f_1_EDX_[8];  } // CMPXCHG8 instruction
    static bool APIC(void)  { return CPU_Rep.f_1_EDX_[9];  } // On-chip APIC Hardware
                                      // reserved bit 10
    static bool SEP(void)   { return CPU_Rep.f_1_EDX_[11]; } // Fast System Call: SYSENTER, SYSEXIT.
    static bool MTRR(void)  { return CPU_Rep.f_1_EDX_[12]; } // Memory Type Range Registers: MTRR_CAP register.
    static bool PGE(void)   { return CPU_Rep.f_1_EDX_[13]; } // Page Global Enable
    static bool MCA(void)   { return CPU_Rep.f_1_EDX_[14]; } // Machine-Check Architecture: MCG_CAP register.
    static bool CMOV(void)  { return CPU_Rep.f_1_EDX_[15]; } // Conditional Move Instruction: CMOV, FCMOVcc, FCOMI with FPU.
    static bool PAT(void)   { return CPU_Rep.f_1_EDX_[16]; } // Page Attribute Table
    static bool PSE36(void) { return CPU_Rep.f_1_EDX_[17]; } // 36-bit Page Size Extension: Processor supports 4MB pages addressing beyond 4GB physical memory.
    static bool PN(void)    { return CPU_Rep.f_1_EDX_[18]; } // Processor serial number
    static bool CLFSH(void) { return CPU_Rep.f_1_EDX_[19]; } // CLFLUSH instruction
    //static bool NX(void)    { return CPU_Rep.f_1_EDX_[20]; }
    static bool DS(void)    { return CPU_Rep.f_1_EDX_[21]; } // "dts" Debug Store: branch trace store (BTS), precise event-based sampling (PEBS).
    static bool ACPI(void)  { return CPU_Rep.f_1_EDX_[22]; } // Thermal Monitor and Software Controlled Clock Facilities
    static bool MMX(void)   { return CPU_Rep.f_1_EDX_[23]; } // MMX technology
    static bool FXSR(void)  { return CPU_Rep.f_1_EDX_[24]; } // FXSAVE/FXRSTOR, CR4.OSFXSR
    static bool SSE(void)   { return CPU_Rep.f_1_EDX_[25]; } // Streaming SIMD Extensions
    static bool SSE2(void)  { return CPU_Rep.f_1_EDX_[26]; } // Streaming SIMD Extensions 2
    static bool SS(void)    { return CPU_Rep.f_1_EDX_[27]; } // "ss" Self-Snoop CPU cache structure.
    static bool HTT(void)   { return CPU_Rep.f_1_EDX_[28]; } // Hyper-Threading/Multi-Threading
    static bool TM(void)    { return CPU_Rep.f_1_EDX_[29]; } // "tm" Thermal Monitor clock control
    static bool IA64(void)  { return CPU_Rep.f_1_EDX_[30]; } // IA64 processor emulating x86
    static bool PBE(void)   { return CPU_Rep.f_1_EDX_[31]; } // Pending Break Enable

    // EDX Register
    // AMD-defined CPU features, CPUID level 0x80000001, word 1

    static bool SYSCALL(void)    { return CPU_Rep.isAMD_ && CPU_Rep.f_81_EDX_[11]; } // SYSCALL/SYSRET
                                                                   // 12-18
    static bool MP(void)         { return CPU_Rep.isAMD_ && CPU_Rep.f_81_EDX_[19]; } // MP Capable
    static bool NX(void)         { return CPU_Rep.isAMD_ && CPU_Rep.f_81_EDX_[20]; } // Execute Disable
    static bool MMXEXT(void)     { return CPU_Rep.isAMD_ && CPU_Rep.f_81_EDX_[22]; } // AMD MMX extensions
    static bool FXSR_OPT(void)   { return CPU_Rep.isAMD_ && CPU_Rep.f_81_EDX_[25]; } // FXSAVE/FXRSTOR optimizations
    static bool GBPAGES(void)    { return CPU_Rep.isAMD_ && CPU_Rep.f_81_EDX_[26]; } // "pdpe1gb" GB pages
    static bool RDTSCP(void)     { return CPU_Rep.isAMD_ && CPU_Rep.f_81_EDX_[27]; } // RDTSCP: Read Time-Stamp Counter and Processor ID.
                                                      // reserved bit 28
    static bool LM(void)         { return CPU_Rep.isAMD_ && CPU_Rep.f_81_EDX_[29]; } // Long Mode (x86-64, 64-bit support)
    static bool _3DNOWEXT(void)  { return CPU_Rep.isAMD_ && CPU_Rep.f_81_EDX_[30]; } // AMD 3DNow extensions
    static bool _3DNOW(void)     { return CPU_Rep.isAMD_ && CPU_Rep.f_81_EDX_[31]; } // 3DNow

    // ECX register
    // Intel-defined CPU features, CPUID level 0x00000001 (ECX), word 4

    static bool SSE3(void)         { return CPU_Rep.f_1_ECX_[0];  } // Streaming SIMD Extensions 3, XMM3, "pni"
    static bool PCLMULQDQ(void)    { return CPU_Rep.f_1_ECX_[1];  } // PCLMULQDQ instruction
    static bool DTES64(void)       { return CPU_Rep.f_1_ECX_[2];  } // 64-Bit Debug Store
    static bool MONITOR(void)      { return CPU_Rep.f_1_ECX_[3];  } // Processor supports MONITOR and MWAIT instructions
    static bool DS_CPL(void)       { return CPU_Rep.f_1_ECX_[4];  } // "ds_cpl" CPL Qualified (filtered) Debug Store
    static bool VMX(void)          { return CPU_Rep.f_1_ECX_[5];  } // Virtual Machine Extensions. Processor supports Virtualization Technology.
    static bool SMX(void)          { return CPU_Rep.f_1_ECX_[6];  } // Safer Mode Extensions. Processor supports  Trusted Execution Technology.
    static bool EST(void)          { return CPU_Rep.f_1_ECX_[7];  } // Enhanced SpeedStep Technology. Has IA32_PERF_STS and IA32_PERF_CTL registers.
    static bool TM2(void)          { return CPU_Rep.f_1_ECX_[8];  } // Thermal Monitor 2
    static bool SSSE3(void)        { return CPU_Rep.f_1_ECX_[9];  } // Supplemental Streaming SIMD Extensions 3, "sse3", SSE-3.
    static bool CNXT_ID(void)      { return CPU_Rep.f_1_ECX_[10]; } // L1 Context ID - L1 data cache mode is set to adaptive mode or shared in BIOS.
    static bool SDBG(void)         { return CPU_Rep.f_1_ECX_[11]; } // Silicon Debug interface. IA32_DEBUG_INTERFACE
    static bool FMA(void)          { return CPU_Rep.f_1_ECX_[12]; } // Fused Multiply Add. The processor supports FMA extensions using YMM state.
    static bool CMPXCHG16B(void)   { return CPU_Rep.f_1_ECX_[13]; } // CMPXCHG16B instruction
    static bool PCID(void)         { return CPU_Rep.f_1_ECX_[17]; } // Process Context Identifiers
    static bool DCA(void)          { return CPU_Rep.f_1_ECX_[18]; } // Direct Cache Access. Processor supports data prefetch from memory mapped device.
    static bool SSE41(void)        { return CPU_Rep.f_1_ECX_[19]; } // Streaming SIMD Extensions 4.1, "sse4_1", SSE-4.1.
    static bool SSE42(void)        { return CPU_Rep.f_1_ECX_[20]; } // Streaming SIMD Extensions 4.2, "sse4_2", SSE-4.2.
    static bool X2APIC(void)       { return CPU_Rep.f_1_ECX_[21]; } // Extended xAPIC Support
    static bool MOVBE(void)        { return CPU_Rep.f_1_ECX_[22]; } // MOVBE instruction
    static bool POPCNT(void)       { return CPU_Rep.f_1_ECX_[23]; } // POPCNT instruction
    static bool TSC_DEADLINE(void) { return CPU_Rep.f_1_ECX_[24]; } // Time Stamp Counter Deadline
    static bool AES(void)          { return CPU_Rep.f_1_ECX_[25]; } // AES Instruction Extensions
    static bool XSAVE(void)        { return CPU_Rep.f_1_ECX_[26]; } // XSAVE/XSTOR States
    static bool OSXSAVE(void)      { return CPU_Rep.f_1_ECX_[27]; } // OS-Enabled Extended State Management. XSETBV/XGETBV, XFEATURE_ENABLED_MASK (XCR0), XSAVE/XRSTOR.
    static bool AVX(void)          { return CPU_Rep.f_1_ECX_[28]; } // Advanced Vector Extensions
    static bool F16C(void)         { return CPU_Rep.f_1_ECX_[29]; } // 16-bit floating-point conversion instructions
    static bool RDRAND(void)       { return CPU_Rep.f_1_ECX_[30]; } // RDRAND instruction
    static bool HYPERVISOR(void)   { return CPU_Rep.f_1_ECX_[31]; } // Hypervisor present (always zero on physical CPUs).

    // ECX register
    // More extended AMD flags: CPUID level 0x80000001, ECX, word 6

    static bool LAHF(void)           { return CPU_Rep.isAMD_   && CPU_Rep.f_81_ECX_[0];  } // LAHF/SAHF in long mode
    static bool CMP_LEGACY(void)     { return CPU_Rep.isAMD_   && CPU_Rep.f_81_ECX_[1];  } // If yes HyperThreading not valid
    static bool SVM(void)            { return CPU_Rep.isAMD_   && CPU_Rep.f_81_ECX_[2];  } // Secure Virtual Machine
    static bool EXTAPIC(void)        { return CPU_Rep.isAMD_   && CPU_Rep.f_81_ECX_[3];  } // Extended APIC space
    static bool CR8_LEGACY(void)     { return CPU_Rep.isAMD_   && CPU_Rep.f_81_ECX_[4];  } // CR8 in 32-bit mode
    static bool ABM(void)            { return CPU_Rep.isAMD_   && CPU_Rep.f_81_ECX_[5];  } // Advanced bit manipulation
    static bool SSE4a(void)          { return CPU_Rep.isAMD_   && CPU_Rep.f_81_ECX_[6];  } // SSE-4A
    static bool MISALIGNSSE(void)    { return CPU_Rep.isAMD_   && CPU_Rep.f_81_ECX_[7];  } // AMD SSE mis-alignment sub-mode
    static bool _3DNOWPREFETCH(void) { return CPU_Rep.isAMD_   && CPU_Rep.f_81_ECX_[8];  } // 3DNow prefetch instructions
    static bool OSVW(void)           { return CPU_Rep.isAMD_   && CPU_Rep.f_81_ECX_[9];  } // OS Visible Workaround
    static bool IBS(void)            { return CPU_Rep.isAMD_   && CPU_Rep.f_81_ECX_[10]; } // Instruction Based Sampling
    static bool XOP(void)            { return CPU_Rep.isAMD_   && CPU_Rep.f_81_ECX_[11]; } // extended AVX instructions
    static bool SKINIT(void)         { return CPU_Rep.isAMD_   && CPU_Rep.f_81_ECX_[12]; } // SKINIT/STGI instructions
    static bool WDT(void)            { return CPU_Rep.isAMD_   && CPU_Rep.f_81_ECX_[13]; } // Watchdog timer
                                                                    // reserved bit 14
    static bool LWP(void)            { return CPU_Rep.isAMD_   && CPU_Rep.f_81_ECX_[15]; } // Light Weight Profiling
    static bool FMA4(void)           { return CPU_Rep.isAMD_   && CPU_Rep.f_81_ECX_[16]; } // 4 operands MAC instructions
    static bool TCE(void)            { return CPU_Rep.isAMD_   && CPU_Rep.f_81_ECX_[17]; } // Translation Cache Extension
                                                                    // reserved bit 18
    static bool NODEID_MSR(void)     { return CPU_Rep.isAMD_   && CPU_Rep.f_81_ECX_[19]; } // NodeId MSR
                                                                    // reserved bit 20
    static bool TBM(void)            { return CPU_Rep.isAMD_   && CPU_Rep.f_81_ECX_[21]; } // AMD. Trailing Bit Manipulation, complements BMI1.
    static bool TOPOEXT(void)        { return CPU_Rep.isAMD_   && CPU_Rep.f_81_ECX_[22]; } // Topology extensions CPUID leafs
    static bool PERFCTR_CORE(void)   { return CPU_Rep.isAMD_   && CPU_Rep.f_81_ECX_[23]; } // Core performance counter extensions
    static bool PERFCTR_NB(void)     { return CPU_Rep.isAMD_   && CPU_Rep.f_81_ECX_[24]; } // NB performance counter extensions
    static bool BPEXT(void)          { return CPU_Rep.isAMD_   && CPU_Rep.f_81_ECX_[26]; } // Data breakpoint extension
    static bool PTSC(void)           { return CPU_Rep.isAMD_   && CPU_Rep.f_81_ECX_[27]; } // Performance time-stamp counter
    static bool PERFCTR_LLC(void)    { return CPU_Rep.isAMD_   && CPU_Rep.f_81_ECX_[28]; } // Last Level Cache performance counter extensions
    static bool MWAITX(void)         { return CPU_Rep.isAMD_   && CPU_Rep.f_81_ECX_[29]; } // MWAIT extension (MONITORX/MWAITX instructions)

    // Intel-defined CPU features, CPUID level 0x00000007:0 (EBX), word 9

    static bool FSGSBASE(void)        { return                     CPU_Rep.f_7_EBX_[0];  } // RDFSBASE, WRFSBASE, RDGSBASE, WRGSBASE instructions
    static bool TSC_ADJUST(void)      { return                     CPU_Rep.f_7_EBX_[1];  } // TSC adjustment MSR 0x3B, IA32_TSC_ADJUST
    static bool SGX(void)             { return CPU_Rep.isIntel_ && CPU_Rep.f_7_EBX_[2];  } // Supports Intel® Software Guard Extensions (Intel® SGX Extensions)
    static bool BMI1(void)            { return                     CPU_Rep.f_7_EBX_[3];  } // 1st group bit manipulation extensions
    static bool HLE(void)             { return CPU_Rep.isIntel_ && CPU_Rep.f_7_EBX_[4];  } // Hardware Lock Elision (HLE, non-TSX)
    static bool AVX2(void)            { return                     CPU_Rep.f_7_EBX_[5];  } // AVX2 instructions
    static bool FDP_EXCPTN_ONLY(void) { return                     CPU_Rep.f_7_EBX_[6];  } // x87 FPU Data Pointer updated only on x87 exceptions if 1.
    static bool SMEP(void)            { return                     CPU_Rep.f_7_EBX_[7];  } // Supervisor Mode Execution Protection
    static bool BMI2(void)            { return                     CPU_Rep.f_7_EBX_[8];  } // 2nd group bit manipulation extensions
    static bool ERMS(void)            { return                     CPU_Rep.f_7_EBX_[9];  } // Enhanced REP MOVSB/STOSB instructions
    static bool INVPCID(void)         { return                     CPU_Rep.f_7_EBX_[10]; } // Invalidate Processor Context ID
    static bool RTM(void)             { return CPU_Rep.isIntel_ && CPU_Rep.f_7_EBX_[11]; } // Restricted Transactional Memory (RTM)
    static bool CQM(void)             { return                     CPU_Rep.f_7_EBX_[12]; } // Cache QoS Monitoring, PQM?
    static bool FPU_CSDS(void)        { return                     CPU_Rep.f_7_EBX_[13]; } // Zero out FPU CS and FPU DS
    static bool MPX(void)             { return                     CPU_Rep.f_7_EBX_[14]; } // Memory Protection Extension
    static bool RDT_A(void)           { return                     CPU_Rep.f_7_EBX_[15]; } // Resource Director Technology Allocation, PQE?
    static bool AVX512F(void)         { return                     CPU_Rep.f_7_EBX_[16]; } // AVX-512 Foundation
    static bool AVX512DQ(void)        { return                     CPU_Rep.f_7_EBX_[17]; } // AVX-512 DQ (Double/Quad granular) Instructions
    static bool RDSEED(void)          { return                     CPU_Rep.f_7_EBX_[18]; } // RDSEED instruction
    static bool ADX(void)             { return                     CPU_Rep.f_7_EBX_[19]; } // ADCX and ADOX instructions
    static bool SMAP(void)            { return                     CPU_Rep.f_7_EBX_[20]; } // Supervisor Mode Access Prevention
    static bool AVX512IFMA(void)      { return                     CPU_Rep.f_7_EBX_[21]; } // AVX-512 Integer Fused Multiply-Add instructions
    static bool PCOMMIT(void)         { return                     CPU_Rep.f_7_EBX_[22]; } // PCOMMIT instruction
    static bool CLFLUSHOPT(void)      { return                     CPU_Rep.f_7_EBX_[23]; } // CLFLUSHOPT instruction
    static bool CLWB(void)            { return                     CPU_Rep.f_7_EBX_[24]; } // CLWB instruction
    static bool IPT(void)             { return CPU_Rep.isIntel_ && CPU_Rep.f_7_EBX_[25]; } // Intel Processor Trace
    static bool AVX512PF(void)        { return                     CPU_Rep.f_7_EBX_[26]; } // AVX-512 Prefetch
    static bool AVX512ER(void)        { return                     CPU_Rep.f_7_EBX_[27]; } // AVX-512 Exponential and Reciprocal
    static bool AVX512CD(void)        { return                     CPU_Rep.f_7_EBX_[28]; } // AVX-512 Conflict Detection
    static bool SHA(void)             { return                     CPU_Rep.f_7_EBX_[29]; } // SHA1/SHA256 Instruction Extensions
    static bool AVX512BW(void)        { return                     CPU_Rep.f_7_EBX_[30]; } // AVX-512 BW (Byte/Word granular) Instructions
    static bool AVX512VL(void)        { return                     CPU_Rep.f_7_EBX_[31]; } // AVX-512 VL (128/256 Vector Length) Extensions

    // EAX=7 ECX=0 -> ECX
    // Intel-defined CPU features, CPUID level 0x00000007:0 (ECX), word 16

    static bool PREFETCHWT1(void)        { return CPU_Rep.f_7_ECX_[0];  } // PREFETCHWT instruction
    static bool AVX512_VBMI(void)        { return CPU_Rep.f_7_ECX_[1];  } // AVX512 Vector Bit Manipulation instructions
    static bool UMIP(void)               { return CPU_Rep.f_7_ECX_[2];  } // User Mode Instruction Protection
    static bool PKU(void)                { return CPU_Rep.f_7_ECX_[3];  } // Protection Keys for Userspace
    static bool OSPKE(void)              { return CPU_Rep.f_7_ECX_[4];  } // OS Protection Keys Enable
    static bool WAITPKG(void)            { return CPU_Rep.f_7_ECX_[5];  } // UMONITOR/UMWAIT/TPAUSE Instructions
    static bool AVX512_VBMI2(void)       { return CPU_Rep.f_7_ECX_[6];  } // Additional AVX512 Vector Bit Manipulation Instructions
    static bool CET_SS(void)             { return CPU_Rep.f_7_ECX_[7];  } // CET shadow stack features
    static bool GFNI(void)               { return CPU_Rep.f_7_ECX_[8];  } // Galois Field New Instructions
    static bool VAES(void)               { return CPU_Rep.f_7_ECX_[9];  } // Vector AES
    static bool VPCLMULQDQ(void)         { return CPU_Rep.f_7_ECX_[10]; } // Carry-Less Multiplication Double Quadword
    static bool AVX512_VNNI(void)        { return CPU_Rep.f_7_ECX_[11]; } // AVX512 Vector Neural Network Instructions
    static bool AVX512_BITALG(void)      { return CPU_Rep.f_7_ECX_[12]; } // AVX512 Support for VPOPCNT[B,W] and VPSHUF-BITQMB instructions
    static bool TME(void)                { return CPU_Rep.f_7_ECX_[12]; } // Intel Total Memory Encryption
    static bool AVX512_VPOPCNTDQ(void)   { return CPU_Rep.f_7_ECX_[14]; } // POPCNT for vectors of DW/QW
                                                   // reserved bit 15
    static bool FIVE_LEVEL_PAGING(void)  { return CPU_Rep.f_7_ECX_[16]; } // 57-bit linear addresses and five-level paging
                  // 17-21 MAWAU: value of MAWAU used by the BNDLDX and BNDSTX instructions in 64-bit mode
    static bool RDPID(void)              { return CPU_Rep.f_7_ECX_[22]; } // RDPID instruction
    static bool KL(void)                 { return CPU_Rep.f_7_ECX_[23]; } // Key Locker
    static bool BUS_LOCK_DETECT(void)    { return CPU_Rep.f_7_ECX_[24]; } // OS bus-lock detection
    static bool CLDEMOTE(void)           { return CPU_Rep.f_7_ECX_[25]; } // CLDEMOTE instruction
                                                   // reserved bit 26
    static bool MOVDIRI(void)            { return CPU_Rep.f_7_ECX_[27]; } // MOVDIRI instruction
    static bool MOVDIR64B(void)          { return CPU_Rep.f_7_ECX_[28]; } // MOVDIR64B instruction
    static bool ENQCMD(void)             { return CPU_Rep.f_7_ECX_[29]; } // ENQCMD and ENQCMDS instructions
    static bool SGX_LC(void)             { return CPU_Rep.f_7_ECX_[30]; } // Software Guard Extensions Launch Control
    static bool PKS(void)                { return CPU_Rep.f_7_ECX_[31]; } // protection keys for supervisor-mode pages

private:
    static const InstructionSet_Internal CPU_Rep;

    class InstructionSet_Internal
    {
    public:
        InstructionSet_Internal()
            : nIds_{ 0 },
            nExIds_{ 0 },
            isIntel_{ false },
            isAMD_{ false },
            f_1_ECX_{ 0 },
            f_1_EDX_{ 0 },
            f_7_EBX_{ 0 },
            f_7_ECX_{ 0 },
            f_81_ECX_{ 0 },
            f_81_EDX_{ 0 },
            data_{},
            extdata_{}
        {
#ifdef _WIN32
            std::array<int, 4> cpui;
#else
            std::array<unsigned int, 4> cpui;
#endif

            // Calling __cpuid with 0x0 as the function_id argument
            // gets the number of the highest valid function ID.
#ifdef _WIN32
            __cpuid(cpui.data(), 0);
#else
            cpui[0] = __get_cpuid_max(0, &cpui[1]);
#endif
            nIds_ = cpui[0];

            for (int i = 0; i <= nIds_; ++i)
            {
#ifdef _WIN32
                __cpuidex(cpui.data(), i, 0);
#else
                __cpuid_count(i, 0, cpui[0], cpui[1], cpui[2], cpui[3]);
#endif

                data_.push_back(cpui);
            }

            // Capture vendor string
            char vendor[0x20];
            memset(vendor, 0, sizeof(vendor));
            *reinterpret_cast<int*>(vendor + 0) = data_[0][1];
            *reinterpret_cast<int*>(vendor + 4) = data_[0][3];
            *reinterpret_cast<int*>(vendor + 8) = data_[0][2];
            vendor_ = vendor;
            if (vendor_ == "GenuineIntel")
            {
                isIntel_ = true;
            }
            else if (vendor_ == "AuthenticAMD")
            {
                isAMD_ = true;
            }

            // load bitset with flags for function 0x00000001
            if (nIds_ >= 1)
            {
                f_1_ECX_ = data_[1][2];
                f_1_EDX_ = data_[1][3];
            }

            // load bitset with flags for function 0x00000007
            if (nIds_ >= 7)
            {
                f_7_EBX_ = data_[7][1];
                f_7_ECX_ = data_[7][2];
            }

            // Calling __cpuid with 0x80000000 as the function_id argument
            // gets the number of the highest valid extended ID.
#ifdef _WIN32
            __cpuid(cpui.data(), 0x80000000);
#else
            __cpuid(0x80000000, cpui[0], cpui[1], cpui[2], cpui[3]);
#endif
            nExIds_ = cpui[0];

            char brand[0x40];
            memset(brand, 0, sizeof(brand));

            for (int i = 0x80000000; i <= nExIds_; ++i)
            {
#ifdef _WIN32
                __cpuidex(cpui.data(), i, 0);
#else
                __cpuid_count(i, 0, cpui[0], cpui[1], cpui[2], cpui[3]);
#endif
                extdata_.push_back(cpui);
            }

            // load bitset with flags for function 0x80000001
            if (nExIds_ >= 0x80000001)
            {
                f_81_ECX_ = extdata_[1][2];
                f_81_EDX_ = extdata_[1][3];
            }

            // Interpret CPU brand string if reported
            if (nExIds_ >= 0x80000004)
            {
                memcpy(brand +  0, extdata_[2].data(), sizeof(cpui));
                memcpy(brand + 16, extdata_[3].data(), sizeof(cpui));
                memcpy(brand + 32, extdata_[4].data(), sizeof(cpui));
                brand_ = brand;
            }
        };

        int nIds_;
        int nExIds_;
        std::string vendor_;
        std::string brand_;
        bool isIntel_;
        bool isAMD_;
        std::bitset<32> f_1_ECX_;
        std::bitset<32> f_1_EDX_;
        std::bitset<32> f_7_EBX_;
        std::bitset<32> f_7_ECX_;
        std::bitset<32> f_81_ECX_;
        std::bitset<32> f_81_EDX_;
#ifdef _WIN32
        std::vector<std::array<int, 4>> data_;
        std::vector<std::array<int, 4>> extdata_;
#else
        std::vector<std::array<unsigned int, 4>> data_;
        std::vector<std::array<unsigned int, 4>> extdata_;
#endif
    };
};

// Initialize static member data
const InstructionSet::InstructionSet_Internal InstructionSet::CPU_Rep;

inline std::string trim(std::string& str)
{
    str.erase(str.find_last_not_of(' ')+1);   // right-trim
    str.erase(0, str.find_first_not_of(' ')); // left-trim
    return str;
}
inline std::string rm_last_char(std::string& str, const std::string& c)
{
    return str.substr(0, str.find_last_of(c));
}
inline std::string getYear()
{
    static const std::time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), "%Y");
    return ss.str();
}

int main()
{
    std::ostringstream outstream;

    // print the json key value pair
    auto print_pair = [&outstream](std::string key, auto val) {
        outstream << "    \"" << key << "\": " << std::boolalpha << val << ",\n";
    };

    print_pair("3DNOW",            InstructionSet::_3DNOW());
    print_pair("3DNOWEXT",         InstructionSet::_3DNOWEXT());
    print_pair("ABM",              InstructionSet::ABM());
    print_pair("ADX",              InstructionSet::ADX());
    print_pair("AES",              InstructionSet::AES());
    print_pair("AVX",              InstructionSet::AVX());
    print_pair("AVX2",             InstructionSet::AVX2());
    print_pair("AVX512CD",         InstructionSet::AVX512CD());
    print_pair("AVX512F",          InstructionSet::AVX512F());
    print_pair("AVX512ER",         InstructionSet::AVX512ER());
    print_pair("AVX512PF",         InstructionSet::AVX512PF());
    print_pair("AVX512BW",         InstructionSet::AVX512BW());
    print_pair("AVX512VL",         InstructionSet::AVX512VL());
    print_pair("AVX512_VBMI",      InstructionSet::AVX512_VBMI());
    print_pair("AVX512_VBMI2",     InstructionSet::AVX512_VBMI2());
    print_pair("AVX512_VNNI",      InstructionSet::AVX512_VNNI());
    print_pair("AVX512_BITALG",    InstructionSet::AVX512_BITALG());
    print_pair("AVX512_VPOPCNTDQ", InstructionSet::AVX512_VPOPCNTDQ());
    print_pair("BMI1",             InstructionSet::BMI1());
    print_pair("BMI2",             InstructionSet::BMI2());
    print_pair("CLFSH",            InstructionSet::CLFSH());
    print_pair("CLFLUSHOPT",       InstructionSet::CLFLUSHOPT());
    print_pair("CMPXCHG16B",       InstructionSet::CMPXCHG16B());
    print_pair("CX8",              InstructionSet::CX8());
    print_pair("ERMS",             InstructionSet::ERMS());
    print_pair("F16C",             InstructionSet::F16C());
    print_pair("FMA",              InstructionSet::FMA());
    print_pair("FSGSBASE",         InstructionSet::FSGSBASE());
    print_pair("FXSR",             InstructionSet::FXSR());
    print_pair("HLE",              InstructionSet::HLE());
    print_pair("INVPCID",          InstructionSet::INVPCID());
    print_pair("IPT",              InstructionSet::IPT());
    print_pair("LAHF",             InstructionSet::LAHF());
    print_pair("MMX",              InstructionSet::MMX());
    print_pair("MMXEXT",           InstructionSet::MMXEXT());
    print_pair("MONITOR",          InstructionSet::MONITOR());
    print_pair("MOVBE",            InstructionSet::MOVBE());
    print_pair("MSR",              InstructionSet::MSR());
    print_pair("OSXSAVE",          InstructionSet::OSXSAVE());
    print_pair("PCLMULQDQ",        InstructionSet::PCLMULQDQ());
    print_pair("POPCNT",           InstructionSet::POPCNT());
    print_pair("PREFETCHWT1",      InstructionSet::PREFETCHWT1());
    print_pair("RDRAND",           InstructionSet::RDRAND());
    print_pair("RDSEED",           InstructionSet::RDSEED());
    print_pair("RDTSCP",           InstructionSet::RDTSCP());
    print_pair("RTM",              InstructionSet::RTM());
    print_pair("SEP",              InstructionSet::SEP());
    print_pair("SHA",              InstructionSet::SHA());
    print_pair("SMAP",             InstructionSet::SMAP());
    print_pair("SSE",              InstructionSet::SSE());
    print_pair("SSE2",             InstructionSet::SSE2());
    print_pair("SSE3",             InstructionSet::SSE3());
    print_pair("SSE4.1",           InstructionSet::SSE41());
    print_pair("SSE4.2",           InstructionSet::SSE42());
    print_pair("SSE4a",            InstructionSet::SSE4a());
    print_pair("SSSE3",            InstructionSet::SSSE3());
    print_pair("SYSCALL",          InstructionSet::SYSCALL());
    print_pair("TBM",              InstructionSet::TBM());
    print_pair("XOP",              InstructionSet::XOP());
    print_pair("XSAVE",            InstructionSet::XSAVE());

    // remove trailing comma from last item in isa-features
    std::string isa_feature = outstream.str();
    isa_feature = rm_last_char(isa_feature, ",");

    // determine architecture level
    std::string architecture;
    if(InstructionSet::AVX512F()) { architecture = "x86-64-v4"; } else
    if(InstructionSet::AVX2())    { architecture = "x86-64-v3"; } else
    if(InstructionSet::SSE42())   { architecture = "x86-64-v2"; } else
    if(InstructionSet::SSE2())    { architecture = "x86-64-v1"; } else
                                  { architecture = "x86-64"; }

    std::string vendor = InstructionSet::Vendor();
    std::string brand = InstructionSet::Brand();
    brand = trim(brand);

    std::string NL = "\n";  // double-escaped new line

    // build json document
    // This uses double escape qouting insanity.
    // We embed json into cpp by escaping it, then we embed escaped cpp into cmake.
    std::string json_str =
        "{"                                                            +NL+
        " \"cpu\": {"                                                  +NL+
        "    \"vendor\": \"" + vendor + "\","                          +NL+
        "    \"brand\": \"" + brand + "\""                             +NL+
        " },"                                                          +NL+
        " \"isa-features\": {" + NL + isa_feature + NL + "  },"        +NL+
        " \"architecture\": \"" + architecture + "\""                  +NL+
        "}";

    bool console_output = true;

    if(console_output)
    {
       // print binary info
       const std::string binary  = "cpuinfo";
       const std::string version = "1.0.0";
       const std::string website = "https://github.com/jakoch/cpuinfo_cmake";
       std::printf("%s v%s, %s\n", binary.c_str(), version.c_str(), website.c_str());
       std::printf("Copyright (c) 2023-%s Jens A. Koch.\n", getYear().c_str());

       // print json to console
       std::printf("\ncpuinfo.json:\n\n");
       std::cout << json_str;
    }

    // write file
    std::ofstream file("cpuinfo.json", std::ios::trunc);

    if(file.is_open())
    {
        file << json_str;
        file.flush();
        file.close();
    } else {
        std::cerr << "[CPUINFO] Could not write cpuinfo.json file" << std::endl;
    }

    return 0;
}
