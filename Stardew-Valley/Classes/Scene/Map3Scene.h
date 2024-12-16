// Map3Scene.h
#ifndef __MAP3_SCENE_H__
#define __MAP3_SCENE_H__

#include "BaseMapScene.h"

class Map3Scene : public BaseMapScene
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init() override;
    virtual void onEnter() override;

    CREATE_FUNC(Map3Scene);

private:
    cocos2d::Sprite* _npc;

    void createNPC();
    void showDialogue();
};

#endif

