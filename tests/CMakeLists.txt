include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

FetchContent_MakeAvailable(googletest)
add_library(GTest::GTest INTERFACE IMPORTED)
target_link_libraries(GTest::GTest INTERFACE gtest_main)

include_directories(${PROJECT_SOURCE_DIR}/src)
add_subdirectory(LanguageRulesParserTest)
add_subdirectory(NFATest)
add_subdirectory(DFAExampleTest)
add_subdirectory(DFABuilderTest)
add_subdirectory(CFGParserTest)
add_subdirectory(LL1GeneratorTest)
add_subdirectory(SimulatorTest)
add_subdirectory(ParseTableGeneratorTest)
