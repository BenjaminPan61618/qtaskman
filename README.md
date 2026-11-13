# qTaskMan

<p align="center">
  <img src="assets/qtaskman.svg" width="96" alt="qTaskMan logo">
</p>

<p align="center">基于 Qt 6 与 Kirigami 的 Linux 系统任务管理器</p>

qTaskMan 是一款使用 **C++20 + Qt 6 + QML/Kirigami** 编写的桌面系统任务管理器，提供进程与应用管理、硬件资源监控、性能浮窗等能力，界面支持明暗主题与中英双语。

## 功能特性

- **系统总览**：展示 CPU、内存、网络、磁盘、温度等关键指标与系统信息。
- **应用管理**：列出带窗口的图形应用，支持窗口探测与切换。
- **进程管理**
  - 列表 / 树形两种视图，列对齐显示。
  - 右键菜单：打开可执行文件位置、重新启动进程、终止进程等。
  - 新打开 / 挂起 / 终止 / 未响应的进程以浅色高亮标识。
- **硬件监控**
  - CPU、内存、网络、磁盘、温度实时曲线，带连贯动画。
  - 手动刷新、清空历史、导出报告。
- **性能浮窗**
  - 总在最前、无边框，可拖动、可缩放。
  - 单图表显示，可在 CPU / 内存 / 网络 / 磁盘 / 温度之间切换。
  - 整体透明度可在设置中调整（20%–100%，最低 20%，不会全透明）。
  - `Esc` 或右上角 × 关闭并返回主窗口。
- **系统托盘**：托盘图标 + 菜单，可设置关闭主窗口时驻留后台。
- **个性化**
  - 外观主题：跟随系统 / 浅色 / 深色（同步切换 Breeze / breeze-dark 图标）。
  - 界面语言：简体中文 / English，切换即时生效。
  - 温度单位：摄氏度 / 华氏度。
  - 数据采集间隔：0.5s / 1s / 2s / 5s。

## 运行环境

- **操作系统**：Linux（读取 `/proc`、`/sys`，依赖 DBus）。
- **构建依赖**
  - CMake ≥ 3.21
  - 支持 C++20 的编译器（GCC / Clang）
  - Qt ≥ 6.5，组件：`Core`、`Gui`、`Qml`、`Quick`、`QuickControls2`、`Widgets`、`DBus`、`LinguistTools`
- **运行时依赖**
  - Kirigami QML 模块（`org.kde.kirigami`）
  - KDE QQC2 风格（`org.kde.desktop`）
  - Breeze 图标主题（深色模式下使用 `breeze-dark`）

## 构建与运行

```bash
# 配置
cmake -B build -S . -DCMAKE_BUILD_TYPE=Release

# 编译
cmake --build build -j$(nproc)

# 运行
./build/qtaskman
```

如果 Qt 安装在非标准前缀（例如通过 aqt 安装），配置时指定：

```bash
cmake -B build -S . -DCMAKE_PREFIX_PATH=/path/to/Qt/6.x/gcc_64
```

### 安装

```bash
cmake --install build --prefix /usr/local   # 可执行文件安装到 <prefix>/bin
```

### 更新翻译

修改 QML/C++ 中的可翻译字符串后，重新提取并编译翻译：

```bash
cmake --build build --target update_translations
```

随后编辑 `i18n/qtaskman_zh_CN.ts` 与 `i18n/qtaskman_en_US.ts`，补全新条目的译文，再重新构建。

## 使用说明

- **进入浮窗**：硬件监控页工具栏的浮窗按钮，可将当前监控类目以悬浮窗形式展示，同时隐藏主窗口。
- **浮窗操作**：拖动标题栏移动，拖动右下角缩放；点击类目胶囊切换监控项；`Esc` 或 × 返回主窗口。
- **后台驻留**：设置中开启「关闭窗口时驻留到系统托盘」后，关闭主窗口仅隐藏到托盘，程序继续运行；关闭该选项则关闭即退出。
- **透明度**：设置 →「浮窗透明度」拖动滑块实时调整。

### 浮窗的平台说明

浮窗的「总在最前」与「透明度」依赖显示服务器与合成器：

| 环境 | 总在最前 | 透明度 |
| --- | --- | --- |
| X11 + 合成器（KWin / Mutter / picom 等） | 支持 | 支持 |
| X11 无合成器 | 支持 | 不支持 |
| Wayland + KWin（KDE Plasma） | 支持 | 支持 |
| Wayland + Mutter（GNOME） | 不支持 | 不支持 |

GNOME/Mutter on Wayland 出于策略不允许应用自行置顶或设置窗口透明度，属于合成器限制；如需该功能，请改用 X11 会话或 KDE Plasma。

## 项目结构

```
qtaskman/
├── CMakeLists.txt          # 构建脚本（Qt 6、资源、翻译）
├── assets/                 # 应用图标
├── cmake/                  # 自定义 CMake 模块
├── i18n/                   # 翻译源文件（zh_CN / en_US）
├── qml/
│   ├── Main.qml            # 主窗口、浮窗调度、托盘相关逻辑
│   ├── Sidebar.qml         # 侧边导航
│   ├── components/         # 通用组件（含 FloatMonitorWindow）
│   └── pages/              # 总览 / 应用 / 进程 / 监控 / 设置 / 关于
└── src/                    # C++ 后端
    ├── main.cpp            # 入口、托盘、上下文属性注册
    ├── systemmonitor.*     # 硬件资源采样
    ├── processsampler.*    # 进程采样
    ├── processmodel.*      # 进程模型
    ├── processcontroller.* # 进程控制（终止、重启等）
    ├── appsmodel.*         # 应用（窗口）模型
    ├── windowprobe.*       # 窗口探测
    ├── systeminfo.*        # 系统信息
    ├── settings.*          # 设置持久化（QSettings）
    ├── thememanager.*      # 主题（QPalette / 图标主题）
    └── translator.*        # 语言切换
```

## 当前版本

**1.0.0** — © 2026 qTaskMan 开发者
