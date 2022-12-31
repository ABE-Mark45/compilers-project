add_executable(SimulatorTest SimulatorTest.cpp)

target_link_libraries(SimulatorTest
PUBLIC
GTest::GTest
Simulator)