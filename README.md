# X-KNOB

去年（2021）网上冲浪发现了[smart_knob](https://github.com/scottbez1/smartknob)，顿时心动不已。但一番了解后，马上被缺货的“天价”无刷电机劝退。今年在刷 B 站时发现 @455555菌 和 虫二大佬（@fywbcchong2）开源的[Super Dial 电机旋钮屏](https://oshwhub.com/45coll/a2fff3c71f5d4de2b899c64b152d3da5)，我知道该是我动手玩一玩的时候了。

- [Super Dial 电机旋钮屏](https://oshwhub.com/45coll/a2fff3c71f5d4de2b899c64b152d3da5)替换了原 [smart_knob](https://github.com/scottbez1/smartknob) 的“昂贵电机”且采用了直筒设计，个人觉得更为美观。
- 另外，[smart_knob](https://github.com/scottbez1/smartknob) 作者在项目中提到未来想使用 lvgl 来设计 UI，之前我也玩过 X-TRACK，深深被其丝滑 UI 吸引。当时基于[稚晖君的Peak](https://github.com/peng-zhihui/Peak) 修改了部分硬件在 ESP32 上把玩了一番[Deck](https://github.com/SmallPond/Deck)。因此，本项目将会是 Smart Knob + Deck(X-Track UI) 的一个合成版。 

X-KNOB 支持的特性：
- [ ] X-TRACK 丝滑 UI
- [ ] 四种旋钮模式


# 制作记录

## LVGL


基于已有框架增添自己的图片：

- 图片生成C代码网站：[Online Image Converter BMP, JPG or PNG to C array or binary](https://lvgl.io/tools/imageconverter),选择 True color with alpha
- 将生成的C文件保存到`src\App\Resources\Image\`目录下
- 在 `src\App\Resources\ResourcePool.cpp` 增加一行声明
- Icons 素材网站：[iconoir](https://iconoir.com/)
  - Size: 42x42
  - Stroke Width: 1.6
  - Color: white

# 参考项目

- [Hardware: Super Dial 电机旋钮屏](https://oshwhub.com/45coll/a2fff3c71f5d4de2b899c64b152d3da5)
- [Firmware: Super Dial 电机旋钮屏-gitee](https://gitee.com/coll45/super-dial-motor-knob-screen)
- [smart_knob](https://github.com/scottbez1/smartknob)
- [Super knob](https://gitee.com/wenzhengclub/super_knob)
- [X-TRACK](https://github.com/FASTSHIFT/X-TRACK)