# **MCU_Project**
  Store some related projects based on C51 single chip microcomputer and Proteus co-simulation, sort out some of the projects you have done
# **目录**
## **MCP3421_Driver**
  基于MSP430F249写的MCP3421底层驱动，可以实现对MCP3421数据读取。
## **Project/Case01_[Proteus_51]_智能料理机Proteus仿真**
### 项目描述
  该项目根据客户要求，实现了基于51单片机的智能料理机的Proteus仿真分析。可以模拟温度（DS18B20）、重量（滑动变阻器）、进出水（DC Motor）、模拟加热（OVEN）、报警（Beef）、显示（LCD1602）。
### 所涉及模块  
  所涉及到的模块有：  
1. 使用PCF8574控制LCD1602，所使用的协议为IIC  
2. 使用PCF8591采集电位器电压，模拟称重  
3. 使用L298控制直流电机转动与停止，暂未进行调速  
4. 使用光耦控制OVEN模拟加热  
5. 使用DS18B20模拟温度采集

## **Project/IAR_STM32_Distance_Car**
### 描述
  遥控小车控制，双电机，使用IAR进行开发，处理器为STM32F103
### Note
  工程使用的软件版本如下：  
  Keil版本为5.14.2  
  **下载链接**  
  链接：https://pan.baidu.com/s/1BH_4BD6fiBDL5O3XE921-g 
  提取码：e2os
  Proteus版本为Proteus 8.6  
  **下载链接**  
  链接：https://pan.baidu.com/s/1rt_knZHxOBIYRNPpWM70zw 
  提取码：7o5o 
  




