// Map2Scene.h
#ifndef __MAP2_SCENE_H__
#define __MAP2_SCENE_H__

#include "BaseMapScene.h"

class Map2Scene : public BaseMapScene
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init() override;
    virtual void onEnter() override;

    CREATE_FUNC(Map2Scene);

private:
    bool _isPlanting;
    std::vector<cocos2d::Sprite*> _crops;

    void initEventListeners();
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void onMouseDown(cocos2d::Event* event);
    void plantCropAt(const cocos2d::Vec2& locationInMap);
    void harvestCropAt(const cocos2d::Vec2& locationInMap);
};

#endif
