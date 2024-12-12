#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include <string>

class Player;

class GameScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init() override;
    virtual void onEnter() override;

    CREATE_FUNC(GameScene);

private:
    std::string currentMap;
    Player* _player; // 用成员变量引用player, 防止通过children搜索
};

#endif
