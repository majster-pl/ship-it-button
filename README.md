## 🚀 Ship It! Button

A 3D-printed USB device designed for dramatic deployments.

## Ship It Box
![Ship It Box](https://github.com/user-attachments/assets/30700b1f-ec0a-4ca5-b91c-f6386e393e66)

## Inside (ESP32-S3)
![Inside](https://github.com/user-attachments/assets/86242750-ff22-465e-a882-d4e9c3609d0f)

## Back Cover
![Back Cover](https://github.com/user-attachments/assets/3225b654-8a4f-4e9f-9bf7-43d51b969906)

## Start up animation
https://github.com/user-attachments/assets/f34a8139-bcb4-40e1-bdf6-34d7109cfc78

## On button pressed animation
https://github.com/user-attachments/assets/eb8da8be-4c95-493a-8b3f-b23c512ba0db

## Schematic
<img width="601" height="539" alt="image" src="https://github.com/user-attachments/assets/c3218229-b406-46ed-9233-f086b9203eab" />


### ✨ Features



- **Black Box Design**: A matte 3D-printed enclosure with a centered button and hidden RGB LEDs around it.
- **"Ship it!" Engraving**: Stylish engraving right under the button for extra flair.
- **USB Plug & Play**: Acts as a keyboard when connected via USB.

### 🔧 🚥

| Action             | Result                                                                 |
|--------------------|------------------------------------------------------------------------|
| Plug in USB        | LEDs perform a **red breathing animation**, then fade to black (standby) 🚥 |
| Press the button   | Triggers the **OBC launch animation** (purple LED chase effect), then sends `:ship: ship it! :rocket:` (**🚢 ship it! 🚀**) as keyboard input followed by `Enter` - ideal for Slack message ✅ |
| Standby Mode       | LEDs stay off until the next button press or randomly every 1-20min red breathing animation with random number of breaths will kick in 🙂 |

### 🌀 Animations

- **Breathing Animation**: Gentle red glow mimicking breathing, plays once at start or in intervals.
- **OBC Animation**: Purple LED "chase" with acceleration, cruise, and deceleration phases.
- **Keyboard Input**: Automatically sends the message using USB HID after the animation.

### 🔩 Hardware

- **Microcontroller**: ESP32-S3
- **LEDs**: 8 × WS2812B RGB LEDs (You can get premade rings by I've made this from scratch by cutting strip into pieces and soldering together 🤦)
- **Button**: Red anodized aluminum push button (momentary) I bought few on [AliExpress](https://a.aliexpress.com/_EvYcgQm)
- **Power/Data**: Standard USB cable (USB HID capable)
- **Enclosure**: Custom 3D-printed case with hidden LEDs and center-mounted button
- **Other Materials**:  
  - *Hot glue* — to fill the gaps around the button  
  - *White silicone* — to fill the "ship it!" text  
  - *M2.5 screws & inserts* — to secure the back cover  
  - *12mm × 2mm rubber pads* — to cover the screws and reduce sliding

### 🧱 3D Model

- Designed using [Tinkercad](https://www.tinkercad.com/)
- `.stl` files included in this repository
- Printed on a **Creality Ender 3 V3 SE** 3D printer using PLA

### 🧠 Built With

- [FastLED](https://github.com/FastLED/FastLED) – RGB LED control
- USB HID keyboard emulation
- Randomized animation patterns for variety

### 🛠️ Code

- Written in **C/C++** for microcontrollers (Arduino-style).
- Developed with some help from **[ChatGPT](https://openai.com/chatgpt)** for animation logic and optimization.
