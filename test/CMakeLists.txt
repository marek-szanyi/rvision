
#find_package(GTest REQUIRED)

set(PROJECT_TEST_NAME "${PROJECT_NAME}_test")
include_directories(${GTEST_INCLUDE_DIRS} ${COMMON_INCLUDES})

set(TEST_SOURCE_FILES
        ./rest_client_test.cpp
        ./rest_api_123sms_test.cpp
        ./rest_api_mailgate_test.cpp
        ./notifier_sms_test.cpp
        )


add_executable(${PROJECT_TEST_NAME} ${TEST_SOURCE_FILES})
add_dependencies(${PROJECT_TEST_NAME} googletest)
target_link_libraries(${PROJECT_TEST_NAME} "${PROJECT_NAME}lib" ${GTEST_LIBS_DIR}/libgtest.a ${GTEST_LIBS_DIR}/libgtest_main.a pthread ${Boost_LIBRARIES} ${OpenCV_LIBS} restbed ssl crypto)

add_test(${PROJECT_TEST_NAME} ${PROJECT_TEST_NAME})