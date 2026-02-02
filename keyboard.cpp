/**********************************************************
 * Qt6 Virtual Keyboard Widget Implementation
 * 支持拼音输入中文
 **********************************************************/

#include "Keyboard.h"
#include <QApplication>
#include <QDebug>
#include <QRegularExpression>
#include <QResizeEvent>

// ==================== ChineseWidget 实现 ====================

ChineseWidget::ChineseWidget(QWidget *parent)
    : QListWidget(parent)
{
    setFocusPolicy(Qt::NoFocus);
    setViewMode(QListView::ListMode);
    setFlow(QListView::LeftToRight);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollMode(QListWidget::ScrollPerPixel);

    setStyleSheet(
        "QListWidget {"
        "   outline: none;"
        "   border: 1px solid #ddd;"
        "   background-color: white;"
        "   color: black;"
        "}"
        "QListWidget::Item {"
        "   padding: 5px 10px;"
        "   border-radius: 3px;"
        "}"
        "QListWidget::Item:hover {"
        "   background: #4395ff;"
        "   color: white;"
        "}"
        "QListWidget::item:selected {"
        "   background: #4395ff;"
        "   color: white;"
        "}"
        );

    connect(this, &QListWidget::itemClicked, this, &ChineseWidget::onItemClicked);

    loadPinyinDict();
}

void ChineseWidget::loadPinyinDict()
{
    // 简化的拼音词典 - 常用汉字
    // 格式: 拼音 -> 汉字列表
    m_pinyinDict["a"] = {"啊", "阿", "呵", "腌"};
    m_pinyinDict["ai"] = {"爱", "哀", "挨", "碍", "癌", "矮", "艾"};
    m_pinyinDict["an"] = {"安", "按", "岸", "暗", "案", "鞍", "俺"};
    m_pinyinDict["ang"] = {"昂", "肮", "盎"};

    m_pinyinDict["ba"] = {"把", "爸", "吧", "八", "巴", "拔", "跋", "霸", "罢", "坝"};
    m_pinyinDict["bai"] = {"白", "百", "摆", "败", "拜", "柏"};
    m_pinyinDict["ban"] = {"办", "半", "班", "板", "版", "伴", "扮", "拌", "颁"};
    m_pinyinDict["bang"] = {"帮", "邦", "棒", "磅", "榜", "绑", "膀", "镑"};
    m_pinyinDict["bao"] = {"报", "保", "包", "薄", "抱", "暴", "爆", "饱", "宝", "堡"};
    m_pinyinDict["bei"] = {"被", "北", "备", "背", "倍", "杯", "悲", "碑", "卑"};
    m_pinyinDict["ben"] = {"本", "奔", "笨", "苯"};
    m_pinyinDict["beng"] = {"蹦", "崩", "绷", "泵"};
    m_pinyinDict["bi"] = {"比", "笔", "必", "闭", "彼", "逼", "毕", "鼻", "碧", "壁"};
    m_pinyinDict["bian"] = {"变", "边", "便", "遍", "编", "辩", "辨", "鞭", "贬"};
    m_pinyinDict["biao"] = {"表", "标", "彪", "膘", "裱", "镖"};
    m_pinyinDict["bie"] = {"别", "憋", "瘪"};
    m_pinyinDict["bin"] = {"宾", "滨", "彬", "濒", "摈"};
    m_pinyinDict["bing"] = {"并", "病", "兵", "冰", "饼", "屏", "炳"};
    m_pinyinDict["bo"] = {"不", "播", "波", "博", "伯", "玻", "剥", "薄", "驳", "泊"};
    m_pinyinDict["bu"] = {"不", "步", "部", "布", "补", "捕", "卜", "哺"};

    m_pinyinDict["ca"] = {"擦", "嚓"};
    m_pinyinDict["cai"] = {"才", "菜", "材", "财", "采", "彩", "睬", "裁"};
    m_pinyinDict["can"] = {"参", "残", "惭", "灿", "餐", "蚕"};
    m_pinyinDict["cang"] = {"藏", "仓", "苍", "沧"};
    m_pinyinDict["cao"] = {"草", "操", "曹", "槽", "糙"};
    m_pinyinDict["ce"] = {"策", "测", "侧", "厕", "册"};
    m_pinyinDict["ceng"] = {"层", "曾", "蹭"};
    m_pinyinDict["cha"] = {"查", "茶", "差", "插", "察", "叉", "刹", "岔"};
    m_pinyinDict["chai"] = {"拆", "柴", "豺"};
    m_pinyinDict["chan"] = {"产", "单", "缠", "掺", "蝉", "馋", "颤", "铲"};
    m_pinyinDict["chang"] = {"长", "常", "场", "唱", "厂", "畅", "昌", "尝", "偿", "肠"};
    m_pinyinDict["chao"] = {"超", "朝", "潮", "吵", "炒", "抄", "钞", "巢", "嘲"};
    m_pinyinDict["che"] = {"车", "彻", "撤", "扯"};
    m_pinyinDict["chen"] = {"沉", "陈", "晨", "臣", "尘", "衬", "趁", "称"};
    m_pinyinDict["cheng"] = {"成", "城", "程", "承", "称", "诚", "乘", "盛", "呈", "撑"};
    m_pinyinDict["chi"] = {"吃", "持", "尺", "赤", "迟", "齿", "耻", "翅", "斥", "炽"};
    m_pinyinDict["chong"] = {"重", "冲", "充", "虫", "崇", "宠", "冲"};
    m_pinyinDict["chou"] = {"抽", "愁", "臭", "仇", "筹", "绸", "稠", "丑"};
    m_pinyinDict["chu"] = {"出", "处", "初", "除", "础", "储", "楚", "触", "畜", "厨"};
    m_pinyinDict["chuan"] = {"传", "川", "船", "穿", "串", "喘"};
    m_pinyinDict["chuang"] = {"创", "窗", "床", "闯", "疮"};
    m_pinyinDict["chui"] = {"吹", "垂", "锤", "炊", "捶"};
    m_pinyinDict["chun"] = {"春", "纯", "唇", "醇", "蠢"};
    m_pinyinDict["chuo"] = {"戳", "绰"};
    m_pinyinDict["ci"] = {"此", "次", "词", "辞", "刺", "赐", "磁", "瓷", "雌"};
    m_pinyinDict["cong"] = {"从", "聪", "葱", "丛", "匆"};
    m_pinyinDict["cou"] = {"凑", "辏"};
    m_pinyinDict["cu"] = {"粗", "促", "醋", "簇", "蔟"};
    m_pinyinDict["cuan"] = {"窜", "篡", "蹿"};
    m_pinyinDict["cui"] = {"催", "脆", "翠", "摧", "璨", "悴"};
    m_pinyinDict["cun"] = {"存", "村", "寸"};
    m_pinyinDict["cuo"] = {"错", "措", "挫", "搓", "磋", "撮"};

    m_pinyinDict["da"] = {"大", "打", "答", "达", "搭"};
    m_pinyinDict["dai"] = {"带", "代", "待", "大", "呆", "贷", "戴", "袋"};
    m_pinyinDict["dan"] = {"但", "单", "担", "弹", "蛋", "淡", "胆", "旦", "氮"};
    m_pinyinDict["dang"] = {"当", "党", "档", "挡", "荡", "宕"};
    m_pinyinDict["dao"] = {"到", "道", "导", "倒", "刀", "岛", "盗", "悼", "稻"};
    m_pinyinDict["de"] = {"的", "得", "地", "德"};
    m_pinyinDict["deng"] = {"等", "灯", "登", "邓", "蹬", "瞪", "凳"};
    m_pinyinDict["di"] = {"地", "第", "底", "低", "敌", "滴", "迪", "的", "弟", "帝"};
    m_pinyinDict["dian"] = {"点", "店", "电", "典", "殿", "碘", "淀", "垫", "惦"};
    m_pinyinDict["diao"] = {"掉", "调", "吊", "钓", "刁", "雕", "凋"};
    m_pinyinDict["die"] = {"跌", "爹", "碟", "蝶", "迭", "谍", "叠"};
    m_pinyinDict["ding"] = {"定", "顶", "订", "钉", "丁", "盯", "叮", "鼎"};
    m_pinyinDict["dong"] = {"东", "动", "懂", "冬", "董", "洞", "冻", "栋"};
    m_pinyinDict["dou"] = {"都", "斗", "豆", "抖", "陡", "逗", "痘"};
    m_pinyinDict["du"] = {"读", "度", "独", "都", "毒", "堵", "赌", "杜", "肚"};
    m_pinyinDict["duan"] = {"段", "短", "断", "端", "锻", "缎", "煅"};
    m_pinyinDict["dui"] = {"对", "队", "堆", "兑", "敦", "碓"};
    m_pinyinDict["dun"] = {"顿", "吨", "蹲", "盾", "敦", "钝", "墩", "囤"};
    m_pinyinDict["duo"] = {"多", "夺", "朵", "躲", "duo", "垛", "堕", "惰"};

    m_pinyinDict["e"] = {"而", "儿", "额", "恶", "饿", "鹅", "蛾", "俄", "扼"};
    m_pinyinDict["en"] = {"恩"};
    m_pinyinDict["er"] = {"而", "儿", "二", "耳", "尔", "饵", "洱"};

    m_pinyinDict["fa"] = {"发", "法", "罚", "乏", "伐", "筏", "阀"};
    m_pinyinDict["fan"] = {"反", "饭", "犯", "范", "返", "翻", "凡", "烦", "繁", "泛"};
    m_pinyinDict["fang"] = {"方", "放", "房", "防", "仿", "访", "纺", "芳"};
    m_pinyinDict["fei"] = {"非", "飞", "费", "肥", "废", "沸", "肺", "菲", "啡"};
    m_pinyinDict["fen"] = {"分", "份", "纷", "粉", "fen", "奋", "愤", "坟", "焚"};
    m_pinyinDict["feng"] = {"风", "丰", "封", "疯", "峰", "锋", "蜂", "逢", "缝", "凤"};
    m_pinyinDict["fo"] = {"佛"};
    m_pinyinDict["fou"] = {"否"};
    m_pinyinDict["fu"] = {"服", "福", "父", "副", "复", "负", "富", "妇", "府", "符"};

    m_pinyinDict["ga"] = {"尴", "嘎", "噶"};
    m_pinyinDict["gai"] = {"该", "改", "盖", "概", "钙", "溉", "丐"};
    m_pinyinDict["gan"] = {"干", "感", "敢", "赶", "刚", "甘", "肝", "杆", "柑"};
    m_pinyinDict["gang"] = {"刚", "钢", "岗", "港", "杠", "纲", "缸"};
    m_pinyinDict["gao"] = {"高", "告", "搞", "稿", "膏", "糕", "镐", "睾"};
    m_pinyinDict["ge"] = {"个", "各", "哥", "歌", "格", "隔", "割", "革", "葛"};
    m_pinyinDict["gei"] = {"给"};
    m_pinyinDict["gen"] = {"根", "跟", "艮"};
    m_pinyinDict["geng"] = {"更", "耕", "颈", "梗", "埂", "耿", "哽"};
    m_pinyinDict["gong"] = {"工", "公", "共", "功", "供", "宫", "恭", "贡", "躬", "弓"};
    m_pinyinDict["gou"] = {"够", "构", "狗", "购", "沟", "勾", "钩", "gouу"};
    m_pinyinDict["gu"] = {"古", "故", "顾", "固", "骨", "谷", "股", "鼓", "雇", "姑"};
    m_pinyinDict["gua"] = {"挂", "刮", "瓜", "寡", "guà", "卦", "褂"};
    m_pinyinDict["guai"] = {"怪", "拐", "乖"};
    m_pinyinDict["guan"] = {"关", "管", "观", "官", "馆", "冠", "贯", "惯", "灌"};
    m_pinyinDict["guang"] = {"光", "广", "逛"};
    m_pinyinDict["gui"] = {"规", "贵", "跪", "鬼", "柜", "归", "桂", "硅", "轨"};
    m_pinyinDict["gun"] = {"滚", "棍", "辊"};
    m_pinyinDict["guo"] = {"国", "过", "果", "锅", "裹", "郭"};

    m_pinyinDict["ha"] = {"哈"};
    m_pinyinDict["hai"] = {"还", "海", "害", "孩", "骸", "骇"};
    m_pinyinDict["han"] = {"和", "汉", "含", "寒", "喊", "汗", "韩", "旱", "焊"};
    m_pinyinDict["hang"] = {"行", "航", "杭", "巷", "夯"};
    m_pinyinDict["hao"] = {"好", "号", "浩", "毫", "豪", "耗", "郝", "嚎"};
    m_pinyinDict["he"] = {"和", "何", "合", "河", "核", "喝", "贺", "荷", "赫"};
    m_pinyinDict["hei"] = {"黑", "嘿"};
    m_pinyinDict["hen"] = {"很", "恨", "痕", "狠"};
    m_pinyinDict["heng"] = {"横", "恒", "衡", "哼"};
    m_pinyinDict["hong"] = {"红", "洪", "宏", "虹", "鸿", "哄", "弘", "轰"};
    m_pinyinDict["hou"] = {"后", "候", "厚", "侯", "喉", "吼"};
    m_pinyinDict["hu"] = {"和", "户", "护", "互", "呼", "胡", "湖", "壶", "糊", "虎"};
    m_pinyinDict["hua"] = {"化", "花", "华", "话", "画", "滑", "划", "哗"};
    m_pinyinDict["huai"] = {"怀", "坏", "淮", "槐"};
    m_pinyinDict["huan"] = {"换", "还", "环", "欢", "缓", "幻", "唤", "患", "焕"};
    m_pinyinDict["huang"] = {"黄", "皇", "荒", "慌", "煌", "晃", "幌", "恍", "谎"};
    m_pinyinDict["hui"] = {"会", "回", "汇", "惠", "灰", "辉", "挥", "毁", "慧"};
    m_pinyinDict["hun"] = {"婚", "混", "昏", "魂", "浑", "荤"};
    m_pinyinDict["huo"] = {"或", "活", "火", "获", "货", "祸", "惑", "霍"};

    // 添加更多常用字...
    m_pinyinDict["ji"] = {"及", "机", "几", "己", "技", "际", "记", "集", "极", "级"};
    m_pinyinDict["jia"] = {"家", "加", "价", "假", "甲", "嘉", "佳", "架", "驾", "稼"};
    m_pinyinDict["jian"] = {"见", "间", "建", "件", "简", "检", "坚", "减", "监", "健"};
    m_pinyinDict["jiang"] = {"将", "江", "讲", "降", "jiang", "奖", "蒋", "僵", "姜", "浆"};
    m_pinyinDict["jiao"] = {"教", "叫", "交", "较", "角", "脚", "觉", "校", "焦", "胶"};
    m_pinyinDict["jie"] = {"接", "解", "结", "节", "界", "姐", "街", "借", "介", "届"};
    m_pinyinDict["jin"] = {"进", "今", "金", "近", "仅", "紧", "尽", "劲", "禁", "斤"};
    m_pinyinDict["jing"] = {"经", "精", "京", "景", "警", "静", "竞", "境", "镜", "惊"};
    m_pinyinDict["jiong"] = {"窘", "炯", "迥"};
    m_pinyinDict["jiu"] = {"就", "九", "久", "旧", "究", "救", "酒", "舅", "纠", "揪"};
    m_pinyinDict["ju"] = {"具", "据", "巨", "举", "局", "句", "拒", "聚", "距", "俱"};
    m_pinyinDict["juan"] = {"卷", "圈", "捐", "juan", "倦", "眷", "绢", "隽"};
    m_pinyinDict["jue"] = {"觉", "决", "绝", "掘", "诀", "抉", "倔", "爵", "嚼"};
    m_pinyinDict["jun"] = {"军", "君", "均", "菌", "俊", "郡", "峻", "竣"};

    m_pinyinDict["ka"] = {"卡", "咖", "喀"};
    m_pinyinDict["kai"] = {"开", "凯", "慨", "楷", "揩", "铠"};
    m_pinyinDict["kan"] = {"看", "刊", "堪", "勘", "坎", "砍", "侃"};
    m_pinyinDict["kang"] = {"康", "抗", "扛", "慷", "糠", "炕"};
    m_pinyinDict["kao"] = {"考", "靠", "烤", "拷"};
    m_pinyinDict["ke"] = {"可", "课", "科", "克", "刻", "客", "渴", "壳", "咳", "颗"};
    m_pinyinDict["ken"] = {"肯", "垦", "恳", "啃"};
    m_pinyinDict["keng"] = {"坑", "铿"};
    m_pinyinDict["kong"] = {"空", "孔", "控", "恐"};
    m_pinyinDict["kou"] = {"口", "扣", "寇", "叩", "抠"};
    m_pinyinDict["ku"] = {"苦", "库", "哭", "酷", "裤", "窟", "骷"};
    m_pinyinDict["kua"] = {"夸", "跨", "垮", "挎", "胯"};
    m_pinyinDict["kuai"] = {"快", "块", "筷", "会", "蒯", "侩"};
    m_pinyinDict["kuan"] = {"宽", "款"};
    m_pinyinDict["kuang"] = {"况", "矿", "框", "狂", "旷", "匡", "筐", "眶"};
    m_pinyinDict["kui"] = {"亏", "愧", "奎", "魁", "傀", "馈", "窥", "溃"};
    m_pinyinDict["kun"] = {"困", "捆", "昆", "坤", "kun", "琨", "髡"};
    m_pinyinDict["kuo"] = {"扩", "括", "阔", "廓"};

    m_pinyinDict["la"] = {"啦", "拉", "辣", "腊", "蜡", "垃", "喇"};
    m_pinyinDict["lai"] = {"来", "莱", "赖", "睐", "濞"};
    m_pinyinDict["lan"] = {"蓝", "览", "懒", "栏", "烂", "兰", "拦", "篮", "澜", "揽"};
    m_pinyinDict["lang"] = {"浪", "郎", "狼", "朗", "廊", "琅", "榔"};
    m_pinyinDict["lao"] = {"老", "劳", "牢", "捞", "涝", "烙", "姥"};
    m_pinyinDict["le"] = {"了", "乐", "勒"};
    m_pinyinDict["lei"] = {"累", "类", "泪", "雷", "垒", "擂", "蕾", "镭"};
    m_pinyinDict["leng"] = {"冷", "愣", "棱"};
    m_pinyinDict["li"] = {"里", "理", "力", "利", "立", "离", "历", "李", "例", "礼"};
    m_pinyinDict["lian"] = {"连", "联", "练", "脸", "恋", "廉", "莲", "链", "帘", "炼"};
    m_pinyinDict["liang"] = {"两", "量", "亮", "良", "辆", "粮", "凉", "梁", "粱", "晾"};
    m_pinyinDict["liao"] = {"了", "料", "疗", "辽", "聊", "廖", "撩", "寥", "嘹"};
    m_pinyinDict["lie"] = {"列", "烈", "裂", "猎", "劣", "冽", "咧"};
    m_pinyinDict["lin"] = {"林", "临", "邻", "淋", "琳", "霖", "磷", "鳞", "麟"};
    m_pinyinDict["ling"] = {"零", "领", "令", "灵", "另", "岭", "玲", "凌", "铃", "陵"};
    m_pinyinDict["liu"] = {"流", "六", "留", "刘", "柳", "溜", "榴", "瘤"};
    m_pinyinDict["long"] = {"龙", "隆", "笼", "聋", "拢", "垄", "陇", "垅"};
    m_pinyinDict["lou"] = {"楼", "漏", "陋", "搂", "篓", "镂"};
    m_pinyinDict["lu"] = {"路", "录", "露", "鲁", "陆", "卢", "炉", "绿", "鹿", "芦"};
    m_pinyinDict["lv"] = {"绿", "率", "律", "虑", "旅", "吕", "铝", "履", "滤", "氯"};
    m_pinyinDict["luan"] = {"乱", "卵", "孪", "峦", "滦", "挛"};
    m_pinyinDict["lue"] = {"略", "掠", "lue"};
    m_pinyinDict["lun"] = {"论", "轮", "伦", "沦", "抡"};
    m_pinyinDict["luo"] = {"落", "罗", "洛", "络", "骆", "锣", "螺", "逻", "裸"};

    m_pinyinDict["ma"] = {"吗", "妈", "马", "骂", "吗", "麻", "嘛", "蚂", "码"};
    m_pinyinDict["mai"] = {"买", "卖", "迈", "麦", "脉", "埋"};
    m_pinyinDict["man"] = {"满", "慢", "漫", "曼", "蛮", "瞒", "馒", "蔓"};
    m_pinyinDict["mang"] = {"忙", "芒", "盲", "茫", "莽", "蟒"};
    m_pinyinDict["mao"] = {"没", "毛", "茂", "冒", "帽", "貌", "贸", "矛", "茅", "锚"};
    m_pinyinDict["me"] = {"么"};
    m_pinyinDict["mei"] = {"没", "每", "美", "妹", "媒", "煤", "梅", "昧", "魅", "枚"};
    m_pinyinDict["men"] = {"们", "门", "闷", "们"};
    m_pinyinDict["meng"] = {"梦", "盟", "猛", "蒙", "萌", "朦", "檬", "孟"};
    m_pinyinDict["mi"] = {"米", "密", "迷", "秘", "蜜", "谜", "觅", "泌", "眯", "靡"};
    m_pinyinDict["mian"] = {"面", "免", "棉", "眠", "绵", "勉", "缅", "腼", "渑"};
    m_pinyinDict["miao"] = {"秒", "妙", "苗", "描", "渺", "缪", "庙", "瞄", "藐"};
    m_pinyinDict["mie"] = {"灭", "蔑", "篾"};
    m_pinyinDict["min"] = {"民", "敏", "闽", "抿", "皿", "泯", "悯", "珉"};
    m_pinyinDict["ming"] = {"明", "名", "命", "鸣", "铭", "冥", "茗", "溟"};
    m_pinyinDict["miu"] = {"谬"};
    m_pinyinDict["mo"] = {"模", "么", "摸", "莫", "磨", "墨", "末", "漠", "默", "膜"};
    m_pinyinDict["mou"] = {"某", "谋", "牟", "眸", "哞"};
    m_pinyinDict["mu"] = {"目", "木", "母", "牧", "幕", "墓", "慕", "睦", "穆", "姆"};

    m_pinyinDict["na"] = {"那", "拿", "哪", "na", "纳", "钠", "娜", "捺"};
    m_pinyinDict["nai"] = {"奶", "耐", "奈", "乃", "氖", "萘"};
    m_pinyinDict["nan"] = {"南", "难", "男", "喃", "楠", "囡"};
    m_pinyinDict["nang"] = {"囊", "馕"};
    m_pinyinDict["nao"] = {"脑", "闹", "恼", "挠", "瑙", "淖"};
    m_pinyinDict["ne"] = {"呢"};
    m_pinyinDict["nei"] = {"内"};
    m_pinyinDict["nen"] = {"嫩", "恁"};
    m_pinyinDict["neng"] = {"能"};
    m_pinyinDict["ni"] = {"你", "尼", "泥", "拟", "逆", "腻", "妮", "匿", "倪", "霓"};
    m_pinyinDict["nian"] = {"年", "念", "娘", "捻", "碾", "辗", "黏", "蔫", "拈"};
    m_pinyinDict["niang"] = {"娘", "酿"};
    m_pinyinDict["niao"] = {"鸟", "尿", "袅", "茑"};
    m_pinyinDict["nie"] = {"捏", "聂", "啮", "镊", "孽", "蹑", "臬"};
    m_pinyinDict["nin"] = {"您", "宁"};
    m_pinyinDict["ning"] = {"宁", "凝", "拧", "泞", "柠", "狞", "咛"};
    m_pinyinDict["niu"] = {"牛", "扭", "纽", "钮", "拗"};
    m_pinyinDict["nong"] = {"农", "浓", "弄", "侬", "脓"};
    m_pinyinDict["nu"] = {"奴", "努", "怒", "弩", "驽"};
    m_pinyinDict["nv"] = {"女", "恼"};
    m_pinyinDict["nuan"] = {"暖"};
    m_pinyinDict["nue"] = {"虐", "疟"};
    m_pinyinDict["nuo"] = {"诺", "懦", "糯", "挪", "傩", "喏"};

    // ... 可以继续添加更多拼音

    // 添加常用词组的拼音映射（简化版）
    m_pinyinDict["nihao"] = {"你好"};
    m_pinyinDict["zhongguo"] = {"中国"};
    m_pinyinDict["xièxiè"] = {"谢谢"};
    m_pinyinDict["zaijian"] = {"再见"};
}

void ChineseWidget::setPinyin(const QString &pinyin)
{
    clear();

    if (pinyin.isEmpty()) {
        return;
    }

    // 显示拼音本身
    addCandidate(pinyin);

    // 查找匹配的汉字
    if (m_pinyinDict.contains(pinyin)) {
        QStringList candidates = m_pinyinDict[pinyin];
        for (const QString &candidate : candidates) {
            addCandidate(candidate);
        }
    }
}

void ChineseWidget::clear()
{
    QListWidget::clear();
}

void ChineseWidget::addCandidate(const QString &text)
{
    QListWidgetItem *item = new QListWidgetItem(text, this);
    QFont font;
    font.setPointSize(16);
    font.setBold(true);
    item->setFont(font);
    item->setTextAlignment(Qt::AlignCenter);

    // 根据文本长度调整宽度
    int width = text.length() * font.pointSize() * 2;
    item->setSizeHint(QSize(width, 45));

    addItem(item);
}

void ChineseWidget::onItemClicked(QListWidgetItem *item)
{
    emit candidateSelected(item->text());
}

// ==================== KeyboardButton 实现 ====================

KeyboardButton::KeyboardButton(const QString &text, int keyCode, QWidget *parent)
    : QPushButton(text, parent), m_keyCode(keyCode)
{
    setFocusPolicy(Qt::NoFocus);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setMinimumSize(50, 50);

    connect(this, &QPushButton::clicked, this, [this]() {
        emit keyPressed(m_keyCode, this->text());
    });
}

void KeyboardButton::setNormalImage(const QString &imagePath)
{
    m_normalImage = imagePath;
    updateStyleSheet();
}

void KeyboardButton::setPressedImage(const QString &imagePath)
{
    m_pressedImage = imagePath;
}

void KeyboardButton::setDisplayText(const QString &text)
{
    setText(text);
}

void KeyboardButton::mousePressEvent(QMouseEvent *event)
{
    if (!m_pressedImage.isEmpty()) {
        setStyleSheet(QString("QPushButton { border-image: url(%1); }").arg(m_pressedImage));
    }
    QPushButton::mousePressEvent(event);
}

void KeyboardButton::mouseReleaseEvent(QMouseEvent *event)
{
    updateStyleSheet();
    QPushButton::mouseReleaseEvent(event);
}

void KeyboardButton::updateStyleSheet()
{
    if (!m_normalImage.isEmpty()) {
        setStyleSheet(QString("QPushButton { border-image: url(%1); }").arg(m_normalImage));
    }
}

// ==================== Keyboard 实现 ====================

Keyboard::Keyboard(QWidget *parent)
    : QWidget(parent)
    , m_targetWidget(nullptr)
    , m_keyboardMode(LowerCase)
    , m_inputMode(English)
    , m_capsLock(false)
    , m_buttonSize(60, 50)
    , m_inputModeButton(nullptr)
{
    setupUI();
}

void Keyboard::setupUI()
{
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setSpacing(5);
    m_mainLayout->setContentsMargins(5, 5, 5, 5);

    // 创建中文候选词控件
    m_chineseWidget = new ChineseWidget(this);
    m_chineseWidget->setFixedHeight(50);  // 使用固定高度而不是最大高度
    m_chineseWidget->hide();  // 默认隐藏
    connect(m_chineseWidget, &ChineseWidget::candidateSelected,
            this, &Keyboard::onCandidateSelected);
    m_mainLayout->addWidget(m_chineseWidget, 0);  // stretch factor 0 - 不占用额外空间

    // 创建字母键盘
    m_letterWidget = new QWidget(this);
    m_letterLayout = new QGridLayout(m_letterWidget);
    m_letterLayout->setSpacing(5);
    m_letterLayout->setContentsMargins(0, 0, 0, 0);
    createLetterKeyboard();

    // 创建数字键盘
    m_numberWidget = new QWidget(this);
    m_numberLayout = new QGridLayout(m_numberWidget);
    m_numberLayout->setSpacing(5);
    m_numberLayout->setContentsMargins(0, 0, 0, 0);
    createNumberKeyboard();

    m_mainLayout->addWidget(m_letterWidget, 1);  // stretch factor 1 - 占用主要空间
    m_mainLayout->addWidget(m_numberWidget, 1);  // stretch factor 1 - 占用主要空间

    // 默认显示字母键盘
    m_numberWidget->hide();

    // 设置默认输入模式（英文）
    setInputMode(English);

    setLayout(m_mainLayout);
}

void Keyboard::createLetterKeyboard()
{
    // 第一行: 数字键
    QStringList row1 = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "0"};
    for (int i = 0; i < row1.size(); ++i) {
        addButtonToLayout(m_letterLayout, row1[i], Qt::Key_0 + i, 0, i);
    }

    // 第二行: QWERTYUIOP (从第0列开始，模拟物理键盘布局)
    QStringList row2 = {"Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P"};
    for (int i = 0; i < row2.size(); ++i) {
        QString lower = row2[i].toLower();
        KeyboardButton *btn = createButton(lower, Qt::Key_A + (row2[i].toLatin1()[0] - 'A'));
        m_letterButtons[lower] = btn;
        m_letterLayout->addWidget(btn, 1, i);
        connect(btn, &KeyboardButton::keyPressed, this, &Keyboard::onKeyButtonPressed);
    }

    // 第三行: ASDFGHJKL (从第0.5列开始，实现错位效果)
    // 使用列跨度和空白空间来实现错位
    QStringList row3 = {"A", "S", "D", "F", "G", "H", "J", "K", "L"};
    // 添加一个半列宽的空白占位符
    QWidget *spacer1 = new QWidget(this);
    spacer1->setMaximumWidth(HALF_BUTTON_WIDTH);  // 半个按钮宽度
    m_letterLayout->addWidget(spacer1, 2, 0, 1, 1);
    
    for (int i = 0; i < row3.size(); ++i) {
        QString lower = row3[i].toLower();
        KeyboardButton *btn = createButton(lower, Qt::Key_A + (row3[i].toLatin1()[0] - 'A'));
        m_letterButtons[lower] = btn;
        m_letterLayout->addWidget(btn, 2, i + 1);  // 从第1列开始，实现右移效果
        connect(btn, &KeyboardButton::keyPressed, this, &Keyboard::onKeyButtonPressed);
    }

    // 第四行: ZXCVBNM + Backspace (从第1列开始，实现更大的错位)
    QStringList row4 = {"Z", "X", "C", "V", "B", "N", "M"};
    // 添加一个一列宽的空白占位符
    QWidget *spacer2 = new QWidget(this);
    spacer2->setMaximumWidth(FULL_BUTTON_WIDTH);  // 一个按钮宽度
    m_letterLayout->addWidget(spacer2, 3, 0, 1, 1);
    
    for (int i = 0; i < row4.size(); ++i) {
        QString lower = row4[i].toLower();
        KeyboardButton *btn = createButton(lower, Qt::Key_A + (row4[i].toLatin1()[0] - 'A'));
        m_letterButtons[lower] = btn;
        m_letterLayout->addWidget(btn, 3, i + 1);  // 从第1列开始
        connect(btn, &KeyboardButton::keyPressed, this, &Keyboard::onKeyButtonPressed);
    }

    // Backspace 按钮
    KeyboardButton *backspaceBtn = createButton("←", Qt::Key_Backspace);
    m_letterLayout->addWidget(backspaceBtn, 3, 8, 1, 2);
    connect(backspaceBtn, &KeyboardButton::keyPressed, this, &Keyboard::onBackspacePressed);

    // 第五行: 数字切换, Caps, 空格, 中/英切换, Enter
    KeyboardButton *modeBtn = createButton("123", Qt::Key_Tab);
    m_letterLayout->addWidget(modeBtn, 4, 0, 1, 1);
    connect(modeBtn, &KeyboardButton::keyPressed, this, &Keyboard::onModeChanged);

    KeyboardButton *capsBtn = createButton("Caps", Qt::Key_CapsLock);
    m_letterLayout->addWidget(capsBtn, 4, 1, 1, 2);
    connect(capsBtn, &KeyboardButton::keyPressed, this, &Keyboard::onCapsLockToggled);

    KeyboardButton *spaceBtn = createButton("Space", Qt::Key_Space);
    m_letterLayout->addWidget(spaceBtn, 4, 3, 1, 3);
    connect(spaceBtn, &KeyboardButton::keyPressed, this, &Keyboard::onKeyButtonPressed);

    // 中英文切换按钮 - 显示当前输入模式
    m_inputModeButton = createButton("中/英", Qt::Key_Mode_switch);
    m_letterLayout->addWidget(m_inputModeButton, 4, 6, 1, 2);
    connect(m_inputModeButton, &KeyboardButton::keyPressed, this, &Keyboard::onInputModeChanged);

    KeyboardButton *enterBtn = createButton("↵", Qt::Key_Return);
    m_letterLayout->addWidget(enterBtn, 4, 8, 1, 2);
    connect(enterBtn, &KeyboardButton::keyPressed, this, &Keyboard::onEnterPressed);
}

void Keyboard::createNumberKeyboard()
{
    // 数字键盘布局
    QStringList numbers = {"7", "8", "9", "4", "5", "6", "1", "2", "3", "0"};
    int row = 0, col = 0;

    for (int i = 0; i < numbers.size(); ++i) {
        row = i / 3;
        col = i % 3;
        addButtonToLayout(m_numberLayout, numbers[i], Qt::Key_0 + numbers[i].toInt(), row, col);
    }

    // 0 按钮占两列
    m_numberLayout->itemAtPosition(3, 0)->widget()->deleteLater();
    addButtonToLayout(m_numberLayout, "0", Qt::Key_0, 3, 0, 1, 2);

    // 符号键
    QStringList symbols = {"+", "-", "*", "/", ".", ",", "!", "?"};
    for (int i = 0; i < symbols.size() && i < 4; ++i) {
        addButtonToLayout(m_numberLayout, symbols[i], Qt::Key_Plus + i, i, 3);
    }

    // 小数点和逗号
    addButtonToLayout(m_numberLayout, ".", Qt::Key_Period, 3, 2);
    addButtonToLayout(m_numberLayout, ",", Qt::Key_Comma, 2, 4);

    // Backspace
    KeyboardButton *backspaceBtn = createButton("←", Qt::Key_Backspace);
    m_numberLayout->addWidget(backspaceBtn, 0, 4, 2, 1);
    connect(backspaceBtn, &KeyboardButton::keyPressed, this, &Keyboard::onBackspacePressed);

    // ABC 切换按钮
    KeyboardButton *modeBtn = createButton("ABC", Qt::Key_Tab);
    m_numberLayout->addWidget(modeBtn, 3, 4, 1, 1);
    connect(modeBtn, &KeyboardButton::keyPressed, this, &Keyboard::onModeChanged);

    // Enter 按钮
    KeyboardButton *enterBtn = createButton("↵", Qt::Key_Return);
    m_numberLayout->addWidget(enterBtn, 3, 3, 1, 1);
    connect(enterBtn, &KeyboardButton::keyPressed, this, &Keyboard::onEnterPressed);
}

KeyboardButton* Keyboard::createButton(const QString &text, int keyCode)
{
    KeyboardButton *button = new KeyboardButton(text, keyCode, this);

    if (!m_buttonStyleSheet.isEmpty()) {
        button->setStyleSheet(m_buttonStyleSheet);
    } else {
        button->setStyleSheet(
            "QPushButton {"
            "   background-color: #4395ff;"
            "   border: 1px solid #3385ef;"
            "   border-radius: 5px;"
            "   color: white;"
            "   font-size: 16px;"
            "   font-weight: bold;"
            "}"
            "QPushButton:pressed {"
            "   background-color: #01ddfd;"
            "}"
            );
    }

    if (!m_normalImage.isEmpty()) {
        button->setNormalImage(m_normalImage);
    }
    if (!m_pressedImage.isEmpty()) {
        button->setPressedImage(m_pressedImage);
    }

    if (!m_buttonSize.isEmpty()) {
        button->setMinimumSize(m_buttonSize);
    }

    return button;
}

void Keyboard::addButtonToLayout(QGridLayout *layout, const QString &text, int keyCode,
                                 int row, int col, int rowSpan, int colSpan)
{
    KeyboardButton *button = createButton(text, keyCode);
    layout->addWidget(button, row, col, rowSpan, colSpan);
    connect(button, &KeyboardButton::keyPressed, this, &Keyboard::onKeyButtonPressed);
}

void Keyboard::setTargetWidget(QWidget *widget)
{
    m_targetWidget = widget;
}

void Keyboard::setButtonStyleSheet(const QString &styleSheet)
{
    m_buttonStyleSheet = styleSheet;

    QList<KeyboardButton*> buttons = findChildren<KeyboardButton*>();
    for (KeyboardButton *btn : buttons) {
        btn->setStyleSheet(styleSheet);
    }
}

void Keyboard::setButtonSize(const QSize &size)
{
    m_buttonSize = size;

    QList<KeyboardButton*> buttons = findChildren<KeyboardButton*>();
    for (KeyboardButton *btn : buttons) {
        btn->setMinimumSize(size);
    }
}

void Keyboard::setButtonBackgroundImage(const QString &normalImage, const QString &pressedImage)
{
    m_normalImage = normalImage;
    m_pressedImage = pressedImage;

    QList<KeyboardButton*> buttons = findChildren<KeyboardButton*>();
    for (KeyboardButton *btn : buttons) {
        btn->setNormalImage(normalImage);
        if (!pressedImage.isEmpty()) {
            btn->setPressedImage(pressedImage);
        }
    }
}

void Keyboard::setKeyboardMode(KeyboardMode mode)
{
    m_keyboardMode = mode;
    updateKeyboardDisplay();
}

void Keyboard::setInputMode(InputMode mode)
{
    m_inputMode = mode;
    if (m_inputModeButton) {
        // 显示当前输入模式
        if (mode == Chinese) {
            m_inputModeButton->setDisplayText("中文");
            m_inputModeButton->setStyleSheet(
                "QPushButton {"
                "   background-color: #ff6b6b;"  // 红色表示中文模式
                "   border: 1px solid #ff5252;"
                "   border-radius: 5px;"
                "   color: white;"
                "   font-size: 16px;"
                "   font-weight: bold;"
                "}"
                "QPushButton:pressed {"
                "   background-color: #ff4444;"
                "}"
                );
        } else {
            m_inputModeButton->setDisplayText("英文");
            m_inputModeButton->setStyleSheet(
                "QPushButton {"
                "   background-color: #4395ff;"  // 蓝色表示英文模式
                "   border: 1px solid #3385ef;"
                "   border-radius: 5px;"
                "   color: white;"
                "   font-size: 16px;"
                "   font-weight: bold;"
                "}"
                "QPushButton:pressed {"
                "   background-color: #01ddfd;"
                "}"
                );
        }
    }

    // 切换到英文时清空拼音缓冲
    if (mode == English) {
        m_pinyinBuffer.clear();
        m_chineseWidget->clear();
        m_chineseWidget->hide();
    }
}

void Keyboard::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    // 如果没有父控件，则不需要调整大小
    if (!parentWidget()) {
        return;
    }

    // 获取父控件的大小
    QSize parentSize = parentWidget()->size();
    
    // 定义键盘的目标宽高比
    const double targetAspectRatio = KEYBOARD_ASPECT_RATIO;  // 宽度是高度的2.5倍
    
    // 计算父控件的可用空间 (留出一些边距)
    int availableWidth = parentSize.width() - KEYBOARD_MARGIN;
    int availableHeight = parentSize.height() - KEYBOARD_MARGIN;
    
    // 根据宽高比计算键盘的尺寸
    int keyboardWidth = availableWidth;
    int keyboardHeight = static_cast<int>(keyboardWidth / targetAspectRatio);
    
    // 如果高度超出可用空间，则根据高度重新计算
    if (keyboardHeight > availableHeight) {
        keyboardHeight = availableHeight;
        keyboardWidth = static_cast<int>(keyboardHeight * targetAspectRatio);
    }
    
    // 设置键盘的大小
    resize(keyboardWidth, keyboardHeight);
    
    // 居中显示
    int x = (parentSize.width() - keyboardWidth) / 2;
    int y = (parentSize.height() - keyboardHeight) / 2;
    move(x, y);
}

void Keyboard::updateKeyboardDisplay()
{
    if (m_keyboardMode == Number) {
        m_letterWidget->hide();
        m_numberWidget->show();
    } else {
        m_letterWidget->show();
        m_numberWidget->hide();

        // 更新字母大小写
        for (auto it = m_letterButtons.begin(); it != m_letterButtons.end(); ++it) {
            QString text = it.key();
            if (m_keyboardMode == UpperCase || m_capsLock) {
                text = text.toUpper();
            }
            it.value()->setDisplayText(text);
        }
    }
}

void Keyboard::onKeyButtonPressed(int keyCode, const QString &text)
{
    // 中文输入模式
    if (m_inputMode == Chinese && QRegularExpression("^[a-zA-Z]$").match(text).hasMatch()) {
        m_pinyinBuffer += text.toLower();
        m_chineseWidget->setPinyin(m_pinyinBuffer);
        m_chineseWidget->show();
        return;
    }

    // 处理空格键 - 发送实际空格字符而不是 "Space" 文本
    QString actualText = text;
    if (keyCode == Qt::Key_Space) {
        actualText = " ";
    }

    // 英文输入模式或非字母键
    sendKeyEventToTarget(keyCode, actualText);
    emit keyClicked(keyCode, actualText);
}

void Keyboard::onCapsLockToggled()
{
    m_capsLock = !m_capsLock;
    m_keyboardMode = m_capsLock ? UpperCase : LowerCase;
    updateKeyboardDisplay();
}

void Keyboard::onModeChanged()
{
    if (m_keyboardMode == Number) {
        m_keyboardMode = LowerCase;
    } else {
        m_keyboardMode = Number;
    }
    updateKeyboardDisplay();
}

void Keyboard::onInputModeChanged()
{
    if (m_inputMode == English) {
        setInputMode(Chinese);
    } else {
        setInputMode(English);
    }
}

void Keyboard::onBackspacePressed()
{
    // 中文输入模式下，先删除拼音缓冲
    if (m_inputMode == Chinese && !m_pinyinBuffer.isEmpty()) {
        m_pinyinBuffer.chop(1);
        if (m_pinyinBuffer.isEmpty()) {
            m_chineseWidget->clear();
            m_chineseWidget->hide();
        } else {
            m_chineseWidget->setPinyin(m_pinyinBuffer);
        }
        return;
    }

    sendKeyEventToTarget(Qt::Key_Backspace, "");
}

void Keyboard::onEnterPressed()
{
    // 中文输入模式下，如果有拼音缓冲，直接输入拼音
    if (m_inputMode == Chinese && !m_pinyinBuffer.isEmpty()) {
        sendTextToTarget(m_pinyinBuffer);
        m_pinyinBuffer.clear();
        m_chineseWidget->clear();
        m_chineseWidget->hide();
        return;
    }

    sendKeyEventToTarget(Qt::Key_Return, "\n");
}

void Keyboard::onCandidateSelected(const QString &text)
{
    // 输入选中的候选词
    sendTextToTarget(text);

    // 清空拼音缓冲
    m_pinyinBuffer.clear();
    m_chineseWidget->clear();
    m_chineseWidget->hide();
}

void Keyboard::sendKeyEventToTarget(int keyCode, const QString &text)
{
    if (!m_targetWidget) {
        m_targetWidget = QApplication::focusWidget();
    }

    if (!m_targetWidget) {
        return;
    }

    QKeyEvent pressEvent(QEvent::KeyPress, keyCode, Qt::NoModifier, text);
    QKeyEvent releaseEvent(QEvent::KeyRelease, keyCode, Qt::NoModifier, text);

    QApplication::sendEvent(m_targetWidget, &pressEvent);
    QApplication::sendEvent(m_targetWidget, &releaseEvent);
}

void Keyboard::sendTextToTarget(const QString &text)
{
    if (!m_targetWidget) {
        m_targetWidget = QApplication::focusWidget();
    }

    if (!m_targetWidget) {
        return;
    }

    // 逐个字符发送
    for (const QChar &ch : text) {
        QString str(ch);
        QKeyEvent pressEvent(QEvent::KeyPress, 0, Qt::NoModifier, str);
        QKeyEvent releaseEvent(QEvent::KeyRelease, 0, Qt::NoModifier, str);

        QApplication::sendEvent(m_targetWidget, &pressEvent);
        QApplication::sendEvent(m_targetWidget, &releaseEvent);
    }
}
