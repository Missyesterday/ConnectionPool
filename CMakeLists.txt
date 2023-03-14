cmake_minimum_required(VERSION 3.20)
project(CommonConnectionPool)

set(CMAKE_CXX_STANDARD 14)

# 设置别名
set(MYSQL_HEADER /usr/local/mysql/include)
set(MYSQL_LIB /usr/local/mysql/lib)

# 添加 MySQL 连接头文件
include_directories(${MYSQL_HEADER})
# 添加 MySQL 外部库
link_libraries(${MYSQL_LIB})


# 指定生成目标
add_executable(CommonConnectionPool  main.cpp CommonConnectionPool.cpp CommonConnectionPool.h Connection.cpp Connection.h public.h)

# 添加链接库
target_link_libraries(CommonConnectionPool ${MYSQL_LIB}/libmysqlclient.a)
target_link_libraries(CommonConnectionPool ${MYSQL_LIB}/libmysqlservices.a)
target_link_libraries(CommonConnectionPool ${MYSQL_LIB}/libssl.1.1.dylib)
target_link_libraries(CommonConnectionPool ${MYSQL_LIB}/libcrypto.1.1.dylib)


