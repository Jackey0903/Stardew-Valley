![](https://img.shields.io/badge/Cocos2d--x--3.17-black?style=flat&logo=cocos&logoColor=%23FFFFFF&logoSize=1000&labelColor=%23000000&color=%230000FF)![](https://img.shields.io/badge/IDE-Visual_Studio_2022-blue?logo=cplusplus&color=%236A5ACD)

> [!IMPORTANT]
>
> You have to work hard every day, just like planting crops. How can you harvest if you don’t sow seeds? ***---Hu Haojie***

> [!WARNING]
>
> ***千里之行，始于足下！***——《老子》

# Stardew-Valley (星露谷)

> [!TIP]
>
> 抵制不良游戏 拒绝盗版游戏 注意自我保护 谨防受骗上当
>
> 适度游戏益脑 沉迷游戏伤身 合理安排时间 享受健康生活

# 项目名称

Stardew-Valley

# 成员分工

|  姓名  |  学号   |                             分工                             | 贡献占比 |
| :----: | :-----: | :----------------------------------------------------------: | :------: |
| 曹津硕 | 2353127 | 项目任务分工<br/>代码审查<br/>`Android`端触摸按键设计与逻辑调整，`Android`项目处理与打包<br/>除tmx地图外所有美术资源搜集与处理<br/>项目说明文档撰写<br/>音频引擎<br/>常量整理与命名<br/>`StartMenu` 类<br/>`Player` 类碰撞检测功能与行走功能<br/>`AudioPlayer` 类<br/>`SettingScene` 类<br/>`Animal` 类整合和完善<br/>`N`pc 类整合和完善<br/>`BackpackScene` 类重构并完善画面<br/>`Mapscene` 类重构并完善画面<br/> |   40%    |
| 胡浩杰 | 2351493 | 项目框架搭建<br/>项目说明文档撰写<br/>项目答辩ppt制作<br/>代码美化与注释<br/>`Player` 类基本初始化和移动功能<br/>`Animal` 类基础功能实现<br/>`Npc` 类基础功能实现<br/>`Backpack` 类<br/>`I`tem 类<br/>`BackpackScene` 类<br/>`Mine` 类<br/>`MapScene` 类基础功能实现<br/>`BaseMapScene`类<br/>`MapLayer` 类<br/>`Map1Scene` 类<br/>`Map2Scene` 类<br/>`Map3Scene` 类<br/> |   40%    |
| 胡正华 | 2353741 | 所有tmx地图绘制以及碰撞对象层的绘制<br/>项目答辩ppt制作<br/>`Player` 类的碰撞检测功能<br/>`MapLayer` 类的碰撞检测功能<br/> 任务委托的策划和人物对话内容设计<br/> |   20%    |





# 项目开发日志

### 2024/11/23

- Python 2.7.10 环境配置
- CMake 3.28.0 环境配置
- Cocos2d-x 3.17 环境配置

### 2024/11/24

- 建立项目托管 Github[仓库](https://github.com/Jackey0903/Stardew-Valley/)

### 2024/11/27

- 实现角色基本移动，启动菜单，角色可以在png地图上移动
- 初探美术素材打包方法（利用TexturePacker），制作地图的方法（利用Tiled）
- MarkDown语言学习

### 2024/12/1

- 实现初始菜单按键高亮，菜单按键位置调整
- 尝试导入tmx瓦片地图
- 实现音频引擎AudioPlayer

### 2024/12/8

- 重构项目源文件，发布第一正式版，抛弃从原有的HelloWord.cpp上编辑，实现并行开发

- 创建Player类，BackPack类，BackPackScene场景类，StartMenu场景类

- 推翻之前按键高亮逻辑，改用通用的HoverButton类

- 在原型tmx地图里基本实现碰撞检测，地图随人物移动

- 实现背包场景BackPackScene，按B进入背包后可以鼠标选择工具

### 2024/12/12

- 成功导入新tmx地图

- 解决地图场景写在Player类的问题

- 实现地图切换器MapScene，按M可以切换地图
### 2024/12/13
- 砍树功能尝试
### 2024/12/16
- 完全实现砍树功能
- 三张地图的封装
### 2024/12/17
- 家畜Animal类，NPC类实现
### 2024/12/20
- 实现设置菜单
### 2024/12/21
- 修复碰撞检测的bug，完全实现碰撞检测

# 免责声明
The code and materials contained in this repository are intended for personal learning and research purposes only and may not be used for any commercial purposes. Other users who download or refer to the content of this repository must strictly adhere to the **principles of academic integrity** and must not use these materials for any form of homework submission or other actions that may violate academic honesty. I am not responsible for any direct or indirect consequences arising from the improper use of the contents of this repository. Please ensure that your actions comply with the regulations of your school or institution, as well as applicable laws and regulations, before using this content. If you have any questions, please contact me via [email](mailto:2353127@tongji.edu.cn).

本仓库包含的代码和资料仅用于个人学习和研究目的，不得用于任何商业用途。请其他用户在下载或参考本仓库内容时，严格遵守**学术诚信原则**，不得将这些资料用于任何形式的作业提交或其他可能违反学术诚信的行为。本人对因不恰当使用仓库内容导致的任何直接或间接后果不承担责任。请在使用前务必确保您的行为符合所在学校或机构的规定，以及适用的法律法规。如有任何问题，请通过[电子邮件](mailto:2353127@tongji.edu.cn)与我联系。

# 更新日期

2024.12.22