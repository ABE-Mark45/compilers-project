add_executable(LL1GeneratorTest LL1GeneratorTest.cpp)

target_link_libraries(LL1GeneratorTest
PUBLIC
GTest::GTest
LL1Generator)