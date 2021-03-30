#ifndef KKPUBLIC_H
#define KKPUBLIC_H

#include <QFont>
#include <iostream>
using namespace std;

namespace KKpub {
class PubEnv
{
public:
    uint32_t addResource();
    void delResource();

    QFont &getIconFont();

private:
    QFont iconFont;


// 以下是单例的机制
public:
    // get 方法
    static PubEnv *getInstance()
    {
        if (instance == nullptr) {
//            lock();
            if (instance == nullptr) {
                instance = new PubEnv();
            }
//            unlock();
        }
        return instance;
    }
    // c++ 嵌套的内部类，作用是删除单例类对象，Garbage被定义为PubEnv的私有内嵌类，以防该类被在其他地方滥用。
    class Garbage
    {
    public:
        ~Garbage()
        {
            // 释放资源
            getInstance()->delResource();

            if (PubEnv::instance != nullptr) {
//                cout << "cc单例类的唯一实例被析构了" << endl;
                delete PubEnv::instance;
            }
        }
    };

private:
    // 单例类中定义一个这样的静态成员变量，而它的唯一工作就是在析构函数中删除单例类的实例，利用程序在结束时析构全局变量的特性，选择最终的释放时机；
    static Garbage garbage;
    // 声明一个静态的实例
    static PubEnv *instance;
    // 单例类的私有构造函数
    PubEnv()
    {
//        cout << "aa调用了单例类的构造函数" << endl;
    }
    // 单例类的私有析构函数
    ~PubEnv()
    {
//        cout << "bb调用了单例类的析构函数" << endl;
    }
    // 把拷贝构造函数声明为私有，就可以禁止外人拷贝对象，也不用实现它，声明私有即可
    PubEnv(const PubEnv &copy);
    // 把赋值运算符重载为私有的，防止对象之间的赋值操作
    PubEnv &operator=(const PubEnv &other);
};

}

#endif  // KKPUBLIC_H
