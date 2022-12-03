add_executable(NFATest NFATest.cpp)

target_link_libraries(NFATest
PUBLIC
GTest::GTest
NFA)