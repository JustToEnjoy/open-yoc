## DMA

### 概要

该文档主要描述DMA模块的CSI1.0自动化测试AT命令。 文档中所涉及的这些命令是用来测试DMA模块的接口工作是否正常、数据传输是否正确、接口返回值是否符合预期。



### 测试原理

测试时，应给被测试板发送DMA测试命令进行相应的测试；测试程序会按照按照测试命令的配置进行相应的DMA测试， 若与预期一致，则相应的测试成功完成，否则测试失败。



### 测试流程

- 测试前应当准备好测试板（以下简称A板）
- 发送DMA测试命令进行测试 （测试命令功能请参照下文）



### AT命令列表

以下是TIMER模块对应的测试命令列表及其功能简要介绍。

| AT命令列表                                                   | 功能                                                         |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| AT+DMA_INTERFACE                                             | DMA模块接口测试。该命令将会测试DMA模块下所有的CSI 接口， 测试CSI接口时将会传入异常值、非法值，验证CSI接口返回值是否符合期望 |
| AT+DMA_MEMORY_TO_MEMORY=channel_idx,data_width,src_addr_inc,<br/>dest_addr_inc,group_length,transfer_length | 测试内存到内存的数据传输通过DMA搬运                          |



### 参数列表

以下是PWM模块对应的AT测试命令共用参数的简要介绍。

| 参数名字        | 释义                       | 取值范围                                                     |
| --------------- | -------------------------- | ------------------------------------------------------------ |
| channel_idx | 选择使用哪一个DMA的通道 | 参考SOC手册，0表示DMA的第一个通道 |
| data_width      | 传输的数据宽度，1表示1字节 | 1，2，4                                                      |
| src_addr_inc    | 源地址的地址模式           | 0：DMA_ADDR_INC<br/>1：DMA_ADDR_DEC<br/>2：DMA_ADDR_CONSTANT |
| src_addr_inc    | 目的地址的地址模式         | 0：DMA_ADDR_INC<br/>1：DMA_ADDR_DEC<br/>2：DMA_ADDR_CONSTANT |
| group_length    | 传输的数据组长度（字节）   | 0x0 - 0xFFFFFFFF                                             |
| transfer_length | 传输的数据长度（字节）     | 0x0 - 0xFFFFFFFF                                             |

  

### 命令详解

#### AT+DMA_INTERFACE

| 命令格式             | AT+DMA_INTERFACE                                             |
| -------------------- | ------------------------------------------------------------ |
| 命令功能             | DMA模块接口测试。该命令将会测试DMA模块下所有的CSI 接口， 测试CSI接口时将会传入异常值、非法值，验证CSI接口返回值是否符合期望 |
| 可搭配的辅助测试命令 | 无                                                           |
| 示例                 | 步骤1:发送测试接口指令<br>AT+DMA_INTERFACE<br>测试完成后， 板子对应串口会打印OK；否则测试失败 |
| 备注                 |                                                              |



#### AT+DMA_MEMORY_TO_MEMORY

| 命令格式             | AT+DMA_MEMORY_TO_MEMORY=data_width,src_addr_inc,<br/>dest_addr_inc,group_length,transfer_length |
| -------------------- | ------------------------------------------------------------ |
| 命令功能             | 测试内存到内存的数据传输通过DMA搬运                          |
| 可搭配的辅助测试命令 | 无                                                           |
| 示例                 | 发送命令AT+DMA_MEMORY_TO_MEMORY=0,1,0,0,32,32到A板，测试完成后， A板对应串口会打印OK；否则测试失败 |
| 备注                 |                                                              |


