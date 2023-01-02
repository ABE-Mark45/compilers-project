add_executable(ParseTableGeneratorTest ParseTableGeneratorTest.cpp)

target_link_libraries(ParseTableGeneratorTest
        PUBLIC
        GTest::GTest
        ParseTableGenerator
        )