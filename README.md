# ⚡ STM32F103 Bare-Metal GA25 Motor Control

This project implements a DC motor control system using STM32F103C8T6 and a GA25 encoder motor.  
The system is developed completely in bare-metal Embedded C without using HAL libraries.

The project includes custom peripheral drivers, PWM motor driving, encoder processing, PID control and LCD interface.

---

## 📌 Objectives

- Control GA25 DC motor speed using PWM
- Read quadrature encoder using Timer Encoder Mode
- Implement PID speed control
- Display system information on LCD1602
- Handle button input using EXTI interrupt
- Practice register-level STM32 programming

---

## 🛠️ Technologies Used

| Component        | Tool / Technology          |
|------------------|----------------------------|
| Microcontroller  | STM32F103C8T6             |
| Programming      | Embedded C (Bare-metal)   |
| IDE              | STM32CubeIDE              |
| Motor            | GA25 Encoder DC Motor     |
| Display          | LCD1602 (4-bit mode)      |
| Control Method   | PID Control               |

---

## 📁 Project Structure

| Folder/File      | Description                                      |
|------------------|--------------------------------------------------|
| `Drivers/Inc/`   | Peripheral driver headers                        |
| `Drivers/Src/`   | Peripheral driver source files                   |
| `Src/`           | Application source code                          |
| `Startup/`       | Startup assembly file                            |
| `.gitignore`     | Git ignored files                                |
| `README.md`      | Project overview and documentation               |

---

## 🔧 Drivers Implemented

- GPIO Driver
- Timer Driver
- EXTI Driver
- LCD1602 Driver
- PID Controller

---

## ⚙️ Functionalities

- PWM motor speed control
- Encoder pulse counting
- Direction detection
- PID-based speed stabilization
- LCD real-time display
- Button interrupt handling with debounce

---

## 📸 Demo
- [Video Demo Speed Control](https://drive.google.com/file/d/1VlM4ZDelYBNgDN_UaBSSuNuaAUNoffds/view?usp=drive_link)
- [Video Demo Position Control](https://drive.google.com/file/d/1hu8l0_IUIsAUD1e6Fb_bbTZblmaXqizu/view?usp=drive_link)

---

## 🚀 Future Improvements

- SysTick scheduler
- UART debugging interface
- FreeRTOS integration
- Better control algorithm tuning

---

## 📄 License

This project is licensed under the MIT License.

---

## 📬 Contact

- Name: Lê Duy Ngọ
- Email: leduyngo@outlook.com
- GitHub: [ngoleduy](https://github.com/ngoduyle)