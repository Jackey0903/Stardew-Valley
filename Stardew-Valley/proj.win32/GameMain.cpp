/****************************************************************
 MIT License

 Copyright (c) 2024 Jinshuo Cao, Haojie Hu, Zhenghua Hu

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 ****************************************************************/



 /****************************************************************
  * 项目名        : Stardew-Valley
  * 文件名        : GameMain.cpp
  * 文件功能      : 游戏程序入口点
  * 作者          : 曹津硕
  * 更新日期      : 2024/12/04
  * 许可证        : MIT License
  ****************************************************************/


#define WIN32_LEAN_AND_MEAN    // 排除 Windows 头文件中不常用的部分

#include <Windows.h>
#include <tchar.h>
#include "platform/CCStdC.h"
#include "AppDelegate/AppDelegate.h"
#include "cocos2d.h"

USING_NS_CC;
/*
 * 函数名          : _tWinMain
 * 功能            : 游戏程序入口点
 * 形参            : HINSTANCE hInstance: 当前实例的句柄
 *                   HINSTANCE hPrevInstance: 前一个实例的句柄
 *                   LPTSTR lpCmdLine: 命令行参数的字符串
 *                   int nCmdShow: 控制窗口的显示状态
 * 返回值          : 程序退出状态
 */
int WINAPI _tWinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPTSTR lpCmdLine,int nCmdShow)
{
    srand(static_cast<unsigned int>(time(0)));

    // 初始化 Cocos2d-x 应用程序实例
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    AppDelegate app;
    return Application::getInstance()->run();
}
