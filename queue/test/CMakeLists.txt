find_package(Catch2 3 REQUIRED)

add_executable(test_queue test_queue.cpp)

target_include_directories(test_queue PRIVATE ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(test_queue PRIVATE Catch2::Catch2WithMain)

target_compile_features(test_queue PRIVATE cxx_std_17)

include(CTest)
include(Catch)
catch_discover_tests(test_queue)