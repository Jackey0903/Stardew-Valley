/****************************************************************
 * 项目名        : Stardew-Valley
 * 文件名        : Map3Scene.cpp
 * 文件功能      : 场景3派生类的功能实现
 * 作者          : 胡浩杰、曹津硕、胡正华
 * 更新日期      : 2024/12/21
 * 许可证        : MIT License
 ****************************************************************/

#include <stdexcept>        // 引入异常处理
#include "Map3Scene.h"
#include "MapLayer.h"
#include "Object/Animal.h"
#include "ui/CocosGUI.h"
#include "Object/Npc.h"
USING_NS_CC;

// 创建场景
Scene* Map3Scene::createScene()
{
    return Map3Scene::create();
}

// 初始化场景
bool Map3Scene::init()
{
    // 调用基类的初始化方法，若失败则返回false
    if (!BaseMapScene::init())
        return false;

    // 加载Map3，假设BaseMapScene的loadMap会将_tiledMap的anchorPoint设为(0,0)、position为(0,0)，并缩放地图
    loadMap("Map/Map3/map3.tmx", 5.0f);
    initPlayer(); // 初始化玩家

    // 定义动物出现的区域
    Rect animalArea(280, 65, 100, 100);

    // 使用auto关键字创建猫对象（C++11特性）
    auto cat = Animal::create(animalArea, "Cat");
    _tiledMap->addChild(cat, 20);
    cat->setScale(0.5f); // 设置猫的缩放比例

    return true;
}

// 场景进入时调用
void Map3Scene::onEnter()
{
    BaseMapScene::onEnter(); // 调用基类的onEnter方法

    // 在Map3上查找npc对象层并创建NPC
    if (_tiledMap)
    {
        // 获取名为"Npc_Leah"的对象层
        auto npcLayer = _tiledMap->getObjectGroup("Npc_Leah");
        if (npcLayer)
        {
            // 获取对象层中的所有对象
            auto npcObjects = npcLayer->getObjects();
            // 使用范围基for循环遍历npcObjects（C++11特性）
            for (auto& obj : npcObjects)
            {
                // 将对象转换为ValueMap类型
                auto npcObject = obj.asValueMap();
                // 检查对象的"name"属性是否为"Npc_Leah"
                if (npcObject["name"].asString() == "Npc_Leah")
                {
                    // 获取NPC在地图坐标系中的位置（Tiled左下为(0,0)）
                    float npcX = npcObject["x"].asFloat();
                    float npcY = npcObject["y"].asFloat();
                    Vec2 npcPosition(npcX, npcY);
                    CCLOG("Creating Npc_Leah at position: (%.2f, %.2f)", npcPosition.x, npcPosition.y);

                    // 初始化NPC的对话内容（使用统一初始化列表，C++11特性）
                    _npcLeahDialogues = {
                        {u8"Hey,Jackey.", ""},
                        {u8"What’s up with you?", ""},
                        {u8"You look tired.", ""},
                        {u8"You must pay attention to rest.", ""}
                    };

                    // 初始化NPC的选项内容（使用统一初始化列表，C++11特性）
                    LeahOption = { {u8"Got it ,thank you!"},{u8"Okay,dear."},{u8"I’m not too tired."} };

                    // 使用try-catch块创建Npc_Leah实例并添加到地图
                    try {
                        auto npcLeah = Npc::createWithPosition(npcPosition, "Leah", _npcLeahDialogues, LeahOption);
                        if (!npcLeah)
                            throw std::runtime_error("Failed to create Npc_Leah instance.");

                        // 其他逻辑
                        CCLOG("Npc_Leah created successfully.");
                        npcLeah->setScale(0.1f);  // 设置NPC的缩放
                        _tiledMap->addChild(npcLeah, 25);
                    }
                    catch (const std::exception& e) {
                        CCLOG("Exception: %s", e.what());
                        Director::getInstance()->end();  // 结束游戏
                    }
                }
            }
        }
    }
}
