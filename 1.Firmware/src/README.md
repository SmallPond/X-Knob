# 一些零碎的记录

lvgl/lvgl@^8.3.4

`Attempting MQTT connection` Error: 

```shell
E (28476) task_wdt: Task watchdog got triggered. The following tasks did not reset the watchdog in time:
E (28476) task_wdt:  - IDLE (CPU 0)
E (28476) task_wdt: Tasks currently running:
E (28476) task_wdt: CPU 0: LvglThread
E (28476) task_wdt: CPU 1: loopTask
E (28476) task_wdt: Aborting.
```


smart_knob 是一个有意思的、惊人的项目，我从 2021 年开始就关注到了，作者真的是太棒了。看到作者和项目中的 issues 中提到期望切换为 LVGL 来设计 UI、增加菜单、增加蓝牙和 WiFi 支持等需求，觉得非常有意思。所有我都尝试实现了一下并开源了，我称它为 X-Knob，主要特性如下：

- 基于 LVGL 重新实现了 smart_knob 的 UI
- 菜单功能
- 蓝牙：实现了兼容 Surface Dial 的功能，可控制 PC
- WiFi：基于 MQTT 实现了接入 Home Assistant 控制智能家居
- 电池供电：并且在硬件上支持 UPS（不间断电源）
- 使用货量充足的无刷电机和采用新的结构设计


不过很遗憾的是，我无法购买到 smart_knob 项目使用到的无刷电机，所以 X-Knob 没办法对 smart_knob 的硬件进行适配。同时，框架（LVGL）上的不一致，我无法将新增的代码合并到 smart_knob 主分支。

最后十分感谢 smart_konb 作者的开源，让我从中收获了乐趣，学习到了很多优秀的代码，认识了很多优秀的小伙伴。