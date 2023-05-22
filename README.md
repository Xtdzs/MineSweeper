# README

## 1 项目描述

这个项目实现了一个简单的扫雷游戏。游戏棋盘由 *ROWS* x *COLS* 个单元格组成,布置有 *numMines* 个地雷。玩家需要通过点击单元格来翻开方块，并标记可能的地雷位置。如果点击到地雷，游戏结束；如果翻开所有非地雷单元格,玩家胜利。游戏有三个难度级别：简单(9 x 9, 10颗雷)、中等(16 x 16, 40颗雷)和困难(16 x 30, 99颗雷)。玩家还可以保存和读取游戏进度。

同时，该游戏实现了开始新游戏、翻开单元格、结束游戏、显示计时等基本功能。用户界面采用*Qt*设计，清晰简洁。

游戏的主要特点是：

1)三个难度级别，适合不同的玩家。

2)可以记录和显示每种难度的最快完成时间，提高游戏的可玩性。

3)采用面向对象的设计，系统划分清晰，便于后续扩展。

4)用户界面简洁直观，采用*Qt*开发，美观易用。

## 2 设计思路

### 2.1 核心思路

考虑一个扫雷游戏，你拿到它的时候，你面向的是一个**棋盘**，**棋盘**的状态影响着你游戏的胜负，揭开**棋盘**上的所有不含雷的**格子**，就能取得游戏胜利。而我们通过和**格子**进行交互，来影响着整个**棋盘**的状态；而**格子**给玩家的反馈，也依赖于**棋盘**的状态——如果我们把**棋盘**和**格子**视为**对象**，来实现他们的功能，我们就能非常自然且简洁地实现扫雷游戏。

### 2.2具体思路

该项目的具体设计思路是采用面向对象的方法，将整个游戏系统分解为多个相对独立的类，每个类都有清晰的功能和职责。

主要的类设计如下：

- __Cell__：表示游戏中的每个单元格，包含是否有雷、是否翻开、雷的数目等信息。该类负责存储单元格的状态和信息。
- __MainWindow__：主窗口类，包含菜单、工具栏、状态栏等界面元素。该类负责构建游戏的总体界面和布局。
- **MSs**：游戏主逻辑类,包含开始新游戏、翻开单元格、结束游戏、计时等主要游戏逻辑。该类是游戏的控制中心，负责实现游戏规则和交互。
- **MineSweeperGrid**：存储和更新游戏棋盘信息，包含*ROWS* x *COLS*个*Cell*。该类负责记录当前游戏棋盘的具体状态，为*MSs*类提供信息支持。

这样将整个游戏系统划分为相对独立但协作的几个类，每个类都有明确的功能职责。这种面向对象的设计思路可以很好地体现各部分的内聚性和扩展性，有利于后续功能的添加和优化。在实现过程中，采用Qt框架进行界面设计，使用面向对象的C++语言编程，并运用了一定的数据结构与算法，如使用 vector 存储游戏棋盘信息，使用广度优先搜索算法(BFS)翻开相连的空单元格等。

## 3 类和函数设计

###  3.1 基础设计(C++ version)

#### 3.1.1 Cell

格子是棋盘的基础，在游戏中，格子的状态包括：是否有雷、是否被揭开、是否被flag、周围有几个雷

于是我们可以根据格子应有的功能，来实现一个Cell类，声明我们需要的函数

```c++
class Cell{
private:
    bool mine,rev,flag;
    int adm;   
public:
    Cell();
        
    bool is_mine()const;
    void set_mine();

    bool is_rev()const;
    void set_rev();

    bool is_flag()const;
    void toggle_flag();

    int get_adm() const;
    void set_adm(int x);
 
};
```

然后我们再具体的实现我们所需要的函数

```c++
Cell::Cell():mine(0),rev(0),flag(0),adm(0){}

bool Cell::is_mine() const{return mine;}
void Cell::set_mine(){mine=1;}
bool Cell::is_rev() const{return rev;}
void Cell::set_rev(){rev=1;}
bool Cell::is_flag() const{return flag;}
void Cell::toggle_flag() {flag = !flag;}
int Cell::get_adm() const {return adm;}
```

我们就初步完成了Cell这一对象

#### 3.1.2 Board

棋盘比格子要复杂得多，它需要调用和管理众多的格子，并且实现格子的交互

但我们依旧可以轻松地抽象出棋盘的状态：棋盘的size、含有的mines、所有的cell、游戏的状态

然后我们就可以声明一个Board类

```c++
class Board
{
private:
    int width,height,mines;
    std::vector<Cell> cells;
    bool gameOver;
public:
    Board(int w,int h,int m);
    void generate();
    
    bool is_valid(int x,int y) const;
    Cell& getCell(int x, int y);
    const Cell& getCell(int x, int y) const;
    int getWidth() const;
    int getHeight() const;

    bool isGameOver() const;
    bool isGameWon() const;

    bool revealCell(int x, int y);
    void toggleFlag(int x, int y);
};
```



其中，有两个函数较为复杂，一个是generate()，它会根据我们的需求生成一个棋盘（同时生成我们需求的格子）

```c++
void Board::generate(){
    srand(time(0));
    std::vector< std::pair<int,int> > v;
    for(int i=0;i<height;i++)
        for(int j=0;j<width;j++)
            v.push_back(std::make_pair(i,j));
    random_shuffle(v.begin(),v.end());
    for(int i=0;i<mines;i++) 
        getCell(v[i].first,v[i].second).set_mine();
    
    for(int i=0;i<height;i++)
        for(int j=0;j<width;j++){
            if(getCell(i,j).is_mine()) continue;
            
            int num=0;
            for(int di=-1;di<=1;di++)
                for(int dj=-1;dj<=1;dj++){
                    if(di==0&&dj==0) continue;
                    int ni=i+di;
                    int nj=j+dj;
                    num+=is_valid(ni,nj)&&getCell(ni,nj).is_mine();
                }
            getCell(i,j).set_adm(num);
        }
```



以及玩家的核心操作，揭开格子，Board::revealCell(int x, int y)

```c++
bool Board::revealCell(int x, int y) {
    if (!is_valid(x, y) || getCell(x, y).is_rev() || getCell(x, y).is_flag()) {
        return false;
    }

    getCell(x, y).set_rev();

    if (getCell(x, y).is_mine()) {
        gameOver = true;
        return false;
    }

    if (getCell(x, y).get_adm() == 0) {
        std::queue<std::pair<int, int>> queue;
        queue.push({x, y});

        while (!queue.empty()) {
            int cur_x = queue.front().first;
            int cur_y = queue.front().second;
            queue.pop();

            for (int dy = -1; dy <= 1; ++dy) {
                for (int dx = -1; dx <= 1; ++dx) {
                    if (dx == 0 && dy == 0) {
                        continue;
                    }

                    int nx = cur_x + dx;
                    int ny = cur_y + dy;

                    if (is_valid(nx, ny) && !getCell(nx, ny).is_rev() && !getCell(nx, ny).is_flag()) {
                        getCell(nx, ny).set_rev();
                        if (getCell(nx, ny).get_adm() == 0) {
                            queue.push({nx, ny});
                        }
                    }
                }
            }
        }
    }
    return true;
}
```

#### 3.1.3 main

最后是控制台部分

我完成的控制台只是个简陋的雏形，通过调用Board和Cell类，在cmd上和玩家实现交互

```c++
void printBoard(Board& board) {
    int width = board.getWidth();
    int height = board.getHeight();

    std::cout << "  ";
    for (int x = 0; x < width; ++x) {
        std::cout << x << ' ';
    }
    std::cout << '\n';

    for (int y = 0; y < height; ++y) {
        std::cout << y << ' ';
        for (int x = 0; x < width; ++x) {
            const Cell& cell = board.getCell(x, y);
            if (cell.is_flag()) {
                std::cout << "F ";
            } else if (!cell.is_rev()) {
                std::cout << "# ";
            } else if (cell.is_mine()) {
                std::cout << "* ";
            } else {
                int adm = cell.get_adm();
                std::cout << (adm == 0 ? ' ' : static_cast<char>(adm + '0')) << ' ';
            }
        }
        std::cout << '\n';
    }
}

int main() {
    int width = 10;
    int height = 10;
    int mines = 6;

    Board board(width, height, mines);
    while (!board.isGameOver()) {
        printBoard(board);
        std::cout << "Enter action (R/F) followed by coordinates (x y): ";
        char action;
        int x, y;
        std::cin >> action >> y >> x;

        if (action == 'R' || action == 'r') {
            if (!board.revealCell(x, y)) {
                std::cout << "Invalid move, try again.\n";
            }
        } else if (action == 'F' || action == 'f') {
            board.toggleFlag(x, y);
        } else {
            std::cout << "Invalid action, try again.\n";
        }

        if (board.isGameWon()) {
            std::cout << "Congratulations! You won!\n";
            break;
        }
    }

    if (board.isGameOver()) {
        std::cout << "Game Over! You hit a mine!\n";
    }
    std::cin.get();
    std::cin.get();
    std::cin.get();
    std::cin.get();
    return 0;
}

```



### 3.2 开发设计(Qt version)

#### 3.2.1 Cell类：

**成员变量**：*mine*(是否有雷)、*marked*(是否翻开)和*num*(周围雷数)

**构造函数Cell(mine)**：初始化成员变量

**成员函数**：*getMine()*、*setMarked()*、*getNum()*，分别获取三个成员变量

#### 3.2.2 MainWindow类：

**菜单栏、工具栏和状态栏**

*QAction* **开始新游戏触发槽函数** *newGame()*

**构造函数** *MainWindow()* **初始化界面**

#### 3.2.3 MSs类：

**成员变量**：*difficulty*(难度)、*timer*(计时器)、*count*(时间计数)等

**构造函数**：*MSs(rows,cols,mines,difficulty)*：初始化游戏参数

**成员函数**:

*startTimer()/updateTimer()*：开始/更新计时器 

r*ead/record_highest_score_n()*：读取和记录最高分

*closeEvent()*：关闭事件,停止计时器

*countAdjacentMines()*：计算周围雷数

*saveGame()*：保存游戏

*revealBoard()*：显示所有雷信息 

*gameOver()*：游戏结束

*updateButtons()*：更新按钮显示

*flipCell()*：翻开单元格及相连空单元格 

*newGame()*：开始新游戏

*isGameWon()*：判断是否胜利

*onCellClicked()*：单元格被点击

*onCellRightClicked()*：单元格被右键点击

等等...

每个函数都有清晰的功能、输入和输出定义,通过*public*和*private*来控制访问限制。

## 4 实现过程

本项目使用C++语言和Qt框架进行开发，在Windows 11系统下使用Qt Creator 10.0.0编写代码并进行调试。

**主要的实现步骤**如下：

1)安装Qt Creator集成开发环境和Qt库，新建Qt Widgets Application项目。

2)设计游戏的整体框架及界面布局，使用Qt Designer构建 *MainWindow* 窗口，包含菜单、工具栏和状态栏。

3)实现 *Cell* 类，用于表示游戏中的每个单元格。

4)实现 *MineSweeperGrid* 类，用于存储和更新游戏棋盘信息，包含一个二维*vector*存储*Cell*。

5)实现 *MSs* 类，用于控制游戏进程，包括开始游戏、翻开单元格、游戏结束判断等主要逻辑。

6)实现 *highest_score_n* 类，用于显示每种难度的最高分，通过读取记录最高分的文本文件实现。

7)实现 *MainWindow* 中开始新游戏的槽函数，连接到 *MSs* 类的 *newGame()* 成员函数。

8)实现 *MSs* 类中单元格点击和右击的相关槽函数，进行单元格翻开、雷标记等操作。

9)进行测试和调试，解决出现的问题，不断优化代码，实现全部功能。

10)进行界面美化，选择扑克牌风格的图片资源。

11)编写 *README* 文档和报告文档。

在实现过程中，使用了C++的面向对象特性，采用Qt提供的布局管理、资源管理、信号-槽机制等功能，使用了 *STL* 中的 *vector* 等容器，使用了算法如广度优先搜索，等等。

## 5 效果展示

### 5.1 图标展示

![icon](.\Images\icon.png) 

### 5.2 开始界面

![start](.\Images\start.png) 

### 5.3 游戏界面(Easy为例)

![Easy](.\Images\Easy.png) 

### 5.4 分数记录

![score](.\Images\score.png) 

### 5.5 开发人员展示

![about](.\Images\about.png) 

## 6 较好的设计部分

本项目较好的设计部分是采用面向对象的方法对系统进行模块化设计，尤其是将游戏主逻辑和界面显示分离到MSs类和MineSweeperGrid类。

MSs类主要负责实现游戏的主要逻辑，如开始新游戏、翻开单元格、游戏结束判断和处理、计时等等。这个类并不直接操控界面，而是通过发射信号的方式通知界面更新显示。

MineSweeperGrid类用于存储游戏棋盘的状态，包含一个二维vector存储每个Cell。这个类记录了每个单元格的具体信息，为MSs类的游戏逻辑提供必要的数据支持。

这样的设计可以最大限度地提高各个模块的内聚性，MSs类专注于游戏逻辑而无需过多关注界面显示，MineSweeperGrid类专注于数据存储而无需关注游戏规则，两者通过信号-槽连接起来，很好地体现了职责分离的原则。

这种设计也很容易进行功能扩展，比如要添加新的难度级别，只需要在MSs类中添加相应变量和逻辑，并发射信号通知显示更新，而不需要修改MineSweeperGrid类。如果要支持更大或更复杂的游戏棋盘，只需要修改MineSweeperGrid类，MSs类的逻辑可以保持不变。

总之，面向对象方法和模块化设计是本项目的最大亮点，可以实现高内聚低耦合，有利于功能扩展和代码维护。通过信号-槽机制实现各模块间的松耦合连接，这是Qt框架的优势之一。

## 7 不足之处

虽然本项目实现了扫雷游戏的基本功能和较好的用户体验，但还存在一定的不足之处，主要有：

1)界面过于简单，没有采用更加美观的图片和样式，界面元素布局也比较基础。可以在未来版本中使用更加丰富的界面元素和布局进行美化。

2)只有三个固定的难度级别，无法自由设置游戏棋盘大小和雷数。可以在后续版本中增加自定义难度设置，让玩家自由选择游戏棋盘大小和地雷数目。

3)没有实现更丰富的游戏功能，如翻开提示、排列雷区提示、支持多人游戏等。后续版本可以添加这些功能，提高游戏的趣味性。

4)代码的可读性和扩展性还需要提高，如增加更详细的注释，改进一些变量和函数的命名，增加一定的异常处理机制等。这需要在开发的过程中不断优化和改善。

5)当前只支持保存和读取最后一次游戏的进度，无法存储多个游戏的数据。可以添加版本控制等机制，支持读取历史游戏数据。
