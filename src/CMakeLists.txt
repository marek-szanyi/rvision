
set(SOURCE_FILES
        ./motion_engine.cpp
        ./motion_engine.h
        ./rest_client.h
        ./rest_api_123sms.h
        ./notificator.h
        ./notificator_builder.h
        ./notifier_sms.h
        ./notifier_email.h
        ./observer.h
        ./event.h
        ./event.cpp
        ./rvision_types.h
        ./rest_api_mailgate.h
        ./notifier_disk.h
        ./rest_client.cpp
        ./network_error.h
        ./config_error.h)

set(LIBB64_FILES
        ../libs/libb64/cdecode.c
        ../libs/libb64/cdecode.h
        ../libs/libb64/cencode.c
        ../libs/libb64/cencode.h
        ../libs/libb64/decode.h
        ../libs/libb64/encode.h
        )

add_executable(raspvision ./main.cpp ${SOURCE_FILES} ${LIBB64_FILES})
add_library(raspvisionlib ${SOURCE_FILES} ${LIBB64_FILES})
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} ${OpenCV_LIBS} ${CMAKE_THREAD_LIBS_INIT} restbed ssl crypto)