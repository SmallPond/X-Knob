# X-Knob

Last year (2021) I discovered [smart_knob(SK)](https://github.com/scottbez1/smartknob) while surfing the Internet, and I was very excited. But after learning more about it,, I was dissuaded by the out-of-stock "sky-high price" brushless motor😅. 

I recently discovered @45coll and @fywbcchong2's open source [Super_Dial](https://oshwhub.com/45coll/a2fff3c71f5d4de2b899c64b152d3da5), and it's time for me to play with it 🤗. The Super_Dial replaces the "expensive motor" used in the original smart_knob project and has a new shape that I personally find more aesthetically pleasing.

In addition, the author of smart_knob mentioned in the project that he wants to use lvgl to design UI in the future.

> migrate to LVGL, for better display rendering and easy support for menus, etc. Shouldn't require a full 240x240x24b framebuffer in memory, freeing some for wifi, etc.

Coincidentally, I have played X-TRACK before, and the UI is extremely silky. Therefore, this project will be a composite version of Smart Knob + X-Track UI. So I call it X-Knob.

Main hardware list:
- 3 PCBs: main control board + driver board + screen board
- 3205 Brushless Direct Current Motor(10 CNY)
- MCU: ESP32-S3 WROOM-1U-N16R8
- Screen: 240x240 round LCD ("GC9A01"), protected by 40.0 mm watch glass
- Magnetic encoder: MT6701CT


Features supported by X-KNOB:

  - X-TRACK UI framework(based on LVGL)
  - 7 Modes: Combinations of BOUND, DETENTS, ENDSTOPS, and more
  - smart-knob UI based on LVGL framework
  - Surface Dial
  - Press vibration feedback


### Demo video

[Demo video](https://www.bilibili.com/video/BV1N3411Z764/)


<a href="https://www.bilibili.com/video/BV1N3411Z764/">
    <img src="https://www.dingmos.com/usr/uploads/2023/01/661252453.png" width="480" />
</a>


### 3D CAD

<img src="docs/3d.gif" width="50%" height="50%" alt="3d" align=center />

> The picture above (3D CAD) is from [455555菌的视频](https://www.bilibili.com/video/BV1cG411c7QS/?vd_source=b234f79516f061233222cc1910681264)


# Acknowledgements

- [Hardware: Super Dial 电机旋钮屏](https://oshwhub.com/45coll/a2fff3c71f5d4de2b899c64b152d3da5)
- [Firmware: Super Dial 电机旋钮屏-gitee](https://gitee.com/coll45/super-dial-motor-knob-screen)
- [smart_knob](https://github.com/scottbez1/smartknob)
- [Super knob](https://gitee.com/wenzhengclub/super_knob)
- [X-TRACK](https://github.com/FASTSHIFT/X-TRACK)
- [Peak](https://github.com/peng-zhihui/Peak)