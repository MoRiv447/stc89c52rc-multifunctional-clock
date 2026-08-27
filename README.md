# STC89C52RC Multifunctional Clock
# STC89C52RC 多功能电子时钟系统

A multifunctional clock system based on the STC89C52RC microcontroller, with an LCD1602 display, DS1302 RTC, DHT11 sensor, and features including alarm, stopwatch, and countdown timer.

基于 STC89C52RC 单片机开发的多功能时钟系统，采用 LCD1602 液晶显示、DS1302 实时时钟、DHT11 温湿度传感器，集成时钟、闹钟、秒表、倒计时等多种功能。

![MCU](https://img.shields.io/badge/MCU-STC89C52RC-orange)
![License](https://img.shields.io/badge/license-MIT-green)
![Language](https://img.shields.io/badge/language-C-blue)

---

## 📖 Project Introduction / 项目简介

**English:** This project is built around the **STC89C52RC MCU** and integrates clock display, alarm, temperature/humidity sensing, stopwatch, and countdown functions. It is a comprehensive learning project for practicing microcontroller application development. The front-end uses an **LCD1602** character display, with multi-key interaction and a buzzer for alerts.

**中文：** 本项目基于 **STC89C52RC 单片机**开发，集成了时钟显示、闹钟、温湿度检测、秒表、倒计时等多种功能，是一个用于学习和实践单片机应用开发的综合项目。前端显示采用 **LCD1602 液晶屏**，支持多按键交互和蜂鸣器提醒。

---

## ✨ Features / 功能特性

**功能一览表 / Feature overview:**

| 功能 Feature | 说明 Description | 驱动芯片/引脚 Driver / Pins |
|-----|------|----------|
| **⏰ 实时时钟 Real-time Clock** | 显示年月日时分秒，支持手动设置 / Show Y-M-D H-M-S, manual setting | DS1302 (P1^0–P1^2) |
| **🔔 闹钟 Alarm** | 可设置闹钟时间、启用/禁用、蜂鸣提醒 / Set time, enable/disable, buzzer alert | 软件实现 Software |
| **🌡️ 温湿度监测 Temp & Humidity** | 实时显示温度和湿度数据 / Live temp & humidity readout | DHT11 (P1^6) |
| **⏱️ 秒表 Stopwatch** | 精准计时，显示分钟和秒数 / Precise timing, min & sec | Timer0 |
| **⏲️ 倒计时 Countdown** | 自定义倒计时，时长可调整 / Adjustable countdown | Timer0 |
| **🔊 声音提示 Buzzer** | 闹钟和倒计时结束蜂鸣 / Beeps on alarm & countdown end | 蜂鸣器 Buzzer (P2^3) |
| **💾 数据存储 Data Storage** ⚠️ | **未启用：驱动已写好但主程序未调用，设置掉电不保存** / **NOT ENABLED: driver written but never called by `main`; settings are not persisted across power loss** | EEPROM (I2C: P2^0–P2^1) |

> ⚠️ **Note on EEPROM / EEPROM 说明:** The `EEPROM.c/.h` driver (I2C `Init` / `Write` / `Read`) is fully implemented, but `main.c` only includes the header and never calls those functions. Therefore the clock/alarm settings are **not** saved to EEPROM and will reset after a power cycle. This is a known limitation, not a working feature. See [Known Limitations](#-known-limitations--已知问题).

---

## 🛠️ Hardware Requirements / 硬件需求

### Core Components / 核心芯片

- **MCU / 主控:** STC89C52RC (8051-compatible)
- **Display / 显示:** LCD1602 (16×2 character)
- **RTC / 实时时钟:** DS1302 module
- **Sensor / 温湿度传感器:** DHT11
- **Storage / 存储:** EEPROM (24C02 or compatible) — *driver present, not wired in / 驱动存在，未接入*
- **Keys / 按键:** 4 tactile switches
- **Buzzer / 蜂鸣器:** active or passive buzzer

### Pin Connection / 引脚连接表

| 器件 Device | 引脚 Pin | 功能 Function |
|-----|-----|------|
| **LCD1602** | P0 | 数据总线 Data bus (D0–D7) |
| | P2^2 (RS) | 寄存器选择 Register select |
| | P2^5 (RW) | 读/写控制 Read/Write |
| | P2^6 (EN) | 使能信号 Enable |
| **DS1302** | P1^0 | CLK (时钟 Clock) |
| | P1^1 | DAT (数据 Data) |
| | P1^2 | RST (复位 Reset) |
| **DHT11** | P1^6 | 单总线数据 1-Wire data |
| **按键 Keys** | P3^0 | 模式切换 Mode switch (Key 1) |
| | P3^1 | 功能按键 Select (Key 2) |
| | P3^2 | 功能按键 Increment (Key 3) |
| | P3^3 | 功能按键 Decrement (Key 4) |
| **蜂鸣器 Buzzer** | P2^3 | 声音输出 Sound output |
| **EEPROM** | P2^0 (SDA) | I2C 数据线 I2C data |
| | P2^1 (SCL) | I2C 时钟线 I2C clock |

---

## 📂 Project Structure / 项目文件说明

```
├── main.c              # 主程序入口，模式管理和主循环 / Main entry, mode mgmt & loop
├── LCD1602.c/h         # LCD1602 液晶屏驱动 / LCD1602 driver
├── DS1302.c/h          # DS1302 实时时钟驱动 / DS1302 RTC driver
├── Timer0.c/h          # Timer0 定时器初始化 / Timer0 init
├── Key.c/h             # 按键扫描驱动 / Key scanning
├── Delay.c/h           # 延时函数库 / Delay routines
├── beep.c/h            # 蜂鸣器驱动 / Buzzer driver
├── alarm.c/h           # 闹钟功能模块 / Alarm module
├── dht11.c/h           # DHT11 温湿度传感器驱动 / DHT11 driver
├── countdown.c/h       # 倒计时功能 / Countdown
├── stopwatch.c/h       # 秒表功能 / Stopwatch
├── EEPROM.c/h          # I2C EEPROM 驱动（已实现，未接入主程序）/ I2C EEPROM driver (implemented, not wired)
└── README.md           # 项目说明文档 / Documentation
```

### Core Modules / 核心模块功能说明

**main.c** — 主程序 / Main program
- 模式切换和管理 / Mode switching & management
- 各功能模块的调用 / Invokes each feature module
- Timer0 中断处理 / Timer0 interrupt handling

**LCD1602.c** — 显示驱动 / Display driver
- `LCD_Init()` — 初始化 / Init
- `LCD_ShowString()` — 显示字符串 / Show string
- `LCD_ShowNum()` — 显示数字 / Show number

**DS1302.c** — 实时时钟 / RTC
- `DS1302_Init()` — 初始化 / Init
- `DS1302_readtime()` — 读取时间 / Read time
- `DS1302_settime()` — 设置时间 / Set time

**alarm.c** — 闹钟模块 / Alarm
- `alarmcheck()` — 检查是否触发 / Check trigger
- `alarmRing()` — 处理蜂鸣 / Handle beep
- `alarmset()` — 闹钟设置界面 / Setting UI

**dht11.c** — 温湿度传感器 / Sensor
- `dht11_Init()` — 初始化 / Init
- `dht11_getdata()` — 获取温湿度值 / Get temp & humidity

**countdown.c** — 倒计时 / Countdown
- `countdownshow()` — 显示倒计时 / Show countdown
- `countdown_timer_handler()` — 倒计时计数 / Tick handler

**stopwatch.c** — 秒表 / Stopwatch
- `stopwatchshow()` — 显示秒表 / Show stopwatch
- `stopwatch_timer_handler()` — 秒表计数 / Tick handler

---

## 🚀 Quick Start / 快速开始

### Prerequisites / 环境要求

- **编译器 Compiler:** Keil uVision 5 (C51) or SDCC
- **烧录工具 Flasher:** STC-ISP v6.88 or later
- **硬件 Hardware:** STC89C52RC dev board + modules

### Build Steps / 编译步骤

1. **创建工程 Create project**
   ```
   打开 Keil uVision → 新建工程 → 选择芯片：STC89C52RC
   Open Keil uVision → New Project → Select device: STC89C52RC
   ```

2. **添加源文件 Add source files**
   ```
   Project → Add Files to Project → 选择所有 .c 和 .h 文件
   Project → Add Files to Project → Select all .c and .h files
   ```

3. **配置编译选项 Configure options**
   ```
   Project → Options for Target
   → C/C++ 标签：优化级别 Level 2 / Optimization Level 2
   → Target 标签：晶体振荡器 12.000 MHz / Crystal 12.000 MHz
   ```

4. **编译项目 Build**
   ```
   按 F7 或 Build → Build Target → 生成 .hex 文件
   Press F7 or Build → Build Target → Generate .hex
   ```

5. **烧录代码 Flash**
   ```
   打开 STC-ISP → 选择 .hex 文件 → 选择 COM 口 → 下载编程
   Open STC-ISP → Select .hex → Select COM port → Download
   ```

---

## 💡 How It Works / 工作原理

### System Architecture / 系统架构

```
┌─────────────────────────────────────────┐
│         STC89C52RC MCU                  │
├─────────────────────────────────────────┤
│  Timer0 ISR (every 2 ms)                │
│  ├─ Blink control (flash flag)          │
│  ├─ DHT11 update counter                │
│  ├─ Stopwatch accumulator              │
│  ├─ Countdown decrement                │
│  └─ Periodic task dispatch             │
│                                         │
│  Main loop                              │
│  ├─ Key scan                            │
│  ├─ Mode switch & dispatch              │
│  ├─ Feature module execution            │
│  └─ LCD refresh                        │
└─────────────────────────────────────────┘
     ↓         ↓         ↓         ↓
 LCD1602    DS1302     DHT11    Buzzer
 显示屏     时钟      传感器    蜂鸣器
```

### System Modes / 系统模式（按键 1 切换 / switched by Key 1）

| 模式 Mode | 功能 Function | 说明 Description | 按键操作 Keys |
|-----|------|------|--------|
| **0** | 时间显示 Time | 显示当前日期时间，监听闹钟 / Show date-time, watch alarm | Key1 → Mode 1 |
| **1** | 时间设置 Set time | 设置年月日时分秒 / Set Y-M-D H-M-S | Key2 select, Key3/4 ± |
| **2** | 闹钟设置 Set alarm | 设置闹钟时间和启用状态 / Set alarm time & enable | Key2 select, Key3/4 ± |
| **3** | 温湿度 Temp/Humi | 实时显示温度和湿度 / Live temp & humidity | Key1 → next |
| **4** | 秒表 Stopwatch | 精准计时功能 / Precise timing | Key2 start/pause, Key3 reset |
| **5** | 倒计时 Countdown | 自定义倒计时 / Custom countdown | Key2 start/pause, Key3/4 ± |

### Key Functions / 按键功能表

| 按键 Key | 功能 Function |
|-----|------|
| **按键 1 Key 1** | 模式切换 Mode switch |
| **按键 2 Key 2** | 选择编辑项 / 启动暂停 Select item / Start-Pause |
| **按键 3 Key 3** | 数值加 1 / 时长加 30s Increment +1 / +30s |
| **按键 4 Key 4** | 数值减 1 / 时长减 30s Decrement −1 / −30s |

---

## 🔧 Code Highlights / 代码亮点

### 1. 非阻塞中断设计 / Non-blocking interrupt design
所有实时任务在 Timer0 中断中处理，主循环保持快速响应，不会阻塞：
All real-time tasks run inside the Timer0 ISR, so the main loop stays responsive and never blocks:

```c
void Timer0_Routine() interrupt 1 {
    // 闪烁控制 / Blink control
    // DHT11 更新计数 / DHT11 update counter
    // 秒表/倒计时计数 / Stopwatch & countdown ticks
    // 所有操作均为非阻塞式 / All operations are non-blocking
}
```

### 2. 模块化代码结构 / Modular structure
每个功能独立实现在单独文件中，便于维护、测试、移植：
Each feature lives in its own file for easy maintenance, testing, and porting:
- `alarm.c` — 闹钟 / Alarm
- `dht11.c` — 温湿度 / Temp & humidity
- `countdown.c` — 倒计时 / Countdown
- `stopwatch.c` — 秒表 / Stopwatch

### 3. 灵活的闹钟响铃 / Flexible alarm ringing
采用计数器实现 500 ms 鸣 + 500 ms 静音循环，无阻塞，任意按键可停止：
A counter drives a 500 ms beep / 500 ms silence loop — non-blocking, any key stops it.

### 4. DHT11 单总线通信 / DHT11 1-Wire comms
正确处理时序，关闭中断避免被打断，数据校验完整性：
Correct timing, interrupts disabled during critical windows, and checksum validation.

---

## ⚠️ Known Limitations / 已知问题

**English:** The EEPROM data-storage feature is **not functional**. The I2C driver in `EEPROM.c/.h` (`EEPROM_Init`, `EEPROM_Write`, `EEPROM_Read`) is complete, but `main.c` includes the header without ever calling those functions. As a result, alarm and time settings are held only in RAM and are lost on power loss. The EEPROM wiring/pins are documented for hardware completeness, but the module is currently dead code. Wiring it up (save on setting-change, load on boot) is a TODO.

**中文：** EEPROM 数据存储功能**当前未生效**。`EEPROM.c/.h` 中的 I2C 驱动（`EEPROM_Init`、`EEPROM_Write`、`EEPROM_Read`）已完整实现，但 `main.c` 仅包含了头文件，从未调用这些函数。因此闹钟和时间的设置只保存在 RAM 中，掉电即丢失。EEPROM 的引脚与接线仅为硬件完整性而列出，该模块目前属于"死代码"。将其真正接入（设置变更时写入、开机时读取）是待办项。

---

## ❓ FAQ / 常见问题与解决

**Q1: 时钟显示不正常或不走？ / Clock shows wrong or doesn't tick?**
- 检查 DS1302 晶体振荡器（32.768 kHz）是否良好 / Check the DS1302 32.768 kHz crystal
- 验证 P1^0, P1^1, P1^2 接线是否正确 / Verify P1^0–P1^2 wiring
- 确认 DS1302 供电正常 / Confirm DS1302 power
- 尝试重新设置时间 / Re-set the time

**Q2: DHT11 无法读取数据？ / DHT11 reads nothing?**
- 检查 P1^6 接线 / Check P1^6
- 确认上拉电阻（通常 4.7 kΩ）已接 / Add 4.7 kΩ pull-up
- DHT11 初始化需要 18 ms 低电平 / Needs 18 ms low pulse to start
- 尝试远离干扰源 / Keep away from noise

**Q3: LCD 显示乱码或不显示？ / LCD garbled or blank?**
- 检查 P0 数据总线接线（8 条线）/ Check the 8-line P0 data bus
- 验证 P2^2, P2^5, P2^6 控制线 / Verify P2^2, P2^5, P2^6
- 调整 LCD 对比度（VL 引脚）/ Adjust contrast (VL pin)
- 检查电源供应（通常需要 5V）/ Check 5V supply

**Q4: 按键无响应？ / Keys unresponsive?**
- 检查 P3^0–P3^3 接线 / Check P3_0–P3_3
- 确认按键机械性能良好 / Check switch mechanics
- 增加 `Delay(20)` 防抖时间 / Increase debounce `Delay(20)`
- 用万用表测试按键电压 / Measure with a multimeter

**Q5: 蜂鸣器不响或声音弱？ / Buzzer silent or weak?**
- 检查 P2^3 输出电平 / Check P2^3 output
- 确认蜂鸣器极性（有源/无源）/ Confirm active vs passive type
- 增加 `Beep()` 调用时长 / Lengthen `Beep()`
- 检查蜂鸣器供电 / Check buzzer power

**Q6: 倒计时或秒表不走？ / Countdown or stopwatch not running?**
- 检查 Timer0 是否初始化成功 / Check Timer0 init
- 验证中断函数是否执行 / Verify ISR runs
- 检查晶体振荡器频率（应为 12 MHz）/ Confirm 12 MHz crystal

---

## 📚 What You'll Learn / 学习内容

通过本项目可以学到 / What this project teaches:

- **单片机基础 MCU basics**
  - GPIO 口配置与控制 / GPIO config & control
  - 中断和定时器应用 / Interrupts & timers
  - 内存管理与变量作用域 / Memory & scope

- **硬件驱动 Hardware drivers**
  - LCD1602 并行接口控制 / LCD1602 parallel interface
  - DS1302 三线制协议 / DS1302 3-wire protocol
  - DHT11 单总线协议 / DHT11 1-Wire protocol
  - I2C EEPROM 通信（驱动已实现，主程序未调用）/ I2C EEPROM (driver done, not called)

- **嵌入式设计 Embedded design**
  - 状态机实现 / State machine
  - 非阻塞编程范式 / Non-blocking paradigm
  - 模块化代码组织 / Modular organization
  - 中断与主循环的协调 / ISR/main-loop coordination

- **调试技巧 Debugging**
  - 硬件故障排查 / Hardware troubleshooting
  - 时序分析 / Timing analysis
  - 数据校验 / Data validation

---

## 🔌 Wiring Checklist / 硬件接线清单

| 器件 Device | 接线 Wiring |
|-----|-----|
| LCD1602 | P0 (D0–D7), P2^2, P2^5, P2^6, 5V, GND |
| DS1302 | P1^0–2, 5V, GND |
| DHT11 | P1^6, 4.7k pull-up, 5V, GND |
| 按键 Keys | P3^0–3, GND |
| 蜂鸣器 Buzzer | P2^3, GND |
| EEPROM | P2^0–1, 4.7k pull-up ×2, 5V, GND *(driver not wired / 驱动未接入)* |

---

## 📄 License / 许可证

本项目采用 **MIT License** 开源协议，可自由使用、修改和分发。
Licensed under the **MIT License** — free to use, modify, and distribute.

```
MIT License

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction...
```

---

## 👨‍💻 Author / 作者

**MoRiv447**
- GitHub: [@MoRiv447](https://github.com/MoRiv447)
- 项目主页 / Project: [STC89C52RC-Clock](https://github.com/MoRiv447/STC89C52RC-Clock)

---

## 💬 Feedback & Support / 反馈与支持

如有问题或建议，欢迎：
Questions or suggestions are welcome:
- 📝 提交 **Issues** 报告问题 / Open an **Issue**
- 💭 发起 **Discussions** 讨论想法 / Start a **Discussion**
- 🤝 贡献代码 **(Pull Request)** / Contribute via **PR**

---

## 🙏 Acknowledgements / 致谢

感谢以下资源和社区的支持 / Thanks to:
- STC 官方单片机文档 / STC MCU docs
- Keil uVision 开发工具 / Keil uVision
- DHT11 传感器 Datasheet
- DS1302 时钟模块文档 / DS1302 docs
- 开源社区的各类教程和支持 / The open-source community

---

**⭐ 如果这个项目对你有帮助，请给个 Star 支持一下！ / If this helps you, please leave a Star!**

最后更新时间 / Last updated: 2025-11-29
