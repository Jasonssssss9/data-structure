find_package(Catch2 3 REQUIRED)

add_executable(test_avl_tree test_avl_tree.cpp)

target_include_directories(test_avl_tree PRIVATE ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(test_avl_tree PRIVATE Catch2::Catch2WithMain)

target_compile_features(test_avl_tree PRIVATE cxx_std_17)

include(CTest)
include(Catch)
catch_discover_tests(test_avl_tree)