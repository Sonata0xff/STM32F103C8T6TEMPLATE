该文档主要用于IIC相关API说明：IIC模块仍在编写中...
1.IIC模块API使用硬件进行驱动。
2.使用该模块需要在bsp_config.h中启用宏IIC_API_EN
3.对于承载该代码的stm32单片机而言，IIC1将作为master发送者，IIC2将作为slave接收
4.IIC模块目前将主要支持“一主多从”通信方式。