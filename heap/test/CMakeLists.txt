find_package(Catch2 3 REQUIRED)

add_executable(test_heap test_heap.cpp)

target_include_directories(test_heap PRIVATE ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(test_heap PRIVATE Catch2::Catch2WithMain)

target_compile_features(test_heap PRIVATE cxx_std_17)

include(CTest)
include(Catch)
catch_discover_tests(test_heap)