/****************************************************************
 * 项目名        : Stardew-Valley
 * 文件名        : AppDelegate.h
 * 文件功能      : AppDelegate的功能实现
 * 作者          : 曹津硕
 * 更新日期      : 2024/12/04
 * 许可证        : MIT License
 ****************************************************************/

#ifndef  _APP_DELEGATE_H_
#define  _APP_DELEGATE_H_

#include "cocos2d.h"

 /**
  * @brief    cocos2d 应用程序类。
  *
  * 通过私有继承隐藏部分 Director 接口，使得外部无法直接访问。
  */
class  AppDelegate : private cocos2d::Application
{
public:
    // 析构函数
    virtual ~AppDelegate();

    // 初始化OpenGL上下文属性
    virtual void initGLContextAttrs();

    /**
     * @brief    应用程序初始化函数，设置导演和场景等。
     * @return true    初始化成功，应用继续执行。
     * @return false   初始化失败，应用终止。
     */
    virtual bool applicationDidFinishLaunching();

    /**
     * @brief  当应用程序进入后台时调用此函数
     * @param  application 指向应用程序的指针
     */
    virtual void applicationDidEnterBackground();

    /**
     * @brief  当应用程序重新进入前台时调用此函数
     * @param  application 指向应用程序的指针
     */
    virtual void applicationWillEnterForeground();
};

#endif // _APP_DELEGATE_H_
