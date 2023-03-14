/*  
 *  Description : 用来测试
 *  Created by 旋风冲锋龙卷风
 *  个人博客 : http://letsgofun.cn/
 */
//

#include <iostream>

using namespace std;

#include "Connection.h"
#include "CommonConnectionPool.h"

//不使用连接池直接连接
void connectWithoutPool()
{
    clock_t begin = clock();
    for(int i  = 0; i < 1000; ++i)
    {
        Connection conn;
        char sql[1024] = {0};
        sprintf(sql, "insert into user(name,age,sex) values('%s',%d, '%s')", "zhang san", 20, "male");
        conn.connect("127.0.0.1", 3306, "root", "123456", "chat");
        conn.update(sql);
    }

    clock_t end = clock();
    cout << end - begin << "us" << endl;
}

//测试配置文件
void readConf()
{
    ConnectionPool *p = ConnectionPool::getConnectionPool();
    // p->loadConfigFile();
}


//使用连接池
void connectWithPool()
{
    clock_t begin = clock();

    ConnectionPool* cp = ConnectionPool::getConnectionPool();
    for(int i = 0; i < 10000; ++i)
    {
        shared_ptr<Connection> sp = cp->getConnection();
        char sql[1024] = {0};
        sprintf(sql, "insert into user(name,age,sex) values('%s',%d, '%s')", "zhang san", 20, "male");
        sp->update(sql);
    }

    clock_t end = clock();
    cout << end - begin << "us" << endl;
}

void connectWithPoolAndThread()
{
    clock_t begin = clock();
    thread t1([](){
        ConnectionPool* cp = ConnectionPool::getConnectionPool();
        for(int i = 0; i < 1250; ++i)
        {
            shared_ptr<Connection> sp = cp->getConnection();
            char sql[1024] = {0};
            sprintf(sql, "insert into user(name,age,sex) values('%s',%d, '%s')", "zhang san", 20, "male");
            sp->update(sql);
        }
    });
    thread t2([](){
        ConnectionPool* cp = ConnectionPool::getConnectionPool();
        for(int i = 0; i < 1250; ++i)
        {
            shared_ptr<Connection> sp = cp->getConnection();
            char sql[1024] = {0};
            sprintf(sql, "insert into user(name,age,sex) values('%s',%d, '%s')", "zhang san", 20, "male");
            sp->update(sql);
        }
    });
    thread t3([](){
        ConnectionPool* cp = ConnectionPool::getConnectionPool();
        for(int i = 0; i < 1250; ++i)
        {
            shared_ptr<Connection> sp = cp->getConnection();
            char sql[1024] = {0};
            sprintf(sql, "insert into user(name,age,sex) values('%s',%d, '%s')", "zhang san", 20, "male");
            sp->update(sql);
        }
    });
    thread t4([](){
        ConnectionPool* cp = ConnectionPool::getConnectionPool();
        for(int i = 0; i < 1250; ++i)
        {
            shared_ptr<Connection> sp = cp->getConnection();
            char sql[1024] = {0};
            sprintf(sql, "insert into user(name,age,sex) values('%s',%d, '%s')", "zhang san", 20, "male");
            sp->update(sql);
        }
    });
    t1.join();
    t2.join();
    t3.join();
    t4.join();

    cout << clock() - begin << endl;
}

void connectWithOnlyThread()
{
    Connection conn;
    conn.connect("127.0.0.1", 3306, "root", "123456", "chat");

    /*Connection conn;
    char sql[1024] = { 0 };
    sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')",
        "zhang san", 20, "male");
    conn.connect("127.0.0.1", 3306, "root", "123456", "chat");
    conn.update(sql);*/

    clock_t begin = clock();

    thread t1([]() {
        //ConnectionPool *cp = ConnectionPool::getConnectionPool();
        for (int i = 0; i < 1250; ++i)
        {
            /*char sql[1024] = { 0 };
            sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')",
                "zhang san", 20, "male");
            shared_ptr<Connection> sp = cp->getConnection();
            sp->update(sql);*/
            Connection conn;
            char sql[1024] = { 0 };
            sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')",
                    "zhang san", 20, "male");
            conn.connect("127.0.0.1", 3306, "root", "123456", "chat");
            conn.update(sql);
        }
    });
    thread t2([]() {
        //ConnectionPool *cp = ConnectionPool::getConnectionPool();
        for (int i = 0; i < 1250; ++i)
        {
            /*char sql[1024] = { 0 };
            sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')",
                "zhang san", 20, "male");
            shared_ptr<Connection> sp = cp->getConnection();
            sp->update(sql);*/
            Connection conn;
            char sql[1024] = { 0 };
            sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')",
                    "zhang san", 20, "male");
            conn.connect("127.0.0.1", 3306, "root", "123456", "chat");
            conn.update(sql);
        }
    });
    thread t3([]() {
        //ConnectionPool *cp = ConnectionPool::getConnectionPool();
        for (int i = 0; i < 1250; ++i)
        {
            /*char sql[1024] = { 0 };
            sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')",
                "zhang san", 20, "male");
            shared_ptr<Connection> sp = cp->getConnection();
            sp->update(sql);*/
            Connection conn;
            char sql[1024] = { 0 };
            sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')",
                    "zhang san", 20, "male");
            conn.connect("127.0.0.1", 3306, "root", "123456", "chat");
            conn.update(sql);
        }
    });
    thread t4([]() {
        //ConnectionPool *cp = ConnectionPool::getConnectionPool();
        for (int i = 0; i < 1250; ++i)
        {
            /*char sql[1024] = { 0 };
            sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')",
                "zhang san", 20, "male");
            shared_ptr<Connection> sp = cp->getConnection();
            sp->update(sql);*/
            Connection conn;
            char sql[1024] = { 0 };
            sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')",
                    "zhang san", 20, "male");
            conn.connect("127.0.0.1", 3306, "root", "123456", "chat");
            conn.update(sql);
        }
    });

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    clock_t end = clock();
    cout << (end - begin) << "ms" << endl;
}

int main()
{
    // connectWithoutPool();
    // connectWithPool();
    // connectWithPoolAndThread();
    connectWithOnlyThread();
    return EXIT_SUCCESS;
}