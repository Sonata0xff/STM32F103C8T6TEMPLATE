该文档说明了ESC固件的设计与使用：
1.使用esc的API时，应开启对应宏开关：
    (1)NSCP_API_EN
    (2)ESC_API_EN
2.如果需要运行esc固件测试代码，可开启宏开关：
    (1)ESC_TEST

3.ESC状态机：主循环负责
启动 -> 初始化 -> 静止态 -> 低速变速 -> 低速稳定 -> 稳定变速 (PID控制)
                ^                            ^
                |                            |
                |                            |
                |                            |
                |----------滤波---------------| 

TIM1 CH3:PWM -- u: HHLLLL ~u: LLLHHL
TIM1 CH2:PWM -- v: LLHHLL ~v: HLLLLH
TIM1 CH1:PWM -- w LLLLHH ~w LHHLLL
TIM1 CH4:timer base

通信：中断负责
nscp -> 解码 -> 油门值