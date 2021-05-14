cmake_minimum_required(VERSION 3.19)
project(payhttp)

set(CMAKE_CXX_FLAGS "${CAMKE_CXX_FLAGS} -std=c++11 -Wall -pthread")

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_BUILD_TYPE Debug)

#boost
#find_package(Boost REQUIRED COMPONENTS
#        # boost组件
#        filesystem
#        system
#        date_time
#        context
#        coroutine
#        thread
#        )
#
#if (NOT Boost_FOUND)
#    message("Not found Boost")
#endif ()
#
#include_directories(${Boost_INCLUDE_DIRS})
#message("${Boost_INCLUDE_DIRS}")
#message("${Boost_LIBRARIES}")

# 服务器
aux_source_directory(server/include INCLUDE)
aux_source_directory(server/src SRC)

# web后台
aux_source_directory(web/core WEB_CORE)
aux_source_directory(web/src/controller SRC_CONTROLLER)
aux_source_directory(web/src/model WEB_MODEL)
aux_source_directory(web/src/service WEB_SERVICE)
aux_source_directory(web/src/dao WEB_DAO)

# 线程池
aux_source_directory(threadpool THREAD_POOL)

add_executable(payhttp  main.cpp ${THREAD_POOL} ${INCLUDE} ${SRC} ${WEB_CORE} ${SRC_CONTROLLER} ${WEB_MODEL} ${WEB_SERVICE} ${WEB_DAO})

#target_link_libraries(payhttp ${Boost_LIBRARIES})