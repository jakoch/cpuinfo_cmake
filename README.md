# CPUINFO for CMake

cpuinfo is a C++ based CLI tool and CMake script for CPU ISA feature flag checking.

The `cpuinfo` CLI tool lists CPU features using the cpuid instruction in JSON format.

The `CPUINFO.cmake` is a CMake script, which list the CPU features in JSON format and
helps you to set feature flag variables and define compiler flags within your CMake build script.

Both tools enable you to confirm that a processor feature is present prior to using the feature.

## What is this?

This project acts as a base for the maintenance of a `CPUINFO.cmake` script,
which can be embeded into your CMake workflow. It will build and run a CPUID utility.

The CPUID utility detects processor features and writes a `cpuinfo.json` file,
containing cpu vendor, brand, isa-features and architecture level.

Internally the CPUID instruction is used, which returns processor identification
and feature information to the EAX, EBX, ECX, and EDX registers,
as determined by input entered in EAX (in some cases, ECX as well).

The variable CPUINFO_OK is set in case of a successful compilation and run.
If successful, we can read the json file, check each feature
and set CMAKE variables accordingly, e.g. HAS_SSE42, HAS_AVX2, HAS_AVX512.

## Why?

I encountered challenges while attempting to modify, execute, and test the
embedded C++ code within the original CMake script.

Detecting C++ errors in the embedded source during CMake runs proves to be quite intricate.
This is because the underlying CMake functions, such as check_cxx_source_runs(),
which utilizes try_runs(), merely provide output of success or failure, lacking
any further detailed indications.
The output of CMakeConfigureLog.yaml provides only limited assistance.
I was not aware of any toggles for enabling better error output as of CMake v3.27.1.

Furthermore, the integration of C++ code into CMake scripts has its complexities,
particularly when dealing with code that already contains escaped JSON elements.
This demands double-escaping, which further complicates matters.

The double-escaping issues are also the reason why you can't
directly read the C++ source file and execute it within CMake scripts,
as you might with the use of file() combined with check_cxx_source_runs().

This project involved extracting and refactoring the embedded C++ code,
transitioning it into an standalone project for execution and testing.
The goal is to ensure that the C++ code operates smoothly before integrating
it back into the CMake script.

## How to use CPUINFO.cmake?

1. Simply download the latest release and copy the `CPUINFO.cmake` file into your CMake modules folder, e.g. `project/cmake`

2. Add `include(CPUINFO)` to your `CMakeLists.txt`

```
# set cmake module path for includes
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(CPUINFO)
```

When you run the configure step you should get the following `[CPUINFO]` output:

```
-- [CPU_INFO] Overview:
-- [CPU_INFO]  - Dataset            -> D:/a/cpuinfo_cmake/cpuinfo_cmake/test/build/cpuinfo.json
-- [CPU_INFO]  - Vendor             -> GenuineIntel
-- [CPU_INFO]  - Brand              -> Intel(R) Xeon(R) Platinum 8272CL CPU @ 2.60GHz
-- [CPU_INFO]  - Architecture Level -> x86-64-v4
-- [CPU_INFO] CPU feature flags:
-- [CPU_INFO]  - HAS_SSE2           -> ON
-- [CPU_INFO]  - HAS_SSE3           -> ON
```

### Checking for CPU feature flags

By default, only two feature flags are set as CMake variables, HAS_SSE2 and HAS_SSE3.

You can set them as compiler directives with the following CMake command in your `CMakeLists.txt`:

```
add_definitions(-DHAS_SSE2=${HAS_SSE2})
add_definitions(-DHAS_SSE3=${HAS_SSE3})
```

The `cpuinfo.json` dataset contains more entries in its "isa-features" section.

### Checking for specific CPU feature flags

Please adjust the feature checking for your purposes.
You can enable specific feature flag checks by editing the `CPUINFO.cmake` script.

1. Find CPU feature flag function

Search for `set(CPUINFO_SOURCE_FILE` in `CPUINFO.cmake`. This marks the start of the embedded C++ source.
When you scroll down, you'll find lots of static bool functions, e.g. `static bool FPU()`.
These are all the available accessor functions for CPU feature flag values.

2. Add CPU feature flag function to JSON output

Find the feature you need and then add it as a new entry to the `print_pair` code section below.
This will add the feature as a JSON key value pair to the JSON output.

Use the name of the feature as key and the accessor function InstructionSet::Feature() as value, e.g.

```
print_pair(\"SSE2\",             InstructionSet::SSE2());
```

Important notice for edits to `CPUINFO.cmake`: please keep the escaping!

Please test this step by running CMake configure.

The `cpuinfo.json` dataset in the CMAKE_BINARY_DIR should now contain the entry for your feature flag.

3. Read JSON and set CMake vars

Then search for `if(CPUINFO_OK)`. This is the code section, where the JSON file
is read from within CMake and the data is assigned to CMake variables.
Please append your entry here.

```
  # access the "isa-features" object
  string(JSON CPUINFO_ISA_OBJECT   GET ${CPUINFO_JSON_STRING} "isa-features")
  string(JSON HAS_SSE2             GET ${CPUINFO_ISA_OBJECT}  "SSE2") # isa-features.SSE2
  string(JSON HAS_SSE3             GET ${CPUINFO_ISA_OBJECT}  "SSE3") # isa-features.SSE3
```

In this example HAS_SSE2 and HAS_SSE3 are made available as CMake variables.

4. Use CMake variable to set compiler flag / preprocessor directive

This enables you to add `add_definitions(-DHAS_SSE2=${HAS_SSE2})` to your CMakeLists.txt.

Finally, you can add a preprocessor condition check for this feature flag in your C++ source:

```
#ifdef HAS_SSE2
    std::cout << "HAS_SSE2: This CPU supports SSE2 features.\n";
#endif
```

For example, this is the output of the `hello_cpuinfo` test application, which uses two feature flags:

```
Hello, World!
This is the test for the CPU feature flag macros!
HAS_SSE2: This CPU supports SSE2 features.
HAS_SSE3: This CPU supports SSE3 features.
```

For more details, see [test/CMakeLists.txt](test/CMakeLists.txt) and [test/src/hello_cpuinfo.cpp](test/src/hello_cpuinfo.cpp).

5. Enjoy! 😎

## List of CPU features

This is a list of common CPU features and their corresponding bit positions within
the EBX, ECX, and EDX registers of the CPUID instruction.

Please note that this list is not exhaustive. It may not include the latest features.
Some features might not be present on all processors.

### Resources

- <https://www.felixcloutier.com/x86/cpuid>
- <https://docs.microsoft.com/en-us/cpp/intrinsics/cpuid-cpuidex?view=vs-2019>
- <https://git.kernel.org/pub/scm/linux/kernel/git/stable/linux.git/tree/arch/x86/include/asm/cpufeatures.h>
- Intel Manual "cpuid": <https://www.scss.tcd.ie/Jeremy.Jones/CS4021/processor-identification-cpuid-instruction-note.pdf>

## Projects and build order in this repository

### Main Project

1. `cpuinfo` and `escaper` binaries are build
2. `cpuinfo` is run and produces `cpuinfo.json`
3. `escaper` is run and produces `CPUINFO.cmake`
     - by escaping `cpuinfo.cpp`
     - and embedding it into `cpuinfo.cmake.tpl`

## Test Project

The test project uses `CPUINFO.cmake` from main project in its CMake configure run.

It sets the CPU feature flag as CMake variables: HAS_SSE2 and HAS_SSE3.
These variables are then set as compiler directies.

Finally, the binary `hello_cpuinfo` is build and shows conditional output,
based on the detected CPU feature flags.
