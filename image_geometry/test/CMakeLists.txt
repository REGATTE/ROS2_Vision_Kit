find_package(ament_cmake_gtest REQUIRED)

ament_add_gtest(${PROJECT_NAME}-utest utest.cpp)
target_link_libraries(${PROJECT_NAME}-utest ${PROJECT_NAME})

ament_add_gtest(${PROJECT_NAME}-utest-equi utest_equi.cpp)
target_link_libraries(${PROJECT_NAME}-utest-equi ${PROJECT_NAME})