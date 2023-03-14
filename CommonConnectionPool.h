/*  
 *  Description : 连接池单例模式, 线程安全
 *  Created by 旋风冲锋龙卷风
 *  个人博客 : http://letsgofun.cn/
 */
//

#pragma once
#ifndef COMMONCONNECTIONPOOL_COMMONCONNECTIONPOOL_H
#define COMMONCONNECTIONPOOL_COMMONCONNECTIONPOOL_H

#include <string>
#include <queue>
#include <iostream>
#include <mutex> //C++11提供的
#include <atomic> //C++11提供的原子类
#include <thread>
#include <functional>
#include <memory> //智能指针
#include <condition_variable>

using namespace std;

#include "Connection.h"
#include "public.h"

class ConnectionPool
{
public:
    //获取连接池对象实例
    static ConnectionPool* getConnectionPool();
    //给外部提供接口, 从连接池中获取一个可用的空闲连接
    //智能指针自动构造析构
    shared_ptr<Connection> getConnection();
private:
    // 单例1, 构造函数私有化
    ConnectionPool();
    //运行在独立线程中, 专门负责生产新连接
    void produceConnectionTask();

    //从配置文件中加载配置项
    bool loadConfigFile();

    //扫描超过maxIdleTime时间的空闲连接, 进行多余的连接回收
    void scannerConnectionTask();

    //一般放在配置文件里
    string _ip; //MySQL的ip
    unsigned short _port; //端口号, 3306
    string _username; //MySQL用户名
    string _password; //MySQL用户密码
    string _dbname; //连接的数据库名称
    int _initSize; // 连接池的初始连接量
    int _maxSize; //连接池最大连接量
    int _maxIdleTime; //连接池最大空闲时间
    int _connectionTimeout; //连接池获取连接的超时时间

    queue<Connection*> _connectionQueue; //存储MySQL连接的队列
    mutex _queueMutex; //维护连接队列的线程安全互斥锁
    atomic_int _connectionCnt; //记录连接新创建的连接的总数量,不能超过_maxSize
    condition_variable cv; //设置条件变量, 用于连接生产线程和连接消费线程的通信
};


#endif //COMMONCONNECTIONPOOL_COMMONCONNECTIONPOOL_H
