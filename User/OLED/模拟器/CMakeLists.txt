cmake_minimum_required(VERSION 3.10)
project(OLED_Simulator C)

# 设置C标准
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# 设置编译选项
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -g -O2")

# 查找SDL2
find_package(SDL2 REQUIRED)

# 设置目录变量
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR})

# 添加OLED库源文件
set(OLED_LIB_SOURCES
    ../logo.c
)

# 创建OLED资源库
add_library(oled_resources STATIC ${OLED_LIB_SOURCES})
target_include_directories(oled_resources PUBLIC ${INCLUDE_DIR} ../OLED)

# 基础模拟器
add_executable(basic_simulator 
    ${SRC_DIR}/oled_simulator.c
)
target_link_libraries(basic_simulator PRIVATE 
    SDL2::SDL2 
    oled_resources
)
target_include_directories(basic_simulator PRIVATE 
    ${INCLUDE_DIR}
    ${SRC_DIR}
    ../OLED
)
target_compile_definitions(basic_simulator PRIVATE 
    OLED_SIMULATOR=1
)

# 增强模拟器
add_executable(enhanced_simulator 
    ${SRC_DIR}/oled_simulator_enhanced.c
)
target_link_libraries(enhanced_simulator PRIVATE 
    SDL2::SDL2 
    oled_resources
)
target_include_directories(enhanced_simulator PRIVATE 
    ${INCLUDE_DIR}
    ${SRC_DIR}
    ../OLED
)
target_compile_definitions(enhanced_simulator PRIVATE 
    OLED_SIMULATOR=1
)

# 简单测试图像
add_executable(simple_test 
    ${SRC_DIR}/simple_test_image.c
)
target_link_libraries(simple_test PRIVATE 
    SDL2::SDL2 
    oled_resources
)
target_include_directories(simple_test PRIVATE 
    ${INCLUDE_DIR}
    ${SRC_DIR}
    ../OLED
)
target_compile_definitions(simple_test PRIVATE 
    OLED_SIMULATOR=1
)

# 数学库（在Linux/macOS上需要）
if(UNIX AND NOT APPLE)
    target_link_libraries(basic_simulator PRIVATE m)
    target_link_libraries(enhanced_simulator PRIVATE m)
    target_link_libraries(simple_test PRIVATE m)
endif()

# 设置输出目录
set_target_properties(basic_simulator enhanced_simulator simple_test PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${BUILD_DIR}/bin
)

# 示例程序
add_executable(basic_example
    ${CMAKE_CURRENT_SOURCE_DIR}/examples/basic_usage.c
)
target_link_libraries(basic_example PRIVATE SDL2::SDL2)
if(UNIX AND NOT APPLE)
    target_link_libraries(basic_example PRIVATE m)
endif()
target_include_directories(basic_example PRIVATE ${INCLUDE_DIR})
target_compile_definitions(basic_example PRIVATE OLED_SIMULATOR=1)

set_target_properties(basic_example PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${BUILD_DIR}/bin
)

# 创建运行目标
add_custom_target(run_enhanced
    COMMAND ${BUILD_DIR}/bin/enhanced_simulator
    DEPENDS enhanced_simulator
    WORKING_DIRECTORY ${BUILD_DIR}
    COMMENT "运行OLED增强模拟器"
)

add_custom_target(run_basic
    COMMAND ${BUILD_DIR}/bin/basic_simulator
    DEPENDS basic_simulator
    WORKING_DIRECTORY ${BUILD_DIR}
    COMMENT "运行OLED基础模拟器"
)

add_custom_target(run_test
    COMMAND ${BUILD_DIR}/bin/simple_test
    DEPENDS simple_test
    WORKING_DIRECTORY ${BUILD_DIR}
    COMMENT "运行OLED简单测试"
)

add_custom_target(run_example
    COMMAND ${BUILD_DIR}/bin/basic_example
    DEPENDS basic_example
    WORKING_DIRECTORY ${BUILD_DIR}
    COMMENT "运行OLED基础示例"
)

# 清理目标
add_custom_target(distclean
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target clean
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}
    COMMENT "完全清理构建目录"
)

# 配置信息
message(STATUS "项目配置完成")
message(STATUS "构建类型: ${CMAKE_BUILD_TYPE}")
message(STATUS "编译器: ${CMAKE_C_COMPILER}")
message(STATUS "SDL2库路径: ${SDL2_LIBRARIES}")
message(STATUS "SDL2头文件路径: ${SDL2_INCLUDE_DIRS}")
message(STATUS "")
message(STATUS "可用的模拟器:")
message(STATUS "  basic_simulator  - 基础OLED模拟器")
message(STATUS "  enhanced_simulator - 增强OLED模拟器")
message(STATUS "  simple_test     - 简单测试程序")
message(STATUS "  basic_example   - 基础使用示例")
message(STATUS "")
message(STATUS "运行方法:")
message(STATUS "  make             - 构建所有程序")
message(STATUS "  make run_enhanced - 构建并运行增强模拟器")
message(STATUS "  make run_basic   - 构建并运行基础模拟器")
message(STATUS "  make run_test    - 构建并运行测试程序")
message(STATUS "  make run_example - 构建并运行基础示例")