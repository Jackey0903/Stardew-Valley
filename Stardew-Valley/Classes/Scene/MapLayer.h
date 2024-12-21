/****************************************************************
 * ��Ŀ��        : Stardew-Valley
 * �ļ���        : MapLayer.h
 * �ļ�����      : ��ͼ����ͷ�ļ�
 * ����          : ���ƽܡ��ܽ�˶
 * ��������      : 2024/12/07
 * ���֤        : MIT License
 ****************************************************************/

#ifndef __MAP_LAYER_H__
#define __MAP_LAYER_H__

#include "cocos2d.h"
#include <vector>        // �����׼��STL��vector����
#include <string>        // �����׼��STL��string��

 // ʹ��cocos2d�����ռ�
class MapLayer : public cocos2d::Layer
{
public:
    // ��ʼ����������д�����init
    virtual bool init() override;

    // ���ص�ͼ��������ײͼ��
    void loadMap(const std::string& mapFile, float scaleFactor);

    // ��ȡǽ�����
    // ʹ��C++11�ĳ�Ա������������
    std::vector<cocos2d::Rect> getWallRects() const { return _wallRects; }

    // ��ȡ��ǰ��ͼ
    // ʹ��C++11�ĳ�Ա������������
    cocos2d::TMXTiledMap* getTMXTiledMap() const { return currentMap; }

    // ʹ��Cocos2d��CREATE_FUNC�괴��MapLayerʵ��
    CREATE_FUNC(MapLayer);

    cocos2d::DrawNode* collisionDrawNode; // ���ڿ��ӻ���ײ����

private:
    cocos2d::TMXTiledMap* currentMap;             // ��ǰ��ͼ����
    std::string currentMapName;                   // ��ǰ��ͼ����
    std::vector<cocos2d::Rect> _wallRects;        // �洢ǽ�����

};

#endif // __MAP_LAYER_H__
