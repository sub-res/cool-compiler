cmake_minimum_required (VERSION 2.6)

project(coolc)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(SOURCES
src/scanner/Scanner.cpp 
src/parser/Parser.cpp
src/compiler/Analyser.cpp 
src/compiler/AsmContainer.cpp 
src/compiler/basics.cpp 
src/compiler/BoolConst.cpp 
src/compiler/CodeGenerator.cpp 
src/compiler/Config.cpp
src/compiler/DispTab.cpp 
src/compiler/driver.cpp 
src/compiler/Environment.cpp 
src/compiler/Error.cpp 
src/compiler/Errors.cpp 
src/compiler/FileIO.cpp 
src/compiler/FreeTreeVisitor.cpp
src/compiler/GraphBuilder.cpp
src/compiler/IgnConstrs.cpp 
src/compiler/Instruction.cpp 
src/compiler/IntConst.cpp 
src/compiler/Intermediater.cpp 
src/compiler/main.cpp 
src/compiler/MemManager.cpp 
src/compiler/Nodes.cpp
src/compiler/Optimizer.cpp 
src/compiler/Printer.cpp 
src/compiler/RegManager.cpp 
src/compiler/StackManager.cpp 
src/compiler/StrConst.cpp 
src/compiler/StringContainer.cpp
src/compiler/SymbolTable.cpp 
src/compiler/TypeValidator.cpp 
src/compiler/UsageChecker.cpp 
src/compiler/Visitor.cpp 
)

add_executable(coolc ${SOURCES})

include_directories(src/compiler src/parser)

#target_compile_features(coolc PRIVATE cxx_range_for)
set_target_properties(coolc
    PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)

add_custom_command(
    OUTPUT ${SRC_DIR}/scanner/Scanner.cpp
    COMMAND 
     flex -+ -o ${SRC_DIR}/scanner/Scanner.cpp 
     ${SRC_DIR}/scanner/scanner.ll
    DEPENDS 
     ${SRC_DIR}/scanner/scanner.ll)
add_custom_target(scanner DEPENDS src/scanner/Scanner.cpp)

add_custom_command(
    OUTPUT ${SRC_DIR}/parser/Parser.cpp
    COMMAND 
     bison -o ${SRC_DIR}/parser/Parser.cpp 
     ${SRC_DIR}/parser/parser.yy 
     --defines=${SRC_DIR}/parser/Parser.h
    DEPENDS ${SRC_DIR}/parser/parser.yy)
add_custom_target(parser DEPENDS ${SRC_DIR}/parser/Parser.cpp)

add_dependencies(coolc scanner parser)

install(TARGETS coolc DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/target/build/bin)

enable_testing()

set(TARGET_FILE ${CMAKE_CURRENT_SOURCE_DIR}/target/build/bin/coolc)

add_test(func_test_err1 ${CMAKE_CURRENT_SOURCE_DIR}/test/err1/testout.sh ${TARGET_FILE})
add_test(func_test_extra_out ${CMAKE_CURRENT_SOURCE_DIR}/test/extra/testout.sh ${TARGET_FILE})
add_test(func_test_extra_asm ${CMAKE_CURRENT_SOURCE_DIR}/test/extra/testasm.sh ${TARGET_FILE})
add_test(func_test_fib_out ${CMAKE_CURRENT_SOURCE_DIR}/test/fib/testout.sh ${TARGET_FILE})
add_test(func_test_fib_asm ${CMAKE_CURRENT_SOURCE_DIR}/test/fib/testasm.sh ${TARGET_FILE})
add_test(func_test_school_out ${CMAKE_CURRENT_SOURCE_DIR}/test/school/testout.sh ${TARGET_FILE})
add_test(func_test_school_asm ${CMAKE_CURRENT_SOURCE_DIR}/test/school/testasm.sh ${TARGET_FILE})
