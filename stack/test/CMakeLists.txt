find_package(Catch2 3 REQUIRED)

add_executable(test_stack test_stack.cpp)

target_include_directories(test_stack PRIVATE ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(test_stack PRIVATE Catch2::Catch2WithMain)

target_compile_features(test_stack PRIVATE cxx_std_17)

include(CTest)
include(Catch)
catch_discover_tests(test_stack)