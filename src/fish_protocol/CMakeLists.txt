cmake_minimum_required(VERSION 3.0)
 
# Define project name
project(fish_protocol)

# add_compile_options(-Werror)
add_compile_options(-Wall)
add_compile_options(-std=c++14)
# 消除boost的note
add_compile_options(-D=BOOST_BIND_GLOBAL_PLACEHOLDERS)
# 开启Debug模式
SET(CMAKE_BUILD_TYPE "RELEASE")

# 添加头文件
include_directories(
    include
    ${Boost_INCLUDE_DIRS}
)

# 查找依赖
find_package(Boost REQUIRED COMPONENTS thread)
if(NOT Boost_FOUND)
    message("NOT found Boost")
endif()




#***************************************动态库生成************************************************#

# 库文件
file(GLOB_RECURSE FISH_PROTOCOL_LIB_SRC "src/*.cpp")

# 添加动态库
add_library(${PROJECT_NAME} SHARED ${FISH_PROTOCOL_LIB_SRC})

target_link_libraries(${PROJECT_NAME} pthread boost_thread )

#***************************************动态库生成************************************************#



# 代码格式检查
set(CPPLINT_SRC ${FISH_PROTOCOL_SRC} ${FISH_PROTOCOL_TESTS})
list(REMOVE_DUPLICATES CPPLINT_SRC)
foreach(v ${CPPLINT_SRC})
  execute_process(
    COMMAND cpplint --quiet ${v}
    ERROR_VARIABLE _result
  )
  if( _result)
    message(FATAL "cpplint check failed!  \n ${_result}")
  endif()
endforeach(v)

#***************************************DEBUG模式配置************************************************#





#***************************************INSTALL相关************************************************#
# 生成配置文件，安装到系统
install(
  DIRECTORY include/
  DESTINATION include
)

install(
  TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
  INCLUDES DESTINATION include
)

# add lib.config
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
        ${PROJECT_NAME}Config.cmake
        VERSION 1.0
        COMPATIBILITY AnyNewerVersion 
        )

install(EXPORT ${PROJECT_NAME}
        FILE ${PROJECT_NAME}Config.cmake
        DESTINATION lib/cmake/${PROJECT_NAME}
        )
#***************************************INSTALL相关************************************************#