find_package(Catch2 3 REQUIRED)

add_executable(test_doubly_list test_doubly_list.cpp)

target_include_directories(test_doubly_list PRIVATE ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(test_doubly_list PRIVATE Catch2::Catch2WithMain)

target_compile_features(test_doubly_list PRIVATE cxx_std_17)

include(CTest)
include(Catch)
catch_discover_tests(test_doubly_list)