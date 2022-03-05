# **Machine Vision Foam Dart Turret**

### **UoG Advanced Computer Engineering Project 2021/22**
<br>

## **About the Project**

This project has been developed by the contributors below as a submission of coursework for the University of Greenwich Advanced Computer Engineering Module, academic year 2021/22.

The Machine Vision Foam Dart Turret is a fully automated object targetting system designed to identify, target, and shoot (with a foam dart to two) a person not recognised by the machine vision. It features a 12 dart magazine, 2 axis movement, a Jetson Nano controlled vision system, and two interfaces (one for LAN based web control and video feedback, and one for automatic downloading of this GitHub repo).

Mechanical parts of this project are mostly custom using standard industry parts for difficult to manufacture parts such as stepper motors and bearings. The software is written mostly from scratch, using part libraries where sensible, particularily for the arduino interfacing with hardware, and the Jetson managing its machine vision identification.

![System block diagram](https://raw.githubusercontent.com/vinthund/UoGACE2022/ad4646d/System%20Documentation/System_Block_Diagram.png)
*System Concept Block Diagram*

## **Contributors**

- [Andrew Dean - ad4646d](https://www.github.com/ad4646d)
- [Chris Halsall - ch6941r -](https://www.github.com/ch6941r)  [AfterEarthLtd](https://www.github.com/AfterEarthLTD)
- [vinthund](https://www.github.com/vinthund)
- [ht5640k](https://www.github.com/ht5640k)
- [JoelSmalls](https://www.github.com/JoelSmalls)
- [kamyar123](https://www.github.com/kamyar123)
- [rowBoat](https://www.github.com/rowboat)
<br>


## **Firing Mechanism Mechanical Design** - [Chris Halsall - ch6941r](https://www.github.com/ch6941r)

The firing mechanism contains a few pre-made parts including a small stepper motor, the firing motors and barrel, a servo, and a bearing. All other parts have been designed in Fusion 360 and printed in orange PLA on an Ender 3 FDM 3D printer. It features a 12 dart rotary magazine driven by the stepper motor from one end and supported by a bearing at the other end. Through the bearing is mounted a magnet and a magnetic position sensor so that the control electronics can measure the angle of the magazine. A servo is mounted in the base and, with a custom arm, is capable of pushing a dart from the magazine into the firing motors and thus propelling it out of the barrel. A mounting plate is attached to the front of the design to securely mount the camera for machine vision. Finally, a hole and hex slot has been designed in to allow for easy mounting to the pan/tilt mehcanism.
<br>
<br>

## **Pan-Tilt Mechanism Mechanical Design** - [Andrew Dean - ad4646d](https://www.github.com/ad4646d)

The [pan-tilt mechanism](https://github.com/vinthund/UoGACE2022/tree/main/Pan-TiltAssembly) is a bespoke 3D printed mechanism that was designed in tandem with bespoke the [firing mechanism](https://github.com/vinthund/UoGACE2022/tree/ad4646d/3D%20Designs/Firing%20Mech) using AutoDesk Fusion 360.

***{insert up-to-date photo}***

Herringbone gears were chosen for both pan and tilt axes for a number of reasons:
* They feature minimal backlash compared to 3D printed spur gears
* They are quieter in operation compared to 3D printed spur gears
* More torque can be transmitted through them vs 3D printed spur gears\

A gearing ratio of 4:1 was chosen to increase the torque output of the NEMA17 steppers and to give greater step-angle resolution... ***add further calculations for speed and torque output***.

All bespoke 3D printed parts for the pan-tilt mechanism were printed in black PLA using an Anycubic i3 Mega.

NEMA17 stepper motors were chosen as they provided adequate torque output to move both axes, they also feature a step angle of 1.8˚.
* A [NEMA17x34](https://www.omc-stepperonline.com/nema-17-bipolar-1-8deg-26ncm-36-8oz-in-0-4a-12v-42x42x34mm-4-wires.html?search=17hs13) was chosen for the tilt axis
* A [NEMA17x39](https://www.omc-stepperonline.com/nema-17-bipolar-45ncm-6374ozin-15a-42x42x39mm-4-wires-w--1m-pin-connector.html?search=17HS15-1504S-X1&description=true) was chosen for the pan axis

[A4988 stepper drivers](https://www.amazon.co.uk/gp/product/B07MXXL2KW) were selected for a number of reasons:
* They can drive bi-polar stepper motors up to 2A/phase
* They only require two control pins from the microcontroller to control steps and direction
* They are still readily available amidst the current global component shortage

A combination of [small neodymium magnets](https://www.first4magnets.com/circular-disc-rod-c34/2mm-dia-x-2mm-thick-n42sh-neodymium-magnet-0-15kg-pull-p3327#ps_0_3379|ps_1_16690) and [hall effect sensors](https://www.amazon.co.uk/gp/product/B08QCRYXPK) are used to both 'home' each axis and to limit their movement, these were chosen over limit switches as they simplified mechanical assembly and design.

An [off-the-shelf electronics enclosure](https://www.screwfix.com/p/schneider-electric-ip66-weatherproof-outdoor-enclosure-164-x-105-x-192mm/) was chosen to be the base of the system, this particular enclosure was chosen as it would provide adequate space to house all system components.

A [lazy susan bearing](https://www.stilesandbates.co.uk/75mm-square-lazy-susan-bearing-1709.php) was chosen for the pan axis as it allowed for a convenient way to manage cables between the combined pan-tilt/firing mech and their control electronics in the enclosure below.

The full parts list for the pan-tilt mechanism can be found [here](https://github.com/vinthund/UoGACE2022/blob/ad4646d/Pan-TiltAssembly/Documentation/Pan-Tilt%20Head%20Parts%20List.pdf).\
STEP files for the pan-tilt mechanism can be found [here](https://github.com/vinthund/UoGACE2022/tree/main/Pan-TiltAssembly/CAD_Files).

<br>


## **Firing Mechanism Communications** - [JoelSmalls](https://www.github.com/JoelSmalls)

**Items that were used**
* Raspberry Pi 3 Model B+
* Arduino Nano 
* Four Male to Female Jumper wires
* Four Male to Male Jumper wires
* Logic Level Shifter: Bi-Directional Module (Between 3V3 and 5V)

In order to make the control of the Firing Mechanism possible through communication between the Jetson Nano and the Arduino (shown on above diagram), a communications protocol had to be utilised. Types of communications protocols, such as I2C(Inter-Integrated Circuit), USB(Universal Serial Bus) Serial and SPI(Serial Peripheral Interface) were each investigated to see which would work efficiently when communicating between a Jetson Nano and an Arduino. As a Jetson Nano was not in possession, a Raspberry Pi was instead used to just simulate how certain communications protocols would work with each other.

The first iteration of a communications code between the Arduino and the Raspberry Pi was through USB Serial, in which a USB Cable was required to connect the Arduino straight to the Raspberry Pi. This was a simple type of communication which only required an Arduino USB Cable (dependent on board type) which could freely be moved around due to the length of the cable. However, we  had later anticipated that it would be necessary for a keyboard and mouse USB input to be left available on the Jetson Nano in case of errors. Thus, this idea was scrapped and was moved on to other types of communications protocols.

Other protocols such as I2C and SPI were explored. According to research, when comparing between I2C and SPI, SPI was a faster option; this did not mean it could be instantly utilised though, as not enough examples of communication between an Arduino and a Jetson/Pi could be found online to make use of SPI. Furthermore, it required more wires to be connected to the Nano through a logic level shifter. Due to this, I2C was settled with. 

**Setup**
The Arduino's SDA (Serial Data) was connected to the High Voltage(HV) pin 1, SCL (Serial Clock) at HV2, Arduino 5V at the HV input pin and Arduino Ground to the shifter's Ground. These were connected to the High Voltage due to the Arduino operating at an output of 5V.

The Raspberry Pi's SDA was connected on Low Voltage pin 1, opposite that of the Arduino's SDA connection; SCL was connected to LV2, 3V3 at LV input and Pi Ground to the shifter's ground. The Pi was connected to the Low Voltage side due to the Pi's output voltage being 3V3.

To fully use I2C, the Arduino and the Raspberry Pi's Python 3's libraries were used, being ***wire*** and ***smbus2*** libraries respectively.
<br>

## **Pan/Tilt Mechanism Communicatons** - [ht5640k](https://www.github.com/ht5640k)

***Add your summary of work here and delete this comment.***

<br>

## **Machine Vision Target Aquisition** - [vinthund](https://www.github.com/vinthund)

***Add your summary of work here and delete this comment.***

<br>

## **Web Server and Website** - [rowBoat](https://www.github.com/rowboat)

The website consist:
* The Login page - the first page that the user will see
* The Home page - once the user has logged in this is the next page that they will be greeted by. this page will consist of the window to the view the live video feed from the turret and button the turret. 
* The Device page - this page can be accessed by the corresponding link in the navigation bar at the top of the screen. This page is just for show but could be used to add more devices.
* The Help page
* A link to the github repository
All pages besides the login page have a navigation bar to navigate to the other pages on the website, the sign out button, and the link to the github repository.

Webserver
OS: Ubuntu
Wireless or wired: wireless
Created to:
* Store learning files for machine learning
* Store login information
* Store and Retrieve video feed and bullet count from the camera and jetson so it could be displayed on the website and app
* Stores and sends button click to the jetson so when turret reacted to the corresponding button input.







<br>

## **Android App** - [kamyar123](https://www.github.com/kamyar123)

***Add your summary of work here and delete this comment.***

<br>

## **Project Management** - [Andrew Dean - ad4646d](https://www.github.com/ad4646d) & [Chris Halsall - ch6941r](https://www.github.com/ch6941r)

***Add your summary of work here and delete this comment.***

<br>

## **GitHub Management** - [vinthund](https://www.github.com/vinthund)

***Add your summary of work here and delete this comment.***

<br>

# **License**

Due to the sensitive nature of projectile firing technology, much thought has gone into the ethics of releasing a publically available repository with details of how the project was created. Please note that the content contained within this repository should be used solely for EDUCSTION purposes. For full details please see the [License](LICENSE) document found in the root of this repository. This license has been based upon the MIT license.