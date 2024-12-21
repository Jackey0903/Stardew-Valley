/****************************************************************
 * 项目名        : Stardew-Valley
 * 文件名        : AppDelegate.cpp
 * 文件功能      : AppDelegate的功能实现
 * 作者          : 曹津硕
 * 更新日期      : 2024/12/04
 * 许可证        : MIT License
 ****************************************************************/

#include "AppDelegate.h"
#include "Scene/StartMenu.h"
#include "proj.win32/Constant.h"
#include "proj.win32/AudioPlayer.h"

USING_NS_CC;

// 设计分辨率大小
static cocos2d::Size designResolutionSize = cocos2d::Size(DESIGN_RESOLUTION_WIDTH, DESIGN_RESOLUTION_HEIGHT);
// 小分辨率大小
static cocos2d::Size smallResolutionSize = cocos2d::Size(SMALL_RESOLUTION_WIDTH, SMALL_RESOLUTION_HEIGHT);
// 中等分辨率大小
static cocos2d::Size mediumResolutionSize = cocos2d::Size(MEDIUM_RESOLUTION_WIDTH, MEDIUM_RESOLUTION_HEIGHT);
// 大分辨率大小
static cocos2d::Size largeResolutionSize = cocos2d::Size(LARGE_RESOLUTION_WIDTH, LARGE_RESOLUTION_HEIGHT);

AppDelegate::~AppDelegate()
{
    // 程序退出时结束音频引擎
    cocos2d::experimental::AudioEngine::end();
}

// 如果需要不同的OpenGL上下文，可以修改glContextAttrs的值
// 这将影响所有平台
void AppDelegate::initGLContextAttrs()
{
    // 设置OpenGL上下文属性：红色、绿色、蓝色、透明度、深度、模板、多重采样数量
    GLContextAttrs glContextAttrs = { 8, 8, 8, 8, 24, 8, 0 };

    GLView::setGLContextAttrs(glContextAttrs);
}

// 如果你想使用包管理器安装更多的包，
// 请不要修改或移除此函数
static int register_all_packages()
{
    return 0; // 标记用于包管理器
}

bool AppDelegate::applicationDidFinishLaunching() {
    // 初始化导演（Director）
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if (!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        // 在桌面平台创建带有指定窗口大小的OpenGL视图
        glview = GLViewImpl::createWithRect("Stardew-Valley", cocos2d::Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
#else
        // 在其他平台创建默认的OpenGL视图
        glview = GLViewImpl::create("Stardew-Valley");
#endif
        director->setOpenGLView(glview);
    }

    // 显示FPS（每秒帧数）
    director->setDisplayStats(true);

    // 设置FPS。默认值是1.0/60，如果不调用这个函数的话
    director->setAnimationInterval(1.0f / 60);

    // 设置设计分辨率
    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::NO_BORDER);
    auto frameSize = glview->getFrameSize();

    // 根据设备的屏幕高度设置内容缩放因子
    if (frameSize.height > mediumResolutionSize.height)
    {
        // 如果屏幕高度大于中等分辨率，则使用大分辨率
        director->setContentScaleFactor(MIN(largeResolutionSize.height / designResolutionSize.height, largeResolutionSize.width / designResolutionSize.width));
    }
    else if (frameSize.height > smallResolutionSize.height)
    {
        // 如果屏幕高度大于小分辨率，则使用中等分辨率
        director->setContentScaleFactor(MIN(mediumResolutionSize.height / designResolutionSize.height, mediumResolutionSize.width / designResolutionSize.width));
    }
    else
    {
        // 如果屏幕高度小于等于小分辨率，则使用小分辨率
        director->setContentScaleFactor(MIN(smallResolutionSize.height / designResolutionSize.height, smallResolutionSize.width / designResolutionSize.width));
    }

    // 注册所有包
    register_all_packages();

    // 运行初始场景：开始菜单
    director->runWithScene(StartMenu::createScene());

    // 可选：再次设置显示FPS和帧率
    director->setDisplayStats(true); // 显示FPS
    director->setAnimationInterval(1.0 / 60); // 设置帧率为60帧每秒
    return true;
}

// 当应用程序进入后台时调用此函数。例如，接到电话时会调用
void AppDelegate::applicationDidEnterBackground() {
    // 停止动画
    Director::getInstance()->stopAnimation();
    // 暂停所有音频
    cocos2d::experimental::AudioEngine::pauseAll();
}

// 当应用程序重新进入前台时调用此函数
void AppDelegate::applicationWillEnterForeground() {
    // 重新启动动画
    Director::getInstance()->startAnimation();
    // 恢复所有音频
    cocos2d::experimental::AudioEngine::resumeAll();
}
