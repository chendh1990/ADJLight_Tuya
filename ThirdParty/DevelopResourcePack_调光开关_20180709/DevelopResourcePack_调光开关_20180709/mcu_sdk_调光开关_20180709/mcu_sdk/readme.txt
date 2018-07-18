
此MCU_SDK是根据涂鸦开发平台上定义的产品功能，自动生成的MCU代码。在此基础上进行修改、补充，可快速完成MCU程序。
MCU_SDK所有需要用户调用或实现的函数内都添加有#error提示，请仔细阅读，按照说明实现后删除#error



开发步骤：

1：需要根据产品实际情况（重置wifi按钮和wifi状态指示灯处理方式、是否支持MCU升级等）进行配置，请在protocol.h内修改此配置；
2：移植此MCU_SDK，请查看protocol.c文件内的移植步骤,并正确完成移植。移植后，请完成数据下发处理、数据上报部分的代码，即可完成全部wifi功能。
 2.1：请在main函数中添加wifi_protocol_init()完成wifi协议初始化
 2.2：请直接在main函数的while(1){}中添加wifi_uart_service(),调用该函数不要加任何条件判断
 2.3：请在串口接收中断中调用uart_receive_input(value),串口数据由MCU_SDK处理,用户请勿再另行处理
 2.4：请将MCU串口发送函数填入uart_transmit_output()函数,MCU_SDK采用查询发送,用户须在发送函数内完成发送标志判断才能返回,请勿采用中断发送
 2.5：请在all_data_update()函数中实现可下发可上报数据及只上报数据填充
3：请勿在调试阶段开启编译器优化，否则MCU_SDK有可能运行不正常。

文件概览：
此MCU_SDK包括7个文件：
	（1）protocol.h和protocol.c是需要你修改的。protocol.h 和protocol.c文件内有详细修改说明，请仔细阅读。
	（2）wifi.h文件为总的.h文件，如需要调用wifi内部功能，请#include "wifi.h"。
	（3）system.c和system.h是wifi功能实现代码，用户请勿修改。
	（4）mcu_api.c和mcu_api.h内实现全部此用户需调用函数，用户请勿修改。