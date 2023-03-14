/*  
 *  Description : 连接池单例模式, 线程安全
 *  Created by 旋风冲锋龙卷风
 *  个人博客 : http://letsgofun.cn/
 */
//

#include "CommonConnectionPool.h"

//线程安全的懒汉单例函数接口
ConnectionPool *ConnectionPool::getConnectionPool()
{
    static ConnectionPool pool; //对于静态局部变量的初始化,编译器会自动lock 和 unlock
    //同时静态局部变量只有第一次运行到它的时候才会初始化
    return &pool;
}

//连接池的构造函数
ConnectionPool::ConnectionPool()
{
    //加载配置项
    if(!loadConfigFile())
    {
        return;
    }

    //创建初始数量的连接
    for(int i = 0; i < _initSize; ++i)
    {
        Connection *p = new Connection();
        p->connect(_ip, _port, _username, _password, _dbname);
        //刷新空闲开始的起始时间
        p->refreshAliveTime();
        _connectionQueue.push(p);
        _connectionCnt++;
    }

    //启动一个新的线程, 作为连接的生产者
    //C++语言的thread调用的就是pthread_create
    //给成员方法绑定一个对象
    //成员函数不能直接作为线程函数
    //创建新线程的过程通过 std::thread 类实现，因此可以将该行代码解释为“创建一个新的线程，并执行 ConnectionPool 对象的 produceConnectionTask 方法”
    thread produce(std::bind(&ConnectionPool::produceConnectionTask, this));
    produce.detach();


    //启动一个新的定时线程, 扫描多余的空闲连接, 超过maxIdleTime时间的空闲连接,进行多余的连接回收
    thread scanner(std::bind(&ConnectionPool::scannerConnectionTask, this));
    scanner.detach();

}

// 从配置文件中加载配置项
bool ConnectionPool::loadConfigFile()
{
    FILE *pf = fopen("../mysql.conf", "r");
    if(pf == nullptr)
    {
        LOG("mysql.conf file is not exit!");
        return false;
    }

    while(!feof(pf))
    {
        char line[1024] = {0};
        fgets(line, 1024, pf);
        string str = line;
        int idx = str.find('=');
        if(idx == -1)
        {
            //无效的配置项,或者错误的配置项
            continue;
        }
        int endidx = str.find('\n', idx);
        string key = str.substr(0, idx);
        string value = str.substr(idx + 1, endidx - idx - 1);

        if(key == "ip")
        {
            _ip = value;
        }
        else if(key == "port")
        {
            _port = stoi(value.c_str());
        }
        else if(key == "username")
        {
            _username = value;
        }
        else if(key == "password")
        {
            _password = value;
        }
        else if(key == "dbname")
        {
            _dbname = value;
        }
        else if(key == "initSize")
        {
            _initSize = atoi(value.c_str());
        }
        else if(key == "maxSize")
        {
            _maxSize = atoi(value.c_str());
        }
        else if(key == "maxIdleTime")
        {
            _maxIdleTime = atoi(value.c_str());
        }
        else if(key == "connectionTimeOut")
        {
            _connectionTimeout = atoi(value.c_str());
        }
    }

    return true;
}

// 给外部提供接口, 从连接池中获取一个可用的空闲连接
//智能指针的默认方式是 delete指针
shared_ptr<Connection> ConnectionPool::getConnection()
{
    unique_lock<mutex> lock(_queueMutex);


    while(_connectionQueue.empty())
    {
        //是被超时唤醒的
        if(cv_status::timeout == cv.wait_for(lock, chrono::milliseconds(_connectionTimeout)))
        {
            if(_connectionQueue.empty())
            {
                LOG("获取空闲连接超时...获取连接失败!");
                return nullptr;
            }
        }
    }
    /*
    * shared_ptr智能指针析构时,会直接delete掉connection,相当于调用调用connection的析构函数
    * 这里需要自定义shared_ptr的释放资源的方式,把 connection直接归还到queue中, 而不是delete
    * 而不是需要用户手动调用归还连接
    * */
    shared_ptr<Connection> sp(_connectionQueue.front(),
                              [&](Connection *pcon)
                              {
                                //这里是在服务器应用线程中调用的,所以需要考虑队列的线程操作安全
                                unique_lock<mutex> lock(_queueMutex);
                                pcon->refreshAliveTime();
                                _connectionQueue.push(pcon);
                              });

    _connectionQueue.pop();
    //条件, 不加这个状态的话也行, 因为生产者是while循环判断
    if(_connectionQueue.empty())
    {
        cv.notify_all(); //谁消费了队列中的最后一个连接, 谁要负责通知生产者生产连接
    }
    return sp;
}

//空闲队列为空, 建立新的连接
void ConnectionPool::produceConnectionTask()
{
    for(;;)
    {
        unique_lock<mutex> lock(_queueMutex);
        while(!_connectionQueue.empty())
        {
            cv.wait(lock); //队列不空, 此处生产线程进入等待状态
        }

        // 连接数量没有到达上限, 继续创建新的连接
        if(_connectionCnt < _maxSize)
        {
            //生产新的连接
            Connection* p = new Connection();
            p->connect(_ip, _port, _username, _password, _dbname);
            p->refreshAliveTime();
            _connectionQueue.push(p);
            _connectionCnt++;
        }
        //通知消费者线程 可以消费连接了
        cv.notify_all();
    }
}

//扫描超过maxIdleTime时间的空闲连接, 进行多余的连接回收
void ConnectionPool::scannerConnectionTask()
{
    for(;;)
    {
        //通过sleep模拟定时效果
        this_thread::sleep_for(chrono::seconds(_maxIdleTime));

        //扫描整个队列, 释放多余的连接
        unique_lock<mutex> lock(_queueMutex);
        while(_connectionCnt > _initSize)
        {
            Connection *p = _connectionQueue.front();
            if(p->getAliveTime() >= (_maxIdleTime) * 1000)
            {
                _connectionQueue.pop();
                _connectionCnt--;
                delete p;
            }
            else
            {
                //队头连接都没有超过_maxIdleTime, 那么其他的更加不会
                break;
            }
        }

    }
}
