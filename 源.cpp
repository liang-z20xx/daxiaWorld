#include <graphics.h>
#include <conio.h>
#include <windows.h>
#include <easyx.h>
#include <mmsystem.h>
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <set>
#include <string>
#pragma comment (lib,"winmm.lib")
#pragma comment(lib, "MSIMG32.LIB")

using namespace std;

const int numS = 27;
const int numA = 40;
const int numB = 33;
const int numC = 33;
const int numD = 25;
const int numR = 15;
int total = numS + numA + numB + numC + numD + numR;//结局数量 
int arrS[numS] = { 0 };
int arrA[numA] = { 0 };
int arrB[numB] = { 0 };
int arrC[numC] = { 0 };
int arrD[numD] = { 0 };
int arrR[numR] = { 0 };

int v1 = 0;
int v2 = 0;
int v3 = 0;
int v4 = 0;
int v5 = 0;
int v6 = 0;
int v7 = 0;
int v8 = 0;
int v9 = 0;
int v10 = 0;
int v11 = 0;
int v12 = 0; // 成就变量 
int come = 1; // 决定是否循环的变量 
double score = 0; // 考试分数 
bool isIntoLook2 = false;

ExMessage m;

bool isIntoMenu = false;//是否进入任何一个目录项
bool isIntoOther = false;//for "other",check if enter it

void cls() {
    cleardevice();
}

inline void putimage_alpha(int x, int y, IMAGE* img)
{
    int w = img->getwidth();
    int h = img->getheight();
    BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
    AlphaBlend(GetImageHDC(NULL), x, y, w, h,
        GetImageHDC(img), 0, 0, w, h, bf);
}

void putImageFullScreen(IMAGE* img)
{
    int win_w = getwidth();
    int win_h = getheight();
    int img_w = img->getwidth();
    int img_h = img->getheight();
    int x = (win_w - img_w) / 2;
    int y = (win_h - img_h) / 2;
    putimage(x, y, img_w, img_h, img, 0, 0);
}

void printStory(int textsize, int startX, int startY, const char* text, int maxlong, int color)
{
    settextstyle(textsize, 0, "楷体");
    if (color == 1) {
        settextcolor(BLACK);
    }
    else if (color == 2) {
        settextcolor(RED);
    }

    std::string fullText = text;
    std::vector<std::string> lines;
    std::string currentLine = "";

    // 重新实现“按字符读取”，但是不拆分中文字符
    for (int i = 0; i < fullText.length(); ) {
        // 判断当前字符是不是中文字符的“首字节”
        // 如果字符大于 127（0x7F），说明它是双字节中文的开头
        if ((fullText[i] & 0x80) != 0) {
            // 这个字占 2 个字节（多字节字符集下就是 2 个字节）
            char ch[3] = { fullText[i], fullText[i + 1], '\0' };
            // 把这两个字节当做整体，算宽度
            if (currentLine.length() > 0 && currentLine.length() + 2 > (textsize + 5)) { // 长度计算需要维护
                // 这里为了简化处理，采取的是比较强硬的断句
            }

            // 如果是双字节字符，直接把完整汉字加到行中
            currentLine += fullText[i];
            currentLine += fullText[i + 1];
            i += 2;  // 每次处理两个字节
        }
        else {
            // 单字节字符（比如标点、英文字母）
            currentLine += fullText[i];
            i += 1;
        }

        // 如果当前行已经太长了，就换行（最简单的粗暴判断法）
        if (textwidth(currentLine.c_str()) > maxlong) {
            lines.push_back(currentLine);
            currentLine = "";
        }
    }
    if (!currentLine.empty()) {
        lines.push_back(currentLine);
    }

    // 打印
    int y = startY;
    for (const auto& line : lines) {
        outtextxy(startX, y, line.c_str());
        y += (textsize + 5);
    }
}

void startbac() {
    IMAGE str1;
    loadimage(&str1, "res\\pir\\start1.jpeg");
    putImageFullScreen(&str1);
    IMAGE pia;
    loadimage(&pia, "res\\pir\\pia.png");
    putimage_alpha(1150, 35, &pia);
    settextcolor(RGB(130, 110, 255));
    settextstyle(75, 0, "楷体");
    outtextxy(450, 600, "大虾世界2.0.0");
    settextcolor(RGB(30, 130, 40));
    settextstyle(50, 0, "仿宋");
    outtextxy(100, 720, "由个人创作，不喜勿喷，作者邮箱：@qq.com3631421390");
    outtextxy(101, 720, "由个人创作，不喜勿喷，作者邮箱：@qq.com3631421390");
    settextcolor(RGB(255, 255, 140));
    settextstyle(32, 0, "楷体");
    outtextxy(0, 20, "《大虾世界》，从惊险刺激的总统体验，掌权【大虾帝国】的生死安危");
    outtextxy(0, 50, "到校园生活——以2024届灵宝一高弘毅班为原型，但稍加改编");
    outtextxy(0, 80, "每一次选择，都是一扇新世界的大门——173种结局，等你来看！");
    outtextxy(0, 110, "点击右下方矩形进入游戏");
    IMAGE huclbe;
    loadimage(&huclbe, "res\\pir\\hucl-be.png");
    putimage_alpha(570, 230, &huclbe);
}

void backA() {
    IMAGE str1;
    loadimage(&str1, "res\\pir\\start1.jpeg");
    putImageFullScreen(&str1);
    IMAGE pia;
    loadimage(&pia, "res\\pir\\pia.png");
    putimage_alpha(1150, 35, &pia);
}

struct PatrolEnemy {
    int x, y;
    int targetX, targetY;
    float speed;
};

void initPatrolEnemy(PatrolEnemy& enemy, int startX, int startY, float spd) {
    enemy.x = startX;
    enemy.y = startY;
    enemy.targetX = -1;
    enemy.targetY = -1;
    enemy.speed = spd;
}

void updateAndDrawPatrolEnemy(PatrolEnemy& enemy, IMAGE* img, int screenWidth, int screenHeight) {
    if (enemy.targetX == -1 && enemy.targetY == -1) {
        enemy.targetX = rand() % (screenWidth - 100) + 50;
        enemy.targetY = rand() % (screenHeight - 100) + 50;
    }

    float dx = enemy.targetX - enemy.x;
    float dy = enemy.targetY - enemy.y;
    float distance = sqrt(dx * dx + dy * dy);
    if (distance < 0.1f) distance = 0.1f;
    if (distance > 10.0f) {
        float ratio = enemy.speed / distance;
        enemy.x += (int)(dx * ratio);
        enemy.y += (int)(dy * ratio);
    }
    else {
        enemy.x = enemy.targetX;
        enemy.y = enemy.targetY;
        enemy.targetX = rand() % (screenWidth - 100) + 50;
        enemy.targetY = rand() % (screenHeight - 100) + 50;
    }
    putimage_alpha(enemy.x, enemy.y, img);
}

void game() {
    IMAGE p;
    loadimage(&p, "res\\pir\\p.png");
    IMAGE sq;
    loadimage(&sq, "res\\pir\\sq.png");
    bool isGame = false;
    bool isFail = false;
    int strtime = time(0);
    int gtime = 0;
    int piax = rand() % 1200 + 100;
    int piay = rand() % 600 + 50;
    int sqx = rand() % 1200 + 100;
    int sqy = rand() % 600 + 50;
    int hp = 100;

    // ★ 新增：防连击和边缘流血计时器
    DWORD lastHitTime = 0;       // 记录上次被撞击的时间
    DWORD lastEdgeTime = 0;      // 记录上次边缘扣血的时间
    const int EDGE_COOLDOWN = 1000; // 边缘扣血间隔：1秒

    if (abs(sqx - piax) < 500 || abs(sqy - piay) < 400) {
        sqx = rand() % 1200 + 100;
        sqy = rand() % 600 + 50;
    }
    ExMessage localM;
    PatrolEnemy redEnemy;
    PatrolEnemy redEnemy2;
    initPatrolEnemy(redEnemy, sqx, sqy, 5.0f);
    initPatrolEnemy(redEnemy2, rand() % 1200 + 100, rand() % 600 + 50, 5.0f);

    while (1) {
        if (isGame == false && isFail == false) {
            setlinecolor(WHITE); setfillcolor(WHITE);
            fillrectangle(0, 0, 1400, 800);
            settextstyle(50, 0, "Microsoft YaHei"); // 修复乱码
            settextcolor(BLUE);
            printStory(40, 200, 200, "使用wasd操控角色，100分以上进入一个分支，100分一下为另一个分支。碰到红色矩形生命值减10，碰到边缘减3生命值。共100生命值，生命值为0时游戏结束。如果你明白了，就按【enter】进入游戏", 800, 2);
            FlushBatchDraw();
            getmessage(&localM, EM_KEY);
            if (localM.vkcode == VK_RETURN) {
                isGame = true;
                strtime = time(0); // ★ 重新计时，防止暂停期间时间偷跑
            }
        }

        if (isGame == true && isFail == false) {
            cls();
            gtime = time(0) - strtime;

            // 时间上限判定
            if (gtime >= 120) {
                isFail = true;
            }

            // 动态得分和速度
            if (gtime <= 30) score = (double)(gtime * 1.5);
            else if (gtime > 30 && gtime <= 50) score = 45 + (gtime - 30) * 2;
            else score = 95 + (gtime - 50) * 3;

            if (gtime < 30) {
                redEnemy.speed = 2.0f; redEnemy2.speed = 2.0f;
            }
            else if (gtime >= 30 && gtime <= 50) {
                redEnemy.speed = 3.5f; redEnemy2.speed = 3.5f;
            }
            else {
                redEnemy.speed = 3.0f; redEnemy2.speed = 3.0f;
            }

            setlinecolor(WHITE); setfillcolor(WHITE);
            fillrectangle(0, 0, 1400, 800);

            updateAndDrawPatrolEnemy(redEnemy, &sq, 1400, 800);
            if (gtime >= 50) {
                updateAndDrawPatrolEnemy(redEnemy2, &sq, 1400, 800);
                // 50秒后加速
                redEnemy.speed = 4.0f; redEnemy2.speed = 4.0f;
            }

            putimage_alpha(piax, piay, &p);

            char scoreStr[30];
            sprintf_s(scoreStr, "分数: %0.1f  HP: %d", score, hp);
            outtextxy(20, 20, scoreStr);
            FlushBatchDraw();

            // ★ 核心修复 1：边界流血惩罚（1秒扣3血）
            if (piax <= 1 || piax >= 1309 || piay <= -10 || piay >= 739) {
                if (GetTickCount() - lastEdgeTime > EDGE_COOLDOWN) {
                    hp -= 3;
                    lastEdgeTime = GetTickCount(); // 重置计时器，防止无限连扣
                }
            }


            // 玩家移动（顺便修复了：移动后不再立即扣血，移动本身安全）
            if (peekmessage(&localM, EM_KEY)) {
                if (localM.vkcode == 'A') { piax -= 20; if (piax <= 1) piax = 1; }
                if (localM.vkcode == 'D') { piax += 20; if (piax >= 1309) piax = 1309; }
                if (localM.vkcode == 'W') { piay -= 20; if (piay <= -10) piay = -10; }
                if (localM.vkcode == 'S') { piay += 20; if (piay >= 739) piay = 739; }
            }

            // ★ 核心修复 2：精确反方向弹开（带 500 毫秒无敌帧）
            if (GetTickCount() - lastHitTime > 500) {
                bool isHit = false;

                // 准备计算反弹所需的变量
                float dx = 0, dy = 0;

                // 与红色方块1碰撞检测
                if (piax + 100 > redEnemy.x && piax < redEnemy.x + 100 &&
                    piay + 100 > redEnemy.y && piay < redEnemy.y + 100) {
                    isHit = true;
                    // 计算玩家中心 指向 方块中心 的向量
                    dx = (piax + 50) - (redEnemy.x + 50);
                    dy = (piay + 50) - (redEnemy.y + 50);
                }

                // 与红色方块2碰撞检测（仅当 50 秒后出现）
                if (gtime >= 50) {
                    if (piax + 100 > redEnemy2.x && piax < redEnemy2.x + 100 &&
                        piay + 100 > redEnemy2.y && piay < redEnemy2.y + 100) {
                        isHit = true;
                        // 如果有两个方块同时撞上，取方块的向量叠加（虽然极少发生）
                        dx += (piax + 50) - (redEnemy2.x + 50);
                        dy += (piay + 50) - (redEnemy2.y + 50);
                    }
                }

                // 执行弹开
                if (isHit) {
                    // 计算向量的模长（防止除零）
                    float dist = sqrt(dx * dx + dy * dy) + 0.1f;

                    // 核心物理：反方向推离 60 像素！（距离大一点）
                    int pushForce = 65; // 这个数值可以调，越大弹得越远
                    piax += (int)((dx / dist) * pushForce);
                    piay += (int)((dy / dist) * pushForce);

                    // 扣血并进入无敌帧
                    hp -= 10;
                    lastHitTime = GetTickCount();
                }
            }

            // 死亡判定
            if (hp <= 0) {
                isFail = true;
            }
        }

        if (isFail == true && isGame == true) {
            cls();
            setlinecolor(WHITE); setfillcolor(WHITE);
            fillrectangle(0, 0, 1400, 800);
            settextstyle(50, 0, "Microsoft YaHei");

            char scoreStr[20];
            sprintf_s(scoreStr, "最终得分: %0.1f", score);
            outtextxy(200, 200, scoreStr);
            outtextxy(200, 400, "按下回车退出");
            FlushBatchDraw();
            getmessage(&localM, EM_KEY);
            getmessage(&localM, EM_KEY);
            if (localM.ch == 13) {
                isGame = false;
            }
        }
        if (isGame == false && isFail == true) {
            break;
        }
    }
}

void cd() {
    IMAGE cd;
    loadimage(&cd, "res\\pir\\cd1.png");
    backA();
    putimage_alpha(150, 50, &cd);
    FlushBatchDraw();
    getmessage(&m, EM_KEY);
    getmessage(&m, EM_KEY);

}

void say() {
    IMAGE SA;
    loadimage(&SA, "res\\pir\\say.png");
    backA();
    putimage_alpha(5, 0, &SA);
    FlushBatchDraw();
    getmessage(&m, EM_KEY);
    getmessage(&m, EM_KEY);
}

void advtan() {
    backA();
    IMAGE adv;
    loadimage(&adv, "res\\pir\\adv.png");
    putimage_alpha(20, 0, &adv);
    if (v12 == 1) {
        settextcolor(GREEN);
        settextstyle(32, 0, "仿宋");
        outtextxy(760, 185, "【已完成！】");
        outtextxy(761, 185, "【已完成！】");
        settextcolor(BLACK);
    }
    if (v1 == 1) {
        settextcolor(GREEN);
        settextstyle(32, 0, "仿宋");
        outtextxy(1185, 245, "【已完成！】");
        outtextxy(1186, 245, "【已完成！】");
        settextcolor(BLACK);
    }
    if (v7 == 1) {
        settextcolor(GREEN);
        settextstyle(32, 0, "仿宋");
        outtextxy(1185, 305, "【已完成！】");
        outtextxy(1186, 305, "【已完成！】");
        settextcolor(BLACK);
    }
    if (v8 == 1) {
        settextcolor(GREEN);
        settextstyle(32, 0, "仿宋");
        outtextxy(1185, 345, "【已完成！】");
        outtextxy(1186, 345, "【已完成！】");
        settextcolor(BLACK);
    }
    if (v2 == 1) {
        settextcolor(GREEN);
        settextstyle(32, 0, "仿宋");
        outtextxy(690, 385, "【已完成！】");
        outtextxy(691, 385, "【已完成！】");
        settextcolor(BLACK);
    }
    if (v9 == 1) {
        settextcolor(GREEN);
        settextstyle(32, 0, "仿宋");
        outtextxy(690, 425, "【已完成！】");
        outtextxy(691, 425, "【已完成！】");
        settextcolor(BLACK);
    }
    if (v10 == 1) {
        settextcolor(GREEN);
        settextstyle(32, 0, "仿宋");
        outtextxy(690, 465, "【已完成！】");
        outtextxy(691, 465, "【已完成！】");
        settextcolor(BLACK);
    }
    if (v3 == 1) {
        settextcolor(GREEN);
        settextstyle(32, 0, "仿宋");
        outtextxy(1175, 525, "【已完成！】");
        outtextxy(1176, 525, "【已完成！】");
        settextcolor(BLACK);
    }
    if (v11 == 1) {
        settextcolor(GREEN);
        settextstyle(32, 0, "仿宋");
        outtextxy(1175, 565, "【已完成！】");
        outtextxy(1176, 565, "【已完成！】");
        settextcolor(BLACK);
    }
    if (v4 == 1) {
        settextcolor(GREEN);
        settextstyle(32, 0, "仿宋");
        outtextxy(880, 625, "【已完成！】");
        outtextxy(881, 625, "【已完成！】");
        settextcolor(BLACK);
    }
    if (v5 == 1) {
        settextcolor(GREEN);
        settextstyle(32, 0, "仿宋");
        outtextxy(820, 665, "【已完成！】");
        outtextxy(821, 665, "【已完成！】");
        settextcolor(BLACK);
    }
    if (v6 == 1) {
        settextcolor(GREEN);
        settextstyle(32, 0, "仿宋");
        outtextxy(730, 705, "【已完成！】");
        outtextxy(731, 705, "【已完成！】");
        settextcolor(BLACK);
    }
}

void funb() {
    IMAGE po;
    loadimage(&po, "res\\pir\\po.png");
    IMAGE pos;
    loadimage(&pos, "res\\pir\\pos.png");
    cls();
    backA();
    putimage_alpha(200, 50, &po);
    printStory(40, 270, 170, "由于M国的国人暴乱，M国最终崩溃", 900, 1);
    printStory(35, 200, 600, "你吞并M国", 450, 1);
    printStory(35, 750, 600, "你不管M国，潜心发展自身经济", 450, 1);
    FlushBatchDraw();
    getmessage(&m, EM_KEY);
    getmessage(&m, EM_KEY);
    if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
        cls();
        backA();
        putimage_alpha(200, 50, &po);
        printStory(40, 270, 170, "获得巨大收益", 900, 1);
        printStory(35, 200, 600, "你使用平和的民族政策", 450, 1);
        printStory(35, 750, 600, "你使用了激进的民族政策（类似于fxs）", 450, 1);
        FlushBatchDraw();
        getmessage(&m, EM_KEY);
        getmessage(&m, EM_KEY);
        if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
            cls();
            backA();
            putimage_alpha(200, 50, &po);
            printStory(40, 270, 170, "很好的稳定了M国遗民的情绪", 900, 1);
            printStory(35, 200, 600, "你带领M国的遗民发展经济", 450, 1);
            printStory(35, 750, 600, "你亲自走访M国遗民的挨家挨户，发送福利", 450, 1);
            FlushBatchDraw();
            getmessage(&m, EM_KEY);
            getmessage(&m, EM_KEY);
            cls();
            backA();
            putimage_alpha(200, 50, &po);
            settextcolor(RED);
            printStory(50, 270, 170, "你是伟大的革命斗士，你解放了M国的遗民，万寿无疆！", 900, 2);
            printStory(40, 200, 600, "等级：S+", 450, 1);
            printStory(40, 750, 600, "任意键退出", 450, 1);
            FlushBatchDraw();
            settextcolor(BLACK);
            arrS[6] = 1;
            getmessage(&m, EM_KEY);
            getmessage(&m, EM_KEY);
        }
        else {
            cls();
            backA();
            putimage_alpha(200, 50, &po);
            printStory(40, 270, 170, "对于非虾族人民（大虾帝国的主题民族是虾族）更是压迫对待", 900, 1);
            printStory(35, 200, 600, "提出二等人制度，将M国遗民称为二等人，并要求他们每年向你进贡9.1*10^7包虾片", 450, 1);
            printStory(35, 750, 600, "你将非虾族人民全部杀害", 450, 1);
            FlushBatchDraw();
            getmessage(&m, EM_KEY);
            getmessage(&m, EM_KEY);
            if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
                cls();
                backA();
                putimage_alpha(200, 50, &po);
                printStory(40, 270, 170, "他们忍无可忍，最终暴乱", 900, 1);
                printStory(35, 200, 600, "a", 450, 1);
                printStory(35, 750, 600, "b", 450, 1);
                FlushBatchDraw();
                getmessage(&m, EM_KEY);
                getmessage(&m, EM_KEY);
                if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
                    cls();
                    backA();
                    putimage_alpha(200, 50, &po);
                    settextcolor(RED);
                    printStory(50, 270, 170, "你的首级被他们割下，暴君的名号流传千年", 900, 2);
                    printStory(40, 200, 600, "等级：D+", 450, 1);
                    printStory(40, 750, 600, "任意键退出", 450, 1);
                    FlushBatchDraw();
                    settextcolor(BLACK);
                    arrD[7] = 1;
                    getmessage(&m, EM_KEY);
                    getmessage(&m, EM_KEY);
                }
                else {
                    cls();
                    backA();
                    putimage_alpha(200, 50, &po);
                    settextcolor(RED);
                    printStory(50, 270, 170, "你被他们赶下总统之位，你曾经的虾族成为了他们眼中的二等人，你曾经的人民们，包括你，收到了你对他们的待遇，你难道不觉得悲哀吗？", 900, 2);
                    printStory(40, 200, 600, "等级：D-", 450, 1);
                    printStory(40, 750, 600, "任意键退出", 450, 1);
                    FlushBatchDraw();
                    settextcolor(BLACK);
                    arrD[8] = 1;
                    getmessage(&m, EM_KEY);
                    getmessage(&m, EM_KEY);
                }
            }
            else {
                int b10 = rand() % 4;
                if (b10 != 3) {
                    cls();
                    backA();
                    putimage_alpha(200, 50, &pos);
                    printStory(40, 270, 170, "由于你的政策，你的民众也对你不放心", 900, 1);
                    printStory(35, 200, 600, "你自我弹劾", 450, 1);
                    printStory(35, 750, 600, "你得知自己命不久矣，将260w和自己转至国外", 450, 1);
                    FlushBatchDraw();
                    getmessage(&m, EM_KEY);
                    getmessage(&m, EM_KEY);
                    if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
                        cls();
                        backA();
                        putimage_alpha(200, 50, &po);
                        settextcolor(RED);
                        printStory(50, 270, 170, "最终失去了你的所有，但好歹，你没有落个死亡", 900, 2);
                        printStory(40, 200, 600, "等级：C+", 450, 1);
                        printStory(40, 750, 600, "任意键退出", 450, 1);
                        FlushBatchDraw();
                        settextcolor(BLACK);
                        arrC[9] = 1;
                        getmessage(&m, EM_KEY);
                        getmessage(&m, EM_KEY);
                    }
                    else {
                        cls();
                        backA();
                        putimage_alpha(200, 50, &po);
                        settextcolor(RED);
                        printStory(50, 270, 170, "260W你在国外算是安宁，但是你想想，难道你不要你的浮木了吗？他们怎么办？", 900, 2);
                        printStory(40, 200, 600, "等级：A-", 450, 1);
                        printStory(40, 750, 600, "任意键退出", 450, 1);
                        FlushBatchDraw();
                        settextcolor(BLACK);
                        arrA[12] = 1;
                        getmessage(&m, EM_KEY);
                        getmessage(&m, EM_KEY);
                    }
                }
                else {
                    cls();
                    backA();
                    putimage_alpha(200, 50, &pos);
                    settextcolor(RED);
                    printStory(50, 270, 170, "你被直接赶下台，你的时代结束了", 900, 2);
                    printStory(40, 200, 600, "等级：B", 450, 1);
                    printStory(40, 750, 600, "任意键退出", 450, 1);
                    FlushBatchDraw();
                    settextcolor(BLACK);
                    arrB[11] = 1;
                    getmessage(&m, EM_KEY);
                    getmessage(&m, EM_KEY);
                }
            }
        }
    }
    else {
        cls();
        backA();
        putimage_alpha(200, 50, &po);
        settextcolor(RED);
        printStory(50, 270, 170, "终于，10年后，你成为了世界第一大经济体", 900, 2);
        printStory(40, 200, 600, "等级：S", 450, 1);
        printStory(40, 750, 600, "任意键退出", 450, 1);
        FlushBatchDraw();
        settextcolor(BLACK);
        arrS[5] = 1;
        getmessage(&m, EM_KEY);
        getmessage(&m, EM_KEY);
    }
}

void func() {
    IMAGE po;
    loadimage(&po, "res\\pir\\po.png");
    IMAGE pos;
    loadimage(&pos, "res\\pir\\pos.png");
    cls();
    backA();
    putimage_alpha(200, 50, &po);
    printStory(40, 270, 170, "M国接受，但是你趁机要求M国与你签订《二一五条约》，M国处于危急存亡之中，只能签订此", 900, 1);
    printStory(35, 200, 600, "你开始打击M国的军事方面", 450, 1);
    printStory(35, 750, 600, "你开始打击M国的经济", 450, 1);
    FlushBatchDraw();
    getmessage(&m, EM_KEY);
    getmessage(&m, EM_KEY);
    if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
        cls();
        backA();
        putimage_alpha(200, 50, &po);
        settextcolor(RED);
        printStory(50, 270, 170, "欲知后事如何，请君自己想象，因为我懒得编了，我想，虾片不会把国家治理的怎么好吧……", 900, 2);
        printStory(40, 200, 600, "等级：R", 450, 1);
        printStory(40, 750, 600, "任意键退出", 450, 1);
        FlushBatchDraw();
        settextcolor(BLACK);
        arrR[7] = 1;
        getmessage(&m, EM_KEY);
        getmessage(&m, EM_KEY);
        say();
    }
    else {
        cls();
        backA();
        putimage_alpha(200, 50, &po);
        printStory(40, 270, 170, "M国新晋总统上任，不甘与此，欲决定改革", 900, 1);
        printStory(35, 200, 600, "派人暗杀新晋总统", 450, 1);
        printStory(35, 750, 600, "暂时按兵不动", 450, 1);
        FlushBatchDraw();
        getmessage(&m, EM_KEY);
        getmessage(&m, EM_KEY);
        if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
            cls();
            backA();
            putimage_alpha(200, 50, &po);
            printStory(40, 270, 170, "M国人民愤怒，将撕毁条约，并开战", 900, 1);
            printStory(35, 200, 600, "THE A OF STORY", 450, 1);
            printStory(35, 750, 600, "THE X OF STORY", 450, 1);
            FlushBatchDraw();
            getmessage(&m, EM_KEY);
            getmessage(&m, EM_KEY);
            if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
                cls();
                backA();
                putimage_alpha(200, 50, &po);
                settextcolor(RED);
                printStory(50, 270, 170, "你胜利了，M国彻底覆灭.M国内心其实是欢喜的， 因为他们被解放了……||你让M国再次伟大！（MMGA,make M great again)", 900, 2);
                printStory(40, 200, 600, "等级：S-", 450, 1);
                printStory(40, 750, 600, "任意键退出", 450, 1);
                FlushBatchDraw();
                settextcolor(BLACK);
                arrS[7] = 1;
                getmessage(&m, EM_KEY);
                getmessage(&m, EM_KEY);
            }
            else {
                cls();
                backA();
                putimage_alpha(200, 50, &po);
                settextcolor(RED);
                printStory(50, 270, 170, "你失败了.但是你是失败的成功者，因为由于M国的统治方式比虾片还要虾片，于是大量人借着你的名号起义“M灭，大虾生”", 900, 2);
                printStory(40, 200, 600, "等级：A+", 450, 1);
                printStory(40, 750, 600, "任意键退出", 450, 1);
                FlushBatchDraw();
                settextcolor(BLACK);
                arrA[13] = 1;
                getmessage(&m, EM_KEY);
                getmessage(&m, EM_KEY);
            }
        }
        else {
            cls();
            backA();
            putimage_alpha(200, 50, &po);
            printStory(40, 270, 170, "新晋总统因不雅往事被弹劾，M国一蹶不振320，你借此机会进攻M国，你成功了", 900, 1);
            printStory(35, 200, 600, "你要实行“MMAXGA“政策", 450, 1);
            printStory(35, 750, 600, "你要实行“MEGAM”政策", 450, 1);
            FlushBatchDraw();
            getmessage(&m, EM_KEY);
            getmessage(&m, EM_KEY);
            if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
                int b11 = rand() % 20;
                if (b11 != 0) {
                    cls();
                    backA();
                    putimage_alpha(200, 50, &pos);
                    settextcolor(RED);
                    printStory(50, 270, 170, "make M and XIAPIAN great again,获得百万人的支持.你赢得了一个好名号", 900, 2);
                    printStory(40, 200, 600, "等级：A", 450, 1);
                    printStory(40, 750, 600, "任意键退出", 450, 1);
                    FlushBatchDraw();
                    settextcolor(BLACK);
                    arrA[14] = 1;
                    getmessage(&m, EM_KEY);
                    getmessage(&m, EM_KEY);
                }
                else {
                    cls();
                    backA();
                    putimage_alpha(200, 50, &pos);
                    settextcolor(RED);
                    printStory(50, 270, 170, "你被摔得粉碎，别问为什么，因为这是计算机的一个概率现象，但是C++的概率函数不是严格意义上的真随机，只能说，你tm能到这里，纯粹是你的手气太背了", 900, 2);
                    printStory(40, 200, 600, "等级：D", 450, 1);
                    printStory(40, 750, 600, "任意键退出", 450, 1);
                    FlushBatchDraw();
                    settextcolor(BLACK);
                    arrD[9] = 1;
                    getmessage(&m, EM_KEY);
                    getmessage(&m, EM_KEY);
                }
            }
            else {
                int b12 = rand() % 4;
                if (b12 != 0) {
                    cls();
                    backA();
                    putimage_alpha(200, 50, &pos);
                    settextcolor(RED);
                    printStory(50, 270, 170, "make everyone go around me,让所有人围绕你.你自己看看你这是什么政策，我都不想吐槽了！？", 900, 2);
                    printStory(40, 200, 600, "等级：R", 450, 1);
                    printStory(40, 750, 600, "任意键退出", 450, 1);
                    FlushBatchDraw();
                    settextcolor(BLACK);
                    arrR[8] = 1;
                    getmessage(&m, EM_KEY);
                    getmessage(&m, EM_KEY);
                    cd();
                }
                else {
                    cls();
                    backA();
                    putimage_alpha(200, 50, &pos);
                    settextcolor(RED);
                    printStory(50, 270, 170, "make everyone go around me,让所有人围绕你.辅佐君王的贤士听完后跳楼了，国家呢？你猜怎么着？嘿，也跳了，好笑吧！……", 900, 2);
                    printStory(40, 200, 600, "等级：C-", 450, 1);
                    printStory(40, 750, 600, "任意键退出", 450, 1);
                    FlushBatchDraw();
                    settextcolor(BLACK);
                    arrC[10] = 1;
                    getmessage(&m, EM_KEY);
                    getmessage(&m, EM_KEY);
                }
            }
        }
    }
}

void fune() {
    IMAGE po;
    loadimage(&po, "res\\pir\\po.png");
    IMAGE pos;
    loadimage(&pos, "res\\pir\\pos.png");
    cls();
    backA();
    putimage_alpha(200, 50, &po);
    printStory(40, 270, 170, "经过5年的努力，研究有了一点苗头。但是与此同时，M国与S国开战，S国是你最重要的盟友，所以说，研究进度中断，而且时间紧迫，你们没有5年，没有3年，甚至2年半都没有，战争是一触即发的，届时，核威胁是最大的隐患！", 900, 1);
    printStory(35, 200, 600, "你开始组建军队，部署海陆空的军事演习，调动全国兵源", 450, 1);
    printStory(35, 750, 600, "你继续研究氢弹", 450, 1);
    FlushBatchDraw();
    getmessage(&m, EM_KEY);
    getmessage(&m, EM_KEY);
    if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
        cls();
        backA();
        putimage_alpha(200, 50, &po);
        printStory(40, 270, 170, "但是，氢弹研究告一段落了80。同时，随着M国偷袭R国的不冻港，战争全面爆发，你为了国家安全，毅然决然选择了与M国开战，与R国形成同盟之势", 900, 1);
        printStory(35, 200, 600, "您与R国分别向东西进攻M国", 450, 1);
        printStory(35, 750, 600, "你与R国一起进攻M国的南方", 450, 1);
        FlushBatchDraw();
        getmessage(&m, EM_KEY);
        getmessage(&m, EM_KEY);
        if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
            cls();
            backA();
            putimage_alpha(200, 50, &po);
            printStory(40, 270, 170, "M国将你们轻松消灭，你们的计谋失败了，M国的军队已经踏入了你的国土", 900, 1);
            printStory(35, 200, 600, "直接投降", 450, 1);
            printStory(35, 750, 600, "顽强抵抗", 450, 1);
            FlushBatchDraw();
            getmessage(&m, EM_KEY);
            getmessage(&m, EM_KEY);
            if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
                cls();
                backA();
                putimage_alpha(200, 50, &po);
                printStory(40, 270, 170, "你被众人唾弃，却以“曲线救国”自圆其说", 900, 1);
                printStory(35, 200, 600, "你反阴了M国一手，让M国撤军", 450, 1);
                printStory(35, 750, 600, "你没有再管你的国家", 450, 1);
                FlushBatchDraw();
                getmessage(&m, EM_KEY);
                getmessage(&m, EM_KEY);
                if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
                    cls();
                    backA();
                    putimage_alpha(200, 50, &po);
                    settextcolor(RED);
                    printStory(50, 270, 170, "你声名远扬！", 900, 2);
                    printStory(40, 200, 600, "等级：S-", 450, 1);
                    printStory(40, 750, 600, "任意键退出", 450, 1);
                    FlushBatchDraw();
                    settextcolor(BLACK);
                    arrS[8] = 1;
                    getmessage(&m, EM_KEY);
                    getmessage(&m, EM_KEY);
                }
                else {
                    cls();
                    backA();
                    putimage_alpha(200, 50, &po);
                    settextcolor(RED);
                    printStory(50, 270, 170, "“大虾帝国，这是什么东西？”你在晚年疑惑到“或许，这是一款游戏的名字吧……”", 900, 2);
                    printStory(40, 200, 600, "等级：B-", 450, 1);
                    printStory(40, 750, 600, "任意键退出", 450, 1);
                    FlushBatchDraw();
                    settextcolor(BLACK);
                    arrB[15] = 1;
                    getmessage(&m, EM_KEY);
                    getmessage(&m, EM_KEY);
                }
            }
            else {
                cls();
                backA();
                putimage_alpha(200, 50, &po);
                settextcolor(RED);
                printStory(50, 270, 170, "你失败了，但是你赢得了骨气", 900, 2);
                printStory(40, 200, 600, "等级：A", 450, 1);
                printStory(40, 750, 600, "任意键退出", 450, 1);
                FlushBatchDraw();
                settextcolor(BLACK);
                arrA[15] = 1;
                getmessage(&m, EM_KEY);
                getmessage(&m, EM_KEY);
            }
        }
        else {
            cls();
            backA();
            putimage_alpha(200, 50, &po);
            printStory(40, 270, 170, "你与R国一起进攻M国的南方.这打破了M国“南线无战事”的经验主义的措辞，打M国一个措手不及！你击败了M国，你是英雄", 900, 1);
            printStory(35, 200, 600, "结束游戏", 450, 1);
            printStory(35, 750, 600, "我不想结束", 450, 1);
            FlushBatchDraw();
            getmessage(&m, EM_KEY);
            getmessage(&m, EM_KEY);
            if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
                cls();
                backA();
                putimage_alpha(200, 50, &po);
                settextcolor(RED);
                printStory(50, 270, 170, "你是英雄！", 900, 2);
                printStory(40, 200, 600, "等级：A+", 450, 1);
                printStory(40, 750, 600, "任意键退出", 450, 1);
                FlushBatchDraw();
                settextcolor(BLACK);
                arrA[16] = 1;
                getmessage(&m, EM_KEY);
                getmessage(&m, EM_KEY);
            }
            else {
                funb();
            }
        }
    }
    else {
        cls();
        backA();
        putimage_alpha(200, 50, &po);
        printStory(40, 270, 170, "你被外人称为“疯子”，这tm是不可能的！客观上来说，这是及其冒险的行为，一旦失败，浪费的精力不是一般的损耗！", 900, 1);
        printStory(35, 200, 600, "W", 450, 1);
        printStory(35, 750, 600, "M", 450, 1);
        FlushBatchDraw();
        getmessage(&m, EM_KEY);
        getmessage(&m, EM_KEY);
        if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
            cls();
            backA();
            putimage_alpha(200, 50, &po);
            settextcolor(RED);
            printStory(50, 270, 170, "你失败了，全世界，包括你的国民，都认为你是彻头彻尾的疯子！你跌得粉碎了，即使你的本意是想让国家更好！", 900, 2);
            printStory(40, 200, 600, "等级：B+", 450, 1);
            printStory(40, 750, 600, "任意键退出", 450, 1);
            FlushBatchDraw();
            settextcolor(BLACK);
            arrB[16] = 1;
            getmessage(&m, EM_KEY);
            getmessage(&m, EM_KEY);
        }
        else {
            cls();
            backA();
            putimage_alpha(200, 50, &po);
            printStory(40, 270, 170, "你成功了，这是奇迹，这是奇迹！这是奇迹！！高原上，那一朵蘑菇云的炸响，是为世界交上了一张虾片答卷！被核威胁的日子没有了，也一去不复返了！孤注一掷的你，得到了命运的肯定，现在，你是地球的主宰！！！", 900, 1);
            printStory(35, 200, 600, "向M国投掷两枚氢弹", 450, 1);
            printStory(35, 750, 600, "你放弃了对M国的进攻，你决定用这两枚氢弹让世界和平！", 450, 1);
            FlushBatchDraw();
            getmessage(&m, EM_KEY);
            getmessage(&m, EM_KEY);
            if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
                cls();
                backA();
                putimage_alpha(200, 50, &po);
                settextcolor(RED);
                printStory(50, 270, 170, "M国火速投降！这是命运的舞蹈！", 900, 2);
                printStory(40, 200, 600, "等级：S-", 450, 1);
                printStory(40, 750, 600, "任意键退出", 450, 1);
                FlushBatchDraw();
                settextcolor(BLACK);
                arrS[9] = 1;
                getmessage(&m, EM_KEY);
                getmessage(&m, EM_KEY);
            }
            else {
                cls();
                backA();
                putimage_alpha(200, 50, &po);
                settextcolor(RED);
                printStory(50, 270, 170, "M国火速投降！这是命运的舞蹈！", 900, 2);
                printStory(40, 200, 600, "等级：S+", 450, 1);
                printStory(40, 750, 600, "任意键退出", 450, 1);
                FlushBatchDraw();
                settextcolor(BLACK);
                arrS[10] = 1;
                getmessage(&m, EM_KEY);
                getmessage(&m, EM_KEY);
            }
        }
    }
}

void funf() {
    IMAGE po;
    loadimage(&po, "res\\pir\\po.png");
    IMAGE pos;
    loadimage(&pos, "res\\pir\\pos.png");
    int b19 = rand() % 100;
    if (b19 < 96) {
        cls();
        backA();
        putimage_alpha(200, 50, &pos);
        settextcolor(RED);
        printStory(50, 270, 170, "你的成功是必然的！M国灭亡！你之后代替了M国的霸主地位，引领世界更好。但是，你的胜利的阴暗面，在未来，还会有人发现吗？", 900, 2);
        printStory(40, 200, 600, "等级：S", 450, 1);
        printStory(40, 750, 600, "任意键退出", 450, 1);
        FlushBatchDraw();
        settextcolor(BLACK);
        arrS[12] = 1;
        getmessage(&m, EM_KEY);
        getmessage(&m, EM_KEY);
    }
    else {
        cls();
        backA();
        putimage_alpha(200, 50, &pos);
        settextcolor(RED);
        printStory(50, 270, 170, "恭喜你，中大奖了！这都能输，果然是虾片啊！！！", 900, 2);
        printStory(40, 200, 600, "等级：D-", 450, 1);
        printStory(40, 750, 600, "任意键退出", 450, 1);
        FlushBatchDraw();
        settextcolor(BLACK);
        arrD[12] = 1;
        getmessage(&m, EM_KEY);
        getmessage(&m, EM_KEY);
    }
}

void fund() {
    IMAGE po;
    loadimage(&po, "res\\pir\\po.png");
    IMAGE pos;
    loadimage(&pos, "res\\pir\\pos.png");
    cls();
    backA();
    putimage_alpha(200, 50, &po);
    printStory(40, 270, 170, "吸引了大量外来企业，同时，你加入了世界贸易组织，这种万物勃发生机勃勃的局势仿佛就在你的眼前", 900, 1);
    printStory(35, 200, 600, "召开大会，提出国防的重要性，决定把提高军事实力写入虾片宪法大纲", 450, 1);
    printStory(35, 750, 600, "召开大会，提出要建设经济强国，成为“世界贸易中心”是你制定的总目标，坚持虾片的总路线一百年不动摇！", 450, 1);
    FlushBatchDraw();
    getmessage(&m, EM_KEY);
    getmessage(&m, EM_KEY);
    if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
        cls();
        backA();
        putimage_alpha(200, 50, &po);
        printStory(40, 270, 170, "国家开始同时发展军事工业", 900, 1);
        printStory(35, 200, 600, "你要独立开始氢弹研究", 450, 1);
        printStory(35, 750, 600, "你与M国形成军事盟友", 450, 1);
        FlushBatchDraw();
        getmessage(&m, EM_KEY);
        getmessage(&m, EM_KEY);
        if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
            cls();
            backA();
            putimage_alpha(200, 50, &po);
            printStory(40, 270, 170, "你开始召集国家人才。但是，核心技术处于一片空白", 900, 1);
            printStory(35, 200, 600, "自力更生，自己研发氢弹", 450, 1);
            printStory(35, 750, 600, "向M国派遣留学生学习", 450, 1);
            FlushBatchDraw();
            getmessage(&m, EM_KEY);
            getmessage(&m, EM_KEY);
            if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
                fune();
            }
            else {
                int b15 = rand() % 4;
                if (b15 == 0) {
                    cls();
                    backA();
                    putimage_alpha(200, 50, &pos);
                    printStory(40, 270, 170, "你十分气愤，于是向M国开战！", 900, 1);
                    printStory(35, 200, 600, "东线进攻", 450, 1);
                    printStory(35, 750, 600, "西线进攻,同上", 450, 1);
                    FlushBatchDraw();
                    getmessage(&m, EM_KEY);
                    getmessage(&m, EM_KEY);
                    cls();
                    backA();
                    putimage_alpha(200, 50, &po);
                    settextcolor(RED);
                    printStory(50, 270, 170, "其实无论怎么样，输是肯定的，你没有技术，什么都没有，怎么和人家打呢？", 900, 2);
                    printStory(40, 200, 600, "等级：C", 450, 1);
                    printStory(40, 750, 600, "任意键退出", 450, 1);
                    FlushBatchDraw();
                    settextcolor(BLACK);
                    arrC[16] = 1;
                    getmessage(&m, EM_KEY);
                    getmessage(&m, EM_KEY);
                }
                else {
                    fune();
                }
            }
        }
        else {
            cls();
            backA();
            putimage_alpha(200, 50, &po);
            printStory(40, 270, 170, "国家民众一片惊诧！", 900, 1);
            printStory(35, 200, 600, "你与M国假交往，浮于表面上的和平", 450, 1);
            printStory(35, 750, 600, "你与M国真交往，心连心的", 450, 1);
            FlushBatchDraw();
            getmessage(&m, EM_KEY);
            getmessage(&m, EM_KEY);
            if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
                cls();
                backA();
                putimage_alpha(200, 50, &po);
                printStory(40, 270, 170, "M国暂时没有识破你，两国友好发展,你派遣间谍假借交好之意窃取M国军事机密", 900, 1);
                printStory(35, 200, 600, "asdfg", 450, 1);
                printStory(35, 750, 600, "qwert", 450, 1);
                FlushBatchDraw();
                getmessage(&m, EM_KEY);
                getmessage(&m, EM_KEY);
                if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
                    cls();
                    backA();
                    putimage_alpha(200, 50, &po);
                    printStory(40, 270, 170, "M国没有发现，但是你得到了M国的机密.国家将M国的先进科技进行研发，军事实力大增", 900, 1);
                    printStory(35, 200, 600, "你要对M国开战", 450, 1);
                    printStory(35, 750, 600, "你不与M国开战，你要玩阴的……", 450, 1);
                    FlushBatchDraw();
                    getmessage(&m, EM_KEY);
                    getmessage(&m, EM_KEY);
                    if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
                        funf();
                    }
                    else {
                        cls();
                        backA();
                        putimage_alpha(200, 50, &po);
                        printStory(40, 270, 170, "M国的总统即将退位，你打算扶植C-爱新觉罗之直布陀螺-安倍敏敏之机挖片虾-Y上任。毕竟，她也是你们虾片一族的（远亲），但是最主要的因素是，她是个**，在政治方面一窍不通！……终于，M国现任总统离世！", 900, 1);
                        printStory(35, 200, 600, "明面上宣扬他的政绩", 450, 1);
                        printStory(35, 750, 600, "暗箱操作", 450, 1);
                        FlushBatchDraw();
                        getmessage(&m, EM_KEY);
                        getmessage(&m, EM_KEY);
                        if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
                            int b16 = rand() % 100;
                            if (b16 < 65) {
                                cls();
                                backA();
                                putimage_alpha(200, 50, &pos);
                                printStory(40, 270, 170, "成功", 900, 1);
                                printStory(35, 200, 600, "你开始捧杀他，积极赞美她的错误决定", 450, 1);
                                printStory(35, 750, 600, "你让她主动想你提供军事相关资料", 450, 1);
                                FlushBatchDraw();
                                getmessage(&m, EM_KEY);
                                getmessage(&m, EM_KEY);
                                if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
                                    funb();
                                }
                                else {
                                    funf();
                                }
                            }
                            else {
                                cls();
                                backA();
                                putimage_alpha(200, 50, &pos);
                                settextcolor(RED);
                                printStory(50, 270, 170, "失败，于是，你变得不再勤于政事，但是，仅仅过了6个月，你就去世了……纵观你的一生，也算得是个贤君吧", 900, 2);
                                printStory(40, 200, 600, "等级：A-", 450, 1);
                                printStory(40, 750, 600, "任意键退出", 450, 1);
                                FlushBatchDraw();
                                settextcolor(BLACK);
                                arrA[17] = 1;
                                getmessage(&m, EM_KEY);
                                getmessage(&m, EM_KEY);
                            }
                        }
                        else {
                            cls();
                            backA();
                            putimage_alpha(200, 50, &po);
                            settextcolor(RED);
                            printStory(50, 270, 170, "失败，被人察觉。于是你被判刑", 900, 2);
                            printStory(40, 200, 600, "等级：C-", 450, 1);
                            printStory(40, 750, 600, "任意键退出", 450, 1);
                            FlushBatchDraw();
                            settextcolor(BLACK);
                            arrC[17] = 1;
                            getmessage(&m, EM_KEY);
                            getmessage(&m, EM_KEY);
                        }
                    }
                }
                else {
                    int b17 = rand() % 100;
                    if (b17 < 95) {
                        cls();
                        backA();
                        putimage_alpha(200, 50, &pos);
                        settextcolor(RED);
                        printStory(50, 270, 170, "你失败了，这是一定的", 900, 2);
                        printStory(40, 200, 600, "等级：D+", 450, 1);
                        printStory(40, 750, 600, "任意键退出", 450, 1);
                        FlushBatchDraw();
                        settextcolor(BLACK);
                        arrD[11] = 1;
                        getmessage(&m, EM_KEY);
                        getmessage(&m, EM_KEY);
                    }
                    else {
                        cls();
                        backA();
                        putimage_alpha(200, 50, &pos);
                        settextcolor(RED);
                        printStory(50, 270, 170, "你成功了？？？别问为什么，c++的代码就是这么说的", 900, 2);
                        printStory(40, 200, 600, "等级：R+", 450, 1);
                        printStory(40, 750, 600, "任意键退出", 450, 1);
                        FlushBatchDraw();
                        settextcolor(BLACK);
                        arrR[9] = 1;
                        getmessage(&m, EM_KEY);
                        getmessage(&m, EM_KEY);
                        cd();
                    }
                }
            }
            else {
                cls();
                backA();
                putimage_alpha(200, 50, &po);
                printStory(40, 270, 170, "M国“似乎”也是这么对你的……，有一天，M国假借学习之意来拜访我国", 900, 1);
                printStory(35, 200, 600, "你认为他们是来盗窃技术的，于是让他们滚！", 450, 1);
                printStory(35, 750, 600, "你不认为他们是坏人", 450, 1);
                FlushBatchDraw();
                getmessage(&m, EM_KEY);
                getmessage(&m, EM_KEY);
                if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
                    cls();
                    backA();
                    putimage_alpha(200, 50, &po);
                    settextcolor(RED);
                    printStory(50, 270, 170, "你的操作惹怒了M国，M国与你开战，你输了，这是一定的。但是，你诚实的好形象流传千古", 900, 2);
                    printStory(40, 200, 600, "等级：A", 450, 1);
                    printStory(40, 750, 600, "任意键退出", 450, 1);
                    FlushBatchDraw();
                    settextcolor(BLACK);
                    arrA[18] = 1;
                    getmessage(&m, EM_KEY);
                    getmessage(&m, EM_KEY);
                }
                else {
                    int b18 = rand() % 10;
                    if (b18 == 0) {
                        cls();
                        backA();
                        putimage_alpha(200, 50, &pos);
                        settextcolor(RED);
                        printStory(50, 270, 170, "其实M国也是真心和你好的。于是，你们一起努力，创造了一个新世界！", 900, 2);
                        printStory(40, 200, 600, "等级：S+", 450, 1);
                        printStory(40, 750, 600, "任意键退出", 450, 1);
                        FlushBatchDraw();
                        settextcolor(BLACK);
                        arrS[11] = 1;
                        getmessage(&m, EM_KEY);
                        getmessage(&m, EM_KEY);
                    }
                    else {
                        cls();
                        backA();
                        putimage_alpha(200, 50, &pos);
                        settextcolor(RED);
                        printStory(50, 270, 170, "M国与你开战，你输了，死于怜悯！", 900, 2);
                        printStory(40, 200, 600, "等级：B", 450, 1);
                        printStory(40, 750, 600, "任意键退出", 450, 1);
                        FlushBatchDraw();
                        settextcolor(BLACK);
                        arrB[17] = 1;
                        getmessage(&m, EM_KEY);
                        getmessage(&m, EM_KEY);
                    }
                }
            }
        }
    }
    else {
        cls();
        backA();
        putimage_alpha(200, 50, &po);
        printStory(40, 270, 170, "渐渐地，贸易成为国家新型潮流", 900, 1);
        printStory(35, 200, 600, "向各国出口商品", 450, 1);
        printStory(35, 750, 600, "发展多种多样的商业", 450, 1);
        FlushBatchDraw();
        getmessage(&m, EM_KEY);
        getmessage(&m, EM_KEY);
        if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
            cls();
            backA();
            putimage_alpha(200, 50, &po);
            printStory(40, 270, 170, "赚取大量外汇", 900, 1);
            printStory(35, 200, 600, "你打算与M国形成合作伙伴", 450, 1);
            printStory(35, 750, 600, "你打算与R国形成合作伙伴", 450, 1);
            FlushBatchDraw();
            getmessage(&m, EM_KEY);
            getmessage(&m, EM_KEY);
            if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
                int b20 = rand() % 10;
                if (b20 < 7) {
                    cls();
                    backA();
                    putimage_alpha(200, 50, &pos);
                    printStory(40, 270, 170, "你被M国坑了，大量资金打水漂！", 900, 1);
                    printStory(35, 200, 600, "向M国开战！", 450, 1);
                    printStory(35, 750, 600, "你稳中求进", 450, 1);
                    FlushBatchDraw();
                    getmessage(&m, EM_KEY);
                    getmessage(&m, EM_KEY);
                    if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
                        int b21 = rand() % 4;
                        if (b21 != 0) {
                            cls();
                            backA();
                            putimage_alpha(200, 50, &pos);
                            settextcolor(RED);
                            printStory(50, 270, 170, "你输了，很彻底地输了，你被气愤冲昏头脑", 900, 2);
                            printStory(40, 200, 600, "等级：C-", 450, 1);
                            printStory(40, 750, 600, "任意键退出", 450, 1);
                            FlushBatchDraw();
                            settextcolor(BLACK);
                            arrC[19] = 1;
                            getmessage(&m, EM_KEY);
                            getmessage(&m, EM_KEY);
                        }
                        else {
                            cls();
                            backA();
                            putimage_alpha(200, 50, &pos);
                            settextcolor(RED);
                            printStory(50, 270, 170, "你赢了，M国已经灭亡，因为自己的愚蠢，但是，以你的军事水平，根本不可能打赢M国，所以，你是被C++救了一命！你被代码救了，快说谢谢作者", 900, 2);
                            printStory(40, 200, 600, "等级：A-", 450, 1);
                            printStory(40, 750, 600, "任意键退出", 450, 1);
                            FlushBatchDraw();
                            settextcolor(BLACK);
                            arrA[20] = 1;
                            getmessage(&m, EM_KEY);
                            getmessage(&m, EM_KEY);
                        }
                    }
                    else {
                        cls();
                        backA();
                        putimage_alpha(200, 50, &po);
                        printStory(40, 270, 170, "你稳中求进.这五年，国家在发展，而你，也开始倡导和平，你不再主动挑起战争，但是你仍然坚定地捍卫国家主权!但是啊，岁月不待人，你的身体一年不如一年了……", 900, 1);
                        printStory(35, 200, 600, "功成身就，你退出了一线", 450, 1);
                        printStory(35, 750, 600, "你继续奋斗", 450, 1);
                        FlushBatchDraw();
                        getmessage(&m, EM_KEY);
                        getmessage(&m, EM_KEY);
                        if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
                            cls();
                            backA();
                            putimage_alpha(200, 50, &po);
                            settextcolor(RED);
                            printStory(50, 270, 170, "你的一生是好的，你所在的大虾帝国积极发展", 900, 2);
                            printStory(40, 200, 600, "等级：S-", 450, 1);
                            printStory(40, 750, 600, "任意键退出", 450, 1);
                            FlushBatchDraw();
                            settextcolor(BLACK);
                            arrS[13] = 1;
                            getmessage(&m, EM_KEY);
                            getmessage(&m, EM_KEY);
                        }
                        else {
                            cls();
                            backA();
                            putimage_alpha(200, 50, &po);
                            settextcolor(RED);
                            printStory(50, 270, 170, "最终，你病死在了书桌前，但是你的敌人（M国）却把你捏造成一位彻头彻尾的反动派，你的事例被轻易抹去……", 900, 2);
                            printStory(40, 200, 600, "等级：B-", 450, 1);
                            printStory(40, 750, 600, "任意键退出", 450, 1);
                            FlushBatchDraw();
                            settextcolor(BLACK);
                            arrB[18] = 1;
                            getmessage(&m, EM_KEY);
                            getmessage(&m, EM_KEY);
                        }
                    }
                }
                else {
                    cls();
                    backA();
                    putimage_alpha(200, 50, &pos);
                    printStory(40, 270, 170, "恭喜你，中大奖了，你没有被M国坑！", 900, 1);
                    printStory(35, 200, 600, "您与M国协同发展全球科技", 450, 1);
                    printStory(35, 750, 600, "你要坑M国", 450, 1);
                    FlushBatchDraw();
                    getmessage(&m, EM_KEY);
                    getmessage(&m, EM_KEY);
                    if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
                        cls();
                        backA();
                        putimage_alpha(200, 50, &po);
                        settextcolor(RED);
                        printStory(50, 270, 170, "如今，智能机器人，AI等技术的不断演化，你们的故事还将进行到哪里？且听后事如何……", 900, 2);
                        printStory(40, 200, 600, "等级：S", 450, 1);
                        printStory(40, 750, 600, "任意键退出", 450, 1);
                        FlushBatchDraw();
                        settextcolor(BLACK);
                        arrS[14] = 1;
                        getmessage(&m, EM_KEY);
                        getmessage(&m, EM_KEY);
                    }
                    else {
                        cls();
                        backA();
                        putimage_alpha(200, 50, &po);
                        printStory(40, 270, 170, "你对M国实施了关税攻击，但是与此同时，M国也意识到了自己的处境，他对您发起战争", 900, 1);
                        printStory(35, 200, 600, "G", 450, 1);
                        printStory(35, 750, 600, "H", 450, 1);
                        FlushBatchDraw();
                        getmessage(&m, EM_KEY);
                        getmessage(&m, EM_KEY);
                        if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
                            cls();
                            backA();
                            putimage_alpha(200, 50, &po);
                            settextcolor(RED);
                            printStory(50, 270, 170, "果不其然，你失败了，我甚至都不想为你写再多的什么了", 900, 2);
                            printStory(40, 200, 600, "等级：D+", 450, 1);
                            printStory(40, 750, 600, "任意键退出", 450, 1);
                            FlushBatchDraw();
                            settextcolor(BLACK);
                            arrD[13] = 1;
                            getmessage(&m, EM_KEY);
                            getmessage(&m, EM_KEY);
                        }
                        else {
                            cls();
                            backA();
                            putimage_alpha(200, 50, &po);
                            settextcolor(RED);
                            printStory(50, 270, 170, "666代码发力了，虽然你赢了，但是，你输的彻底", 900, 2);
                            printStory(40, 200, 600, "等级：B-", 450, 1);
                            printStory(40, 750, 600, "任意键退出", 450, 1);
                            FlushBatchDraw();
                            settextcolor(BLACK);
                            arrB[19] = 1;
                            getmessage(&m, EM_KEY);
                            getmessage(&m, EM_KEY);
                        }
                    }
                }
            }
            else {
                cls();
                backA();
                putimage_alpha(200, 50, &po);
                settextcolor(RED);
                printStory(50, 270, 170, "真不幸，当年在走向“******”的时候，R国是极力反对的它认为****不能与****并行，要不然你就是******!要知道，R国是****所以说你激怒了R国，你最终没有好下场!", 900, 2);
                printStory(40, 200, 600, "等级：C+", 450, 1);
                printStory(40, 750, 600, "任意键退出", 450, 1);
                FlushBatchDraw();
                settextcolor(BLACK);
                arrC[18] = 1;
                getmessage(&m, EM_KEY);
                getmessage(&m, EM_KEY);
            }
        }
        else {
            cls();
            backA();
            putimage_alpha(200, 50, &po);
            settextcolor(RED);
            printStory(50, 270, 170, "这么做，你自然是可以成功的，所以，后面的事情，我就不必再赘述了，这是一个好结局", 900, 2);
            printStory(40, 200, 600, "等级：A", 450, 1);
            printStory(40, 750, 600, "任意键退出", 450, 1);
            FlushBatchDraw();
            settextcolor(BLACK);
            arrA[19] = 1;
            getmessage(&m, EM_KEY);
            getmessage(&m, EM_KEY);
        }
    }
}

void funa() {
    IMAGE po;
    loadimage(&po, "res\\pir\\po.png");
    IMAGE pos;
    loadimage(&pos, "res\\pir\\pos.png");
    cls();
    backA();
    putimage_alpha(200, 50, &po);
    printStory(40, 270, 170, "深耕于核心技术", 900, 1);
    printStory(35, 200, 600, "继续研发", 450, 1);
    printStory(35, 750, 600, "放弃研发", 450, 1);
    FlushBatchDraw();
    getmessage(&m, EM_KEY);
    getmessage(&m, EM_KEY);
    if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
        cls();
        backA();
        putimage_alpha(200, 50, &po);
        printStory(40, 270, 170, "研发成功", 900, 1);
        printStory(35, 200, 600, "将其运用于农业", 450, 1);
        printStory(35, 750, 600, "垄断全球北斗技术", 450, 1);
        FlushBatchDraw();
        getmessage(&m, EM_KEY);
        getmessage(&m, EM_KEY);
        if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
            cls();
            backA();
            putimage_alpha(200, 50, &po);
            printStory(40, 270, 170, "农业大获成功", 900, 1);
            printStory(35, 200, 600, "召开五届会议，提出北斗产业振兴农业只是第一步！", 450, 1);
            printStory(35, 750, 600, "召开五届会议，提出接下来要多快好省地发展其他产业", 450, 1);
            FlushBatchDraw();
            getmessage(&m, EM_KEY);
            getmessage(&m, EM_KEY);
            if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
                cls();
                backA();
                putimage_alpha(200, 50, &po);
                printStory(40, 270, 170, "举国欢腾", 900, 1);
                printStory(35, 200, 600, "开展北斗+相关产业", 450, 1);
                printStory(35, 750, 600, "将北斗应用在军事方面", 450, 1);
                FlushBatchDraw();
                getmessage(&m, EM_KEY);
                getmessage(&m, EM_KEY);
                if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
                    cls();
                    backA();
                    putimage_alpha(200, 50, &po);
                    printStory(40, 270, 170, "便利了人民的生活", 900, 1);
                    printStory(35, 200, 600, "召开六届会议，提出“重返乡村”，全面开始脱贫致富战略！", 450, 1);
                    printStory(35, 750, 600, "其他", 450, 1);
                    FlushBatchDraw();
                    getmessage(&m, EM_KEY);
                    getmessage(&m, EM_KEY);
                    if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
                        cls();
                        backA();
                        putimage_alpha(200, 50, &po);
                        settextcolor(RED);
                        printStory(50, 270, 170, "你在高原修铁路,在沙漠修高速,在山区架起“天桥”.经过一番努力，脱贫致富攻坚战圆满完成，取之于民，还之于民，人民万岁！", 900, 2);
                        printStory(40, 200, 600, "等级：S", 450, 1);
                        printStory(40, 750, 600, "任意键退出", 450, 1);
                        FlushBatchDraw();
                        settextcolor(BLACK);
                        arrS[0] = 1;
                        getmessage(&m, EM_KEY);
                        getmessage(&m, EM_KEY);
                    }
                    else {
                        cls();
                        backA();
                        putimage_alpha(200, 50, &po);
                        settextcolor(RED);
                        printStory(50, 270, 170, "666编不下去了，虾片不会看到这吧", 900, 2);
                        printStory(40, 200, 600, "等级：R", 450, 1);
                        printStory(40, 750, 600, "任意键退出", 450, 1);
                        FlushBatchDraw();
                        settextcolor(BLACK);
                        arrR[4] = 1;
                        getmessage(&m, EM_KEY);
                        getmessage(&m, EM_KEY);
                        cd();
                    }
                }
                else {
                    cls();
                    backA();
                    putimage_alpha(200, 50, &po);
                    printStory(40, 270, 170, "军事实力显著提升", 900, 1);
                    printStory(35, 200, 600, "向他国开战！", 450, 1);
                    printStory(35, 750, 600, "与各国友好交往", 450, 1);
                    FlushBatchDraw();
                    getmessage(&m, EM_KEY);
                    getmessage(&m, EM_KEY);
                    if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
                        int b4 = rand() % 4;
                        if (b4 < 3) {
                            cls();
                            backA();
                            putimage_alpha(200, 50, &pos);
                            printStory(40, 270, 170, "成功，你成为了世界的霸主！", 900, 1);
                            printStory(35, 200, 600, "你掠夺其他地区的财产", 450, 1);
                            printStory(35, 750, 600, "你主导了全球", 450, 1);
                            FlushBatchDraw();
                            getmessage(&m, EM_KEY);
                            getmessage(&m, EM_KEY);
                            if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
                                cls();
                                backA();
                                putimage_alpha(200, 50, &po);
                                settextcolor(RED);
                                printStory(50, 270, 170, "这难道不是另一个M国吗，虽然看似是霸主", 900, 2);
                                printStory(40, 200, 600, "等级：A-", 450, 1);
                                printStory(40, 750, 600, "任意键退出", 450, 1);
                                FlushBatchDraw();
                                settextcolor(BLACK);
                                arrA[1] = 1;
                                getmessage(&m, EM_KEY);
                                getmessage(&m, EM_KEY);
                            }
                            else {
                                cls();
                                backA();
                                putimage_alpha(200, 50, &po);
                                settextcolor(RED);
                                printStory(50, 270, 170, "会不会重蹈覆辙M国，仍是后事!", 900, 2);
                                printStory(40, 200, 600, "等级：A", 450, 1);
                                printStory(40, 750, 600, "任意键退出", 450, 1);
                                FlushBatchDraw();
                                settextcolor(BLACK);
                                arrA[2] = 1;
                                getmessage(&m, EM_KEY);
                                getmessage(&m, EM_KEY);
                            }
                        }
                        else {
                            cls();
                            backA();
                            putimage_alpha(200, 50, &po);
                            printStory(40, 270, 170, "失败，你被摔了个粉碎", 900, 1);
                            printStory(35, 200, 600, "你向他们缴纳了赔款", 450, 1);
                            printStory(35, 750, 600, "die", 450, 1);
                            FlushBatchDraw();
                            getmessage(&m, EM_KEY);
                            getmessage(&m, EM_KEY);
                            if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
                                cls();
                                backA();
                                putimage_alpha(200, 50, &po);
                                settextcolor(RED);
                                printStory(50, 270, 170, "屈辱地活着", 900, 2);
                                printStory(40, 200, 600, "等级：B", 450, 1);
                                printStory(40, 750, 600, "任意键退出", 450, 1);
                                FlushBatchDraw();
                                settextcolor(BLACK);
                                arrB[3] = 1;
                                getmessage(&m, EM_KEY);
                                getmessage(&m, EM_KEY);
                            }
                            else {
                                cls();
                                backA();
                                putimage_alpha(200, 50, &po);
                                settextcolor(RED);
                                printStory(50, 270, 170, "身死而国灭", 900, 2);
                                printStory(40, 200, 600, "等级：B", 450, 1);
                                printStory(40, 750, 600, "任意键退出", 450, 1);
                                FlushBatchDraw();
                                settextcolor(BLACK);
                                arrB[4] = 1;
                                getmessage(&m, EM_KEY);
                                getmessage(&m, EM_KEY);
                            }
                        }
                    }
                    else {
                        cls();
                        backA();
                        putimage_alpha(200, 50, &po);
                        settextcolor(RED);
                        printStory(50, 270, 170, "在你的带领下，世界安然无恙！这是全人类的福音", 900, 2);
                        printStory(40, 200, 600, "等级：S", 450, 1);
                        printStory(40, 750, 600, "任意键退出", 450, 1);
                        FlushBatchDraw();
                        settextcolor(BLACK);
                        arrS[1] = 1;
                        getmessage(&m, EM_KEY);
                        getmessage(&m, EM_KEY);
                    }
                }
            }
            else {
                cls();
                backA();
                putimage_alpha(200, 50, &po);
                printStory(40, 270, 170, "举国鼎沸,全国建立多个大型厂区，但是似乎有点太快了......", 900, 1);
                printStory(35, 200, 600, "你开始思考", 450, 1);
                printStory(35, 750, 600, "不管事，继续干", 450, 1);
                FlushBatchDraw();
                getmessage(&m, EM_KEY);
                getmessage(&m, EM_KEY);
                if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
                    cls();
                    backA();
                    putimage_alpha(200, 50, &po);
                    printStory(40, 270, 170, "你怀疑你最信任的“五人小组”", 900, 1);
                    printStory(35, 200, 600, "成立专案组调查", 450, 1);
                    printStory(35, 750, 600, "你不动声色，因为你是最了解他们的人啊！", 450, 1);
                    FlushBatchDraw();
                    getmessage(&m, EM_KEY);
                    getmessage(&m, EM_KEY);
                    if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
                        cls();
                        backA();
                        putimage_alpha(200, 50, &po);
                        printStory(40, 270, 170, "五人帮（等同于“五人小组”）有强烈的反侦查意识，没有透露东西。你对他们彻底放下疑心。与此同时，五人帮认为国家发展的主要问题是资产阶级复辟等相关问题，想要开展“思想革新运动”", 900, 1);
                        printStory(35, 200, 600, "同意", 450, 1);
                        printStory(35, 750, 600, "不同意", 450, 1);
                        FlushBatchDraw();
                        getmessage(&m, EM_KEY);
                        getmessage(&m, EM_KEY);
                        if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
                            cls();
                            backA();
                            putimage_alpha(200, 50, &po);
                            printStory(40, 270, 170, "你开始大肆批斗一些“ 反动人物”，当然，其中不乏有被冤枉的，但是，我们每个人都身处历史的洪流之中，您又怎么能看透呢？", 900, 1);
                            printStory(30, 200, 600, "你开始对五人帮起疑（当然，身处历史洪流的你，又怎能意识得到呢？）", 450, 1);
                            printStory(35, 750, 600, "你没有怀疑五人帮", 450, 1);
                            FlushBatchDraw();
                            getmessage(&m, EM_KEY);
                            getmessage(&m, EM_KEY);
                            if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
                                cls();
                                backA();
                                putimage_alpha(200, 50, &po);
                                printStory(40, 270, 170, "你开始调查他们，最终，你经过多方证实，确实了他们的罪行。", 900, 1);
                                printStory(30, 200, 600, "平反一些人", 450, 1);
                                printStory(35, 750, 600, "你仍然保持着原来的方式不变", 450, 1);
                                FlushBatchDraw();
                                getmessage(&m, EM_KEY);
                                getmessage(&m, EM_KEY);
                                if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
                                    cls();
                                    backA();
                                    putimage_alpha(200, 50, &po);
                                    printStory(40, 270, 170, "受害者家属得到安慰", 900, 1);
                                    printStory(30, 200, 600, "你最后召开一次大会，总结在于五人小组斗争中的经验", 450, 1);
                                    printStory(35, 750, 600, "你决定——？", 450, 1);
                                    FlushBatchDraw();
                                    getmessage(&m, EM_KEY);
                                    getmessage(&m, EM_KEY);
                                    if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
                                        cls();
                                        backA();
                                        putimage_alpha(200, 50, &po);
                                        settextcolor(RED);
                                        printStory(50, 270, 170, "国家恢复了正常，接下来呢？你安度晚年，这难道不好吗？", 900, 2);
                                        printStory(40, 200, 600, "等级：A+", 450, 1);
                                        printStory(40, 750, 600, "任意键退出", 450, 1);
                                        FlushBatchDraw();
                                        settextcolor(BLACK);
                                        arrA[5] = 1;
                                        getmessage(&m, EM_KEY);
                                        getmessage(&m, EM_KEY);
                                    }
                                    else {
                                        cls();
                                        backA();
                                        putimage_alpha(200, 50, &po);
                                        printStory(40, 270, 170, "你决定走上一条不一样的小路......", 900, 1);
                                        printStory(30, 200, 600, "你决定从沿海城市开始，让一部分人先富有起来", 450, 1);
                                        printStory(35, 750, 600, "其他", 450, 1);
                                        FlushBatchDraw();
                                        getmessage(&m, EM_KEY);
                                        getmessage(&m, EM_KEY);
                                        if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
                                            cls();
                                            backA();
                                            putimage_alpha(200, 50, &po);
                                            settextcolor(RED);
                                            printStory(50, 270, 170, "当然你这个决定遭到了不少人的反对，最后，你去世了。不是以为此。而是身体原因。太↗好→了↘(CY口音）", 900, 2);
                                            printStory(40, 200, 600, "等级：A+", 450, 1);
                                            printStory(40, 750, 600, "任意键退出", 450, 1);
                                            FlushBatchDraw();
                                            settextcolor(BLACK);
                                            arrA[6] = 1;
                                            getmessage(&m, EM_KEY);
                                            getmessage(&m, EM_KEY);
                                        }
                                        else {
                                            cls();
                                            backA();
                                            putimage_alpha(200, 50, &po);
                                            settextcolor(RED);
                                            printStory(50, 270, 170, "卧槽，真的tm的编不下去了，作者在写这段代码时已经0：30了，算了，洗洗睡了", 900, 2);
                                            printStory(40, 200, 600, "等级：R+", 450, 1);
                                            printStory(40, 750, 600, "任意键退出", 450, 1);
                                            FlushBatchDraw();
                                            settextcolor(BLACK);
                                            arrR[5] = 1;
                                            getmessage(&m, EM_KEY);
                                            getmessage(&m, EM_KEY);
                                        }
                                    }
                                }
                                else {
                                    cls();
                                    backA();
                                    putimage_alpha(200, 50, &po);
                                    printStory(40, 270, 170, "渐渐地，你发现国家在走下坡路，你开始思考，是否国家需要一场大刀阔斧的改革，但是，你纵观你的前半生，研发新技术，发展农业......但是时代在发展，你也已经老了。“天要下雨，娘要嫁人，随他去吧！”", 900, 1);
                                    printStory(30, 200, 600, "你知了天命", 450, 1);
                                    printStory(35, 750, 600, "你不屈于天命", 450, 1);
                                    FlushBatchDraw();
                                    getmessage(&m, EM_KEY);
                                    getmessage(&m, EM_KEY);

                                    if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
                                        cls();
                                        backA();
                                        putimage_alpha(200, 50, &po);
                                        settextcolor(RED);
                                        printStory(50, 270, 170, "你安详地过完了晚年，没有大刀阔斧的改革，因为你真的老了，你想起自己年轻时意气风发的样子，还有点怀念嘞", 900, 2);
                                        printStory(40, 200, 600, "等级：A+", 450, 1);
                                        printStory(40, 750, 600, "任意键退出", 450, 1);
                                        FlushBatchDraw();
                                        settextcolor(BLACK);
                                        arrA[7] = 1;
                                        getmessage(&m, EM_KEY);
                                        getmessage(&m, EM_KEY);
                                    }
                                    else {
                                        cls();
                                        backA();
                                        putimage_alpha(200, 50, &po);
                                        settextcolor(RED);
                                        printStory(40, 270, 170, "用尽了自己最后一丝气力，但是正如先前所说“天要下雨，娘要嫁人，随他去吧！”是啊随他去吧，你看着这个没有完全改革的大虾帝国，你留下遗嘱“继续革命，为了大虾帝国！”......至于说，下任总统怎么样，随他去吧", 900, 2);
                                        printStory(40, 200, 600, "等级：S-", 450, 1);
                                        printStory(40, 750, 600, "任意键退出", 450, 1);
                                        FlushBatchDraw();
                                        settextcolor(BLACK);
                                        arrS[2] = 1;
                                        getmessage(&m, EM_KEY);
                                        getmessage(&m, EM_KEY);
                                    }
                                }
                            }
                            else {
                                cls();
                                backA();
                                putimage_alpha(200, 50, &po);
                                printStory(40, 270, 170, "可以说，你没有怀疑过他们，直到他们把枪架在你的头上！", 900, 1);
                                printStory(35, 200, 600, "你向他们求饶", 450, 1);
                                printStory(35, 750, 600, "你英勇就义！", 450, 1);
                                FlushBatchDraw();
                                getmessage(&m, EM_KEY);
                                getmessage(&m, EM_KEY);
                                if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
                                    int b5 = rand() % 10;
                                    if (b5 < 7) {
                                        cls();
                                        backA();
                                        putimage_alpha(200, 50, &pos);
                                        settextcolor(RED);
                                        printStory(50, 270, 170, "他们不同意.你死了", 900, 2);
                                        printStory(40, 200, 600, "等级：C", 450, 1);
                                        printStory(40, 750, 600, "任意键退出", 450, 1);
                                        FlushBatchDraw();
                                        settextcolor(BLACK);
                                        arrC[4] = 1;
                                        getmessage(&m, EM_KEY);
                                        getmessage(&m, EM_KEY);
                                    }
                                    else {
                                        cls();
                                        backA();
                                        putimage_alpha(200, 50, &pos);
                                        settextcolor(RED);
                                        printStory(50, 270, 170, "他们同意。你苟且偷生", 900, 2);
                                        printStory(40, 200, 600, "等级：B-", 450, 1);
                                        printStory(40, 750, 600, "任意键退出", 450, 1);
                                        FlushBatchDraw();
                                        settextcolor(BLACK);
                                        arrB[8] = 1;
                                        getmessage(&m, EM_KEY);
                                        getmessage(&m, EM_KEY);
                                    }
                                }
                                else {
                                    cls();
                                    backA();
                                    putimage_alpha(200, 50, &po);
                                    settextcolor(RED);
                                    printStory(50, 270, 170, "那就是结局了……", 900, 2);
                                    printStory(40, 200, 600, "等级：A", 450, 1);
                                    printStory(40, 750, 600, "任意键退出", 450, 1);
                                    FlushBatchDraw();
                                    settextcolor(BLACK);
                                    arrA[8] = 1;
                                    getmessage(&m, EM_KEY);
                                    getmessage(&m, EM_KEY);
                                }
                            }
                        }
                        else {
                            cls();
                            backA();
                            putimage_alpha(200, 50, &po);
                            printStory(40, 270, 170, "他们依然猖獗", 900, 1);
                            printStory(35, 200, 600, "你意识到不对，派人斩杀五人帮", 450, 1);
                            printStory(35, 750, 600, "你拒绝服从五人帮", 450, 1);
                            FlushBatchDraw();
                            getmessage(&m, EM_KEY);
                            getmessage(&m, EM_KEY);
                            if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
                                cls();
                                backA();
                                putimage_alpha(200, 50, &po);
                                settextcolor(RED);
                                printStory(50, 270, 170, "但失败。你是五人帮政治博弈的牺牲品", 900, 2);
                                printStory(40, 200, 600, "等级：B", 450, 1);
                                printStory(40, 750, 600, "任意键退出", 450, 1);
                                FlushBatchDraw();
                                settextcolor(BLACK);
                                arrB[7] = 1;
                                getmessage(&m, EM_KEY);
                                getmessage(&m, EM_KEY);
                            }
                            else {
                                cls();
                                backA();
                                putimage_alpha(200, 50, &po);
                                printStory(40, 270, 170, "几乎没用", 900, 1);
                                printStory(35, 200, 600, "留下遗嘱秘密给下代接班人：清算五人帮", 450, 1);
                                printStory(35, 750, 600, "你写诗明志", 450, 1);
                                FlushBatchDraw();
                                getmessage(&m, EM_KEY);
                                getmessage(&m, EM_KEY);
                                if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
                                    cls();
                                    backA();
                                    putimage_alpha(200, 50, &po);
                                    settextcolor(RED);
                                    printStory(50, 270, 170, "英勇就义", 900, 2);
                                    printStory(40, 200, 600, "等级：A", 450, 1);
                                    printStory(40, 750, 600, "任意键退出", 450, 1);
                                    FlushBatchDraw();
                                    settextcolor(BLACK);
                                    arrA[3] = 1;
                                    getmessage(&m, EM_KEY);
                                    getmessage(&m, EM_KEY);
                                }
                                else {
                                    cls();
                                    backA();
                                    putimage_alpha(200, 50, &po);
                                    settextcolor(RED);
                                    printStory(50, 270, 170, "不屈服地死去了！", 900, 2);
                                    printStory(40, 200, 600, "等级：A", 450, 1);
                                    printStory(40, 750, 600, "任意键退出", 450, 1);
                                    FlushBatchDraw();
                                    settextcolor(BLACK);
                                    arrA[4] = 1;
                                    getmessage(&m, EM_KEY);
                                    getmessage(&m, EM_KEY);
                                }
                            }
                        }
                    }
                    else {
                        cls();
                        backA();
                        putimage_alpha(200, 50, &po);
                        printStory(40, 270, 170, "你找到机会，把他们一锅端！您开始大力批斗五人帮。渐渐地，你发现国家需要一场思想革命，于是，你开始了思想大革命，你开始批斗他人，你为了保住自己的下一任期，批斗并且暗杀了国家副总统，社会各界反向批斗你", 900, 1);
                        printStory(35, 200, 600, "你不管事，与这个社会为敌", 450, 1);
                        printStory(35, 750, 600, "你试图压制舆论", 450, 1);
                        FlushBatchDraw();
                        getmessage(&m, EM_KEY);
                        getmessage(&m, EM_KEY);
                        if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
                            cls();
                            backA();
                            putimage_alpha(200, 50, &po);
                            printStory(40, 270, 170, "新总统最终还是上任，你被弹劾", 900, 1);
                            printStory(35, 200, 600, "a", 450, 1);
                            printStory(35, 750, 600, "b", 450, 1);
                            FlushBatchDraw();
                            getmessage(&m, EM_KEY);
                            getmessage(&m, EM_KEY);
                            if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
                                cls();
                                backA();
                                putimage_alpha(200, 50, &po);
                                settextcolor(RED);
                                printStory(50, 270, 170, "你在狱中死亡", 900, 2);
                                printStory(40, 200, 600, "等级：B", 450, 1);
                                printStory(40, 750, 600, "任意键退出", 450, 1);
                                FlushBatchDraw();
                                settextcolor(BLACK);
                                arrB[9] = 1;
                                getmessage(&m, EM_KEY);
                                getmessage(&m, EM_KEY);
                            }
                            else {
                                cls();
                                backA();
                                putimage_alpha(200, 50, &po);
                                settextcolor(RED);
                                printStory(50, 270, 170, "你被保释，但是最终屈辱而死", 900, 2);
                                printStory(40, 200, 600, "等级：B-", 450, 1);
                                printStory(40, 750, 600, "任意键退出", 450, 1);
                                FlushBatchDraw();
                                settextcolor(BLACK);
                                arrB[10] = 1;
                                getmessage(&m, EM_KEY);
                                getmessage(&m, EM_KEY);
                            }
                        }
                        else {
                            cls();
                            backA();
                            putimage_alpha(200, 50, &po);
                            settextcolor(RED);
                            printStory(50, 270, 170, "没用！你被摔个粉碎！", 900, 2);
                            printStory(40, 200, 600, "等级：C", 450, 1);
                            printStory(40, 750, 600, "任意键退出", 450, 1);
                            FlushBatchDraw();
                            settextcolor(BLACK);
                            arrC[5] = 1;
                            getmessage(&m, EM_KEY);
                            getmessage(&m, EM_KEY);
                        }
                    }
                }
                else {
                    cls();
                    backA();
                    putimage_alpha(200, 50, &po);
                    printStory(40, 270, 170, "你发现生产的东西质量严重不合格！", 900, 1);
                    printStory(35, 200, 600, "大怒，叫停全国产业", 450, 1);
                    printStory(35, 750, 600, "捣毁所有涉及企业", 450, 1);
                    FlushBatchDraw();
                    getmessage(&m, EM_KEY);
                    getmessage(&m, EM_KEY);
                    if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
                        cls();
                        backA();
                        putimage_alpha(200, 50, &po);
                        settextcolor(RED);
                        printStory(50, 270, 170, "你至死不知原因，只知国家一夜之间经济衰弱", 900, 2);
                        printStory(40, 200, 600, "等级：B", 450, 1);
                        printStory(40, 750, 600, "任意键退出", 450, 1);
                        FlushBatchDraw();
                        settextcolor(BLACK);
                        arrB[5] = 1;
                        getmessage(&m, EM_KEY);
                        getmessage(&m, EM_KEY);
                    }
                    else {
                        cls();
                        backA();
                        putimage_alpha(200, 50, &po);
                        settextcolor(RED);
                        printStory(50, 270, 170, "你至死不明白为什么，难道...难道...是我最信任的五人小组？", 900, 2);
                        printStory(40, 200, 600, "等级：B+", 450, 1);
                        printStory(40, 750, 600, "任意键退出", 450, 1);
                        FlushBatchDraw();
                        settextcolor(BLACK);
                        arrB[6] = 1;
                        getmessage(&m, EM_KEY);
                        getmessage(&m, EM_KEY);
                    }
                }
            }
        }
        else {
            cls();
            backA();
            putimage_alpha(200, 50, &po);
            printStory(40, 270, 170, "获得大量资金", 900, 1);
            printStory(35, 200, 600, "将北斗技术收费使用，同时扼制其他国家研发北斗技术的门道", 450, 1);
            printStory(35, 750, 600, "制裁M国，在M国开展军事演练时关闭北斗系统", 450, 1);
            FlushBatchDraw();
            getmessage(&m, EM_KEY);
            getmessage(&m, EM_KEY);
            if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
                cls();
                backA();
                putimage_alpha(200, 50, &po);
                printStory(40, 270, 170, "国家经济持续走高230，但是M国联合其他国家发出异议，声明应保障他们的“合法权益”", 900, 1);
                printStory(35, 200, 600, "以四倍价钱出卖假技术", 450, 1);
                printStory(35, 750, 600, "坚决保障技术壁垒，不让步迁就", 450, 1);
                FlushBatchDraw();
                getmessage(&m, EM_KEY);
                getmessage(&m, EM_KEY);
                if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
                    cls();
                    backA();
                    putimage_alpha(200, 50, &po);
                    printStory(40, 270, 170, "国家经济持续走高230，但是M国联合其他国家发出异议，声明应保障他们的“合法权益”", 900, 1);
                    printStory(35, 200, 600, "以四倍价钱出卖假技术", 450, 1);
                    printStory(35, 750, 600, "坚决保障技术壁垒，不让步迁就", 450, 1);
                    FlushBatchDraw();
                    getmessage(&m, EM_KEY);
                    getmessage(&m, EM_KEY);
                    int b6 = rand() % 10;
                    if (b6 < 7) {
                        cls();
                        backA();
                        putimage_alpha(200, 50, &pos);
                        settextcolor(RED);
                        printStory(50, 270, 170, "M国不是“失败”，他是有脑子的，你被发现了，M国表示将与你开战.和强大的M国开战是必然会失败的。哎，哄哄碰碰哄哄碰碰(CY口音）", 900, 2);
                        printStory(40, 200, 600, "等级：C", 450, 1);
                        printStory(40, 750, 600, "任意键退出", 450, 1);
                        FlushBatchDraw();
                        settextcolor(BLACK);
                        arrC[6] = 1;
                        getmessage(&m, EM_KEY);
                        getmessage(&m, EM_KEY);
                    }
                    else {
                        cls();
                        backA();
                        putimage_alpha(200, 50, &pos);
                        printStory(40, 270, 170, "M国没有发现，他们投入了大量国力，但是失败了，他们直到死亡也不知道原因.M国的实力大幅下降", 900, 1);
                        printStory(35, 200, 600, "你让M国成为你的附庸", 450, 1);
                        printStory(35, 750, 600, "你选择了一条其他的路", 450, 1);
                        FlushBatchDraw();
                        getmessage(&m, EM_KEY);
                        getmessage(&m, EM_KEY);
                        if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
                            cls();
                            backA();
                            putimage_alpha(200, 50, &po);
                            settextcolor(RED);
                            printStory(50, 270, 170, "国内反响十分好，你成为了一代贤君", 900, 2);
                            printStory(40, 200, 600, "等级：A+", 450, 1);
                            printStory(40, 750, 600, "任意键退出", 450, 1);
                            FlushBatchDraw();
                            settextcolor(BLACK);
                            arrA[9] = 1;
                            getmessage(&m, EM_KEY);
                            getmessage(&m, EM_KEY);
                        }
                        else {
                            cls();
                            backA();
                            putimage_alpha(200, 50, &po);
                            settextcolor(RED);
                            printStory(50, 270, 170, "好吧我坦白了，我其实不知道怎么编了，这段代码于2.16号完成，也就是除夕，在这里虽然有点晚，但是还是住大家春节快乐！", 900, 2);
                            printStory(40, 200, 600, "等级：R+", 450, 1);
                            printStory(40, 750, 600, "任意键退出", 450, 1);
                            FlushBatchDraw();
                            settextcolor(BLACK);
                            arrR[6] = 1;
                            getmessage(&m, EM_KEY);
                            getmessage(&m, EM_KEY);
                            cd();
                        }
                    }
                }
                else {
                    cls();
                    backA();
                    putimage_alpha(200, 50, &po);
                    printStory(40, 270, 170, "M国气急败坏，打响贸易战，加收200%的关税", 900, 1);
                    printStory(35, 200, 600, "依旧不让步，保障自身安全", 450, 1);
                    printStory(35, 750, 600, "你让步了", 450, 1);
                    FlushBatchDraw();
                    getmessage(&m, EM_KEY);
                    getmessage(&m, EM_KEY);
                    if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
                        cls();
                        backA();
                        putimage_alpha(200, 50, &po);
                        printStory(40, 270, 170, "双方在打消耗战，但是随着消耗战的打响，M国粮食告急，但是由于M国的极端政策，几乎没有国家愿意出口粮食给他，与此同时，我国运用北斗技术，今年粮食又是大丰收！", 900, 1);
                        printStory(35, 200, 600, "以16倍价格出口给M国", 450, 1);
                        printStory(35, 750, 600, "见死不救，等待M国找自己", 450, 1);
                        FlushBatchDraw();
                        getmessage(&m, EM_KEY);
                        getmessage(&m, EM_KEY);
                        if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
                            func();
                        }
                        else {
                            int b7 = rand() % 10;
                            if (b7 < 7) {
                                cls();
                                backA();
                                putimage_alpha(200, 50, &pos);
                                printStory(40, 270, 170, "M国主动找你，你提出3200%的涨价,M国咬牙答应", 900, 1);
                                printStory(35, 200, 600, "你要求M国和你签订《二一五条约》", 450, 1);
                                printStory(35, 750, 600, "你不再管M国，只仅仅享受每年100w的资金，你开始大力发展经济", 450, 1);
                                FlushBatchDraw();
                                getmessage(&m, EM_KEY);
                                getmessage(&m, EM_KEY);
                                if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
                                    int b8 = rand() % 10;
                                    if (b8 < 7) {
                                        funb();
                                    }
                                    else {
                                        func();
                                    }
                                }
                                else {
                                    cls();
                                    backA();
                                    putimage_alpha(200, 50, &po);
                                    printStory(40, 270, 170, "经过10年的发展，你的经济体量早已超越M国", 900, 1);
                                    printStory(35, 200, 600, "你开始思考人民需要什么？……", 450, 1);
                                    printStory(35, 750, 600, "你开始思考自己需要什么？……", 450, 1);
                                    FlushBatchDraw();
                                    getmessage(&m, EM_KEY);
                                    getmessage(&m, EM_KEY);
                                    if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
                                        cls();
                                        backA();
                                        putimage_alpha(200, 50, &po);
                                        printStory(40, 270, 170, "你开始了伟大的脱贫攻坚", 900, 1);
                                        printStory(35, 200, 600, "你让那些陪你走来的农民们富了起来！", 450, 1);
                                        printStory(35, 750, 600, "你让那些贫困的人小康了起来", 450, 1);
                                        FlushBatchDraw();
                                        getmessage(&m, EM_KEY);
                                        getmessage(&m, EM_KEY);
                                        if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
                                            cls();
                                            backA();
                                            putimage_alpha(200, 50, &po);
                                            settextcolor(RED);
                                            printStory(50, 270, 170, "人民万岁!", 900, 2);
                                            printStory(40, 200, 600, "等级：S", 450, 1);
                                            printStory(40, 750, 600, "任意键退出", 450, 1);
                                            FlushBatchDraw();
                                            settextcolor(BLACK);
                                            arrS[3] = 1;
                                            getmessage(&m, EM_KEY);
                                            getmessage(&m, EM_KEY);
                                        }
                                        else {
                                            cls();
                                            backA();
                                            putimage_alpha(200, 50, &po);
                                            settextcolor(RED);
                                            printStory(50, 270, 170, "那个理想的社会还有距离，但是你已经老了，你用了全身气力于历史周期打了个平手！", 900, 2);
                                            printStory(40, 200, 600, "等级：S+", 450, 1);
                                            printStory(40, 750, 600, "任意键退出", 450, 1);
                                            FlushBatchDraw();
                                            settextcolor(BLACK);
                                            arrS[4] = 1;
                                            getmessage(&m, EM_KEY);
                                            getmessage(&m, EM_KEY);
                                        }
                                    }
                                    else {
                                        cls();
                                        backA();
                                        putimage_alpha(200, 50, &po);
                                        printStory(40, 270, 170, "……", 900, 1);
                                        printStory(35, 200, 600, "你挪用公款，只为饮酒作乐，你纳妾3000！", 450, 1);
                                        printStory(35, 750, 600, "你想要寻求长生不死，于是派人大力研究化学！", 450, 1);
                                        FlushBatchDraw();
                                        getmessage(&m, EM_KEY);
                                        getmessage(&m, EM_KEY);
                                        if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
                                            cls();
                                            backA();
                                            putimage_alpha(200, 50, &po);
                                            printStory(40, 270, 170, "你也失去了民心", 900, 1);
                                            printStory(35, 200, 600, "您觉得不对劲，跑到了国外", 450, 1);
                                            printStory(35, 750, 600, "你不想走，觉得没有必要", 450, 1);
                                            FlushBatchDraw();
                                            getmessage(&m, EM_KEY);
                                            getmessage(&m, EM_KEY);
                                            if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
                                                cls();
                                                backA();
                                                putimage_alpha(200, 50, &po);
                                                settextcolor(RED);
                                                printStory(50, 270, 170, "你依旧过着骄奢淫逸的生活", 900, 2);
                                                printStory(40, 200, 600, "等级：A-", 450, 1);
                                                printStory(40, 750, 600, "任意键退出", 450, 1);
                                                FlushBatchDraw();
                                                settextcolor(BLACK);
                                                arrA[10] = 1;
                                                getmessage(&m, EM_KEY);
                                                getmessage(&m, EM_KEY);
                                            }
                                            else {
                                                cls();
                                                backA();
                                                putimage_alpha(200, 50, &po);
                                                settextcolor(RED);
                                                printStory(50, 270, 170, "你被不满的农民割下首级！", 900, 2);
                                                printStory(40, 200, 600, "等级：C-", 450, 1);
                                                printStory(40, 750, 600, "任意键退出", 450, 1);
                                                FlushBatchDraw();
                                                settextcolor(BLACK);
                                                arrC[7] = 1;
                                                getmessage(&m, EM_KEY);
                                                getmessage(&m, EM_KEY);
                                            }
                                        }
                                        else {
                                            int b9 = rand() % 10;
                                            if (b9 == 0) {
                                                cls();
                                                backA();
                                                putimage_alpha(200, 50, &pos);
                                                settextcolor(RED);
                                                printStory(50, 270, 170, "他们误打误撞研究得了诺贝尔奖,与此同时，你也暴毙，你获得了一个不错的身后名", 900, 2);
                                                printStory(40, 200, 600, "等级：A+", 450, 1);
                                                printStory(40, 750, 600, "任意键退出", 450, 1);
                                                FlushBatchDraw();
                                                settextcolor(BLACK);
                                                arrA[11] = 1;
                                                getmessage(&m, EM_KEY);
                                                getmessage(&m, EM_KEY);
                                            }
                                            else {
                                                cls();
                                                backA();
                                                putimage_alpha(200, 50, &pos);
                                                settextcolor(RED);
                                                printStory(50, 270, 170, "他们什么都没有研究出来，反而浪费了大量资金||你被气死了", 900, 2);
                                                printStory(40, 200, 600, "等级：C+", 450, 1);
                                                printStory(40, 750, 600, "任意键退出", 450, 1);
                                                FlushBatchDraw();
                                                settextcolor(BLACK);
                                                arrC[8] = 1;
                                                getmessage(&m, EM_KEY);
                                                getmessage(&m, EM_KEY);
                                            }
                                        }
                                    }
                                }
                            }
                            else {
                                funb();
                            }
                        }
                    }
                    else {
                        cls();
                        backA();
                        putimage_alpha(200, 50, &po);
                        settextcolor(RED);
                        printStory(50, 270, 170, "你tmd看看你是人吗？骨气呢？比虾片都无能！你被群众集体弹劾了！", 900, 2);
                        printStory(40, 200, 600, "等级：D", 450, 1);
                        printStory(40, 750, 600, "任意键退出", 450, 1);
                        FlushBatchDraw();
                        settextcolor(BLACK);
                        arrD[6] = 1;
                        getmessage(&m, EM_KEY);
                        getmessage(&m, EM_KEY);
                    }
                }
            }
            else {
                funb();
            }
        }
    }
    else {
        int b3 = rand() % 2;
        if (b3 == 0) {
            cls();
            backA();
            putimage_alpha(200, 50, &pos);
            settextcolor(RED);
            printStory(50, 270, 170, "被农民暗杀", 900, 2);
            printStory(40, 200, 600, "等级：C", 450, 1);
            printStory(40, 750, 600, "任意键退出", 450, 1);
            FlushBatchDraw();
            settextcolor(BLACK);
            arrC[3] = 1;
            getmessage(&m, EM_KEY);
            getmessage(&m, EM_KEY);
        }
        else {
            cls();
            backA();
            putimage_alpha(200, 50, &pos);
            settextcolor(RED);
            printStory(50, 270, 170, "自己愧对自己，自缢", 900, 2);
            printStory(40, 200, 600, "等级：B-", 450, 1);
            printStory(40, 750, 600, "任意键退出", 450, 1);
            FlushBatchDraw();
            settextcolor(BLACK);
            arrB[2] = 1;
            getmessage(&m, EM_KEY);
            getmessage(&m, EM_KEY);
        }
    }
}

void funr() {
    IMAGE po;
    loadimage(&po, "res\\pir\\po.png");
    IMAGE pos;
    loadimage(&pos, "res\\pir\\pos.png");
    IMAGE n1;
    loadimage(&n1, "res\\pir\\ncd1.jpg");
    IMAGE n2;
    loadimage(&n2, "res\\pir\\ncd2.jpg");
    IMAGE n3;
    loadimage(&n3, "res\\pir\\ncd3.jpg");
    cls();
    backA();
    putimage_alpha(200, 50, &po);
    printStory(40, 270, 170, "你和洋盆，娃娃同行", 900, 1);
    printStory(35, 200, 600, "你选择让娃娃用无人机探路", 450, 1);
    printStory(35, 750, 600, "你选择欣赏大美春光", 450, 1);
    FlushBatchDraw();
    getmessage(&m, EM_KEY);
    getmessage(&m, EM_KEY);
    if (m.vkcode == 'M') {
        cls();
        backA();
        putimage_alpha(200, 50, &po);
        printStory(40, 270, 170, "娃娃发现一条野路", 900, 1);
        printStory(35, 200, 600, "你选择直接进入", 450, 1);
        printStory(35, 750, 600, "你选择老老实实地走大路", 450, 1);
        FlushBatchDraw();
        getmessage(&m, EM_KEY);
        getmessage(&m, EM_KEY);
        if (m.vkcode == 'M') {
            cls();
            backA();
            putimage_alpha(200, 50, &po);
            settextcolor(RED);
            printStory(50, 270, 170, "666老子又没有走小路你让我怎么编？", 900, 2);
            printStory(40, 200, 600, "等级：R", 450, 1);
            printStory(40, 750, 600, "任意键退出", 450, 1);
            FlushBatchDraw();
            settextcolor(BLACK);
            arrR[10] = 1;
            getmessage(&m, EM_KEY);
            getmessage(&m, EM_KEY);
            say();
        }
        else {
            cls();
            backA();
            putimage_alpha(200, 50, &po);
            printStory(40, 270, 170, "此时，你们似乎发现了终点。但是娃娃的无人机又打破了这一切……", 900, 1);
            printStory(35, 200, 600, "你受不了了，选择当勾坐车", 450, 1);
            printStory(35, 750, 600, "你继续坚持", 450, 1);
            FlushBatchDraw();
            getmessage(&m, EM_KEY);
            getmessage(&m, EM_KEY);
            if (m.vkcode == 'M') {
                cls();
                backA();
                putimage_alpha(200, 50, &po);
                settextcolor(RED);
                printStory(50, 270, 170, "不是你还真的坐啊？", 900, 2);
                printStory(40, 200, 600, "等级：C", 450, 1);
                printStory(40, 750, 600, "任意键退出", 450, 1);
                FlushBatchDraw();
                settextcolor(BLACK);
                arrC[25] = 1;
                getmessage(&m, EM_KEY);
                getmessage(&m, EM_KEY);
            }
            else {
                cls();
                backA();
                putimage_alpha(200, 50, &po);
                settextcolor(RED);
                printStory(50, 270, 170, "在此期间，你们遇到了在你们身后的江老师和一群依附于她身旁的女同学。你继续走，见到了李校长。校长为你们加油打气。冲上了这个坡，一切好像都明朗了，你是胜利者", 900, 2);
                printStory(40, 200, 600, "等级：S", 450, 1);
                printStory(40, 750, 600, "《远足》已解锁。任意键退出", 450, 1);
                FlushBatchDraw();
                settextcolor(BLACK);
                arrS[19] = 1;
                getmessage(&m, EM_KEY);
                getmessage(&m, EM_KEY);
                isIntoLook2 = true;
                cls();
                backA();
                putimage(50, 30, &n1);
                settextstyle(50, 0, "楷体");
                outtextxy(30, 30, "远足时的珍贵相片资料,enter继续");
                outtextxy(31, 30, "远足时的珍贵相片资料,enter继续");
                FlushBatchDraw();
                getmessage(&m, EM_KEY);
                getmessage(&m, EM_KEY);
                cls();
                backA();
                putimage(50, 30, &n2);
                settextstyle(50, 0, "楷体");
                outtextxy(30, 30, "远足时的珍贵相片资料,enter继续");
                outtextxy(31, 30, "远足时的珍贵相片资料,enter继续");
                FlushBatchDraw();
                getmessage(&m, EM_KEY);
                getmessage(&m, EM_KEY);
                cls();
                backA();
                putimage(50, 30, &n3);
                settextstyle(50, 0, "楷体");
                outtextxy(30, 30, "远足时的珍贵相片资料,enter继续");
                outtextxy(31, 30, "远足时的珍贵相片资料,enter继续");
                FlushBatchDraw();
                getmessage(&m, EM_KEY);
                getmessage(&m, EM_KEY);
            }
        }
    }
    else {
        cls();
        backA();
        putimage_alpha(200, 50, &po);
        printStory(40, 270, 170, "此时，洋盆不想帮麦子拿蛋糕（在一开始，洋盆和麦子走在一起，同时在一开始校方给每个人都发了蛋糕。麦子让洋盆帮他拿蛋糕，但是后期麦子掉队了，洋盆在麦子的前面，远超他。）", 900, 1);
        printStory(35, 200, 600, "你建议让洋盆把蛋糕扔到沟底", 450, 1);
        printStory(35, 750, 600, "你建议让虾片自己吃掉", 450, 1);
        FlushBatchDraw();
        getmessage(&m, EM_KEY);
        getmessage(&m, EM_KEY);
        if (m.vkcode == 'M') {
            cls();
            backA();
            putimage_alpha(200, 50, &po);
            settextcolor(RED);
            printStory(50, 270, 170, "洋盆照做了。但是到了山顶，当麦子找洋盆的时候，你却躲起来了……让人忍俊不禁", 900, 2);
            printStory(40, 200, 600, "等级：A-", 450, 1);
            printStory(40, 750, 600, "任意键退出", 450, 1);
            FlushBatchDraw();
            settextcolor(BLACK);
            arrA[24] = 1;
            getmessage(&m, EM_KEY);
            getmessage(&m, EM_KEY);
        }
        else {
            cls();
            backA();
            putimage_alpha(200, 50, &po);
            settextcolor(RED);
            printStory(50, 270, 170, "这个行为确实比较符合虾片的行径。但是由于不当的饮食行为，仅仅走了三分之一，你便大喊“我肚肚”。于是只能乘救援车“百万撤离”。但是这个行为我只能咱们你是dog了", 900, 2);
            printStory(40, 200, 600, "等级：C+", 450, 1);
            printStory(40, 750, 600, "任意键退出", 450, 1);
            FlushBatchDraw();
            settextcolor(BLACK);
            arrC[24] = 1;
            getmessage(&m, EM_KEY);
            getmessage(&m, EM_KEY);
        }
    }
}

void funq() {
    ExMessage Lm;
    IMAGE u1;
    loadimage(&u1, "res\\pir\\uncd1.jpg");
    IMAGE u2;
    loadimage(&u2, "res\\pir\\uncd2.jpg");
    IMAGE u3;
    loadimage(&u3, "res\\pir\\uncd3.jpg");
    IMAGE po;
    loadimage(&po, "res\\pir\\po.png");
    IMAGE pos;
    loadimage(&pos, "res\\pir\\pos.png");
    cls();
    backA();
    putimage_alpha(200, 50, &po);
    printStory(40, 270, 170, "你们继续走着，唱起来《团结就是力量》，歌声振奋人心，响彻四野！", 900, 1);
    printStory(35, 200, 600, "a", 450, 1);
    printStory(35, 750, 600, "b", 450, 1);
    FlushBatchDraw();
    getmessage(&Lm, EM_KEY);
    getmessage(&Lm, EM_KEY);
    if (Lm.vkcode == 'M') {
        cls();
        backA();
        putimage_alpha(200, 50, &po);
        settextcolor(RED);
        printStory(50, 270, 170, "你们成功地追上了翱翔班的旗帜，这场远足你们——胜利了！", 900, 2);
        printStory(40, 200, 600, "等级：S", 450, 1);
        printStory(40, 750, 600, "任意键退出", 450, 1);
        FlushBatchDraw();
        settextcolor(BLACK);
        arrS[18] = 1;
        getmessage(&Lm, EM_KEY);
        getmessage(&Lm, EM_KEY);
        cls();
        backA();
        putimage(50, 30, &u1);
        settextstyle(50, 0, "楷体");
        outtextxy(30, 30, "远足时的珍贵相片资料,enter继续");
        outtextxy(31, 30, "远足时的珍贵相片资料,enter继续");
        FlushBatchDraw();
        getmessage(&Lm, EM_KEY);
        getmessage(&Lm, EM_KEY);
        cls();
        backA();
        putimage(50, 30, &u2);
        settextstyle(50, 0, "楷体");
        outtextxy(30, 30, "远足时的珍贵相片资料,enter继续");
        outtextxy(31, 30, "远足时的珍贵相片资料,enter继续");
        FlushBatchDraw();
        getmessage(&Lm, EM_KEY);
        getmessage(&Lm, EM_KEY);
        cls();
        backA();
        putimage(50, 30, &u3);
        settextstyle(50, 0, "楷体");
        outtextxy(30, 30, "远足时的珍贵相片资料,enter继续");
        outtextxy(31, 30, "远足时的珍贵相片资料,enter继续");
        FlushBatchDraw();
        getmessage(&Lm, EM_KEY);
        getmessage(&Lm, EM_KEY);
    }
    else {
        cls();
        backA();
        putimage_alpha(200, 50, &po);
        settextcolor(RED);
        printStory(50, 270, 170, "你们还是没有打败翱翔，但是你们领略到了这大好春光，这何尝不是一种胜利！", 900, 2);
        printStory(40, 200, 600, "等级：A+", 450, 1);
        printStory(40, 750, 600, "任意键退出", 450, 1);
        FlushBatchDraw();
        settextcolor(BLACK);
        arrA[23] = 1;
        getmessage(&Lm, EM_KEY);
        getmessage(&Lm, EM_KEY);
    }
}


void news() {
    IMAGE ne1;
    loadimage(&ne1, "res\\pir\\new1.png");
    IMAGE ne2;
    loadimage(&ne2, "res\\pir\\new2.png");
    IMAGE ne3;
    loadimage(&ne3, "res\\pir\\new3.png");
    static int newsPage = 1;
    cls();
    backA();

    if (newsPage == 1) {
        putimage_alpha(-10, 0, &ne1);
    }
    else if (newsPage == 2) {
        putimage_alpha(-10, 0, &ne2);
    }
    else if (newsPage == 3) {
        putimage_alpha(-10, 0, &ne3);
    }

    FlushBatchDraw();

    ExMessage localM;
    getmessage(&localM, EM_KEY);
    if (localM.message == WM_KEYDOWN)
    {
        if (localM.vkcode == 'P') {
            if (newsPage == 1) newsPage = 2;
            else if (newsPage == 2) newsPage = 3;
        }
        else if (localM.vkcode == 'S') {
            isIntoMenu = false;
            newsPage = 1;
        }
    }
}

//dont move or try writing it again!it is very soft !
//i think if it is after many years.this coding all neednt add anything new
void peo() {
    ExMessage localM;
    IMAGE peo1;
    loadimage(&peo1, "res\\pir\\peo1.png");
    IMAGE peo2;
    loadimage(&peo2, "res\\pir\\peo2.png");
    IMAGE peo3;
    loadimage(&peo3, "res\\pir\\peo3.png");
    IMAGE peo4;
    loadimage(&peo4, "res\\pir\\peo4.png");
    IMAGE peo5;
    loadimage(&peo5, "res\\pir\\peo5.png");
    IMAGE peo6;
    loadimage(&peo6, "res\\pir\\peo6.png");
    IMAGE peo7;
    loadimage(&peo7, "res\\pir\\peo7.png");
    cls();
    backA();
    putimage_alpha(0, 10, &peo1);
    FlushBatchDraw();
    getmessage(&localM, EM_KEY);
    if (localM.message == WM_KEYDOWN) {
        if (localM.vkcode == 'D') {
            cls();
            backA();
            FlushBatchDraw();
            getmessage(&localM, EM_KEY);
            if (localM.vkcode == 'D') {
                cls();
                backA();
                putimage_alpha(0, 10, &peo2);
                FlushBatchDraw();
                getmessage(&localM, EM_KEY);
                if (localM.vkcode == 'D') {
                    cls();
                    backA();
                    FlushBatchDraw();
                    getmessage(&localM, EM_KEY);
                    if (localM.vkcode == 'D') {
                        cls();
                        backA();
                        putimage_alpha(0, 10, &peo3);
                        FlushBatchDraw();
                        getmessage(&localM, EM_KEY);
                        if (localM.vkcode == 'D') {
                            cls();
                            backA();
                            FlushBatchDraw();
                            getmessage(&localM, EM_KEY);
                            if (localM.vkcode == 'D') {
                                cls();
                                backA();
                                putimage_alpha(0, 10, &peo4);
                                FlushBatchDraw();
                                getmessage(&localM, EM_KEY);
                                if (localM.vkcode == 'D') {
                                    cls();
                                    backA();
                                    FlushBatchDraw();
                                    getmessage(&localM, EM_KEY);
                                    if (localM.vkcode == 'D') {
                                        cls();
                                        backA();
                                        putimage_alpha(0, 10, &peo5);
                                        FlushBatchDraw();
                                        getmessage(&localM, EM_KEY);
                                        if (localM.vkcode == 'D') {
                                            cls();
                                            backA();
                                            FlushBatchDraw();
                                            getmessage(&localM, EM_KEY);
                                            if (localM.vkcode == 'D') {
                                                cls();
                                                backA();
                                                putimage_alpha(0, 10, &peo6);
                                                FlushBatchDraw();
                                                getmessage(&localM, EM_KEY);
                                                if (localM.vkcode == 'D') {
                                                    cls();
                                                    backA();
                                                    FlushBatchDraw();
                                                    getmessage(&localM, EM_KEY);
                                                    if (localM.vkcode == 'D') {
                                                        cls();
                                                        backA();
                                                        putimage_alpha(0, 10, &peo7);
                                                        FlushBatchDraw();
                                                        getmessage(&localM, EM_KEY);
                                                        if (localM.vkcode == 'F') {
                                                            isIntoMenu = false;
                                                        }
                                                    }
                                                    else {
                                                        isIntoMenu = false;
                                                    }
                                                }
                                                else {
                                                    isIntoMenu = false;
                                                }
                                            }
                                            else {
                                                isIntoMenu = false;
                                            }
                                        }
                                        else {
                                            isIntoMenu = false;
                                        }
                                    }
                                    else {
                                        isIntoMenu = false;
                                    }
                                }
                                else {
                                    isIntoMenu = false;
                                }
                            }
                            else {
                                isIntoMenu = false;
                            }
                        }
                        else {
                            isIntoMenu = false;
                        }
                    }
                    else {
                        isIntoMenu = false;
                    }
                }
                else {
                    isIntoMenu = false;
                }
            }
            else {
                isIntoMenu = false;
            }
        }
        else {
            isIntoMenu = false;
        }
    }
    FlushBatchDraw();
}
//dont move or try writing it(up these about "peo()") again!it is very soft !
//i think if it is after many years.this coding all needn`t add anything new
//if u dont know why it is this.u dont need to know or understand this
//it is truly ……what can i say !
//it uses a bug to realise the game

int main() {
    initgraph(1400, 800);
    BeginBatchDraw();
    setbkmode(TRANSPARENT);
    srand((unsigned int)time(NULL));
    IMAGE str1;
    loadimage(&str1, "res\\pir\\start1.jpeg");
    IMAGE pia;
    loadimage(&pia, "res\\pir\\pia.png");
    IMAGE huclbe;
    loadimage(&huclbe, "res\\pir\\hucl-be.png");
    IMAGE menu;
    loadimage(&menu, "res\\pir\\menu.png");
    IMAGE free;
    loadimage(&free, "res\\pir\\free.png");
    IMAGE ne1;
    loadimage(&ne1, "res\\pir\\new1.png");
    IMAGE ne2;
    loadimage(&ne2, "res\\pir\\new2.png");
    IMAGE ne3;
    loadimage(&ne3, "res\\pir\\new3.png");
    IMAGE q213;
    loadimage(&q213, "res\\pir\\213.png");
    IMAGE art;
    loadimage(&art, "res\\pir\\art.png");
    IMAGE com;
    loadimage(&com, "res\\pir\\com.png");
    IMAGE csr;
    loadimage(&csr, "res\\pir\\csr.png");
    IMAGE hike;
    loadimage(&hike, "res\\pir\\hike.png");
    IMAGE math;
    loadimage(&math, "res\\pir\\math.png");
    IMAGE hy;
    loadimage(&hy, "res\\pir\\hy.png");
    IMAGE wj;
    loadimage(&wj, "res\\pir\\wj.png");
    IMAGE oth;
    loadimage(&oth, "res\\pir\\other.png");
    IMAGE vc;
    loadimage(&vc, "res\\pir\\vc.png");
    IMAGE line;
    loadimage(&line, "res\\pir\\line.png");
    IMAGE po;
    loadimage(&po, "res\\pir\\po.png");
    IMAGE pos;
    loadimage(&pos, "res\\pir\\pos.png");
    IMAGE end;
    loadimage(&end, "res\\pir\\end.png");
    IMAGE fa;
    loadimage(&fa, "res\\pir\\family.jpg");
    IMAGE u1;
    loadimage(&u1, "res\\pir\\uncd1.jpg");
    IMAGE u2;
    loadimage(&u2, "res\\pir\\uncd2.jpg");
    IMAGE u3;
    loadimage(&u3, "res\\pir\\uncd3.jpg");
    IMAGE m1;
    loadimage(&m1, "res\\pir\\m1.png");
    IMAGE m2;
    loadimage(&m2, "res\\pir\\m2.png");
    IMAGE m3;
    loadimage(&m3, "res\\pir\\m3.png");
    IMAGE um1;
    loadimage(&um1, "res\\pir\\um1.png");
    IMAGE um2;
    loadimage(&um2, "res\\pir\\um2.png");
    IMAGE um3;
    loadimage(&um3, "res\\pir\\um3.png");
    IMAGE um4;
    loadimage(&um4, "res\\pir\\um4.png");
    IMAGE um5;
    loadimage(&um5, "res\\pir\\um5.png");
    IMAGE um6;
    loadimage(&um6, "res\\pir\\um6.png");
    bool isIntoGame = false;//check if player enter the game
    bool isRealGame = false;//enter the body of the game
    bool isIntoArt = false;
    bool isIntoLook = false;
    //bool isIntoLook2 = false; 全局变量
    bool isIntoLook3 = false;
    bool isIntoLook4 = false;
    bool isIntoLook5 = false;
    int choiceMenu = 0;//choice the type of the menu
    int NewsPageNumber = 0;//the page of news to protect the bug
    int start = time(0);
    int ending = start;
    int gtime = 0;
    int hy1 = 0;
    int hy2 = 0;
    int hy3 = 0;
    while (1) {
        getmessage(&m, EM_MOUSE | EM_KEY);
        cls();
        if (isIntoGame == false) {
            startbac();
            FlushBatchDraw();
        }
        if (isIntoGame == false) {
            if (m.message == WM_LBUTTONDOWN) {
                if (m.x >= 570 && m.x <= 570 + 250 && m.y >= 230 && m.y <= 230 + 150) {
                    cls();
                    while (1) {
                        putimage_alpha(0, 12, &free);
                        FlushBatchDraw();
                        getmessage(&m, EM_MOUSE | EM_KEY);
                        if (m.vkcode == 'Y') {
                            isIntoGame = true;
                            break;
                        }
                        else if (m.vkcode == 'U') {
                            closegraph();
                            _getch();
                            return 0;
                        }
                    }
                }
            }
        }
        //进入游戏内部[out-in]
        if (isIntoGame == true) {
            do {
                cls();
                backA();
                score = 0;
                if (isIntoMenu == false && isRealGame == false) {
                    putimage_alpha(500, 170, &menu);
                }
                FlushBatchDraw();
                if (isIntoMenu == false) {
                    if (peekmessage(&m, EM_KEY)) {
                        if (m.message == WM_KEYDOWN) {
                            if (m.vkcode == 'Q') {
                                choiceMenu = 1;
                                isIntoMenu = true;
                            }
                            else if (m.vkcode == 'W') {
                                choiceMenu = 2;
                                isIntoMenu = true;
                            }
                            else if (m.vkcode == 'E') {
                                choiceMenu = 3;
                                isIntoMenu = true;
                            }
                            else if (m.vkcode == 'R') {
                                choiceMenu = 4;
                                isIntoMenu = true;
                            }
                            else if (m.vkcode == 'T') {
                                choiceMenu = 5;
                                isIntoMenu = true;
                            }
                            Sleep(10);
                        }
                    }
                }
                if (isIntoMenu == true && isIntoOther == false) {
                    switch (choiceMenu)
                    {
                    case 1: {
                        isRealGame = true;
                        break;
                    }
                    case 2: {
                        advtan();
                        FlushBatchDraw();
                        getmessage(&m, EM_KEY);
                        if (m.vkcode == 'O') {
                            isIntoMenu = false;
                            break;
                        }
                        break;
                    }
                    case 3: {
                        news();
                        break;
                    }

                    case 4: {
                        peo();
                        break;
                    }
                    case 5: {
                        isIntoOther = true;
                        break;
                    }
                    }
                }
                if (isIntoOther == true && isIntoMenu == true && isIntoArt == false) {
                    cls();
                    backA();
                    putimage_alpha(500, 100, &oth);
                    FlushBatchDraw();
                    getmessage(&m, EM_KEY);
                    if (m.message == WM_KEYDOWN) {
                        if (m.vkcode == 'G') {
                            cls();
                            backA();
                            putimage_alpha(70, 0, &vc);
                            FlushBatchDraw();
                            getmessage(&m, EM_KEY);
                            getmessage(&m, EM_KEY);
                            if (m.ch == 13) {
                                isIntoOther = true;
                            }
                        }
                        else if (m.vkcode == 'H') {
                            cls();
                            backA();
                            putimage_alpha(0, 26, &com);
                            FlushBatchDraw();
                            getmessage(&m, EM_KEY);
                            getmessage(&m, EM_KEY);
                            if (m.ch == 13) {
                                isIntoOther = true;
                            }
                        }
                        else if (m.vkcode == 'J') {
                            cls();
                            backA();
                            putimage_alpha(0, 17, &csr);
                            FlushBatchDraw();
                            getmessage(&m, EM_KEY);
                            getmessage(&m, EM_KEY);
                            if (m.ch == 13) {
                                isIntoOther = true;
                            }
                        }
                        else if (m.vkcode == 'K') {
                            isIntoArt = true;

                        }
                        else if (m.vkcode == 'L') {
                            isIntoOther = false;
                            isIntoMenu = false;
                        }
                    }
                }
                if (isIntoArt == true) {
                    cls();
                    backA();
                    putimage_alpha(480, 50, &art);
                    FlushBatchDraw();
                    if ((hy1 + hy2 + hy3) == 3) {
                        isIntoLook5 = true;
                    }
                    else {
                        isIntoLook5 = false;
                    }
                    getmessage(&m, EM_KEY);
                    if (m.message == WM_KEYDOWN) {
                        if (m.vkcode == 'Z') {
                            if (isIntoLook == false) {
                                cls();
                                backA();
                                settextcolor(BLACK);
                                settextstyle(80, 0, "微软雅黑");
                                outtextxy(200, 400, "您未解锁此区域,按【enter】退出");
                                FlushBatchDraw();
                                getmessage(&m, EM_KEY);
                                getmessage(&m, EM_KEY);
                                if (m.ch == 13) {
                                    isIntoArt = true;
                                }
                            }
                            else {
                                cls();
                                backA();
                                putimage_alpha(20, 0, &math);
                                FlushBatchDraw();
                                getmessage(&m, EM_KEY);
                                getmessage(&m, EM_KEY);
                                if (m.ch == 13) {
                                    isIntoArt = true;
                                }
                            }
                        }
                        else if (m.vkcode == 'X') {
                            if (isIntoLook2 == false) {
                                cls();
                                backA();
                                settextcolor(BLACK);
                                settextstyle(80, 0, "微软雅黑");
                                outtextxy(200, 400, "您未解锁此区域,按【enter】退出");
                                FlushBatchDraw();
                                getmessage(&m, EM_KEY);
                                getmessage(&m, EM_KEY);
                                if (m.ch == 13) {
                                    isIntoArt = true;
                                }
                            }
                            else {
                                cls();
                                backA();
                                putimage_alpha(20, 0, &hike);
                                FlushBatchDraw();
                                getmessage(&m, EM_KEY);
                                getmessage(&m, EM_KEY);
                                if (m.ch == 13) {
                                    isIntoArt = true;
                                }
                            }
                        }
                        else if (m.vkcode == 'C') {
                            if (isIntoLook3 == false) {
                                cls();
                                backA();
                                settextcolor(BLACK);
                                settextstyle(80, 0, "微软雅黑");
                                outtextxy(200, 400, "您未解锁此区域,按【enter】退出");
                                FlushBatchDraw();
                                getmessage(&m, EM_KEY);
                                getmessage(&m, EM_KEY);
                                if (m.ch == 13) {
                                    isIntoArt = true;
                                }
                            }
                            else {
                                cls();
                                backA();
                                putimage_alpha(20, 0, &q213);
                                FlushBatchDraw();
                                getmessage(&m, EM_KEY);
                                getmessage(&m, EM_KEY);
                                if (m.ch == 13) {
                                    isIntoArt = true;
                                }
                            }
                        }
                        else if (m.vkcode == 'V') {
                            if (isIntoLook4 == false) {
                                cls();
                                backA();
                                settextcolor(BLACK);
                                settextstyle(80, 0, "微软雅黑");
                                outtextxy(200, 400, "您未解锁此区域,按【enter】退出");
                                FlushBatchDraw();
                                getmessage(&m, EM_KEY);
                                getmessage(&m, EM_KEY);
                                if (m.ch == 13) {
                                    isIntoArt = true;
                                }
                            }
                            else {
                                cls();
                                backA();
                                putimage_alpha(20, 0, &wj);
                                FlushBatchDraw();
                                getmessage(&m, EM_KEY);
                                getmessage(&m, EM_KEY);
                                if (m.ch == 13) {
                                    isIntoArt = true;
                                }
                            }
                        }
                        else if (m.vkcode == 'B') {
                            if (isIntoLook5 == false) {
                                cls();
                                backA();
                                settextcolor(BLACK);
                                settextstyle(80, 0, "微软雅黑");
                                outtextxy(200, 400, "您未解锁此区域,按【enter】退出");
                                FlushBatchDraw();
                                getmessage(&m, EM_KEY);
                                getmessage(&m, EM_KEY);
                                if (m.ch == 13) {
                                    isIntoArt = true;
                                }
                            }
                            else {
                                cls();
                                backA();
                                putimage_alpha(20, 0, &hy);
                                FlushBatchDraw();
                                getmessage(&m, EM_KEY);
                                getmessage(&m, EM_KEY);
                                if (m.ch == 13) {
                                    isIntoArt = true;
                                }
                            }
                        }
                        else if (m.ch == 13) {
                            isIntoArt = 0;
                        }
                    }
                }

                //the baody story of game
                if (isRealGame == true) {
                    start = time(0);
                    cls();
                    backA();
                    putimage_alpha(130, 50, &line);
                    FlushBatchDraw();
                    getmessage(&m, EM_KEY);
                    getmessage(&m, EM_KEY);
                    if (m.message == WM_KEYDOWN) {
                        if (m.vkcode == '1') {

                            cls();
                            backA();
                            putimage_alpha(200, 50, &po);
                            printStory(50, 270, 170, "国内经济有序发展", 900, 1);
                            printStory(35, 200, 600, "平均发展，共同进步，集体主义,发展农业科技", 450, 1);
                            printStory(35, 750, 600, "召开第一次会议，提出“ 先富带动后富”方针", 450, 1);
                            FlushBatchDraw();
                            getmessage(&m, EM_KEY);
                            getmessage(&m, EM_KEY);
                            if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
                                cls();
                                backA();
                                putimage_alpha(200, 50, &po);
                                printStory(50, 270, 170, "把粮食紧紧握在自己的饭碗里", 900, 1);
                                printStory(35, 200, 600, "开发北斗技术", 450, 1);
                                printStory(35, 750, 600, "发展新型水稻", 450, 1);
                                FlushBatchDraw();
                                getmessage(&m, EM_KEY);
                                getmessage(&m, EM_KEY);
                                if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
                                    cls();
                                    backA();
                                    putimage_alpha(200, 50, &po);
                                    printStory(50, 270, 170, "开发北斗技术", 900, 1);
                                    printStory(35, 200, 600, "自力更生", 450, 1);
                                    printStory(35, 750, 600, "加入Y国主导的计划", 450, 1);
                                    FlushBatchDraw();
                                    getmessage(&m, EM_KEY);
                                    getmessage(&m, EM_KEY);
                                    if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
                                        cls();
                                        backA();
                                        putimage_alpha(200, 50, &po);
                                        printStory(50, 270, 170, "暂无进展", 900, 1);
                                        printStory(35, 200, 600, "深耕于核心技术", 450, 1);
                                        printStory(35, 750, 600, "仍无突破，求于M国", 450, 1);
                                        FlushBatchDraw();
                                        getmessage(&m, EM_KEY);
                                        getmessage(&m, EM_KEY);
                                        if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
                                            funa();
                                        }
                                        else {
                                            cls();
                                            backA();
                                            putimage_alpha(200, 50, &po);
                                            printStory(50, 270, 170, "M国狮子大开口", 900, 1);
                                            printStory(35, 200, 600, "接受天价金额", 450, 1);
                                            printStory(35, 750, 600, "不接收", 450, 1);
                                            FlushBatchDraw();
                                            getmessage(&m, EM_KEY);
                                            getmessage(&m, EM_KEY);
                                            if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
                                                cls();
                                                backA();
                                                putimage_alpha(200, 50, &po);
                                                printStory(50, 270, 170, "花了大价钱，不对，这个技术有问题！", 900, 1);
                                                printStory(35, 200, 600, "找M国质疑", 450, 1);
                                                printStory(35, 750, 600, "气急败坏，向M国开战", 450, 1);
                                                FlushBatchDraw();
                                                getmessage(&m, EM_KEY);
                                                getmessage(&m, EM_KEY);
                                                if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
                                                    cls();
                                                    backA();
                                                    putimage_alpha(200, 50, &po);
                                                    printStory(50, 270, 170, "换得嘲讽", 900, 1);
                                                    printStory(35, 200, 600, "痛定思痛，好好研究技术", 450, 1);
                                                    printStory(35, 750, 600, "你彻底摆烂", 450, 1);
                                                    FlushBatchDraw();
                                                    getmessage(&m, EM_KEY);
                                                    getmessage(&m, EM_KEY);
                                                    if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
                                                        funa();
                                                    }
                                                    else {
                                                        cls();
                                                        backA();
                                                        putimage_alpha(200, 50, &po);
                                                        settextcolor(RED);
                                                        printStory(50, 270, 170, "……", 900, 2);
                                                        printStory(40, 200, 600, "等级：D", 450, 1);
                                                        printStory(40, 750, 600, "任意键退出", 450, 1);
                                                        FlushBatchDraw();
                                                        settextcolor(BLACK);
                                                        arrD[5] = 1;
                                                        getmessage(&m, EM_KEY);
                                                        getmessage(&m, EM_KEY);
                                                    }
                                                }
                                                else {
                                                    cls();
                                                    backA();
                                                    putimage_alpha(200, 50, &po);
                                                    printStory(50, 270, 170, "一溃千里", 900, 1);
                                                    printStory(35, 200, 600, "为了国家", 450, 1);
                                                    printStory(35, 750, 600, "逃亡他地", 450, 1);
                                                    FlushBatchDraw();
                                                    getmessage(&m, EM_KEY);
                                                    getmessage(&m, EM_KEY);
                                                    if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
                                                        cls();
                                                        backA();
                                                        putimage_alpha(200, 50, &po);
                                                        settextcolor(RED);
                                                        printStory(50, 270, 170, "自缢", 900, 2);
                                                        printStory(40, 200, 600, "等级：C", 450, 1);
                                                        printStory(40, 750, 600, "任意键退出", 450, 1);
                                                        FlushBatchDraw();
                                                        settextcolor(BLACK);
                                                        arrC[1] = 1;
                                                        getmessage(&m, EM_KEY);
                                                        getmessage(&m, EM_KEY);
                                                    }
                                                    else {
                                                        cls();
                                                        backA();
                                                        putimage_alpha(200, 50, &po);
                                                        settextcolor(RED);
                                                        printStory(50, 270, 170, "流亡至死", 900, 2);
                                                        printStory(40, 200, 600, "等级：C-", 450, 1);
                                                        printStory(40, 750, 600, "任意键退出", 450, 1);
                                                        FlushBatchDraw();
                                                        settextcolor(BLACK);
                                                        arrC[2] = 1;
                                                        getmessage(&m, EM_KEY);
                                                        getmessage(&m, EM_KEY);
                                                    }
                                                }
                                            }
                                            else {
                                                funa();
                                            }
                                        }
                                    }
                                    else {
                                        cls();
                                        backA();
                                        putimage_alpha(200, 50, &po);
                                        printStory(50, 270, 170, "被Y国戏弄", 900, 1);
                                        printStory(35, 200, 600, "不向Y国开战", 450, 1);
                                        printStory(35, 750, 600, "向Y国开战", 450, 1);
                                        FlushBatchDraw();
                                        getmessage(&m, EM_KEY);
                                        getmessage(&m, EM_KEY);
                                        if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
                                            cls();
                                            backA();
                                            putimage_alpha(200, 50, &po);
                                            settextcolor(RED);
                                            printStory(50, 270, 170, "越想越气，气死了（悲)", 900, 2);
                                            printStory(40, 200, 600, "等级：D-", 450, 1);
                                            printStory(40, 750, 600, "任意键退出", 450, 1);
                                            FlushBatchDraw();
                                            settextcolor(BLACK);
                                            arrD[3] = 1;
                                            getmessage(&m, EM_KEY);
                                            getmessage(&m, EM_KEY);
                                        }
                                        else {
                                            cls();
                                            backA();
                                            putimage_alpha(200, 50, &po);
                                            settextcolor(RED);
                                            printStory(50, 270, 170, "失败了，国家破灭", 900, 2);
                                            printStory(40, 200, 600, "等级：D", 450, 1);
                                            printStory(40, 750, 600, "任意键退出", 450, 1);
                                            FlushBatchDraw();
                                            settextcolor(BLACK);
                                            arrD[4] = 1;
                                            getmessage(&m, EM_KEY);
                                            getmessage(&m, EM_KEY);
                                        }
                                    }
                                }
                                else {
                                    //new 水稻
                                    cls();
                                    backA();
                                    putimage_alpha(200, 50, &po);
                                    printStory(50, 270, 170, "有了突破", 900, 1);
                                    printStory(35, 200, 600, "大力在国内发展此种水稻", 450, 1);
                                    printStory(35, 750, 600, "以高价出售M国", 450, 1);
                                    FlushBatchDraw();
                                    getmessage(&m, EM_KEY);
                                    getmessage(&m, EM_KEY);
                                    if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
                                        cls();
                                        backA();
                                        putimage_alpha(200, 50, &po);
                                        printStory(50, 270, 170, "水稻亩产千斤", 900, 1);
                                        printStory(35, 200, 600, "你只发展水稻", 450, 1);
                                        printStory(35, 750, 600, "你决心发展其他高新技术产业！", 450, 1);
                                        FlushBatchDraw();
                                        getmessage(&m, EM_KEY);
                                        getmessage(&m, EM_KEY);
                                        if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
                                            cls();
                                            backA();
                                            putimage_alpha(200, 50, &po);
                                            printStory(50, 270, 170, "但是你忽略了其他方面", 900, 1);
                                            printStory(35, 200, 600, "你接下来开始发展北斗技术。并深耕核心内容", 450, 1);
                                            printStory(35, 750, 600, "你只管发展农业", 450, 1);
                                            FlushBatchDraw();
                                            getmessage(&m, EM_KEY);
                                            getmessage(&m, EM_KEY);
                                            if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
                                                funa();
                                            }
                                            else {
                                                cls();
                                                backA();
                                                putimage_alpha(200, 50, &po);
                                                settextcolor(RED);
                                                printStory(50, 270, 170, "在你死之前，农业确实有很大发展，但是这太单一了，你死后被人亲切地称为“农业战神”", 900, 2);
                                                printStory(40, 200, 600, "等级：B+", 450, 1);
                                                printStory(40, 750, 600, "任意键退出", 450, 1);
                                                FlushBatchDraw();
                                                settextcolor(BLACK);
                                                arrB[12] = 1;
                                                getmessage(&m, EM_KEY);
                                                getmessage(&m, EM_KEY);
                                            }
                                        }
                                        else {
                                            funa();
                                        }
                                    }
                                    else {
                                        cls();
                                        backA();
                                        putimage_alpha(200, 50, &po);
                                        printStory(50, 270, 170, "高价赚取外汇，M国不满，选择措施", 900, 1);
                                        printStory(35, 200, 600, "开战！", 450, 1);
                                        printStory(35, 750, 600, "进行粮食制裁", 450, 1);
                                        FlushBatchDraw();
                                        getmessage(&m, EM_KEY);
                                        getmessage(&m, EM_KEY);
                                        if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
                                            int b13 = rand() % 10;
                                            if (b13 < 7) {
                                                cls();
                                                backA();
                                                putimage_alpha(200, 50, &pos);
                                                settextcolor(RED);
                                                printStory(50, 270, 170, "失败.你知道，这或许就是天命了", 900, 2);
                                                printStory(40, 200, 600, "等级：C+", 450, 1);
                                                printStory(40, 750, 600, "任意键退出", 450, 1);
                                                FlushBatchDraw();
                                                settextcolor(BLACK);
                                                arrC[11] = 1;
                                                getmessage(&m, EM_KEY);
                                                getmessage(&m, EM_KEY);
                                            }
                                            else {
                                                funb();
                                            }
                                        }
                                        else {
                                            int b14 = rand() % 10;
                                            if (b14 < 3) {
                                                func();
                                            }
                                            else {
                                                funb();
                                            }
                                        }
                                    }
                                }
                            }
                            else {
                                //先富带动后富
                                cls();
                                backA();
                                putimage_alpha(200, 50, &po);
                                printStory(50, 270, 170, "选取7个经济特区", 900, 1);
                                printStory(35, 200, 600, "在经济特区建立第一家对外企业", 450, 1);
                                printStory(35, 750, 600, "引入外国时代潮流", 450, 1);
                                FlushBatchDraw();
                                getmessage(&m, EM_KEY);
                                getmessage(&m, EM_KEY);
                                if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
                                    fund();
                                }
                                else {
                                    cls();
                                    backA();
                                    putimage_alpha(200, 50, &po);
                                    printStory(50, 270, 170, "民间掀起一股思想革新的风气", 900, 1);
                                    printStory(35, 200, 600, "你加以管制，认为这不利于你的统治", 450, 1);
                                    printStory(35, 750, 600, "你不管，让民众广泛了解新鲜事物", 450, 1);
                                    FlushBatchDraw();
                                    getmessage(&m, EM_KEY);
                                    getmessage(&m, EM_KEY);
                                    if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
                                        cls();
                                        backA();
                                        putimage_alpha(200, 50, &po);
                                        printStory(50, 270, 170, "民众怨言四起", 900, 1);
                                        printStory(35, 200, 600, "你认为这更加危害了你的统治", 450, 1);
                                        printStory(35, 750, 600, "你选择偏激的做法", 450, 1);
                                        FlushBatchDraw();
                                        getmessage(&m, EM_KEY);
                                        getmessage(&m, EM_KEY);
                                        if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
                                            cls();
                                            backA();
                                            putimage_alpha(200, 50, &po);
                                            printStory(50, 270, 170, "你发现国家需要一场思想革命，于是，你开始了思想大革命，你开始批斗他人，哪怕他们只是有一点不忠诚于你", 900, 1);
                                            printStory(35, 200, 600, "你为了保住自己的下一任期，批斗并且暗杀了国家副总统", 450, 1);
                                            printStory(35, 750, 600, "你创建了中心思想，决定对外封闭，自我革新", 450, 1);
                                            FlushBatchDraw();
                                            getmessage(&m, EM_KEY);
                                            getmessage(&m, EM_KEY);
                                            if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
                                                cls();
                                                backA();
                                                putimage_alpha(200, 50, &po);
                                                printStory(50, 270, 170, "国家热议，社会各界反向批斗你", 900, 1);
                                                printStory(35, 200, 600, "你不管事，与这个社会为敌", 450, 1);
                                                printStory(35, 750, 600, "你试图压制舆论", 450, 1);
                                                FlushBatchDraw();
                                                getmessage(&m, EM_KEY);
                                                getmessage(&m, EM_KEY);
                                                if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
                                                    cls();
                                                    backA();
                                                    putimage_alpha(200, 50, &po);
                                                    printStory(50, 270, 170, "新总统最终还是上任，你被弹劾", 900, 1);
                                                    printStory(35, 200, 600, "a", 450, 1);
                                                    printStory(35, 750, 600, "b", 450, 1);
                                                    FlushBatchDraw();
                                                    getmessage(&m, EM_KEY);
                                                    getmessage(&m, EM_KEY);
                                                    if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
                                                        cls();
                                                        backA();
                                                        putimage_alpha(200, 50, &po);
                                                        settextcolor(RED);
                                                        printStory(50, 270, 170, "你在狱中死亡，一切努力化为土", 900, 2);
                                                        printStory(40, 200, 600, "等级：B-", 450, 1);
                                                        printStory(40, 750, 600, "任意键退出", 450, 1);
                                                        FlushBatchDraw();
                                                        settextcolor(BLACK);
                                                        arrB[13] = 1;
                                                        getmessage(&m, EM_KEY);
                                                        getmessage(&m, EM_KEY);
                                                    }
                                                    else {
                                                        cls();
                                                        backA();
                                                        putimage_alpha(200, 50, &po);
                                                        settextcolor(RED);
                                                        printStory(50, 270, 170, "你被保释，但是最终屈辱而死，所有的一切这些都与你无关了", 900, 2);
                                                        printStory(40, 200, 600, "等级：C", 450, 1);
                                                        printStory(40, 750, 600, "任意键退出", 450, 1);
                                                        FlushBatchDraw();
                                                        settextcolor(BLACK);
                                                        arrC[12] = 1;
                                                        getmessage(&m, EM_KEY);
                                                        getmessage(&m, EM_KEY);
                                                    }
                                                }
                                                else {
                                                    cls();
                                                    backA();
                                                    putimage_alpha(200, 50, &po);
                                                    settextcolor(RED);
                                                    printStory(50, 270, 170, "没用！你被摔个粉碎！得道多助失道寡助啊", 900, 2);
                                                    printStory(40, 200, 600, "等级：C+", 450, 1);
                                                    printStory(40, 750, 600, "任意键退出", 450, 1);
                                                    FlushBatchDraw();
                                                    settextcolor(BLACK);
                                                    arrC[13] = 1;
                                                    getmessage(&m, EM_KEY);
                                                    getmessage(&m, EM_KEY);
                                                }
                                            }
                                            else {
                                                cls();
                                                backA();
                                                putimage_alpha(200, 50, &po);
                                                printStory(50, 270, 170, "后来，你撤销了经济特区，标志着一个白色时代的到来", 900, 1);
                                                printStory(35, 200, 600, "你开始政治宣传", 450, 1);
                                                printStory(35, 750, 600, "你宣扬自己是多么的高明", 450, 1);
                                                FlushBatchDraw();
                                                getmessage(&m, EM_KEY);
                                                getmessage(&m, EM_KEY);
                                                if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
                                                    cls();
                                                    backA();
                                                    putimage_alpha(200, 50, &po);
                                                    settextcolor(RED);
                                                    printStory(50, 270, 170, "因为民众的麻痹，社会一直在走下坡路，国家的灭亡也只是时间问题", 900, 2);
                                                    printStory(40, 200, 600, "等级：C-", 450, 1);
                                                    printStory(40, 750, 600, "任意键退出", 450, 1);
                                                    FlushBatchDraw();
                                                    settextcolor(BLACK);
                                                    arrC[14] = 1;
                                                    getmessage(&m, EM_KEY);
                                                    getmessage(&m, EM_KEY);
                                                }
                                                else {
                                                    cls();
                                                    backA();
                                                    putimage_alpha(200, 50, &po);
                                                    settextcolor(RED);
                                                    printStory(50, 270, 170, "还存在一部分清醒的民众，他们推翻了你的政权，你最终死亡", 900, 2);
                                                    printStory(40, 200, 600, "等级：D", 450, 1);
                                                    printStory(40, 750, 600, "任意键退出", 450, 1);
                                                    FlushBatchDraw();
                                                    settextcolor(BLACK);
                                                    arrD[10] = 1;
                                                    getmessage(&m, EM_KEY);
                                                    getmessage(&m, EM_KEY);
                                                }
                                            }
                                        }
                                        else {
                                            cls();
                                            backA();
                                            putimage_alpha(200, 50, &po);
                                            printStory(50, 270, 170, "你选择偏激的做法", 900, 1);
                                            printStory(35, 200, 600, "你用军队镇压", 450, 1);
                                            printStory(35, 750, 600, "你实行举报有奖政策，让民众从内部瓦解", 450, 1);
                                            FlushBatchDraw();
                                            getmessage(&m, EM_KEY);
                                            getmessage(&m, EM_KEY);
                                            if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
                                                cls();
                                                backA();
                                                putimage_alpha(200, 50, &po);
                                                settextcolor(RED);
                                                printStory(50, 270, 170, "但是，人民的热血是杀不尽的，你最终被辞职", 900, 2);
                                                printStory(40, 200, 600, "等级：C+", 450, 1);
                                                printStory(40, 750, 600, "任意键退出", 450, 1);
                                                FlushBatchDraw();
                                                settextcolor(BLACK);
                                                arrC[15] = 1;
                                                getmessage(&m, EM_KEY);
                                                getmessage(&m, EM_KEY);
                                            }
                                            else {
                                                cls();
                                                backA();
                                                putimage_alpha(200, 50, &po);
                                                settextcolor(RED);
                                                printStory(50, 270, 170, "民众自相残杀,你得到了大量资金,你成功了，你拿钱跑路去喽", 900, 2);
                                                printStory(40, 200, 600, "等级：B+", 450, 1);
                                                printStory(40, 750, 600, "任意键退出", 450, 1);
                                                FlushBatchDraw();
                                                settextcolor(BLACK);
                                                arrB[14] = 1;
                                                getmessage(&m, EM_KEY);
                                                getmessage(&m, EM_KEY);
                                            }
                                        }
                                    }
                                    else {
                                        cls();
                                        backA();
                                        putimage_alpha(200, 50, &po);
                                        printStory(50, 270, 170, "民众继续发展,素质显著提升!", 900, 1);
                                        printStory(35, 200, 600, "发展科技，科技是第一大本！", 450, 1);
                                        printStory(35, 750, 600, "继续发展经济，建立中外合作区域", 450, 1);
                                        FlushBatchDraw();
                                        getmessage(&m, EM_KEY);
                                        getmessage(&m, EM_KEY);
                                        if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
                                            funa();
                                        }
                                        else {
                                            fund();
                                        }
                                    }
                                }
                            }


                        }
                        else if (m.vkcode == '2') {
                            //funny line
                            cls();
                            backA();
                            putimage_alpha(200, 50, &po);
                            printStory(50, 270, 170, "你颁布《大虾帝国二十条》【内容来自213寝室中的杂谈和对虾片的采访，以寝室成员所著《大虾帝国二十条》为基本依托，亦有个人补充】", 900, 1);
                            printStory(35, 200, 600, "将首都从bj迁址改名为阴京（666这个是虾片亲口真言）", 450, 1);
                            printStory(35, 750, 600, "用国库里的钱投资“超自然行动”这款游戏（我tm说白了就是充钱，而且这tm也是虾片真言）", 450, 1);
                            FlushBatchDraw();
                            getmessage(&m, EM_KEY);
                            getmessage(&m, EM_KEY);
                            if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
                                cls();
                                backA();
                                putimage_alpha(200, 50, &po);
                                printStory(50, 270, 170, "引发部分民众不满，同时为之后的政策做铺垫", 900, 1);
                                printStory(35, 200, 600, "你放开了一些软件的下载渠道……", 450, 1);
                                printStory(35, 750, 600, "你让pc合法（先别急着骂作者，自如先前所说，这是虾片在寝室里的真言！）", 450, 1);
                                FlushBatchDraw();
                                getmessage(&m, EM_KEY);
                                getmessage(&m, EM_KEY);
                                if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
                                    cls();
                                    backA();
                                    putimage_alpha(200, 50, &po);
                                    settextcolor(RED);
                                    printStory(50, 270, 170, "不行，我的良知不允许我再往下编下去！", 900, 2);
                                    printStory(40, 200, 600, "等级：R+", 450, 1);
                                    printStory(40, 750, 600, "任意键退出", 450, 1);
                                    FlushBatchDraw();
                                    settextcolor(BLACK);
                                    arrR[0] = 1;
                                    getmessage(&m, EM_KEY);
                                    getmessage(&m, EM_KEY);
                                    cd();
                                }
                                else {
                                    cls();
                                    backA();
                                    putimage_alpha(200, 50, &po);
                                    printStory(40, 270, 170, "这波操作拉动了民众的生育积极性，同时，在一定程度上，缓解了大虾帝国人口老龄化的问题（我知道这很扯，但是这也是213寝室里杂谈的一部分）", 900, 1);
                                    printStory(35, 200, 600, "既然放开，你也选择了……", 450, 1);
                                    printStory(35, 750, 600, "为你的不合理选择承担责任", 450, 1);
                                    FlushBatchDraw();
                                    getmessage(&m, EM_KEY);
                                    getmessage(&m, EM_KEY);
                                    if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
                                        cls();
                                        backA();
                                        putimage_alpha(200, 50, &po);
                                        printStory(50, 270, 170, "你的名声彻底碎裂，因为，民间完全没有认可这个“霸王条款”！", 900, 1);
                                        printStory(35, 200, 600, "你躲到国外", 450, 1);
                                        printStory(35, 750, 600, "你受不了了，你这荒唐的一生，你选择了自缢", 450, 1);
                                        FlushBatchDraw();
                                        getmessage(&m, EM_KEY);
                                        getmessage(&m, EM_KEY);
                                        if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
                                            cls();
                                            backA();
                                            putimage_alpha(200, 50, &po);
                                            settextcolor(RED);
                                            printStory(50, 270, 170, "666居然是****吗，有点意思……", 900, 2);
                                            printStory(40, 200, 600, "等级：R", 450, 1);
                                            printStory(40, 750, 600, "任意键退出", 450, 1);
                                            FlushBatchDraw();
                                            settextcolor(BLACK);
                                            arrR[1] = 1;
                                            getmessage(&m, EM_KEY);
                                            getmessage(&m, EM_KEY);
                                            say();
                                        }
                                        else {
                                            cls();
                                            backA();
                                            putimage_alpha(200, 50, &po);
                                            settextcolor(RED);
                                            printStory(50, 270, 170, "但是，即使你自杀“殉国”，也改变不了后人对你的看法", 900, 2);
                                            printStory(40, 200, 600, "等级：D-", 450, 1);
                                            printStory(40, 750, 600, "任意键退出", 450, 1);
                                            FlushBatchDraw();
                                            settextcolor(BLACK);
                                            arrD[0] = 1;
                                            getmessage(&m, EM_KEY);
                                            getmessage(&m, EM_KEY);
                                        }
                                    }
                                    else {
                                        cls();
                                        backA();
                                        putimage_alpha(200, 50, &po);
                                        settextcolor(RED);
                                        printStory(50, 270, 170, "你被众人罢免了职务", 900, 2);
                                        printStory(40, 200, 600, "等级：C-", 450, 1);
                                        printStory(40, 750, 600, "任意键退出", 450, 1);
                                        FlushBatchDraw();
                                        settextcolor(BLACK);
                                        arrC[0] = 1;
                                        getmessage(&m, EM_KEY);
                                        getmessage(&m, EM_KEY);
                                    }
                                }
                            }
                            else {
                                cls();
                                backA();
                                putimage_alpha(200, 50, &po);
                                printStory(50, 270, 170, "这简直是最伟大的投资，作者五体投地，传说巴菲特就是向他学的！但是，因为资本，国库亏空了", 900, 1);
                                printStory(30, 200, 600, "卖掉国内的几座大城市，把他们租给外国，用租金继续“投资”超自然", 450, 1);
                                printStory(30, 750, 600, "你开始向各级人民加征“虾片税”，每人每年要缴纳100包虾片", 450, 1);
                                FlushBatchDraw();
                                getmessage(&m, EM_KEY);
                                getmessage(&m, EM_KEY);
                                if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
                                    cls();
                                    backA();
                                    putimage_alpha(200, 50, &po);
                                    printStory(50, 270, 170, "彻底引发民众公愤，民众游街抗议", 900, 1);
                                    printStory(30, 200, 600, "你不管事，让民众抗议着", 450, 1);
                                    printStory(30, 750, 600, "你开始镇压民众", 450, 1);
                                    FlushBatchDraw();
                                    getmessage(&m, EM_KEY);
                                    getmessage(&m, EM_KEY);
                                    if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
                                        cls();
                                        backA();
                                        putimage_alpha(200, 50, &po);
                                        settextcolor(RED);
                                        printStory(50, 270, 170, "但是，你没有任何改变，终于，有人受不了了，他将你赶下台去，毕竟，《二十条》中没有说明不能将总统赶下台", 900, 2);
                                        printStory(40, 200, 600, "等级：D", 450, 1);
                                        printStory(40, 750, 600, "任意键退出", 450, 1);
                                        FlushBatchDraw();
                                        settextcolor(BLACK);
                                        arrD[2] = 1;
                                        getmessage(&m, EM_KEY);
                                        getmessage(&m, EM_KEY);
                                    }
                                    else {
                                        cls();
                                        backA();
                                        putimage_alpha(200, 50, &po);
                                        printStory(50, 270, 170, "你的做法有用了，民众只能屈服于你的统治", 900, 1);
                                        printStory(30, 200, 600, "你让民众对你臣服", 450, 1);
                                        printStory(30, 750, 600, "你开始压力你的民众，就像小时候c老师对你做的那样！", 450, 1);
                                        FlushBatchDraw();
                                        getmessage(&m, EM_KEY);
                                        getmessage(&m, EM_KEY);
                                        if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
                                            cls();
                                            backA();
                                            putimage_alpha(200, 50, &po);
                                            printStory(50, 270, 170, "这tm叫复辟帝制！", 900, 1);
                                            printStory(30, 200, 600, "你发明了“大虾礼教”以礼教为由，囚禁人民的思想", 450, 1);
                                            printStory(30, 750, 600, "另一条路……", 450, 1);
                                            FlushBatchDraw();
                                            getmessage(&m, EM_KEY);
                                            getmessage(&m, EM_KEY);
                                            if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
                                                cls();
                                                backA();
                                                putimage_alpha(200, 50, &po);
                                                settextcolor(RED);
                                                printStory(50, 270, 170, "人民最终变得麻木不仁，但是你呢？我的虾片……只有天知道！", 900, 2);
                                                printStory(40, 200, 600, "等级：B-", 450, 1);
                                                printStory(40, 750, 600, "任意键退出", 450, 1);
                                                FlushBatchDraw();
                                                settextcolor(BLACK);
                                                arrB[0] = 1;
                                                getmessage(&m, EM_KEY);
                                                getmessage(&m, EM_KEY);
                                            }
                                            else {
                                                cls();
                                                backA();
                                                putimage_alpha(200, 50, &po);
                                                settextcolor(RED);
                                                printStory(50, 270, 170, "666我实在编不下去了……反正复辟帝制没有好下场！", 900, 2);
                                                printStory(40, 200, 600, "等级：R", 450, 1);
                                                printStory(40, 750, 600, "任意键退出", 450, 1);
                                                FlushBatchDraw();
                                                settextcolor(BLACK);
                                                arrR[2] = 1;
                                                getmessage(&m, EM_KEY);
                                                getmessage(&m, EM_KEY);
                                                cd();
                                            }
                                        }
                                        else {
                                            int b2 = rand() % 4;
                                            if (b2 < 3) {
                                                cls();
                                                backA();
                                                putimage_alpha(200, 50, &pos);
                                                settextcolor(RED);
                                                printStory(50, 270, 200, "民众不买账！所以，你把***给lc了（真言）,你以为作为总统就可以这么做吗？得民心者得天下啊……", 900, 2);
                                                printStory(40, 200, 600, "等级：B+", 450, 1);
                                                printStory(40, 750, 600, "任意键退出", 450, 1);
                                                FlushBatchDraw();
                                                settextcolor(BLACK);
                                                arrB[1] = 1;
                                                getmessage(&m, EM_KEY);
                                                getmessage(&m, EM_KEY);
                                            }
                                            else {
                                                cls();
                                                backA();
                                                putimage_alpha(200, 50, &pos);
                                                settextcolor(RED);
                                                printStory(50, 270, 170, "民众不买账！所以，你把***给lc了（真言）,未完待续……", 900, 2);
                                                printStory(40, 200, 600, "等级：R+", 450, 1);
                                                printStory(40, 750, 600, "任意键退出", 450, 1);
                                                FlushBatchDraw();
                                                settextcolor(BLACK);
                                                arrR[3] = 1;
                                                getmessage(&m, EM_KEY);
                                                getmessage(&m, EM_KEY);
                                                say();
                                            }
                                        }
                                    }
                                }
                                else {
                                    int b1 = rand() % 4;
                                    if (b1 < 3) {
                                        cls();
                                        backA();
                                        putimage_alpha(200, 50, &pos);
                                        settextcolor(RED);
                                        printStory(50, 270, 200, "久而久之，国家经济开始复苏，同时，虾片在全球范围都成为了硬通货，大虾帝国发展成为了一个经济强国", 900, 2);
                                        printStory(40, 200, 600, "等级：A+", 450, 1);
                                        printStory(40, 750, 600, "任意键退出", 450, 1);
                                        FlushBatchDraw();
                                        settextcolor(BLACK);
                                        arrA[0] = 1;
                                        getmessage(&m, EM_KEY);
                                        getmessage(&m, EM_KEY);
                                    }
                                    else {
                                        cls();
                                        backA();
                                        putimage_alpha(200, 50, &pos);
                                        settextcolor(RED);
                                        printStory(50, 270, 170, "民众愤怒，揭竿而起，你这个荒唐的总统，也当到头了……", 900, 2);
                                        printStory(40, 200, 600, "等级：D", 450, 1);
                                        printStory(40, 750, 600, "任意键退出", 450, 1);
                                        FlushBatchDraw();
                                        settextcolor(BLACK);
                                        arrD[1] = 1;
                                        getmessage(&m, EM_KEY);
                                        getmessage(&m, EM_KEY);
                                    }
                                }
                            }
                        }
                        else {
                            //school line
                            cls();
                            backA();
                            putimage_alpha(200, 50, &po);
                            printStory(40, 270, 170, "忽然，你好像梦醒了，在CY的课堂上。此时，大家站在齐刷刷地看你。卤蛋调侃说：”昨天晚上一定没干好事！“子二代附和。全班都在笑话你。但是，你不知道为什么。在你这里，你好像刚刚还在治理国家，现在就来到了这里———你的母校。", 900, 1);
                            printStory(35, 200, 600, "你接受事实", 450, 1);
                            printStory(35, 750, 600, "你不想接受事实", 450, 1);
                            FlushBatchDraw();
                            getmessage(&m, EM_KEY);
                            getmessage(&m, EM_KEY);
                            if (m.vkcode == 'M') {
                                cls();
                                backA();
                                putimage_alpha(200, 50, &po);
                                printStory(40, 270, 170, "经过一段时间后，你适应了校园生活。一天早上，下雨了。弘毅班（虾片的班级）的垫子还在外面。你选择？", 900, 1);
                                printStory(35, 200, 600, "去搬垫子", 450, 1);
                                printStory(35, 750, 600, "不去搬垫子，因为大家都没有去", 450, 1);
                                FlushBatchDraw();
                                getmessage(&m, EM_KEY);
                                getmessage(&m, EM_KEY);
                                if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
                                    cls();
                                    backA();
                                    putimage_alpha(200, 50, &po);
                                    printStory(40, 270, 115, "CY说：“早读时间很宝贵你不知道吗？？？生物知识点（当天是生物早读）你都背会了吗？？？我看你没有怎么好心，你就是纯粹地想要出来玩！……”讽刺的是，她进班了，又说：“为什么不是所有人都去搬垫子？？！！人品极其糟糕，思想品质极差！”“哪怕你搬垫子了，你也不是什么好东西”【这是什么暴论啊！】……。后来，你渐渐淡忘了这件事，你不想和她有仇有怨。", 900, 1);
                                    printStory(35, 200, 600, "你回寝室的路上和cy糖笑", 450, 1);
                                    printStory(35, 750, 600, "你回寝室的路上和cy对视", 450, 1);
                                    FlushBatchDraw();
                                    getmessage(&m, EM_KEY);
                                    getmessage(&m, EM_KEY);
                                    if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
                                        cls();
                                        backA();
                                        putimage_alpha(200, 50, &po);
                                        printStory(40, 270, 170, "cy糖笑回应无事发生", 900, 1);
                                        printStory(35, 200, 600, "安全回寝室开始写作业", 450, 1);
                                        printStory(35, 750, 600, "你选择自己写", 450, 1);
                                        FlushBatchDraw();
                                        getmessage(&m, EM_KEY);
                                        getmessage(&m, EM_KEY);
                                        if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
                                            cls();
                                            backA();
                                            putimage_alpha(200, 50, &po);
                                            settextcolor(RED);
                                            printStory(40, 270, 170, "常在河边走哪有不湿鞋，CY杀了一个回马枪，发现之后火冒三丈，让你抄写100边“抄作业的10大坏处”，与此同时，这是你第三遍抄这个东西了", 900, 2);
                                            printStory(40, 200, 600, "等级：C-", 450, 1);
                                            printStory(40, 750, 600, "任意键退出", 450, 1);
                                            FlushBatchDraw();
                                            settextcolor(BLACK);
                                            arrC[22] = 1;
                                            getmessage(&m, EM_KEY);
                                            getmessage(&m, EM_KEY);
                                        }
                                        else {
                                            cls();
                                            backA();
                                            putimage_alpha(200, 50, &po);
                                            printStory(40, 270, 170, "你也没有料到CY会杀一个回马枪，所以CY表扬了你一顿.顿时，你发现校园生活也并非如同你想象得你那么艰苦，前途难道不是一片光明吗？", 900, 1);
                                            printStory(35, 200, 600, "你担任地理课代表", 450, 1);
                                            printStory(35, 750, 600, "你选择潜心学习", 450, 1);
                                            FlushBatchDraw();
                                            getmessage(&m, EM_KEY);
                                            getmessage(&m, EM_KEY);
                                            if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
                                                cls();
                                                backA();
                                                putimage_alpha(200, 50, &po);
                                                printStory(40, 270, 130, "(有人也许会问，虾片原本的设定就是地理课代表，但是为什么你还要说他选要当地理课代表？这里运用了倒叙的手法)是啊，吗，每天就是去取地理导学案。有人也许会问为什么你要这样做，是啊，为什么呢？因为我深深地爱着地理……到了一节数学课", 900, 1);
                                                printStory(35, 200, 600, "数学课？算了算了，还是睡觉吧", 450, 1);
                                                printStory(35, 750, 600, "你不想睡觉，还是下一节语文再睡吧", 450, 1);
                                                FlushBatchDraw();
                                                getmessage(&m, EM_KEY);
                                                getmessage(&m, EM_KEY);
                                                if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
                                                    cls();
                                                    backA();
                                                    putimage_alpha(200, 50, &po);
                                                    settextcolor(RED);
                                                    printStory(40, 270, 130, "出乎意料的是，挖机居然没有制裁你。你睡眼朦胧地盯着挖机，看着黑板上的二次函数，还有一位同学的激情讲题（这位同学就是作者本人）.你忽然发觉——自己的青春都TM地留着干了什么？为什么要追悔莫及呢？没有任何必要了，保持奋斗，CY在后方大迈步地催着！", 900, 2);
                                                    printStory(40, 200, 600, "等级：A", 450, 1);
                                                    printStory(40, 750, 600, "任意键退出", 450, 1);
                                                    FlushBatchDraw();
                                                    settextcolor(BLACK);
                                                    arrA[21] = 1;
                                                    getmessage(&m, EM_KEY);
                                                    getmessage(&m, EM_KEY);
                                                }
                                                else {
                                                    cls();
                                                    backA();
                                                    putimage_alpha(200, 50, &po);
                                                    settextcolor(RED);
                                                    printStory(40, 270, 120, "用心写好每一题，开心过好每一天。不是看到了希望才去坚持，而是坚持了就会看到希望。为坚持不断的自己点赞。姬蛙的教诲深深镌刻在你的心里。姬蛙是一个什么样的人？你或许有点明白了。人民心中最红最红的红太阳。谁一心一意只为教学——姬蛙；谁一心一意只为学生——姬蛙。伟大的姬蛙！！！……", 900, 2);
                                                    printStory(40, 200, 600, "等级：S+", 450, 1);
                                                    printStory(40, 750, 600, "《挖机》文章已解锁，任意键退出", 450, 1);
                                                    FlushBatchDraw();
                                                    settextcolor(BLACK);
                                                    arrS[15] = 1;
                                                    getmessage(&m, EM_KEY);
                                                    getmessage(&m, EM_KEY);
                                                    isIntoLook4 = true;
                                                }
                                            }
                                            else {
                                                cls();
                                                backA();
                                                putimage_alpha(200, 50, &po);
                                                printStory(40, 270, 170, "转眼间到了跨年之际，班级将举行元旦晚会。", 900, 1);
                                                printStory(35, 200, 600, "你要参与", 450, 1);
                                                printStory(35, 750, 600, "你不参与", 450, 1);
                                                FlushBatchDraw();
                                                getmessage(&m, EM_KEY);
                                                getmessage(&m, EM_KEY);
                                                if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
                                                    cls();
                                                    backA();
                                                    putimage_alpha(200, 50, &po);
                                                    settextcolor(RED);
                                                    printStory(40, 270, 115, "你勇敢展示自己（的糖分），同时，卤蛋的舞蹈，子二代的朗诵“我爱数学”同样令人难忘。这注定是不眠之夜。到了213寝室，麦芒给了你们一人一个福橘。你们彻夜畅谈，彻夜畅谈……这或许就是美好吧……到了第二天。班主任要给213寝室合影留念。你们站在六角亭中，这时，躲在CY后面的F突然露面，露出邪魅的笑容。你实在绷不住唐笑了出来……这就是青春最根本的底色吧", 900, 2);
                                                    printStory(40, 200, 600, "等级：S-", 450, 1);
                                                    printStory(40, 750, 600, "《弘毅班》文章解锁中1/3，任意键退出", 450, 1);
                                                    FlushBatchDraw();
                                                    hy2 = 1;
                                                    settextcolor(BLACK);
                                                    arrS[16] = 1;
                                                    getmessage(&m, EM_KEY);
                                                    getmessage(&m, EM_KEY);
                                                    settextstyle(40, 0, "仿宋");
                                                    putimage(50, 30, &fa);
                                                    outtextxy(5, 5, "元旦晚会上弘毅班的合影纪念");
                                                    outtextxy(6, 5, "元旦晚会上弘毅班的合影纪念");
                                                    FlushBatchDraw();
                                                    getmessage(&m, EM_KEY);
                                                    getmessage(&m, EM_KEY);
                                                }
                                                else {
                                                    cls();
                                                    backA();
                                                    putimage_alpha(200, 50, &po);
                                                    printStory(40, 270, 130, "到了元旦晚会的那一天了，真可以说是群贤毕至。其中，惊险刺激的《荆轲刺秦王》，由灯塔出品。30min的旷世巨作，令人实在佩服……。后来啊，时间越走越快。一天早上。后面堆放的气球仍然放着。你没有觉得有什么不对", 900, 1);
                                                    printStory(35, 200, 600, "你大声早读", 450, 1);
                                                    printStory(35, 750, 600, "你早读大声", 450, 1);
                                                    FlushBatchDraw();
                                                    getmessage(&m, EM_KEY);
                                                    getmessage(&m, EM_KEY);
                                                    if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
                                                        cls();
                                                        backA();
                                                        putimage_alpha(200, 50, &po);
                                                        settextcolor(RED);
                                                        printStory(40, 270, 170, "忽然，CY脸色阴沉地进班，默默地一个一个地扎破那些气球。事实上，对于CY同志，我们需要辩证思考，你不能说她好，也不能说她差。至于说我们要如何去评价她呢？只能交给时间了。", 900, 2);
                                                        printStory(40, 200, 600, "等级：B", 450, 1);
                                                        printStory(40, 750, 600, "任意键退出", 450, 1);
                                                        FlushBatchDraw();
                                                        settextcolor(BLACK);
                                                        arrB[20] = 1;
                                                        getmessage(&m, EM_KEY);
                                                        getmessage(&m, EM_KEY);
                                                    }
                                                    else {
                                                        cls();
                                                        backA();
                                                        putimage_alpha(200, 50, &po);
                                                        settextcolor(RED);
                                                        printStory(40, 270, 170, "没有事情发生。在这个选项里，你身上好像没有什么值得记录的事情，你无疑是普通的，但是普通难道不也可贵吗？", 900, 2);
                                                        printStory(40, 200, 600, "等级：B+", 450, 1);
                                                        printStory(40, 750, 600, "任意键退出", 450, 1);
                                                        FlushBatchDraw();
                                                        settextcolor(BLACK);
                                                        arrB[21] = 1;
                                                        getmessage(&m, EM_KEY);
                                                        getmessage(&m, EM_KEY);
                                                    }
                                                }
                                            }
                                        }
                                    }
                                    else {
                                        cls();
                                        backA();
                                        putimage_alpha(200, 50, &po);
                                        printStory(40, 270, 170, "cy黑着脸不做回应.你仓皇逃窜", 900, 1);
                                        printStory(35, 200, 600, "回到寝室躲进厕所逃过一劫", 450, 1);
                                        printStory(35, 750, 600, "在床上和他人说话", 450, 1);
                                        FlushBatchDraw();
                                        getmessage(&m, EM_KEY);
                                        getmessage(&m, EM_KEY);
                                        if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
                                            cls();
                                            backA();
                                            putimage_alpha(200, 50, &po);
                                            printStory(40, 270, 170, "但是今天晚上的213寝室已经不是人了。首先，以卤蛋，灯塔为首的用床单，在两个床架之间以人力固定，做了一个吊床。现在他们邀请你去体验", 900, 1);
                                            printStory(35, 200, 600, "去", 450, 1);
                                            printStory(35, 750, 600, "不去", 450, 1);
                                            FlushBatchDraw();
                                            getmessage(&m, EM_KEY);
                                            getmessage(&m, EM_KEY);
                                            if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
                                                cls();
                                                backA();
                                                putimage_alpha(200, 50, &po);
                                                settextcolor(RED);
                                                printStory(40, 270, 170, "恭喜你中大奖了，你不知道你的重量吗。床单成功地被撕裂，但是幸运的是你的胃袋帮助你吸收了一定的冲击。你死里逃生", 900, 2);
                                                printStory(40, 200, 600, "等级：C-", 450, 1);
                                                printStory(40, 750, 600, "任意键退出", 450, 1);
                                                FlushBatchDraw();
                                                settextcolor(BLACK);
                                                arrC[23] = 1;
                                                getmessage(&m, EM_KEY);
                                                getmessage(&m, EM_KEY);
                                            }
                                            else {
                                                cls();
                                                backA();
                                                putimage_alpha(200, 50, &po);
                                                settextcolor(RED);
                                                printStory(40, 270, 170, "好的你居然有自知之明。现在，还是他们几个， 决定在寝室洗澡。哦天哪这都是一些什么东西啊？……但是，就是这些奇奇怪怪的东西，组成了213，组成了你的宝贵的青春啊", 900, 2);
                                                printStory(40, 200, 600, "等级：A", 450, 1);
                                                printStory(40, 750, 600, "《213》已解锁。任意键退出", 450, 1);
                                                FlushBatchDraw();
                                                settextcolor(BLACK);
                                                arrA[22] = 1;
                                                getmessage(&m, EM_KEY);
                                                getmessage(&m, EM_KEY);
                                                isIntoLook3 = true;
                                            }
                                        }
                                        else {
                                            cls();
                                            backA();
                                            putimage_alpha(200, 50, &po);
                                            printStory(40, 270, 170, "cy推门而入，cy 怒气冲冲的盯着你质问你的英语练习册呢.你沉默无言", 900, 1);
                                            printStory(35, 200, 600, "你给CY解释", 450, 1);
                                            printStory(35, 750, 600, "你不说话", 450, 1);
                                            FlushBatchDraw();
                                            getmessage(&m, EM_KEY);
                                            getmessage(&m, EM_KEY);
                                            if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
                                                cls();
                                                backA();
                                                putimage_alpha(200, 50, &po);
                                                printStory(40, 270, 170, "cy让你等着说下午讲练习册没写完就别要了.你点点头再也没说什么", 900, 1);
                                                printStory(35, 200, 600, "你选择和寝室的人说话", 450, 1);
                                                printStory(35, 750, 600, "你感到十分委屈", 450, 1);
                                                FlushBatchDraw();
                                                getmessage(&m, EM_KEY);
                                                getmessage(&m, EM_KEY);
                                                if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
                                                    cls();
                                                    backA();
                                                    putimage_alpha(200, 50, &po);
                                                    printStory(40, 270, 170, "门被推开", 900, 1);
                                                    printStory(35, 200, 600, "你背过身子", 450, 1);
                                                    printStory(35, 750, 600, "你眼神瞥向厕所", 450, 1);
                                                    FlushBatchDraw();
                                                    getmessage(&m, EM_KEY);
                                                    getmessage(&m, EM_KEY);
                                                    if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
                                                        cls();
                                                        backA();
                                                        putimage_alpha(200, 50, &po);
                                                        settextcolor(RED);
                                                        printStory(40, 270, 170, "你到口的话被堵住心情低落到了谷底联想到了今天的点点缩着被子靠着墙沉默无言，留下了泪水", 900, 2);
                                                        printStory(40, 200, 600, "等级：D-", 450, 1);
                                                        printStory(40, 750, 600, "任意键退出", 450, 1);
                                                        FlushBatchDraw();
                                                        settextcolor(BLACK);
                                                        arrD[14] = 1;
                                                        getmessage(&m, EM_KEY);
                                                        getmessage(&m, EM_KEY);
                                                    }
                                                    else {
                                                        cls();
                                                        backA();
                                                        putimage_alpha(200, 50, &po);
                                                        settextcolor(RED);
                                                        printStory(40, 270, 170, "老猫推开了门喊道身处厕所的2号唐笑着“是尿黄河还是拉井绳”你的心情被治愈开心的睡了过去", 900, 2);
                                                        printStory(40, 200, 600, "等级：B+", 450, 1);
                                                        printStory(40, 750, 600, "任意键退出", 450, 1);
                                                        FlushBatchDraw();
                                                        settextcolor(BLACK);
                                                        arrB[22] = 1;
                                                        getmessage(&m, EM_KEY);
                                                        getmessage(&m, EM_KEY);
                                                    }
                                                }
                                                else {
                                                    game();
                                                    while (peekmessage(&m, EM_KEY)) {

                                                    }
                                                    if (score > 100) {
                                                        cls();
                                                        backA();
                                                        putimage_alpha(200, 50, &po);
                                                        printStory(40, 270, 170, "无事发生。后来，你们学校良心发现，开展了远足活动", 900, 1);
                                                        printStory(35, 200, 600, "报名", 450, 1);
                                                        printStory(35, 750, 600, "不报名", 450, 1);
                                                        FlushBatchDraw();
                                                        getmessage(&m, EM_KEY);
                                                        getmessage(&m, EM_KEY);
                                                        if (m.vkcode == 'M') {
                                                            cls();
                                                            backA();
                                                            putimage_alpha(200, 50, &po);
                                                            printStory(40, 270, 170, "你们出发了，徒步地点是灵宝市寺河乡。此时春光正好，鸟语花香，你们开始了30余km的徒步之旅", 900, 1);
                                                            printStory(35, 200, 600, "你选择和先锋部队一起前行", 450, 1);
                                                            printStory(35, 750, 600, "你选择在后面慢慢悠悠", 450, 1);
                                                            FlushBatchDraw();
                                                            getmessage(&m, EM_KEY);
                                                            getmessage(&m, EM_KEY);
                                                            if (m.vkcode == 'M') {
                                                                cls();
                                                                backA();
                                                                putimage_alpha(200, 50, &po);
                                                                printStory(40, 270, 170, "渐渐，只因为CY的一句话：“我之前在弘毅班，只要是做什么都是第一名，但现在连翱翔班都走不过”，你便开启了一场追风（翱翔班）之旅，你们在前面叫嚣，被他们班班主任听见了。他们班任也允许了这场“追逐战”。大战一触即发……", 900, 1);
                                                                printStory(35, 200, 600, "你选择奋力冲锋！", 450, 1);
                                                                printStory(35, 750, 600, "你选择休息", 450, 1);
                                                                FlushBatchDraw();
                                                                getmessage(&m, EM_KEY);
                                                                getmessage(&m, EM_KEY);
                                                                if (m.vkcode == 'M') {
                                                                    cls();
                                                                    backA();
                                                                    putimage_alpha(200, 50, &po);
                                                                    printStory(40, 270, 170, "你们第一次打倒了翱翔班的班旗。然后，马哥直接发力。称此次行动为”打倒翱翔旗行动“。但是之后，翱翔班人数壮大起来，但是弘毅班只有零星几位同志。", 900, 1);
                                                                    printStory(35, 200, 600, "你选择紧跟马哥的步伐", 450, 1);
                                                                    printStory(35, 750, 600, "你选择突击激进：跟着杜浩，卤蛋，子二代三人的队伍走", 450, 1);
                                                                    FlushBatchDraw();
                                                                    getmessage(&m, EM_KEY);
                                                                    getmessage(&m, EM_KEY);
                                                                    if (m.vkcode == 'M') {
                                                                        funq();
                                                                    }
                                                                    else {
                                                                        cls();
                                                                        backA();
                                                                        putimage_alpha(200, 50, &po);
                                                                        printStory(40, 270, 130, "你们到了翱翔班的前面，但是还是寡不敌众，于是你们对着后方的马哥一行人回眸，他们明白了你们的用意，瞬间，两队的呼唤把翱翔班团团包围，余音绕梁！与此同时，卤蛋，子二代将上衣脱去因为炎热。不仅被一众女生骂“变态”，而且还有被杀虫剂袭击的风险……", 900, 1);
                                                                        printStory(35, 200, 600, "你选择和马哥回合", 450, 1);
                                                                        printStory(35, 750, 600, "你选择休息", 450, 1);
                                                                        FlushBatchDraw();
                                                                        getmessage(&m, EM_KEY);
                                                                        getmessage(&m, EM_KEY);
                                                                        if (m.vkcode == 'M') {
                                                                            funq();
                                                                        }
                                                                        else {
                                                                            funr();
                                                                        }
                                                                    }
                                                                }
                                                                else {
                                                                    funr();
                                                                }
                                                            }
                                                            else {
                                                                funr();
                                                            }
                                                        }
                                                        else {
                                                            cls();
                                                            backA();
                                                            putimage_alpha(200, 50, &po);
                                                            settextcolor(RED);
                                                            printStory(40, 270, 170, "你因参与集体活动不积极被CY扣上了一顶大帽子！", 900, 2);
                                                            printStory(40, 200, 600, "等级：D", 450, 1);
                                                            printStory(40, 750, 600, "任意键退出", 450, 1);
                                                            FlushBatchDraw();
                                                            settextcolor(BLACK);
                                                            arrD[15] = 1;
                                                            getmessage(&m, EM_KEY);
                                                            getmessage(&m, EM_KEY);
                                                        }
                                                    }
                                                    else {
                                                        cls();
                                                        backA();
                                                        putimage_alpha(200, 50, &po);
                                                        printStory(40, 270, 170, "CY决定压力你的父母，他们听从了CY的说辞，在周末回家教育了你一顿", 900, 1);
                                                        printStory(35, 200, 600, "你与他们辩解，说明CY有多么的偏激", 450, 1);
                                                        printStory(35, 750, 600, "你没有和他们辩解", 450, 1);
                                                        FlushBatchDraw();
                                                        getmessage(&m, EM_KEY);
                                                        getmessage(&m, EM_KEY);
                                                        if (m.vkcode == 'M') {
                                                            int b22 = rand() & 10;
                                                            if (b22 < 7) {
                                                                cls();
                                                                backA();
                                                                putimage_alpha(200, 50, &pos);
                                                                settextcolor(RED);
                                                                printStory(40, 270, 170, "他们相信了，从此，他们与你站在了一起！面对CY的高压统治，你无所畏惧，因为你有最强的后盾", 900, 2);
                                                                printStory(40, 200, 600, "等级：S-", 450, 1);
                                                                printStory(40, 750, 600, "任意键退出", 450, 1);
                                                                FlushBatchDraw();
                                                                settextcolor(BLACK);
                                                                arrS[17] = 1;
                                                                getmessage(&m, EM_KEY);
                                                                getmessage(&m, EM_KEY);
                                                            }
                                                            else {
                                                                cls();
                                                                backA();
                                                                putimage_alpha(200, 50, &pos);
                                                                settextcolor(RED);
                                                                printStory(40, 270, 170, "他们没有相信WHAT CAN I SAY!", 900, 2);
                                                                printStory(40, 200, 600, "等级：D+", 450, 1);
                                                                printStory(40, 750, 600, "任意键退出", 450, 1);
                                                                FlushBatchDraw();
                                                                settextcolor(BLACK);
                                                                arrD[16] = 1;
                                                                getmessage(&m, EM_KEY);
                                                                getmessage(&m, EM_KEY);
                                                            }
                                                        }
                                                        else {
                                                            cls();
                                                            backA();
                                                            putimage_alpha(200, 50, &po);
                                                            printStory(40, 270, 170, "因为您明白这无济于事，你便厌恶英语", 900, 1);
                                                            printStory(35, 200, 600, "你开始喜欢上数学", 450, 1);
                                                            printStory(35, 750, 600, "你开始喜欢地理", 450, 1);
                                                            FlushBatchDraw();
                                                            getmessage(&m, EM_KEY);
                                                            getmessage(&m, EM_KEY);
                                                            if (m.vkcode == 'M') {
                                                                cls();
                                                                backA();
                                                                putimage_alpha(200, 50, &po);
                                                                printStory(40, 270, 120, "范老师（挖机）的谆谆教诲让你进入了一个有声有色的数学世界里：这个世界里有迷人的二次函数曲线，有完美的圆形，有五彩缤纷的相似，有充满生命气息的函数……渐渐地你沉醉在数学的世界里了……是啊，数学是初中阶段最热烈的学科，就像初中生活一样……", 900, 1);
                                                                printStory(35, 200, 600, "你选择深耕数学", 450, 1);
                                                                printStory(35, 750, 600, "你选择把你的数学精神发扬光大", 450, 1);
                                                                FlushBatchDraw();
                                                                getmessage(&m, EM_KEY);
                                                                getmessage(&m, EM_KEY);
                                                                if (m.vkcode == 'M') {
                                                                    cls();
                                                                    backA();
                                                                    putimage_alpha(200, 50, &po);
                                                                    settextcolor(RED);
                                                                    printStory(40, 270, 170, "你成为了一名擅长数学的人", 900, 2);
                                                                    printStory(40, 200, 600, "等级：A+", 450, 1);
                                                                    printStory(40, 750, 600, "任意键退出", 450, 1);
                                                                    FlushBatchDraw();
                                                                    settextcolor(BLACK);
                                                                    arrA[25] = 1;
                                                                    getmessage(&m, EM_KEY);
                                                                    getmessage(&m, EM_KEY);
                                                                }
                                                                else {
                                                                    cls();
                                                                    backA();
                                                                    putimage_alpha(200, 50, &po);
                                                                    settextcolor(RED);
                                                                    printStory(40, 270, 120, "你每周6早上10——12点雷打不动的开展数学公益补弱，你为此无偿写点人程序，作PPT，作报表EXCEL……。有人问你？为什么你总是坚持它并为此奉献，只见你默默地回答——这个时代需要像雷锋一样的人，我这样做只是因为对数学，对挖机的热爱！", 900, 2);
                                                                    printStory(40, 200, 600, "等级：S+", 450, 1);
                                                                    printStory(40, 750, 600, "《数学补弱》已解锁。任意键退出", 450, 1);
                                                                    FlushBatchDraw();
                                                                    settextcolor(BLACK);
                                                                    arrS[20] = 1;
                                                                    isIntoLook = true;
                                                                    getmessage(&m, EM_KEY);
                                                                    getmessage(&m, EM_KEY);
                                                                    cls();
                                                                    backA();
                                                                    putimage(50, 30, &m1);
                                                                    settextstyle(50, 0, "楷体");
                                                                    outtextxy(30, 30, "数学补弱的珍贵相片资料,enter继续");
                                                                    outtextxy(31, 30, "数学补弱的珍贵相片资料,enter继续");
                                                                    FlushBatchDraw();
                                                                    getmessage(&m, EM_KEY);
                                                                    getmessage(&m, EM_KEY);
                                                                    cls();
                                                                    backA();
                                                                    putimage(50, 30, &m2);
                                                                    settextstyle(50, 0, "楷体");
                                                                    outtextxy(30, 30, "数学补弱的珍贵相片资料,enter继续");
                                                                    outtextxy(31, 30, "数学补弱的珍贵相片资料,enter继续");
                                                                    FlushBatchDraw();
                                                                    getmessage(&m, EM_KEY);
                                                                    getmessage(&m, EM_KEY);
                                                                    cls();
                                                                    backA();
                                                                    putimage(50, 30, &m3);
                                                                    settextstyle(50, 0, "楷体");
                                                                    outtextxy(30, 30, "数学补弱的珍贵相片资料,enter继续");
                                                                    outtextxy(31, 30, "数学补弱的珍贵相片资料,enter继续");
                                                                    FlushBatchDraw();
                                                                    getmessage(&m, EM_KEY);
                                                                    getmessage(&m, EM_KEY);
                                                                }
                                                            }
                                                            else {
                                                                cls();
                                                                backA();
                                                                putimage_alpha(200, 50, &po);
                                                                printStory(40, 270, 170, "身为地理课代表，却被地理老师说“不配当地理课代表”，这是什么样的行为？于是，你要为自己正名！", 900, 1);
                                                                printStory(35, 200, 600, "你在书本里体验地理", 450, 1);
                                                                printStory(35, 750, 600, "你在现实里体验地理", 450, 1);
                                                                FlushBatchDraw();
                                                                getmessage(&m, EM_KEY);
                                                                getmessage(&m, EM_KEY);
                                                                if (m.vkcode == 'M') {
                                                                    cls();
                                                                    backA();
                                                                    putimage_alpha(200, 50, &po);
                                                                    printStory(40, 270, 140, "你随着敏敏的课堂，进入了漠河那冰天雪地的北国风光，进入了海南那热情满满的热带地区，进入了“远看是山，近看成川”的青藏高原【藏南地区是中国不可分割的一部分】，进入了宝岛台湾，感受了那独具一格的民族风情【台湾是祖国不可分割的一部分】", 900, 1);
                                                                    printStory(35, 200, 600, "你帮敏敏干活", 450, 1);
                                                                    printStory(35, 750, 600, "你为同学们服务", 450, 1);
                                                                    FlushBatchDraw();
                                                                    getmessage(&m, EM_KEY);
                                                                    getmessage(&m, EM_KEY);
                                                                    if (m.vkcode == 'M') {
                                                                        cls();
                                                                        backA();
                                                                        putimage_alpha(200, 50, &po);
                                                                        settextcolor(RED);
                                                                        printStory(40, 270, 170, "你摸清了学校的文印室，一次次的导学案分发，都是你不可忘记的一部分", 900, 2);
                                                                        printStory(40, 200, 600, "等级：S", 450, 1);
                                                                        printStory(40, 750, 600, "任意键退出", 450, 1);
                                                                        FlushBatchDraw();
                                                                        settextcolor(BLACK);
                                                                        arrS[21] = 1;
                                                                        getmessage(&m, EM_KEY);
                                                                        getmessage(&m, EM_KEY);
                                                                    }
                                                                    else {
                                                                        cls();
                                                                        backA();
                                                                        putimage_alpha(200, 50, &po);
                                                                        settextcolor(RED);
                                                                        printStory(40, 270, 170, "一次期中考试完成后，你顶着压力，为班级同学放映《航拍中国·台湾》，你是人民的好地理课代表！", 900, 2);
                                                                        printStory(40, 200, 600, "等级：S+", 450, 1);
                                                                        printStory(40, 750, 600, "任意键退出", 450, 1);
                                                                        FlushBatchDraw();
                                                                        settextcolor(BLACK);
                                                                        arrS[22] = 1;
                                                                        getmessage(&m, EM_KEY);
                                                                        getmessage(&m, EM_KEY);
                                                                    }
                                                                }
                                                                else {
                                                                    cls();
                                                                    backA();
                                                                    putimage_alpha(200, 50, &po);
                                                                    settextcolor(RED);
                                                                    printStory(40, 270, 170, "由于作者自己都没有这种体验，所以我就不写了，希望玩到这里的玩家日后有可以实现环球旅行的资本！", 900, 2);
                                                                    printStory(40, 200, 600, "等级：R+", 450, 1);
                                                                    printStory(40, 750, 600, "任意键退出", 450, 1);
                                                                    FlushBatchDraw();
                                                                    settextcolor(BLACK);
                                                                    arrR[11] = 1;
                                                                    getmessage(&m, EM_KEY);
                                                                    getmessage(&m, EM_KEY);
                                                                    cd();
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                            else {
                                                cls();
                                                backA();
                                                putimage_alpha(200, 50, &po);
                                                printStory(40, 270, 170, "cy让你滚回班取练习册你在心里埋下了一颗仇恨的种子", 900, 1);
                                                printStory(35, 200, 600, "你学会了反抗", 450, 1);
                                                printStory(35, 750, 600, "你默默地拿出练习册去写", 450, 1);
                                                FlushBatchDraw();
                                                getmessage(&m, EM_KEY);
                                                getmessage(&m, EM_KEY);
                                                if (m.vkcode == 'M') {
                                                    cls();
                                                    backA();
                                                    putimage_alpha(200, 50, &po);
                                                    settextcolor(RED);
                                                    printStory(40, 270, 170, "为什么要想象一些不存在的东西？", 900, 2);
                                                    printStory(40, 200, 600, "等级：C+", 450, 1);
                                                    printStory(40, 750, 600, "任意键退出", 450, 1);
                                                    FlushBatchDraw();
                                                    settextcolor(BLACK);
                                                    arrC[26] = 1;
                                                    getmessage(&m, EM_KEY);
                                                    getmessage(&m, EM_KEY);
                                                }
                                                else {
                                                    cls();
                                                    backA();
                                                    putimage_alpha(200, 50, &po);
                                                    printStory(40, 270, 170, "到了下午，英语老师照常讲练习册，你正好写了，这时CY表扬了你", 900, 1);
                                                    printStory(35, 200, 600, "你认为这是鳄鱼的伪善", 450, 1);
                                                    printStory(35, 750, 600, "你认为这是浪子的回头", 450, 1);
                                                    FlushBatchDraw();
                                                    getmessage(&m, EM_KEY);
                                                    getmessage(&m, EM_KEY);
                                                    if (m.vkcode == 'M') {
                                                        cls();
                                                        backA();
                                                        putimage_alpha(200, 50, &po);
                                                        printStory(40, 270, 170, "此后，你开始从内心里厌恶CY，不是因为什么，而是因为你的偏见。后来，进行了一场考试", 900, 1);
                                                        printStory(35, 200, 600, "o", 450, 1);
                                                        printStory(35, 750, 600, "p", 450, 1);
                                                        FlushBatchDraw();
                                                        getmessage(&m, EM_KEY);
                                                        getmessage(&m, EM_KEY);
                                                        if (m.vkcode == 'M') {
                                                            cls();
                                                            backA();
                                                            putimage_alpha(200, 50, &po);
                                                            settextcolor(RED);
                                                            printStory(40, 270, 170, "你过关，此后啊，CY同志的声影在你的脑海里被淡忘，她是什么？几十年后的你想回答这个问题，却“张口欲唱声却哑”了……", 900, 2);
                                                            printStory(40, 200, 600, "等级：C+", 450, 1);
                                                            printStory(40, 750, 600, "任意键退出", 450, 1);
                                                            FlushBatchDraw();
                                                            settextcolor(BLACK);
                                                            arrC[27] = 1;
                                                            getmessage(&m, EM_KEY);
                                                            getmessage(&m, EM_KEY);
                                                        }
                                                        else {
                                                            cls();
                                                            backA();
                                                            putimage_alpha(200, 50, &po);
                                                            settextcolor(RED);
                                                            printStory(40, 270, 170, "你明白了你对CY的差劲决定了你的差劲，在最后的日子里，CY也不再说你。“我不打你也不骂你但是我就是不管你”……", 900, 2);
                                                            printStory(40, 200, 600, "等级：D", 450, 1);
                                                            printStory(40, 750, 600, "任意键退出", 450, 1);
                                                            FlushBatchDraw();
                                                            settextcolor(BLACK);
                                                            arrD[17] = 1;
                                                            getmessage(&m, EM_KEY);
                                                            getmessage(&m, EM_KEY);
                                                        }
                                                    }
                                                    else {
                                                        cls();
                                                        backA();
                                                        putimage_alpha(200, 50, &po);
                                                        settextcolor(RED);
                                                        printStory(40, 270, 170, "此后，你对CY笑脸响应。正如CY所说：”当你看我不顺时，说明你在走下坡路，反之亦然“。这句话的思想深度非常有！", 900, 2);
                                                        printStory(40, 200, 600, "等级：A", 450, 1);
                                                        printStory(40, 750, 600, "任意键退出", 450, 1);
                                                        FlushBatchDraw();
                                                        settextcolor(BLACK);
                                                        arrA[26] = 1;
                                                        getmessage(&m, EM_KEY);
                                                        getmessage(&m, EM_KEY);
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                                else {
                                    cls();
                                    backA();
                                    putimage_alpha(200, 50, &po);
                                    printStory(40, 270, 170, "CY说：“为什么不是所有人都去搬垫子？？！！人品极其糟糕，思想品质极差！“她像当年红卫兵批斗他人一样批斗你们！她骂骂咧咧地让你们都去搬垫子！F小声地说了一句“你的意思是？”", 900, 1);
                                    printStory(35, 200, 600, "你又冒雨搬垫子", 450, 1);
                                    printStory(35, 750, 600, "你不说话", 450, 1);
                                    FlushBatchDraw();
                                    getmessage(&m, EM_KEY);
                                    getmessage(&m, EM_KEY);
                                    if (m.vkcode == 'M') {
                                        cls();
                                        backA();
                                        putimage_alpha(200, 50, &po);
                                        printStory(40, 270, 130, "雨水打湿了你的衣裳，也打湿了你的心……回来后CY又不满足，让你们站在后面写检讨。你tm实在不知道她意欲何为？别的不说，关键是她占用的是生物早读！!该死不得活的东西啊，你心里想着……。后来，CY开始主抓体育：“鼓足干劲力争上游，多快好省地使弘毅班体育突飞猛进！”而现在，你正好吃完了晚饭", 900, 1);
                                        printStory(35, 200, 600, "你去练屈", 450, 1);
                                        printStory(35, 750, 600, "你去练排球", 450, 1);
                                        FlushBatchDraw();
                                        getmessage(&m, EM_KEY);
                                        getmessage(&m, EM_KEY);
                                        if (m.vkcode == 'M') {
                                            cls();
                                            backA();
                                            putimage_alpha(200, 50, &po);
                                            printStory(40, 270, 170, "CY瞪了你一眼，但没有说什么。之后，她让你们进班，没想到是因为体育考试的事情，她让你们这群体前屈不满分的上台检讨——这已经是老生常谈了。时间一转，来到了第二天的体育训练", 900, 1);
                                            printStory(35, 200, 600, "你选择大跨步走——因为昨天CY对你的“洗脑”有用了", 450, 1);
                                            printStory(35, 750, 600, "你选择正常训练", 450, 1);
                                            FlushBatchDraw();
                                            getmessage(&m, EM_KEY);
                                            getmessage(&m, EM_KEY);
                                            if (m.vkcode == 'M') {
                                                cls();
                                                backA();
                                                putimage_alpha(200, 50, &po);
                                                printStory(40, 270, 170, "突然，你好像受伤了一般倒地，与此同时，CY也来了，但是CY并没有骂你，而是亲切地把你送去医院去。甚至，她还请你喝一杯奶茶。从此以后，你对CY这个人有了新的看法。", 900, 1);
                                                printStory(35, 200, 600, "你偷偷飘到她的手机", 450, 1);
                                                printStory(35, 750, 600, "你潜入他的办公室", 450, 1);
                                                FlushBatchDraw();
                                                getmessage(&m, EM_KEY);
                                                getmessage(&m, EM_KEY);
                                                if (m.vkcode == 'M') {
                                                    cls();
                                                    backA();
                                                    putimage_alpha(200, 50, &po);
                                                    settextcolor(RED);
                                                    printStory(40, 270, 170, "你看见了她的病情，你又想到她之前对您们说的“我过年都是在黄河医院过的”……哎，空悲切", 900, 2);
                                                    printStory(40, 200, 600, "等级：B", 450, 1);
                                                    printStory(40, 750, 600, "任意键退出", 450, 1);
                                                    FlushBatchDraw();
                                                    settextcolor(BLACK);
                                                    arrB[23] = 1;
                                                    getmessage(&m, EM_KEY);
                                                    getmessage(&m, EM_KEY);
                                                }
                                                else {
                                                    cls();
                                                    backA();
                                                    putimage_alpha(200, 50, &po);
                                                    settextcolor(RED);
                                                    printStory(40, 270, 170, "你看着她的荣誉，心里不觉五味杂陈。或许，CY真的不是你想象的那样坏", 900, 2);
                                                    printStory(40, 200, 600, "等级：A+", 450, 1);
                                                    printStory(40, 750, 600, "任意键退出", 450, 1);
                                                    FlushBatchDraw();
                                                    settextcolor(BLACK);
                                                    arrA[27] = 1;
                                                    getmessage(&m, EM_KEY);
                                                    getmessage(&m, EM_KEY);
                                                }
                                            }
                                            else {
                                                cls();
                                                backA();
                                                putimage_alpha(200, 50, &po);
                                                printStory(40, 270, 170, "无事发生，过了很久很久，要体育考试了，可见仅仅是从东门入还是西门入两个考官就争论了一辈子“脏谁摊子！”【化用CY经典语言】", 900, 1);
                                                printStory(35, 200, 600, "你先去测跳绳", 450, 1);
                                                printStory(35, 750, 600, "你先去测屈", 450, 1);
                                                FlushBatchDraw();
                                                getmessage(&m, EM_KEY);
                                                getmessage(&m, EM_KEY);
                                                if (m.vkcode == 'M') {
                                                    cls();
                                                    backA();
                                                    putimage_alpha(200, 50, &po);
                                                    settextcolor(RED);
                                                    printStory(40, 270, 170, "你成功了，你们几个男生先考完，用教室的多媒体放了一曲《辞九门回忆》，这就是最热血沸腾啊！", 900, 2);
                                                    printStory(40, 200, 600, "等级：S+", 450, 1);
                                                    printStory(40, 750, 600, "任意键退出", 450, 1);
                                                    FlushBatchDraw();
                                                    settextcolor(BLACK);
                                                    arrS[23] = 1;
                                                    getmessage(&m, EM_KEY);
                                                    getmessage(&m, EM_KEY);
                                                }
                                                else {
                                                    cls();
                                                    backA();
                                                    putimage_alpha(200, 50, &po);
                                                    settextcolor(RED);
                                                    printStory(40, 270, 170, "你没有取得满分.你被CY批斗，但——总归一切都结束了", 900, 2);
                                                    printStory(40, 200, 600, "等级：B-", 450, 1);
                                                    printStory(40, 750, 600, "任意键退出", 450, 1);
                                                    FlushBatchDraw();
                                                    settextcolor(BLACK);
                                                    arrB[24] = 1;
                                                    getmessage(&m, EM_KEY);
                                                    getmessage(&m, EM_KEY);
                                                }
                                            }
                                        }
                                        else {
                                            cls();
                                            backA();
                                            putimage_alpha(200, 50, &po);
                                            printStory(40, 270, 170, "CY说你是“不好好练”，在班级里批评了你。现在晚上回寝了", 900, 1);
                                            printStory(35, 200, 600, "你嚎啕大哭", 450, 1);
                                            printStory(35, 750, 600, "你坚强起来", 450, 1);
                                            FlushBatchDraw();
                                            getmessage(&m, EM_KEY);
                                            getmessage(&m, EM_KEY);
                                            if (m.vkcode == 'M') {
                                                cls();
                                                backA();
                                                putimage_alpha(200, 50, &po);
                                                printStory(40, 270, 170, "哦我的天啊，213室的成员对你嘘寒问暖？不一会，你便恢复好了你的神情", 900, 1);
                                                printStory(35, 200, 600, "你照镜子", 450, 1);
                                                printStory(35, 750, 600, "你和洋盆聊天", 450, 1);
                                                FlushBatchDraw();
                                                getmessage(&m, EM_KEY);
                                                getmessage(&m, EM_KEY);
                                                if (m.vkcode == 'M') {
                                                    int b23 = rand() % 2;
                                                    if (b23 == 0) {
                                                        cls();
                                                        backA();
                                                        putimage_alpha(200, 50, &pos);
                                                        settextcolor(RED);
                                                        printStory(40, 270, 170, "麦芒说，你太帅了。虽然这个概率比作者成为百万富翁的概率还低", 900, 2);
                                                        printStory(40, 200, 600, "等级：S-", 450, 1);
                                                        printStory(40, 750, 600, "任意键退出", 450, 1);
                                                        FlushBatchDraw();
                                                        settextcolor(BLACK);
                                                        arrS[24] = 1;
                                                        getmessage(&m, EM_KEY);
                                                        getmessage(&m, EM_KEY);
                                                    }
                                                    else {
                                                        cls();
                                                        backA();
                                                        putimage_alpha(200, 50, &pos);
                                                        printStory(40, 270, 170, "麦芒直接拿你和蛆比较", 900, 1);
                                                        printStory(35, 200, 600, "你直接反驳", 450, 1);
                                                        printStory(35, 750, 600, "你直接又嚎啕大哭", 450, 1);
                                                        FlushBatchDraw();
                                                        getmessage(&m, EM_KEY);
                                                        getmessage(&m, EM_KEY);
                                                        if (m.vkcode == 'M') {
                                                            cls();
                                                            backA();
                                                            putimage_alpha(200, 50, &po);
                                                            settextcolor(RED);
                                                            printStory(40, 270, 170, "你败了下风，毕竟鲶鱼也是这么认为的！。“我心隐隐作痛”你无助地喊着", 900, 2);
                                                            printStory(40, 200, 600, "等级：B", 450, 1);
                                                            printStory(40, 750, 600, "任意键退出", 450, 1);
                                                            FlushBatchDraw();
                                                            settextcolor(BLACK);
                                                            arrB[25] = 1;
                                                            getmessage(&m, EM_KEY);
                                                            getmessage(&m, EM_KEY);
                                                        }
                                                        else {
                                                            cls();
                                                            backA();
                                                            putimage_alpha(200, 50, &po);
                                                            settextcolor(RED);
                                                            printStory(40, 270, 170, "可惜的是，你中途唐笑被卤蛋发现了，你的阴谋失败了", 900, 2);
                                                            printStory(40, 200, 600, "等级：C-", 450, 1);
                                                            printStory(40, 750, 600, "任意键退出", 450, 1);
                                                            FlushBatchDraw();
                                                            settextcolor(BLACK);
                                                            arrC[28] = 1;
                                                            getmessage(&m, EM_KEY);
                                                            getmessage(&m, EM_KEY);
                                                        }
                                                    }
                                                }
                                                else {
                                                    cls();
                                                    backA();
                                                    putimage_alpha(200, 50, &po);
                                                    settextcolor(RED);
                                                    printStory(40, 270, 170, "可见仅仅5分钟你们就说了13遍超自然，31遍“你糖精吧”因为太糖了，所以我拒绝继续编", 900, 2);
                                                    printStory(40, 200, 600, "等级：R", 450, 1);
                                                    printStory(40, 750, 600, "任意键退出", 450, 1);
                                                    FlushBatchDraw();
                                                    settextcolor(BLACK);
                                                    arrR[12] = 1;
                                                    getmessage(&m, EM_KEY);
                                                    getmessage(&m, EM_KEY);
                                                    say();
                                                }
                                            }
                                            else {
                                                cls();
                                                backA();
                                                putimage_alpha(200, 50, &po);
                                                printStory(40, 270, 170, "你像一个无事人一样", 900, 1);
                                                printStory(35, 200, 600, "你与洋盆谈论“换绑”", 450, 1);
                                                printStory(35, 750, 600, "你与麦子谈论“糖心病毒”", 450, 1);
                                                FlushBatchDraw();
                                                getmessage(&m, EM_KEY);
                                                getmessage(&m, EM_KEY);
                                                if (m.vkcode == 'M') {
                                                    cls();
                                                    backA();
                                                    putimage_alpha(200, 50, &po);
                                                    settextcolor(RED);
                                                    printStory(40, 270, 170, "你真的TMD是一个SB，没给钱就换绑。老子不编了", 900, 2);
                                                    printStory(40, 200, 600, "等级：D-", 450, 1);
                                                    printStory(40, 750, 600, "任意键退出", 450, 1);
                                                    FlushBatchDraw();
                                                    settextcolor(BLACK);
                                                    arrD[18] = 1;
                                                    getmessage(&m, EM_KEY);
                                                    getmessage(&m, EM_KEY);
                                                }
                                                else {
                                                    cls();
                                                    backA();
                                                    putimage_alpha(200, 50, &po);
                                                    settextcolor(RED);
                                                    printStory(40, 270, 170, "你也真的是一个SB，我不想多说", 900, 2);
                                                    printStory(40, 200, 600, "等级：D", 450, 1);
                                                    printStory(40, 750, 600, "任意键退出", 450, 1);
                                                    FlushBatchDraw();
                                                    settextcolor(BLACK);
                                                    arrD[19] = 1;
                                                    getmessage(&m, EM_KEY);
                                                    getmessage(&m, EM_KEY);
                                                }
                                            }
                                        }
                                    }
                                    else {
                                        cls();
                                        backA();
                                        putimage_alpha(200, 50, &po);
                                        printStory(40, 270, 170, "CY把你数落了一顿。原因你下辈子都想不到！只因你在某一次讲话中没有问好，但是当时你上去发言都算是给CY面子了。所以，你这辈子都不会想到这两件事之间有什么联系！", 900, 1);
                                        printStory(35, 200, 600, "你觉得不合理，但你没有反驳", 450, 1);
                                        printStory(35, 750, 600, "你反驳了CY", 450, 1);
                                        FlushBatchDraw();
                                        getmessage(&m, EM_KEY);
                                        getmessage(&m, EM_KEY);
                                        if (m.vkcode == 'M') {
                                            cls();
                                            backA();
                                            putimage_alpha(200, 50, &po);
                                            printStory(40, 270, 170, "没事发生，下节是英语课，但是你们才做完生物实验。", 900, 1);
                                            printStory(35, 200, 600, "你选择快步回班", 450, 1);
                                            printStory(35, 750, 600, "你选择慢慢回班，反正还有这么多人！", 450, 1);
                                            FlushBatchDraw();
                                            getmessage(&m, EM_KEY);
                                            getmessage(&m, EM_KEY);
                                            if (m.vkcode == 'M') {
                                                cls();
                                                backA();
                                                putimage_alpha(200, 50, &po);
                                                printStory(40, 270, 170, "老师选择考试，请你自求多福", 900, 1);
                                                printStory(35, 200, 600, "若k属于N,记2k+1属于T，a属于T,b属于T.ab属于T", 450, 1);
                                                printStory(35, 750, 600, "若k属于N,记2k+1属于T，a属于T,b属于T.ab不属于T", 450, 1);
                                                FlushBatchDraw();
                                                getmessage(&m, EM_KEY);
                                                getmessage(&m, EM_KEY);
                                                if (m.vkcode == 'M') {
                                                    cls();
                                                    backA();
                                                    putimage_alpha(200, 50, &po);
                                                    printStory(40, 270, 170, "你被评为优秀学生，CY想把你单独培养", 900, 1);
                                                    printStory(35, 200, 600, "去", 450, 1);
                                                    printStory(35, 750, 600, "不去", 450, 1);
                                                    FlushBatchDraw();
                                                    getmessage(&m, EM_KEY);
                                                    getmessage(&m, EM_KEY);
                                                    if (m.vkcode == 'M') {
                                                        cls();
                                                        backA();
                                                        putimage_alpha(200, 50, &po);
                                                        settextcolor(RED);
                                                        printStory(40, 270, 170, "你成为了你以前所梦寐以求的好学生，但是洋盆等你以前的挚友渐渐疏远了", 900, 2);
                                                        printStory(40, 200, 600, "等级：A+", 450, 1);
                                                        printStory(40, 750, 600, "任意键退出", 450, 1);
                                                        FlushBatchDraw();
                                                        settextcolor(BLACK);
                                                        arrA[28] = 1;
                                                        getmessage(&m, EM_KEY);
                                                        getmessage(&m, EM_KEY);
                                                    }
                                                    else {
                                                        cls();
                                                        backA();
                                                        putimage_alpha(200, 50, &po);
                                                        settextcolor(RED);
                                                        printStory(40, 270, 170, "你又成为了以前的样子，和你的朋友", 900, 2);
                                                        printStory(40, 200, 600, "等级：A", 450, 1);
                                                        printStory(40, 750, 600, "任意键退出", 450, 1);
                                                        FlushBatchDraw();
                                                        settextcolor(BLACK);
                                                        arrA[29] = 1;
                                                        getmessage(&m, EM_KEY);
                                                        getmessage(&m, EM_KEY);
                                                    }
                                                }
                                                else {
                                                    cls();
                                                    backA();
                                                    putimage_alpha(200, 50, &po);
                                                    settextcolor(RED);
                                                    printStory(40, 270, 170, "直接劝退", 900, 2);
                                                    printStory(40, 200, 600, "等级：D", 450, 1);
                                                    printStory(40, 750, 600, "任意键退出", 450, 1);
                                                    FlushBatchDraw();
                                                    settextcolor(BLACK);
                                                    arrD[20] = 1;
                                                    getmessage(&m, EM_KEY);
                                                    getmessage(&m, EM_KEY);
                                                }
                                            }
                                            else {
                                                cls();
                                                backA();
                                                putimage_alpha(200, 50, &po);
                                                printStory(40, 270, 150, "不幸的是，你们都被CY制裁了，美其名曰“没有时间观念”，但是完全忽视普遍自然规律！她让你们写检讨【讲真的，这个东西已经见怪不怪了！】接着，她便开始上课。她叫你回答问题，可是你不会", 900, 1);
                                                printStory(35, 200, 600, "你诚实地说“我不会”", 450, 1);
                                                printStory(35, 750, 600, "你说你试试.但是你支支吾吾半天却不说话", 450, 1);
                                                FlushBatchDraw();
                                                getmessage(&m, EM_KEY);
                                                getmessage(&m, EM_KEY);
                                                if (m.vkcode == 'M') {
                                                    cls();
                                                    backA();
                                                    putimage_alpha(200, 50, &po);
                                                    printStory(40, 270, 150, "恭喜您，触发CY连招【在我滴课上（停顿5s，瞪你一眼），你可以说“我试试”，你可以说“我看看”，但是（停顿3s），你不能说，“我不会”！！！】，你被CY思想教育了，但是你始终不认为你是错的", 900, 1);
                                                    printStory(35, 200, 600, "你揭竿而起，骂了CY【参见7年级（灯塔）事件】", 450, 1);
                                                    printStory(35, 750, 600, "你没有说话", 450, 1);
                                                    FlushBatchDraw();
                                                    getmessage(&m, EM_KEY);
                                                    getmessage(&m, EM_KEY);
                                                    if (m.vkcode == 'M') {
                                                        cls();
                                                        backA();
                                                        putimage_alpha(200, 50, &po);
                                                        printStory(40, 270, 170, "CY现实一点点的震惊，然后反应过来后直接把你劝退", 900, 1);
                                                        printStory(35, 200, 600, "你与CY硬钢到底", 450, 1);
                                                        printStory(35, 750, 600, "你选择服软", 450, 1);
                                                        FlushBatchDraw();
                                                        getmessage(&m, EM_KEY);
                                                        getmessage(&m, EM_KEY);
                                                        if (m.vkcode == 'M') {
                                                            cls();
                                                            backA();
                                                            putimage_alpha(200, 50, &po);
                                                            settextcolor(RED);
                                                            printStory(40, 270, 170, "最后CY迫不得已只能请你来学校", 900, 2);
                                                            printStory(40, 200, 600, "等级：A-", 450, 1);
                                                            printStory(40, 750, 600, "任意键退出", 450, 1);
                                                            FlushBatchDraw();
                                                            settextcolor(BLACK);
                                                            arrA[30] = 1;
                                                            getmessage(&m, EM_KEY);
                                                            getmessage(&m, EM_KEY);
                                                        }
                                                        else {
                                                            cls();
                                                            backA();
                                                            putimage_alpha(200, 50, &po);
                                                            settextcolor(RED);
                                                            printStory(40, 270, 170, "CY一直拿你的事情说事，你说说你为什么不再坚持一会呢？", 900, 2);
                                                            printStory(40, 200, 600, "等级：C", 450, 1);
                                                            printStory(40, 750, 600, "任意键退出", 450, 1);
                                                            FlushBatchDraw();
                                                            settextcolor(BLACK);
                                                            arrC[29] = 1;
                                                            getmessage(&m, EM_KEY);
                                                            getmessage(&m, EM_KEY);
                                                        }
                                                    }
                                                    else {
                                                        cls();
                                                        backA();
                                                        putimage_alpha(200, 50, &po);
                                                        printStory(40, 270, 170, "忽然，你晕倒了，再睁眼，你回到了一个非常神奇的地方。你在这里四处游荡，《4.11》《6.6》……这些是什么意思？……你看到了一些图片，上面好像记载着什么东西……“TD万岁”？“为人民服务”……", 900, 1);
                                                        printStory(35, 200, 600, "你发现了一个程序", 450, 1);
                                                        printStory(35, 750, 600, "你发现了一些文档", 450, 1);
                                                        FlushBatchDraw();
                                                        getmessage(&m, EM_KEY);
                                                        getmessage(&m, EM_KEY);
                                                        if (m.vkcode == 'M') {
                                                            cls();
                                                            backA();
                                                            putimage_alpha(200, 50, &po);
                                                            settextcolor(RED);
                                                            printStory(40, 270, 130, "程序非常简单，一个用来点名的……你看着上面的人啊，若有所思……你手误输入了“2”……；……这里似乎有什么非常重要的东西……终于，你在一个角落发现了一摞数学卷子，上面用红笔标注了题……依稀可见【画圈为补弱讲题】的字样……；……这里好像发生过什么，但你不记得了……。", 900, 2);
                                                            printStory(40, 200, 600, "等级：R+", 450, 1);
                                                            printStory(40, 750, 600, "任意键退出", 450, 1);
                                                            FlushBatchDraw();
                                                            settextcolor(BLACK);
                                                            arrR[13] = 1;
                                                            getmessage(&m, EM_KEY);
                                                            getmessage(&m, EM_KEY);
                                                            cls();
                                                            backA();
                                                            putimage(50, 30, &um1);
                                                            settextstyle(50, 0, "楷体");
                                                            outtextxy(30, 30, "数学补弱的珍贵相片资料,enter继续");
                                                            outtextxy(31, 30, "数学补弱的珍贵相片资料,enter继续");
                                                            FlushBatchDraw();
                                                            getmessage(&m, EM_KEY);
                                                            getmessage(&m, EM_KEY);
                                                            cls();
                                                            backA();
                                                            putimage(50, 30, &um2);
                                                            settextstyle(50, 0, "楷体");
                                                            outtextxy(30, 30, "数学补弱的珍贵相片资料,enter继续");
                                                            outtextxy(31, 30, "数学补弱的珍贵相片资料,enter继续");
                                                            FlushBatchDraw();
                                                            getmessage(&m, EM_KEY);
                                                            getmessage(&m, EM_KEY);
                                                            cls();
                                                            backA();
                                                            putimage(50, 30, &um3);
                                                            settextstyle(50, 0, "楷体");
                                                            outtextxy(30, 30, "数学补弱的珍贵相片资料,enter继续");
                                                            outtextxy(31, 30, "数学补弱的珍贵相片资料,enter继续");
                                                            FlushBatchDraw();
                                                            getmessage(&m, EM_KEY);
                                                            getmessage(&m, EM_KEY);
                                                            cd();
                                                        }
                                                        else {
                                                            cls();
                                                            backA();
                                                            putimage_alpha(200, 50, &po);
                                                            settextcolor(RED);
                                                            printStory(40, 270, 130, "“用心写好每一题，开心过好每一天！”你呢喃道……“fan”……突然，你脑海了出现了一些绮丽的场面：那是一个晚上，台下观众欣喜若狂啊！……好像以一个叫“娃娃”的人为首……你们在会议里激烈讨论着……你不知道这些事什么东西，但是，仿佛在你的心底，有着一种极其深远的力量，促使着你……", 900, 2);
                                                            printStory(40, 200, 600, "等级：R+", 450, 1);
                                                            printStory(40, 750, 600, "《弘毅班》前1/3已解锁。任意键退出", 450, 1);
                                                            FlushBatchDraw();
                                                            settextcolor(BLACK);
                                                            hy3 = 1;
                                                            arrR[14] = 1;
                                                            getmessage(&m, EM_KEY);
                                                            getmessage(&m, EM_KEY);
                                                            cls();
                                                            backA();
                                                            putimage(50, 30, &um4);
                                                            settextstyle(50, 0, "楷体");
                                                            outtextxy(30, 30, "数学补弱的珍贵相片资料,enter继续");
                                                            outtextxy(31, 30, "数学补弱的珍贵相片资料,enter继续");
                                                            FlushBatchDraw();
                                                            getmessage(&m, EM_KEY);
                                                            getmessage(&m, EM_KEY);
                                                            cls();
                                                            backA();
                                                            putimage(50, 30, &um5);
                                                            settextstyle(50, 0, "楷体");
                                                            outtextxy(30, 30, "数学补弱的珍贵相片资料,enter继续");
                                                            outtextxy(31, 30, "数学补弱的珍贵相片资料,enter继续");
                                                            FlushBatchDraw();
                                                            getmessage(&m, EM_KEY);
                                                            getmessage(&m, EM_KEY);
                                                            cls();
                                                            backA();
                                                            putimage(50, 30, &um6);
                                                            settextstyle(50, 0, "楷体");
                                                            outtextxy(30, 30, "数学补弱的珍贵相片资料,enter继续");
                                                            outtextxy(31, 30, "数学补弱的珍贵相片资料,enter继续");
                                                            FlushBatchDraw();
                                                            getmessage(&m, EM_KEY);
                                                            getmessage(&m, EM_KEY);
                                                            say();
                                                        }
                                                    }
                                                }
                                                else {
                                                    cls();
                                                    backA();
                                                    putimage_alpha(200, 50, &po);
                                                    settextcolor(RED);
                                                    printStory(40, 270, 170, "恭喜您，触发CY连招【在我滴课上（停顿5s，瞪你一眼），你会就是会，不会就是不会，没必要装，没有必要。你升学是为我生的？】", 900, 2);
                                                    printStory(40, 200, 600, "等级：B", 450, 1);
                                                    printStory(40, 750, 600, "任意键退出", 450, 1);
                                                    FlushBatchDraw();
                                                    settextcolor(BLACK);
                                                    arrB[26] = 1;
                                                    getmessage(&m, EM_KEY);
                                                    getmessage(&m, EM_KEY);
                                                }
                                            }
                                        }
                                        else {
                                            cls();
                                            backA();
                                            putimage_alpha(200, 50, &po);
                                            printStory(40, 270, 170, "CY说你是不是找死？", 900, 1);
                                            printStory(35, 200, 600, "“如果威胁是你的力量，那没有它你又算什么？”", 450, 1);
                                            printStory(35, 750, 600, "“子非我，安知我找死乎？”", 450, 1);
                                            FlushBatchDraw();
                                            getmessage(&m, EM_KEY);
                                            getmessage(&m, EM_KEY);
                                            if (m.vkcode == 'M') {
                                                cls();
                                                backA();
                                                putimage_alpha(200, 50, &po);
                                                printStory(40, 270, 170, "CY说：“我又没有威胁你！”", 900, 1);
                                                printStory(35, 200, 600, "“如果说空是你的力量，那没有它你又算什么？”", 450, 1);
                                                printStory(35, 750, 600, "“如果反驳是你的力量，那没有它你又算什么？”", 450, 1);
                                                FlushBatchDraw();
                                                getmessage(&m, EM_KEY);
                                                getmessage(&m, EM_KEY);
                                                if (m.vkcode == 'M') {
                                                    cls();
                                                    backA();
                                                    putimage_alpha(200, 50, &po);
                                                    printStory(40, 270, 170, "CY说：“你竟然敢顶撞老师？我不活了，我做错什么了，我rtm……“", 900, 1);
                                                    printStory(35, 200, 600, "”如果卖惨是你的力量，那没有它你又算什么？“", 450, 1);
                                                    printStory(35, 750, 600, "请循其本,你缓缓地说", 450, 1);
                                                    FlushBatchDraw();
                                                    getmessage(&m, EM_KEY);
                                                    getmessage(&m, EM_KEY);
                                                    if (m.vkcode == 'M') {
                                                        cls();
                                                        backA();
                                                        putimage_alpha(200, 50, &po);
                                                        printStory(40, 270, 170, "大家的眼睛紧紧地盯着你，很明显，你为他们出了气！。CY被怼无言，去了隔壁办公室嚎啕大哭！只见她狂扇自己大嘴巴子“wrtm,wrtm……！”大家一个接一个地去找老师道歉——即使他们不知道为什么！", 900, 1);
                                                        printStory(35, 200, 600, "你对洋盆说“猪子你看这个场面像不像丧葬会？”", 450, 1);
                                                        printStory(35, 750, 600, "你打算为人民服务", 450, 1);
                                                        FlushBatchDraw();
                                                        getmessage(&m, EM_KEY);
                                                        getmessage(&m, EM_KEY);
                                                        if (m.vkcode == 'M') {
                                                            cls();
                                                            backA();
                                                            putimage_alpha(200, 50, &po);
                                                            printStory(40, 270, 170, "洋盆差点没崩住，骂了你一句“你糖精吧！”。后来这件事便落幕了，但是你开始打心底认为CY真的不是人！好了这一节是体育课。你在和精子对打排球", 900, 1);
                                                            printStory(35, 200, 600, "你选择一记暴力抽射——大力丸", 450, 1);
                                                            printStory(35, 750, 600, "你对精子说……", 450, 1);
                                                            FlushBatchDraw();
                                                            getmessage(&m, EM_KEY);
                                                            getmessage(&m, EM_KEY);
                                                            if (m.vkcode == 'M') {
                                                                cls();
                                                                backA();
                                                                putimage_alpha(200, 50, &po);
                                                                printStory(40, 270, 170, "只见排球飞到了天边。不巧的是，它落到了寝室窗边的防护网上", 900, 1);
                                                                printStory(35, 200, 600, "你用竹竿试图把它搞下来", 450, 1);
                                                                printStory(35, 750, 600, "你一把抢过旁边卤蛋的球，试图用他的球换取你的球的解脱", 450, 1);
                                                                FlushBatchDraw();
                                                                getmessage(&m, EM_KEY);
                                                                getmessage(&m, EM_KEY);
                                                                if (m.vkcode == 'M') {
                                                                    int b25 = rand() % 2;
                                                                    if (b25 == 0) {
                                                                        cls();
                                                                        backA();
                                                                        putimage_alpha(200, 50, &pos);
                                                                        settextcolor(RED);
                                                                        printStory(40, 270, 170, "你失败了，更糟糕的是，竹竿也被卡住了，你沦为了全班甚至全校的笑话", 900, 2);
                                                                        printStory(40, 200, 600, "等级：C+", 450, 1);
                                                                        printStory(40, 750, 600, "任意键退出", 450, 1);
                                                                        FlushBatchDraw();
                                                                        settextcolor(BLACK);
                                                                        arrC[30] = 1;
                                                                        getmessage(&m, EM_KEY);
                                                                        getmessage(&m, EM_KEY);
                                                                    }
                                                                    else {
                                                                        cls();
                                                                        backA();
                                                                        putimage_alpha(200, 50, &pos);
                                                                        settextcolor(RED);
                                                                        printStory(40, 270, 170, "你成功了。啊，青春真的美好啊。你看着眼前的云，树，操场上的小草……心中感慨万千！", 900, 2);
                                                                        printStory(40, 200, 600, "等级：A+", 450, 1);
                                                                        printStory(40, 750, 600, "任意键退出", 450, 1);
                                                                        FlushBatchDraw();
                                                                        settextcolor(BLACK);
                                                                        arrA[32] = 1;
                                                                        getmessage(&m, EM_KEY);
                                                                        getmessage(&m, EM_KEY);
                                                                    }
                                                                }
                                                                else {
                                                                    cls();
                                                                    backA();
                                                                    putimage_alpha(200, 50, &po);
                                                                    printStory(40, 270, 170, "你一把抢过旁边卤蛋的球，试图用他的球换取你的球的解脱", 900, 1);
                                                                    printStory(35, 200, 600, "你选择80%力度", 450, 1);
                                                                    printStory(35, 750, 600, "你选择20%的力度", 450, 1);
                                                                    FlushBatchDraw();
                                                                    getmessage(&m, EM_KEY);
                                                                    getmessage(&m, EM_KEY);
                                                                    if (m.vkcode == 'M') {
                                                                        cls();
                                                                        backA();
                                                                        putimage_alpha(200, 50, &po);
                                                                        printStory(40, 270, 170, "排球在天上实现了一记完美的托马斯大回旋，正巧砸到了你的球正上方，双双陨落", 900, 1);
                                                                        printStory(35, 200, 600, "你笑了笑", 450, 1);
                                                                        printStory(35, 750, 600, "你忽然一脸严肃地看着卤蛋", 450, 1);
                                                                        FlushBatchDraw();
                                                                        getmessage(&m, EM_KEY);
                                                                        getmessage(&m, EM_KEY);
                                                                        if (m.vkcode == 'M') {
                                                                            cls();
                                                                            backA();
                                                                            putimage_alpha(200, 50, &po);
                                                                            settextcolor(RED);
                                                                            printStory(40, 270, 170, "你对卤蛋说：”康爹实力！！！“", 900, 2);
                                                                            printStory(40, 200, 600, "等级：A-", 450, 1);
                                                                            printStory(40, 750, 600, "任意键退出", 450, 1);
                                                                            FlushBatchDraw();
                                                                            settextcolor(BLACK);
                                                                            arrA[34] = 1;
                                                                            getmessage(&m, EM_KEY);
                                                                            getmessage(&m, EM_KEY);
                                                                        }
                                                                        else {
                                                                            cls();
                                                                            backA();
                                                                            putimage_alpha(200, 50, &po);
                                                                            printStory(40, 270, 170, "要不是你的球，我的排球也不会下不来的？卤蛋不语，只是唐笑着说：”请循其本！“", 900, 1);
                                                                            printStory(35, 200, 600, "你对其唐笑", 450, 1);
                                                                            printStory(35, 750, 600, "你对其说", 450, 1);
                                                                            FlushBatchDraw();
                                                                            getmessage(&m, EM_KEY);
                                                                            getmessage(&m, EM_KEY);
                                                                            if (m.vkcode == 'M') {
                                                                                cls();
                                                                                backA();
                                                                                putimage_alpha(200, 50, &po);
                                                                                settextcolor(RED);
                                                                                printStory(40, 270, 170, "我不管事", 900, 2);
                                                                                printStory(40, 200, 600, "等级：B+", 450, 1);
                                                                                printStory(40, 750, 600, "任意键退出", 450, 1);
                                                                                FlushBatchDraw();
                                                                                settextcolor(BLACK);
                                                                                arrB[28] = 1;
                                                                                getmessage(&m, EM_KEY);
                                                                                getmessage(&m, EM_KEY);
                                                                            }
                                                                            else {
                                                                                cls();
                                                                                backA();
                                                                                putimage_alpha(200, 50, &po);
                                                                                settextcolor(RED);
                                                                                printStory(40, 270, 170, "你当你是庄子？【化用CY著名句式】", 900, 2);
                                                                                printStory(40, 200, 600, "等级：B+", 450, 1);
                                                                                printStory(40, 750, 600, "任意键退出", 450, 1);
                                                                                FlushBatchDraw();
                                                                                settextcolor(BLACK);
                                                                                arrB[29] = 1;
                                                                                getmessage(&m, EM_KEY);
                                                                                getmessage(&m, EM_KEY);
                                                                            }
                                                                        }
                                                                    }
                                                                    else {
                                                                        cls();
                                                                        backA();
                                                                        putimage_alpha(200, 50, &po);
                                                                        settextcolor(RED);
                                                                        printStory(40, 270, 170, "旁敲侧击下，球终于落地了。你被同学们尊称为”挖机的运气“【化用经典典故”万物皆可以成为挖机“】", 900, 2);
                                                                        printStory(40, 200, 600, "等级：A+", 450, 1);
                                                                        printStory(40, 750, 600, "任意键退出", 450, 1);
                                                                        FlushBatchDraw();
                                                                        settextcolor(BLACK);
                                                                        arrA[33] = 1;
                                                                        getmessage(&m, EM_KEY);
                                                                        getmessage(&m, EM_KEY);
                                                                    }
                                                                }
                                                            }
                                                            else {
                                                                cls();
                                                                backA();
                                                                putimage_alpha(200, 50, &po);
                                                                printStory(40, 270, 170, "你们加入了以马哥为主的对打中，正好决胜局了", 900, 1);
                                                                printStory(35, 200, 600, "你把球传给了对面的action", 450, 1);
                                                                printStory(35, 750, 600, "你把球传给对面的马哥", 450, 1);
                                                                FlushBatchDraw();
                                                                getmessage(&m, EM_KEY);
                                                                getmessage(&m, EM_KEY);
                                                                if (m.vkcode == 'M') {
                                                                    cls();
                                                                    backA();
                                                                    putimage_alpha(200, 50, &po);
                                                                    settextcolor(RED);
                                                                    printStory(40, 270, 170, "果不其然action没有让我们失望，完美的出界球，how much money?但是，或许你们在一起畅快淋漓打球的时光也不多了吧。空悲切！", 900, 2);
                                                                    printStory(40, 200, 600, "等级：A", 450, 1);
                                                                    printStory(40, 750, 600, "《弘毅班》1/3解锁。任意键退出", 450, 1);
                                                                    hy1 = 1;
                                                                    FlushBatchDraw();
                                                                    settextcolor(BLACK);
                                                                    arrA[35] = 1;
                                                                    getmessage(&m, EM_KEY);
                                                                    getmessage(&m, EM_KEY);
                                                                }
                                                                else {
                                                                    cls();
                                                                    backA();
                                                                    putimage_alpha(200, 50, &po);
                                                                    printStory(40, 270, 170, "他没有接到，但是他却说：“再加1个球定胜负”", 900, 1);
                                                                    printStory(35, 200, 600, "你找裁判粉哥争论", 450, 1);
                                                                    printStory(35, 750, 600, "你继续打", 450, 1);
                                                                    FlushBatchDraw();
                                                                    getmessage(&m, EM_KEY);
                                                                    getmessage(&m, EM_KEY);
                                                                    if (m.vkcode == 'M') {
                                                                        cls();
                                                                        backA();
                                                                        putimage_alpha(200, 50, &po);
                                                                        settextcolor(RED);
                                                                        printStory(40, 270, 170, "他言：“我不管事”。最后，你还是输了，因为你没有把球传给action", 900, 2);
                                                                        printStory(40, 200, 600, "等级：B", 450, 1);
                                                                        printStory(40, 750, 600, "任意键退出", 450, 1);
                                                                        FlushBatchDraw();
                                                                        settextcolor(BLACK);
                                                                        arrB[30] = 1;
                                                                        getmessage(&m, EM_KEY);
                                                                        getmessage(&m, EM_KEY);
                                                                    }
                                                                    else {
                                                                        cls();
                                                                        backA();
                                                                        putimage_alpha(200, 50, &po);
                                                                        settextcolor(RED);
                                                                        printStory(40, 270, 170, "不知不觉中，本来十个球，现在被马哥加到了20个球。【话说粉哥也不说什么，黑哨中的黑哨啊】，最后，你只能认输了，马哥的计划“得逞”了", 900, 2);
                                                                        printStory(40, 200, 600, "等级：A", 450, 1);
                                                                        printStory(40, 750, 600, "任意键退出", 450, 1);
                                                                        FlushBatchDraw();
                                                                        settextcolor(BLACK);
                                                                        arrA[36] = 1;
                                                                        getmessage(&m, EM_KEY);
                                                                        getmessage(&m, EM_KEY);
                                                                    }
                                                                }
                                                            }
                                                        }
                                                        else {
                                                            cls();
                                                            backA();
                                                            putimage_alpha(200, 50, &po);
                                                            printStory(40, 270, 170, "你想到下一节是地理【敏敏】的课，于是你直接去叫敏敏来上课。成功的化解了CY布置的危机（吧）。现在是地理课，但是你找不到你的导学案了", 900, 1);
                                                            printStory(35, 200, 600, "你选择装糖阴敏敏一手", 450, 1);
                                                            printStory(35, 750, 600, "你临危不惧，把旁边麦子的导学案抢来", 450, 1);
                                                            FlushBatchDraw();
                                                            getmessage(&m, EM_KEY);
                                                            getmessage(&m, EM_KEY);
                                                            if (m.vkcode == 'M') {
                                                                cls();
                                                                backA();
                                                                putimage_alpha(200, 50, &po);
                                                                printStory(40, 270, 170, "你失败了，你没有导学案的面目被拆穿了，敏敏说“虾片你看别人都笑话你，你不配当地理课代表”。之后，敏敏继续正常讲解台湾。突然，她讲到“从台湾进口的水果”，把你吓得汗毛耸立！", 900, 1);
                                                                printStory(35, 200, 600, "你没有拆穿她", 450, 1);
                                                                printStory(35, 750, 600, "你拆穿她", 450, 1);
                                                                FlushBatchDraw();
                                                                getmessage(&m, EM_KEY);
                                                                getmessage(&m, EM_KEY);
                                                                if (m.vkcode == 'M') {
                                                                    cls();
                                                                    backA();
                                                                    putimage_alpha(200, 50, &po);
                                                                    printStory(40, 270, 170, "无事发生。忽然你看见敏敏导学案上中国领土没有藏南地区【事实上，8年级第一张导学案真的没有】你要怎么做？", 900, 1);
                                                                    printStory(35, 200, 600, "你依旧默不作声", 450, 1);
                                                                    printStory(35, 750, 600, "你选择了勇敢的发声", 450, 1);
                                                                    FlushBatchDraw();
                                                                    getmessage(&m, EM_KEY);
                                                                    getmessage(&m, EM_KEY);
                                                                    if (m.vkcode == 'M') {
                                                                        cls();
                                                                        backA();
                                                                        putimage_alpha(200, 50, &po);
                                                                        settextcolor(RED);
                                                                        printStory(40, 270, 170, "你作为敏敏的课代表都不敢指正敏敏的错误，果然当时敏敏的话是正确的", 900, 2);
                                                                        printStory(40, 200, 600, "等级：C", 450, 1);
                                                                        printStory(40, 750, 600, "任意键退出", 450, 1);
                                                                        FlushBatchDraw();
                                                                        settextcolor(BLACK);
                                                                        arrC[31] = 1;
                                                                        getmessage(&m, EM_KEY);
                                                                        getmessage(&m, EM_KEY);
                                                                    }
                                                                    else {
                                                                        cls();
                                                                        backA();
                                                                        putimage_alpha(200, 50, &po);
                                                                        settextcolor(RED);
                                                                        printStory(40, 270, 170, "敏敏非常感动，她认为你指正了她的教学失误，你被评为了优秀少年", 900, 2);
                                                                        printStory(40, 200, 600, "等级：S", 450, 1);
                                                                        printStory(40, 750, 600, "任意键退出", 450, 1);
                                                                        FlushBatchDraw();
                                                                        settextcolor(BLACK);
                                                                        arrS[26] = 1;
                                                                        getmessage(&m, EM_KEY);
                                                                        getmessage(&m, EM_KEY);
                                                                    }
                                                                }
                                                                else {
                                                                    cls();
                                                                    backA();
                                                                    putimage_alpha(200, 50, &po);
                                                                    settextcolor(RED);
                                                                    printStory(40, 270, 170, "老师，你的意思是， 你是td分子，你和***是出于统一战线的吗？你能给敏敏带这么多帽子，你也不是什么好东西", 900, 2);
                                                                    printStory(40, 200, 600, "等级：B+", 450, 1);
                                                                    printStory(40, 750, 600, "任意键退出", 450, 1);
                                                                    FlushBatchDraw();
                                                                    settextcolor(BLACK);
                                                                    arrB[31] = 1;
                                                                    getmessage(&m, EM_KEY);
                                                                    getmessage(&m, EM_KEY);
                                                                }
                                                            }
                                                            else {
                                                                cls();
                                                                backA();
                                                                putimage_alpha(200, 50, &po);
                                                                settextcolor(RED);
                                                                printStory(40, 270, 170, "但是敏敏还是发现了，然后麦子直接出卖【麦芒】了。但是出乎意料的是，敏敏只是唐笑了几下。你的记忆便定格在这里，在每一个唐笑的瞬间", 900, 2);
                                                                printStory(40, 200, 600, "等级：S-", 450, 1);
                                                                printStory(40, 750, 600, "任意键退出", 450, 1);
                                                                FlushBatchDraw();
                                                                settextcolor(BLACK);
                                                                arrS[25] = 1;
                                                                getmessage(&m, EM_KEY);
                                                                getmessage(&m, EM_KEY);
                                                            }
                                                        }
                                                    }
                                                    else {
                                                        int b23 = rand() % 10;
                                                        if (b23 < 7) {
                                                            cls();
                                                            backA();
                                                            putimage_alpha(200, 50, &pos);
                                                            settextcolor(RED);
                                                            printStory(40, 270, 170, "你凭借三寸不烂之舌赢得了与他的争斗", 900, 2);
                                                            printStory(40, 200, 600, "等级：A", 450, 1);
                                                            printStory(40, 750, 600, "任意键退出", 450, 1);
                                                            FlushBatchDraw();
                                                            settextcolor(BLACK);
                                                            arrA[31] = 1;
                                                            getmessage(&m, EM_KEY);
                                                            getmessage(&m, EM_KEY);
                                                        }
                                                        else {
                                                            cls();
                                                            backA();
                                                            putimage_alpha(200, 50, &pos);
                                                            settextcolor(RED);
                                                            printStory(40, 270, 170, "你真的以为她会认错吗 ？ 你输了！", 900, 2);
                                                            printStory(40, 200, 600, "等级：B", 450, 1);
                                                            printStory(40, 750, 600, "任意键退出", 450, 1);
                                                            FlushBatchDraw();
                                                            settextcolor(BLACK);
                                                            arrB[27] = 1;
                                                            getmessage(&m, EM_KEY);
                                                            getmessage(&m, EM_KEY);
                                                        }
                                                    }
                                                }
                                                else {
                                                    cls();
                                                    backA();
                                                    putimage_alpha(200, 50, &po);
                                                    printStory(40, 270, 170, "CY说：“你难道不是在说自己吗？先学会说话，再学会做人！”", 900, 1);
                                                    printStory(35, 200, 600, "你认瘪", 450, 1);
                                                    printStory(35, 750, 600, "你选择继续争", 450, 1);
                                                    FlushBatchDraw();
                                                    getmessage(&m, EM_KEY);
                                                    getmessage(&m, EM_KEY);
                                                    if (m.vkcode == 'M') {
                                                        cls();
                                                        backA();
                                                        putimage_alpha(200, 50, &po);
                                                        settextcolor(RED);
                                                        printStory(40, 270, 170, "认瘪太没有意思了！老子不编了", 900, 2);
                                                        printStory(40, 200, 600, "等级：B-", 450, 1);
                                                        printStory(40, 750, 600, "任意键退出", 450, 1);
                                                        FlushBatchDraw();
                                                        settextcolor(BLACK);
                                                        arrB[32] = 1;
                                                        getmessage(&m, EM_KEY);
                                                        getmessage(&m, EM_KEY);
                                                    }
                                                    else {
                                                        cls();
                                                        backA();
                                                        putimage_alpha(200, 50, &po);
                                                        printStory(40, 270, 170, "谁曾想，此时挖机到场，二者开始对你混合攻击！", 900, 1);
                                                        printStory(35, 200, 600, "你直接跑出学校", 450, 1);
                                                        printStory(35, 750, 600, "你的意思是？……", 450, 1);
                                                        FlushBatchDraw();
                                                        getmessage(&m, EM_KEY);
                                                        getmessage(&m, EM_KEY);
                                                        if (m.vkcode == 'M') {
                                                            cls();
                                                            backA();
                                                            putimage_alpha(200, 50, &po);
                                                            settextcolor(RED);
                                                            printStory(40, 270, 170, "兄弟这不是一个好的选择啊！", 900, 2);
                                                            printStory(40, 200, 600, "等级：C", 450, 1);
                                                            printStory(40, 750, 600, "任意键退出", 450, 1);
                                                            FlushBatchDraw();
                                                            settextcolor(BLACK);
                                                            arrC[32] = 1;
                                                            getmessage(&m, EM_KEY);
                                                            getmessage(&m, EM_KEY);
                                                        }
                                                        else {
                                                            cls();
                                                            backA();
                                                            putimage_alpha(200, 50, &po);
                                                            settextcolor(RED);
                                                            printStory(40, 270, 170, "你直接把他们告上法庭。法庭上，CY和挖机向你忏悔。你与挖机和解，因为挖机NB【作者是数学课代表】。至于CY那厮呢？只有天知道……", 900, 2);
                                                            printStory(40, 200, 600, "等级：A", 450, 1);
                                                            printStory(40, 750, 600, "任意键退出", 450, 1);
                                                            FlushBatchDraw();
                                                            settextcolor(BLACK);
                                                            arrA[37] = 1;
                                                            getmessage(&m, EM_KEY);
                                                            getmessage(&m, EM_KEY);
                                                        }
                                                    }
                                                }
                                            }
                                            else {
                                                cls();
                                                backA();
                                                putimage_alpha(200, 50, &po);
                                                printStory(40, 270, 170, "666CY听不懂，还以为你在用文言文忏悔。便放过你一马。这件事便告一段落了。经过了一天了劳累，你回到了寝室【213室】现在已经熄灯，你在床上躺着", 900, 1);
                                                printStory(35, 200, 600, "你选择在睡觉之前看一下门洞", 450, 1);
                                                printStory(35, 750, 600, "你在寝室里吃板面", 450, 1);
                                                FlushBatchDraw();
                                                getmessage(&m, EM_KEY);
                                                getmessage(&m, EM_KEY);
                                                if (m.vkcode == 'M') {
                                                    cls();
                                                    backA();
                                                    putimage_alpha(200, 50, &po);
                                                    settextcolor(RED);
                                                    printStory(40, 270, 170, "你与老猫撞了一个照面，老猫说你“挑衅人家”这样寝室违纪的这辈子已经没了", 900, 2);
                                                    printStory(40, 200, 600, "等级：D+", 450, 1);
                                                    printStory(40, 750, 600, "任意键退出", 450, 1);
                                                    FlushBatchDraw();
                                                    settextcolor(BLACK);
                                                    arrD[21] = 1;
                                                    getmessage(&m, EM_KEY);
                                                    getmessage(&m, EM_KEY);
                                                }
                                                else {
                                                    int b26 = rand() % 2;
                                                    if (b26 == 0) {
                                                        cls();
                                                        backA();
                                                        putimage_alpha(200, 50, &pos);
                                                        settextcolor(RED);
                                                        printStory(40, 270, 170, "你被老猫发现了，而且第二天还被通报了，在CY质问你为什么吃泡面的情况下，你说出了令弘毅班人民此生难忘的句子“我吃的是板面，不是泡面”", 900, 2);
                                                        printStory(40, 200, 600, "等级：A", 450, 1);
                                                        printStory(40, 750, 600, "任意键退出", 450, 1);
                                                        FlushBatchDraw();
                                                        settextcolor(BLACK);
                                                        arrA[38] = 1;
                                                        getmessage(&m, EM_KEY);
                                                        getmessage(&m, EM_KEY);
                                                    }
                                                    else {
                                                        cls();
                                                        backA();
                                                        putimage_alpha(200, 50, &po);
                                                        settextcolor(RED);
                                                        printStory(30, 270, 120, "与此同时，灯塔带来两瓶酒，你，灯塔，鲶鱼三人对酒当歌。你们从秦始皇谈到蒋介石；从CY的历程聊到校长的无能。从就业环境聊到未来趋势【卤蛋此时重复了一声“区”】。说到这里，你想起了你的表爷， 他生前留下十个预言，9个已经实现：2000年我国会进入一个新的世纪；2001年，一定是这个新世纪的第二年；2008年，一定会有一件事情发生；2009,2010,2011也是如此；2012年，二月一定会有29天；2013年的二月一定不会有30天；与此同时，2014年领导人一定不是毛泽东那一届的；我的后代，如果是孙子一定是男的，如果是孙女一定是女的；2019年一定会是新中国成立70周年……", 900, 2);
                                                        printStory(40, 200, 600, "你选择继续谈你爷的10个预言", 450, 1);
                                                        printStory(40, 750, 600, "你选择了闭嘴，你意识到你爷真tm是你爸爸的父亲啊", 450, 1);
                                                        FlushBatchDraw();
                                                        getmessage(&m, EM_KEY);
                                                        getmessage(&m, EM_KEY);
                                                        if (m.vkcode == 'M') {
                                                            cls();
                                                            backA();
                                                            putimage_alpha(200, 50, &po);
                                                            printStory(40, 270, 170, "突然，老猫闯入：“你的演讲结束了？”。你现在非常慌，请选择明天给CY交差的理由", 900, 1);
                                                            printStory(35, 200, 600, "晚上写作业被发现通报", 450, 1);
                                                            printStory(35, 750, 600, "上厕所被通报", 450, 1);
                                                            FlushBatchDraw();
                                                            getmessage(&m, EM_KEY);
                                                            getmessage(&m, EM_KEY);
                                                            if (m.vkcode == 'M') {
                                                                cls();
                                                                backA();
                                                                putimage_alpha(200, 50, &po);
                                                                settextcolor(RED);
                                                                printStory(40, 270, 170, "寝室违纪，学习不背锅，回家反省！", 900, 2);
                                                                printStory(40, 200, 600, "等级：D+", 450, 1);
                                                                printStory(40, 750, 600, "任意键退出", 450, 1);
                                                                FlushBatchDraw();
                                                                settextcolor(BLACK);
                                                                arrD[22] = 1;
                                                                getmessage(&m, EM_KEY);
                                                                getmessage(&m, EM_KEY);
                                                            }
                                                            else {
                                                                cls();
                                                                backA();
                                                                putimage_alpha(200, 50, &po);
                                                                settextcolor(RED);
                                                                printStory(40, 270, 170, "好好好，扒十层皮！写检查，计入档案，回家反省！", 900, 2);
                                                                printStory(40, 200, 600, "等级：D", 450, 1);
                                                                printStory(40, 750, 600, "任意键退出", 450, 1);
                                                                FlushBatchDraw();
                                                                settextcolor(BLACK);
                                                                arrD[23] = 1;
                                                                getmessage(&m, EM_KEY);
                                                                getmessage(&m, EM_KEY);
                                                            }
                                                        }
                                                        else {
                                                            cls();
                                                            backA();
                                                            putimage_alpha(200, 50, &po);
                                                            printStory(40, 270, 170, "你巧妙地躲过老猫的查寝，但是喝了这么多的酒，你的神情变得极度亢奋，到了凌晨2点你仍然睡不着", 900, 1);
                                                            printStory(35, 200, 600, "你选择去洗头", 450, 1);
                                                            printStory(35, 750, 600, "你选择硬熬", 450, 1);
                                                            FlushBatchDraw();
                                                            getmessage(&m, EM_KEY);
                                                            getmessage(&m, EM_KEY);
                                                            if (m.vkcode == 'M') {
                                                                cls();
                                                                backA();
                                                                putimage_alpha(200, 50, &po);
                                                                settextcolor(RED);
                                                                printStory(40, 270, 170, "流水声传遍这个宿舍，你猜猜“蛆”这个外号是怎么来的呢", 900, 2);
                                                                printStory(40, 200, 600, "等级：D", 450, 1);
                                                                printStory(40, 750, 600, "任意键退出", 450, 1);
                                                                FlushBatchDraw();
                                                                settextcolor(BLACK);
                                                                arrD[24] = 1;
                                                                getmessage(&m, EM_KEY);
                                                                getmessage(&m, EM_KEY);
                                                            }
                                                            else {
                                                                cls();
                                                                backA();
                                                                putimage_alpha(200, 50, &po);
                                                                settextcolor(RED);
                                                                printStory(40, 270, 170, "你直到3点才睡，但是5点你又要起床，你成功地因病请假，回家打超自然了", 900, 2);
                                                                printStory(40, 200, 600, "等级：A+", 450, 1);
                                                                printStory(40, 750, 600, "任意键退出", 450, 1);
                                                                FlushBatchDraw();
                                                                settextcolor(BLACK);
                                                                arrA[39] = 1;
                                                                getmessage(&m, EM_KEY);
                                                                getmessage(&m, EM_KEY);
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                            else {
                                cls();
                                backA();
                                putimage_alpha(200, 50, &po);
                                printStory(40, 270, 170, "你大喊：”为什么？？为什么？？？？我不是皇帝吗？？？我不是……吗？？？……“不等你说完，CY就打断你：虾片同学，要学学，不学滚出去（与此同时，卤蛋小声地重复了一声”去“，还有灯塔。他俩对你嬉笑）", 900, 1);
                                printStory(35, 200, 600, "你滚了", 450, 1);
                                printStory(35, 750, 600, "你与CY解释", 450, 1);
                                FlushBatchDraw();
                                getmessage(&m, EM_KEY);
                                getmessage(&m, EM_KEY);
                                if (m.vkcode == 'M' && m.message == WM_KEYDOWN) {
                                    cls();
                                    backA();
                                    putimage_alpha(200, 50, &po);
                                    settextcolor(RED);
                                    printStory(40, 270, 170, "你听见CY在批判你，但是这已经是最小的问题了！你不知道怎么办……最后，CY认为你患有精神性疾病，把你送去了焦村半坡", 900, 2);
                                    printStory(40, 200, 600, "等级：C-", 450, 1);
                                    printStory(40, 750, 600, "任意键退出", 450, 1);
                                    FlushBatchDraw();
                                    settextcolor(BLACK);
                                    arrC[20] = 1;
                                    getmessage(&m, EM_KEY);
                                    getmessage(&m, EM_KEY);
                                }
                                else {
                                    cls();
                                    backA();
                                    putimage_alpha(200, 50, &po);
                                    settextcolor(RED);
                                    printStory(40, 270, 170, "你可曾听闻：“我不管事！”", 900, 2);
                                    printStory(40, 200, 600, "等级：C", 450, 1);
                                    printStory(40, 750, 600, "任意键退出", 450, 1);
                                    FlushBatchDraw();
                                    settextcolor(BLACK);
                                    arrC[21] = 1;
                                    getmessage(&m, EM_KEY);
                                    getmessage(&m, EM_KEY);
                                }
                            }
                        }
                    }
                    //the end of the story
                    cls();
                    backA();
                    putimage_alpha(50, 80, &end);
                    ending = time(0);
                    gtime = (ending - start) * 2;
                    settextstyle(40, 0, "微软雅黑");
                    settextcolor(BLACK);
                    int arrSsum = 0;
                    for (int iS = 0; iS < numS; iS++) {
                        arrSsum += arrS[iS];
                    }
                    int arrAsum = 0;
                    for (int iA = 0; iA < numA; iA++) {
                        arrAsum += arrA[iA];
                    }
                    int arrBsum = 0;
                    for (int iB = 0; iB < numB; iB++) {
                        arrBsum += arrB[iB];
                    }
                    int arrCsum = 0;
                    for (int iC = 0; iC < numC; iC++) {
                        arrCsum += arrC[iC];
                    }
                    int arrDsum = 0;
                    for (int iD = 0; iD < numD; iD++) {
                        arrDsum += arrD[iD];
                    }
                    int arrRsum = 0;
                    for (int iR = 0; iR < numR; iR++) {
                        arrRsum += arrR[iR];
                    }
                re:
                    settextstyle(40, 0, "微软雅黑");
                    settextcolor(BLACK);
                    int endsum = arrSsum + arrAsum + arrBsum + arrCsum + arrDsum + arrRsum;
                    char scoreStr[20];
                    sprintf_s(scoreStr, "用时： %d", gtime);
                    outtextxy(150, 170, scoreStr);
                    char scoreStr1[20];
                    sprintf_s(scoreStr1, "获得S等级数量： %d", arrSsum);
                    outtextxy(150, 215, scoreStr1);
                    char scoreStr2[20];
                    sprintf_s(scoreStr2, "获得A等级数量： %d", arrAsum);
                    outtextxy(150, 260, scoreStr2);
                    char scoreStr3[20];
                    sprintf_s(scoreStr3, "获得B等级数量： %d", arrBsum);
                    outtextxy(150, 305, scoreStr3);
                    char scoreStr4[20];
                    sprintf_s(scoreStr4, "获得C等级数量： %d", arrCsum);
                    outtextxy(150, 350, scoreStr4);
                    char scoreStr5[20];
                    sprintf_s(scoreStr5, "获得D等级数量： %d", arrDsum);
                    outtextxy(150, 395, scoreStr5);
                    char scoreStr6[20];
                    sprintf_s(scoreStr6, "获得R等级数量： %d", arrRsum);
                    outtextxy(150, 440, scoreStr6);
                    settextstyle(60, 0, "微软雅黑");
                    char scoreStr7[20];
                    sprintf_s(scoreStr7, "解锁结局数量： %d", endsum);
                    outtextxy(550, 200, scoreStr7);
                    settextstyle(30, 0, "微软雅黑");
                    if (endsum == total) {
                        settextcolor(RED);
                        outtextxy(150, 485, "您已通关游戏！");
                        FlushBatchDraw();
                        getmessage(&m, EM_KEY);
                        getmessage(&m, EM_KEY);
                        if (m.ch == 13) {
                            say();
                            closegraph();
                            _getch();
                            return 0;
                        }
                    }
                    if (endsum == 100) {
                        if (v12 == 0) {
                            settextcolor(RED);
                            outtextxy(150, 485, "恭喜获得成就“小满贯”！");
                            v12++;
                        }
                    }

                    if (endsum > 19 && (arrSsum + arrAsum) > endsum * 0.8) {
                        if (v1 == 0) {
                            settextcolor(RED);
                            outtextxy(150, 520, "恭喜获得成就“贤才”1！");
                            v1++;
                        }
                    }
                    if (endsum > 49 && (arrSsum + arrAsum) > endsum * 0.8) {
                        if (v7 == 0) {
                            settextcolor(RED);
                            outtextxy(150, 520, "恭喜获得成就“贤才”2！");
                            v7++;
                        }
                    }
                    if (endsum > 69 && (arrSsum + arrAsum) > endsum * 0.8) {
                        if (v8 == 0) {
                            settextcolor(RED);
                            outtextxy(150, 520, "恭喜获得成就“贤才”3！");
                            v8++;
                        }
                    }

                    if (arrDsum == 10) {
                        if (v2 == 0) {
                            settextcolor(RED);
                            outtextxy(150, 555, "恭喜获得成就“赤石先锋”1！");
                            v2++;
                        }
                    }
                    if (arrDsum == 20) {
                        if (v9 == 0) {
                            settextcolor(RED);
                            outtextxy(150, 555, "恭喜获得成就“赤石先锋”2！");
                            v9++;
                        }
                    }
                    if (arrDsum == numD) {
                        if (v10 == 0) {
                            settextcolor(RED);
                            outtextxy(150, 555, "恭喜获得成就“赤石先锋”3！");
                            v10++;
                        }
                    }

                    if (endsum == 10 && ((arrBsum + arrCsum + arrAsum + arrRsum) == 0)) {
                        if (v3 == 0) {
                            settextcolor(RED);
                            outtextxy(150, 590, "恭喜获得成就“极限玩家”1！");
                            v3++;
                        }
                    }
                    if (endsum == 20 && ((arrBsum + arrCsum + arrAsum + arrRsum) == 0)) {
                        if (v11 == 0) {
                            settextcolor(RED);
                            outtextxy(150, 590, "恭喜获得成就“极限玩家”2！");
                            v11++;
                        }
                    }

                    if (gtime < 16) {
                        if (v4 == 0) {
                            settextcolor(RED);
                            outtextxy(150, 625, "恭喜获得成就“效率”！");
                            v4++;
                        }
                    }
                    if (gtime > 240) {
                        if (v5 == 0) {
                            settextcolor(RED);
                            outtextxy(150, 625, "恭喜获得成就“有没有效率”！");
                            v5++;
                        }
                    }
                    if (arrRsum == numR) {
                        if (v6 == 0) {
                            settextcolor(RED);
                            outtextxy(150, 600, "恭喜获得成就“彩蛋收集者”！");
                            v6++;
                        }
                    }

                    FlushBatchDraw();
                    getmessage(&m, EM_KEY);
                    getmessage(&m, EM_KEY);
                    settextcolor(BLACK);
                    if (m.vkcode == '0') {
                        closegraph();
                        _getch();
                        return 0;
                    }
                    else if (m.ch == 13) {
                        isIntoMenu = false;
                        isRealGame = false;
                    }
                    else {
                        goto re;
                    }
                }
                Sleep(10);
                FlushBatchDraw();
            } while (come != 0);
        }
        FlushBatchDraw();
    }
    closegraph();
    return 0;
}