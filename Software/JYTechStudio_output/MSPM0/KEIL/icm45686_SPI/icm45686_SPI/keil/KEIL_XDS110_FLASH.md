# Keil SPI 示例的 XDS110 烧录

SPI 示例的源文件和 `empty.syscfg` 已与 CCS 示例逐字一致。由于 Keil 的 TI XDS/CMSIS-DAP 插件无法枚举当前 XDS110，本工程改为在 Keil 完成编译后调用 TI UniFlash。

使用方法：连接 XDS110 后，在 Keil 中按 **F7**（Build）。构建结束会自动执行 `flash_xds110.bat`，完成写入、校验、系统复位和运行。

默认 UniFlash 路径为 `C:\ti\uniflash_9.2.0`。如果安装在其他位置，请把该路径设置为系统环境变量 `UNIFLASH_ROOT`，例如 `D:\ti\uniflash_9.2.0`。

不要使用 F8；它仍会调用 Keil 自身失效的调试下载插件。
