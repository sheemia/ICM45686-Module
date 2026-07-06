# ICM-45686 六轴 IMU 模块 / 6-Axis IMU Module

[中文](#中文) | [English](#english)

基于 TDK InvenSense ICM-45686 的六轴惯性测量模块开源工程，包含硬件生产资料、BOM，以及适用于 MSPM0、STM32 和 Arduino 的 I2C/SPI 示例。

An open-source six-axis inertial measurement module based on the TDK InvenSense ICM-45686. This repository includes hardware manufacturing files, a BOM, and I2C/SPI examples for MSPM0, STM32, and Arduino.

> 示例程序输出加速度、角速度和温度数据，不包含完整的姿态融合算法。  
> The examples output acceleration, angular velocity, and temperature data. A complete attitude-fusion algorithm is not included.

---

## 中文

### 项目特点

- 15.2 mm × 12.7 mm 四层模块板
- ICM-45686 三轴陀螺仪与三轴加速度计
- 支持 I2C 与 SPI 通信
- 提供原理图、BOM 和 Gerber 生产文件
- 提供 MSPM0G3507、STM32F103、STM32F407 和 Arduino 示例
- 支持普通串口文本输出与 JYTech AHRS Studio 二进制输出

### 在线资料

- [立创开源硬件平台：ICM-45686 原理图与 PCB 工程](https://oshwhub.com/sheemia/project_ngmuvyoo)
- [模块测试视频](https://www.bilibili.com/video/BV1RqT96wEXt/?spm_id_from=333.1387.homepage.video_card.click&vd_source=835a56ded42cebdec9cf08a5cdbdcd14)

### 仓库结构

```text
.
├── Hardware/
│   ├── BOM_Board1_Schematic1.xlsx   # 物料清单
│   ├── Gerber_PCB1.zip              # PCB 生产文件
│   └── SCH_Schematic1.pdf           # 原理图
├── Software/
│   ├── 纯串口输出/                    # 可读文本输出示例
│   ├── JYTech上位机输出/              # 二进制数据帧输出示例
│   └── README.md                    # 示例工程与数据帧说明
├── Tools/
│   ├── JYTechAhrsStudio_v1.2.0_Setup.exe
│   └── JYTech_AHRS_Studio_Communication Protocol_V1.0.pdf
├── .gitignore
└── README.md
```

### 支持平台

| 平台 | 开发环境 | 通信接口 |
| --- | --- | --- |
| MSPM0G3507 | Code Composer Studio | I2C、SPI |
| MSPM0G3507 | Keil MDK | I2C、SPI |
| STM32F103 | Keil MDK | I2C、SPI |
| STM32F407 | Keil MDK / HAL | 以具体工程配置为准 |
| Arduino | Arduino IDE | I2C |

每个 MCU 子工程均包含完整工程文件。接线、工程导入方式和引脚配置请查看对应子目录中的 `README.md`。

### 引脚说明

| 模块引脚 | 功能 | 接线说明 |
| --- | --- | --- |
| VIN | 电源输入 | 连接 3.3 V 电源 |
| GND | 电源地 | 与主控共地 |
| AD0 / MISO | I2C 地址选择 / SPI 数据输出 | I2C 模式用于 `AD0`；SPI 模式连接主控 `MISO` |
| SDA / MOSI | I2C 数据 / SPI 数据输入 | I2C 模式连接 `SDA`；SPI 模式连接主控 `MOSI` |
| SCL / SCLK | I2C / SPI 时钟 | I2C 模式连接 `SCL`；SPI 模式连接主控 `SCLK` |
| CS | SPI 片选 | SPI 模式低电平有效；I2C 模式保持高电平 |
| ASD | 辅助 I2C 数据 | 不使用时可悬空 |
| ASL | 辅助 I2C 时钟 | 不使用时可悬空 |
| INT1 | 中断输出 1 | 可配置为数据就绪等中断信号 |
| INT2 | 中断输出 2 | 可配置中断信号 |

> 模块使用 3.3 V 逻辑电平，请勿直接接入 5 V 逻辑电平。接线前请同时核对原理图和示例工程中的 GPIO 配置。

### 快速开始

1. 使用成品模块，或通过以下资料自行打样：
   - [`Hardware/Gerber_PCB1.zip`](Hardware/Gerber_PCB1.zip)：PCB 生产文件
   - [`Hardware/BOM_Board1_Schematic1.xlsx`](Hardware/BOM_Board1_Schematic1.xlsx)：物料清单
   - [`Hardware/SCH_Schematic1.pdf`](Hardware/SCH_Schematic1.pdf)：原理图
2. 在 [`Software/`](Software/) 中选择输出方式：
   - `纯串口输出/`：适合首次上电、检查接线及观察传感器数据
   - `JYTech上位机输出/`：通过固定长度二进制帧连接 JYTech AHRS Studio
3. 根据主控、开发环境和通信接口选择子工程。
4. 按子工程 README 完成接线，检查芯片型号、下载器和 GPIO 配置。
5. 编译并下载程序。纯串口示例默认使用 `115200 8N1`。

ICM-45686 采用底部焊盘封装，建议使用钢网、回流焊和可靠的贴装工艺。焊接后应先检查电源是否短路，再连接主控。

### 数据输出

纯串口模式输出三轴加速度（`mg`）、三轴角速度（`°/s`）和温度（`°C`，Arduino 示例除外）。

JYTech 上位机模式连续发送 56 字节二进制帧。请在 AHRS Studio 中选择 **ICM45686** 产品类型，不要使用串口助手的文本模式查看该数据。当前示例仅上传传感器数据，Roll、Pitch、Yaw 字段固定为 `0`，不表示已经完成姿态解算。帧结构、校验方式和默认输出频率请参阅 [`Software/README.md`](Software/README.md)。

配套软件和通信协议位于 [`Tools/`](Tools/)。

### 移植提示

移植到其他 MCU 时，建议保留官方驱动层，并适配以下平台接口：

- SPI/I2C 寄存器读写
- 毫秒或微秒延时
- 片选与中断 GPIO
- 串口输出（如需要）

修改加速度计或陀螺仪量程后，必须同步修改原始数据换算系数。使用 SPI 时，还应确认 SPI 模式、最高时钟频率和片选时序符合数据手册要求。

### 常见问题

**无法读取传感器**

依次检查 3.3 V 供电、共地、示例与接线所用接口是否一致、I2C 地址或 SPI 片选配置，以及是否存在虚焊、连锡或器件方向错误。

**串口显示乱码**

- 纯串口版本：确认串口参数为 `115200 8N1`
- 上位机版本：输出的是二进制数据，请使用配套上位机解析
- 不要在二进制数据流中混入 `printf`、`Serial.print` 等调试文本

**静止时仍有零偏**

MEMS 传感器存在零偏和温漂属于正常现象。高精度应用应在设备静止时进行零偏标定，并根据使用环境进行温度补偿。

### 参与贡献

欢迎通过 GitHub Issues 提交问题或建议，也欢迎提交 Pull Request。报告问题时，请尽量附上主控型号、通信方式、使用的子工程、接线图和完整报错信息。

### 开源许可

本项目采用分级授权：

- **硬件参考设计（Hardware Design）**：基于 `CC BY-NC-SA 4.0` 协议开放。允许个人学习与学术竞赛免费使用；未经 JYTech 书面授权，禁止商业制造、销售或倒卖。
- **底层驱动源代码（Firmware Driver）**：基于 `MIT License` 开源，可集成至个人或商业项目，但须遵守 MIT License 的版权声明保留要求。

上述授权仅适用于 JYTech 拥有著作权的原创内容。仓库中的 TDK InvenSense 驱动、CMSIS、芯片厂商库及其他第三方代码仍适用各自的许可条款；使用和再发布时请保留原始版权及许可声明。

如需硬件设计商业授权或批量采购，请联系 **sheemia**。

---

## English

### Features

- Compact 15.2 mm × 12.7 mm four-layer module
- ICM-45686 three-axis gyroscope and three-axis accelerometer
- I2C and SPI interfaces
- Schematic, BOM, and Gerber manufacturing files
- Examples for MSPM0G3507, STM32F103, STM32F407, and Arduino
- Human-readable serial output and JYTech AHRS Studio binary output

### Online Resources

- [OSHWHub: ICM-45686 schematic and PCB project](https://oshwhub.com/sheemia/project_ngmuvyoo)
- [Module test video: coming soon](https://www.bilibili.com/video/BV1RqT96wEXt/?spm_id_from=333.1387.homepage.video_card.click&vd_source=835a56ded42cebdec9cf08a5cdbdcd14)

### Repository Layout

```text
.
├── Hardware/   # Schematic, BOM, and Gerber files
├── Software/   # MCU and Arduino examples
├── Tools/      # JYTech AHRS Studio and communication protocol
└── README.md
```

### Supported Platforms

| Platform | Toolchain | Interface |
| --- | --- | --- |
| MSPM0G3507 | Code Composer Studio | I2C, SPI |
| MSPM0G3507 | Keil MDK | I2C, SPI |
| STM32F103 | Keil MDK | I2C, SPI |
| STM32F407 | Keil MDK / HAL | Depends on the selected project |
| Arduino | Arduino IDE | I2C |

Each MCU example contains a complete project. Refer to the `README.md` in the corresponding subdirectory for wiring, project import, and pin configuration.

### Pinout

| Pin | Function | Description |
| --- | --- | --- |
| VIN | Power input | Connect to 3.3 V |
| GND | Ground | Connect to the controller ground |
| AD0 / MISO | I2C address select / SPI data output | `AD0` in I2C mode; controller `MISO` in SPI mode |
| SDA / MOSI | I2C data / SPI data input | `SDA` in I2C mode; controller `MOSI` in SPI mode |
| SCL / SCLK | I2C / SPI clock | `SCL` in I2C mode; controller `SCLK` in SPI mode |
| CS | SPI chip select | Active low in SPI mode; keep high in I2C mode |
| ASD | Auxiliary I2C data | Leave unconnected when unused |
| ASL | Auxiliary I2C clock | Leave unconnected when unused |
| INT1 | Interrupt output 1 | Configurable, for example as data-ready |
| INT2 | Interrupt output 2 | Configurable interrupt output |

> The module uses 3.3 V logic. Do not connect it directly to 5 V logic. Verify both the schematic and the GPIO configuration of the selected example before wiring.

### Quick Start

1. Use a ready-made module or manufacture one with:
   - [`Hardware/Gerber_PCB1.zip`](Hardware/Gerber_PCB1.zip)
   - [`Hardware/BOM_Board1_Schematic1.xlsx`](Hardware/BOM_Board1_Schematic1.xlsx)
   - [`Hardware/SCH_Schematic1.pdf`](Hardware/SCH_Schematic1.pdf)
2. Select an output mode under [`Software/`](Software/):
   - `纯串口输出/` (`Plain Serial Output`): recommended for initial bring-up and debugging
   - `JYTech上位机输出/` (`JYTech Host Output`): fixed-length binary frames for JYTech AHRS Studio
3. Choose a project for your controller, toolchain, and communication interface.
4. Follow the project README to wire the module and verify the target, programmer, and GPIO configuration.
5. Build and flash the firmware. Plain serial examples use `115200 8N1` by default.

The ICM-45686 uses a bottom-terminated package. A stencil, reflow soldering, and a reliable assembly process are recommended. Check the board for power shorts before connecting it to a controller.

### Data Output

Plain serial mode outputs three-axis acceleration (`mg`), three-axis angular velocity (`°/s`), and temperature (`°C`, except for the Arduino example).

JYTech host mode continuously transmits 56-byte binary frames. Select **ICM45686** in AHRS Studio and do not inspect these frames as text in a serial terminal. The current examples only transmit sensor measurements; Roll, Pitch, and Yaw are fixed at `0` and do not indicate attitude estimation. See [`Software/README.md`](Software/README.md) for the frame layout, checksum, and default output rates.

The desktop application and protocol document are available in [`Tools/`](Tools/).

### Porting

When porting to another MCU, retain the vendor driver where practical and implement:

- SPI/I2C register access
- Millisecond or microsecond delays
- Chip-select and interrupt GPIO
- Serial output, if required

If you change the accelerometer or gyroscope full-scale range, update the raw-data conversion factors accordingly. For SPI, verify the mode, maximum clock rate, and chip-select timing against the datasheet.

### Troubleshooting

**The sensor cannot be detected**

Check the 3.3 V supply, common ground, interface selection, I2C address or SPI chip-select configuration, solder joints, bridges, and component orientation.

**Unreadable serial output**

- Plain serial version: verify `115200 8N1`
- Host version: the stream is binary and must be decoded by the supplied desktop application
- Do not mix `printf`, `Serial.print`, or other debug text into the binary stream

**Non-zero readings while stationary**

MEMS sensors naturally exhibit offset and temperature drift. Precision applications should perform stationary bias calibration and temperature compensation where necessary.

### Contributing

Bug reports, suggestions, and Pull Requests are welcome on GitHub. When opening an Issue, include the controller model, interface, selected example, wiring diagram, and complete error output whenever possible.

### License

This project uses separate licensing terms:

- **Hardware Design**: released under `CC BY-NC-SA 4.0`. Personal learning and academic competition use is permitted. Commercial manufacturing, sale, or resale requires prior written authorization from JYTech.
- **Firmware Driver**: released under the `MIT License` and may be integrated into personal or commercial projects, subject to preservation of the required copyright and license notices.

These terms apply only to original material copyrighted by JYTech. TDK InvenSense drivers, CMSIS, semiconductor vendor libraries, and other third-party code remain subject to their respective licenses. Preserve all original copyright and license notices when using or redistributing them.

For commercial hardware licensing or volume purchasing, contact **sheemia**.

---

如果这个项目对你有帮助，欢迎 Star、提交 Issue 或分享你的移植与应用案例。  
If this project helps you, please consider starring it, opening an Issue, or sharing your port and application.
