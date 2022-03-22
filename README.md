![Title Image](https://github.com/vinthund/UoGACE2022/blob/ad4646d/Pan-TiltAssembly/Documentation/Assets/Title_Image.jpg?raw=true)
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
- [ht5640k - Harshitha Thimmegowda](https://www.github.com/ht5640k)
- [JoelSmalls - Joel A. Okanta](https://www.github.com/JoelSmalls)
- [kamyar123](https://www.github.com/kamyar123)
- [rowBoat](https://www.github.com/rowboat)
<br>

 ## **Viva Video**

[![Viva_Video_Thumbnail](https://github.com/vinthund/UoGACE2022/blob/ad4646d/Project%20Management%20Documentation/Viva_Video_Thumbnail.png)](https://youtu.be/HJBah1AWvD8)

 ## **Firing Mechanism Mechanical Design** - [Chris Halsall - ch6941r](https://www.github.com/ch6941r)

The firing mechanism contains a few pre-made parts including a small stepper motor, the firing motors and barrel, a servo, and a bearing. All other parts have been designed in Fusion 360 and printed in orange PLA on an Ender 3 FDM 3D printer. It features a 12 dart rotary magazine driven by the stepper motor from one end and supported by a bearing at the other end. Through the bearing is mounted a magnet and a magnetic position sensor so that the control electronics can measure the angle of the magazine. A servo is mounted in the base and, with a custom arm, is capable of pushing a dart from the magazine into the firing motors and thus propelling it out of the barrel. A mounting plate is attached to the front of the design to securely mount the camera for machine vision. Finally, a hole and hex slot has been designed in to allow for easy mounting to the pan/tilt mehcanism.

The control electoronics includes an A4988 stepper driver, current limited to 0.5A per phase, an I2C connection for the magnetic position sensor, and a MOSFET (and protection circuitry) for driving the firing motors. These all interface with the joint Arduino Nano that controls all aspects of the moveable mech.
<br>
<br>

## **Pan-Tilt Mechanism Mechanical Design** - [Andrew Dean - ad4646d](https://www.github.com/ad4646d)

The [pan-tilt mechanism](https://github.com/vinthund/UoGACE2022/tree/main/Pan-TiltAssembly) is a bespoke 3D printed mechanism that was designed in tandem with bespoke the [firing mechanism](https://github.com/vinthund/UoGACE2022/tree/ad4646d/3D%20Designs/Firing%20Mech) using AutoDesk Fusion 360.

Herringbone gears were chosen for both pan and tilt axes for a number of reasons:
* They feature minimal backlash compared to 3D printed spur gears
* They are quieter in operation compared to 3D printed spur gears
* More torque can be transmitted through them vs 3D printed spur gears 

![Herringbone_Gears](https://raw.githubusercontent.com/vinthund/UoGACE2022/main/Pan-TiltAssembly/Documentation/Assets/Herringbone_Gears.jpg)

A plug-in for Fusion 360 called [GF Gear Generator](https://apps.autodesk.com/FUSION/en/Detail/Index?id=1236778940008086660&appLang=en&os=Win64) was used to generate the herringbone gears. \

![GF_Gear_Screenshot](https://raw.githubusercontent.com/vinthund/UoGACE2022/ad4646d/Pan-TiltAssembly/Documentation/Assets/GF_Gear_Generator_Screenshot_Example.png) \
This produces a finished gear as a body, which can then be adapted 

The following parameters were found to produce gears with teeth that printed well and engaged with each other nicely.
> * Module - 1mm
> * Pressure angle - 14.5˚
> * Helix angle - 40˚ 

The number of teeth can be varied but the other parameters must remain the same for all meshing gears; the number of teeth and the module define the diameter of the gear:
> Diameter of gear = module x number of teeth

A gearing ratio of 4:1 was chosen to increase the torque output of the NEMA17 steppers and to give greater step-angle resolution.

The centre distance between the pinion gear and the output gear is calculated as follows:
> Centre distance = (Module*(No. teeth on pinion + No. teeth on output gear))/2

[Flange bearings](https://www.amazon.co.uk/gp/product/B07SHSLTKD/ref=ppx_yo_dt_b_asin_title_o03_s00?ie=UTF8&psc=1) are used to attach the tilt axis to the assembly, highlighted in the cross-section below.
![Flange_Bearings](https://raw.githubusercontent.com/vinthund/UoGACE2022/ad4646d/Pan-TiltAssembly/Documentation/Assets/Flange_Bearings.png) 
These flange bearings interference fit into the firing mech support bracket (pink) and tilt axis gear (yellow), removing the need for glue and simplifying assembly.

Locator pins have been designed into the bases of the two supporting supporting, these help to ensure that both supporting brackets are installed square to eachother, these are highlighted in the image below.
![Locator_pins](https://raw.githubusercontent.com/vinthund/UoGACE2022/ad4646d/Pan-TiltAssembly/Documentation/Assets/Support_Bracket_Locator_Pins.png)

All bespoke 3D printed parts for the pan-tilt mechanism were printed in black PLA using an Anycubic i3 Mega.

An [off-the-shelf electronics enclosure](https://www.screwfix.com/p/schneider-electric-ip66-weatherproof-outdoor-enclosure-164-x-105-x-192mm/) was chosen to be the base of the system, this particular enclosure was chosen as it would provide adequate space to house all system components.

A [lazy susan bearing](https://www.stilesandbates.co.uk/75mm-square-lazy-susan-bearing-1709.php) was chosen for the pan axis as it allowed for a convenient way to manage cables between the combined pan-tilt/firing mech and their control electronics in the enclosure below. \
The lazy susan did introduce a lot of audible noise to the system, this is due to a combination of the fact it is an unlubricated bearing and that the enclosure it is attached to was acting like a voice box, amplifying its vibrations. 
A foam pad was added between the bearing and the enclosure and the bearing lubricated with silicone grease, this helped somewhat with the audible noise. Half-step microstepping was also used to reduce the audible noise.

The full parts list for the pan-tilt mechanism can be found [here](https://github.com/vinthund/UoGACE2022/blob/ad4646d/Pan-TiltAssembly/Documentation/Pan-Tilt%20Head%20Parts%20List.pdf).\
STEP files for the pan-tilt mechanism can be found [here](https://github.com/vinthund/UoGACE2022/tree/main/Pan-TiltAssembly/CAD_Files).

<br>


## **Hardware for Pan/Tilt and Firing Mechanisms** - [Chris Halsall - ch6941r](https://www.github.com/ch6941r) & [Andrew Dean - ad4646d](https://www.github.com/ad4646d)
**Pan/Tilt Mechanism Hardware - [Andrew Dean - ad4646d](https://www.github.com/ad4646d)**

NEMA17 stepper motors were chosen for the Pan/Tilt axis as they provided adequate torque output to move both axes, they also feature a step angle of 1.8
* A [NEMA17x34](https://www.omc-stepperonline.com/nema-17-bipolar-1-8deg-26ncm-36-8oz-in-0-4a-12v-42x42x34mm-4-wires.html?search=17hs13) was chosen for the tilt axis
* A [NEMA17x39](https://www.omc-stepperonline.com/nema-17-bipolar-45ncm-6374ozin-15a-42x42x39mm-4-wires-w--1m-pin-connector.html?search=17HS15-1504S-X1&description=true) was chosen for the pan axis

[A4988 stepper drivers](https://www.amazon.co.uk/gp/product/B07MXXL2KW) were selected for a number of reasons:
* They can drive bi-polar stepper motors up to 2A/phase
* They only require two control pins from the microcontroller to control steps and direction
* They can support microstepping
* They are still readily available amidst the current global component shortage

A combination of [small neodymium magnets](https://www.first4magnets.com/circular-disc-rod-c34/2mm-dia-x-2mm-thick-n42sh-neodymium-magnet-0-15kg-pull-p3327#ps_0_3379|ps_1_16690) and [hall effect sensors](https://www.amazon.co.uk/gp/product/B08QCRYXPK) are used to both 'home' each axis and to and to limit the movement on the tilt axis, these were chosen over limit switches as they simplified mechanical assembly and design.

The hardware was connected up on a breadboard for testing ahead of the design of a veroboard prototype control board.

**Firing Mechanism Hardware - [Chris Halsall - ch6941r](https://www.github.com/ch6941r)**

## **Movement Control Circuitry** - [Chris Halsall - ch6941r](https://www.github.com/ch6941r)

A custom circuit board has been designed and built to accomodate the requirements for electrical control. A limitations prevented the use of a custom designed PCB including time, budget, and that continuous development was required therefore dynamically changing the structure of the circuit. A stripboard was chosen as it is relatively versatile and can accomodate throughhole components with ease. 

The bottom of the board is dedicated to power rails, 0V and 12V from an external PSU, and 5V generated by an L7805 linear regulator with the 12V input. All other board components are spread evenly to attempt to seperate wiring connectors as much as possible. The component parts are 
* Arduino Nano
* 3x A4988 stepper motor drivers
* 6x Hall effect sensor headers
* 1x Servo header
* 1x I2C header
* 1x MOSFET, flyback diode, and associated motor header

Some additional capacitors and a diode help to reduce input shock current (and therefore prevent brownout) and reverse power connection damage.

![Control Circuitry](https://github.com/ch6941r/UoGACE2022/blob/main_ch6941r/Schematics/circuitry_photo.jpg)

<br>

## **Firing Mechanism Communications** - [JoelSmalls - Joel A. Okanta](https://www.github.com/JoelSmalls)

**Items that were used**
* Raspberry Pi 3 Model B+
* Arduino Nano 
* Four Male to Female Jumper wires
* Four Male to Male Jumper wires
* Logic Level Shifter: Bi-Directional Module (Between 3V3 and 5V)
* Ethernet Cable: To allow for ssh into Pi to script in Python.

In order to make the control of the Firing Mechanism possible through communication between the Jetson Nano and the Arduino (shown on above diagram), a communications protocol had to be utilised. Types of communications protocols, such as I2C(Inter-Integrated Circuit), USB(Universal Serial Bus) Serial and SPI(Serial Peripheral Interface) were each investigated to see which would work efficiently when communicating between a Jetson Nano and an Arduino. As a Jetson Nano was not in possession, a Raspberry Pi was instead used to just simulate how certain communications protocols would work with each other.

The first iteration of a communications code between the Arduino and the Raspberry Pi was through USB Serial, in which a USB Cable was required to connect the Arduino straight to the Raspberry Pi. This was a simple type of communication which only required an Arduino USB Cable (dependent on board type) which could freely be moved around due to the length of the cable. However, we  had later anticipated that it would be necessary for a keyboard and mouse USB input to be left available on the Jetson Nano in case of errors. Thus, this idea was scrapped and was moved on to other types of communications protocols.

Other protocols such as I2C and SPI were explored. According to research, when comparing between I2C and SPI, SPI was a faster option; this did not mean it could be instantly utilised though, as not enough examples of communication between an Arduino and a Jetson/Pi could be found online to make use of SPI. Furthermore, it required more wires to be connected to the Nano through a logic level shifter. Due to this, I2C was settled with. 

This, was initially the case, on utilising I2C Communication to transmit data between the Raspberry Pi and the Arduino. Unfortunately though, as the gun firing mech used a sensor which required the Arduino to be a master over I2C, this idea of using I2C could not be applied. Providentially, there was another type of communications I had found through research, named Serial UART. Serial UART, according to research, is a "hardware communication protocol" that uses "asynchronous serial communication", meaning that unlike I2C, it does not have to synchronise to a clock signal. UART was made possible with a library named **SoftwareSerial** on the Arduino, and a library named **Serial** on Python were used to replace the original I2C system implemented.

**Setup**

[I2C]
The Arduino's SDA (Serial Data) was connected to the High Voltage(HV) pin 1, SCL (Serial Clock) at HV2, Arduino 5V at the HV input pin and Arduino Ground to the shifter's Ground. These were connected to the High Voltage due to the Arduino operating at an output of 5V.

The Raspberry Pi's SDA was connected on Low Voltage pin 1, opposite that of the Arduino's SDA connection; SCL was connected to LV2, 3V3 at LV input and Pi Ground to the shifter's ground. The Pi was connected to the Low Voltage side due to the Pi's output voltage being 3V3.

To fully use I2C, the Arduino and the Raspberry Pi's Python 3's libraries were used, being ***wire*** and ***smbus2*** libraries respectively.

[UART]
SoftwareSerial's library allows for the creation of Rx Tx pins over any digital pins, so 2 and 3 were selected in this case. Pin 2, Rx, was connected to HV1 of the logic level shifter; Pin 3 Tx was connected to HV2. Arduino 5V was connected to the HV of the Level Shifter, along with Arduino Ground connected to the component's ground. Similar steps were taken on the opposite side, with the Raspberry Pi's Tx on pin 8 connected on the opposite of the Arduino's Rx. Same followed for the Pi's pin 10 Rx connected opposite the Arduino's Tx on the shifter. The Pi's 3V3 Power was connected to LV, along with the Pi's ground connected to the component's ground.

![I22ImagePiArduino](https://user-images.githubusercontent.com/48869133/157861137-2b457d97-22b6-4f79-860c-77544ec5475e.jpg)

![UARTExample](https://user-images.githubusercontent.com/48869133/158718321-f61861dc-86f8-4079-bac4-c2d13c3c3fba.jpg)

<br>

##  I2C Communicatons , Jetson Nano  - [ht5640k- Harshitha Thimmegowda ](https://www.github.com/ht5640k)

 **Jetson NANO** 

Items used

* Jetson nano
* HDMI cable

To get the Jetson Nano started:
* Basic files were installed.
* Stow was used which is a symlink farm manager which takes distinct sets of software and/or data located in separate directories on the filesystem, and makes them all appear to be installed in a single directory tree.
* Aliases were created to make abrieviation to soomthen the further procedures.
* Created /etc/network/interfaces  

**I2C communications**

Items used

* Raspberry Pi 3 Model B+
* Arduino Nano 
* Four Male to Female Jumper wires
* Four Male to Male Jumper wires
* Logic Level Shifter: Bi-Directional Module (Between 3V3 and 5V)
* Ethernet Cable: To allow for ssh into Pi to script in Python.

Above mentioned items were used to setup a to make an effictive communication to get the hardware working The Arduino's SDA (Serial Data) was connected to the High Voltage(HV) pin 1, SCL (Serial Clock) at HV2, Arduino 5V at the HV input pin and Arduino Ground to the shifter's Ground. These were connected to the High Voltage due to the Arduino operating at an output of 5V.
The Raspberry Pi's SDA was connected on Low Voltage pin 1, opposite that of the Arduino's SDA connection; SCL was connected to LV2, 3V3 at LV input and Pi Ground to the shifter's ground. The Pi was connected to the Low Voltage side due to the Pi's output voltage being 3V3 to get the RapberryPi and Arduino communicating.
![20220315_164032](https://user-images.githubusercontent.com/92383132/159386133-138882af-c237-4338-9208-c3437ec3124e.jpg)


**Shortening of ethernet cable**

Items used

* Ethernet cable
* Crimping tool
* Cable Connectors

Above mentioned items were used to shorten a ethernet cable to connect RaspberryPi and Jetson nano, It was done by cutting the available ethernet cable to the required length using a crimping tool and then pushing the wires into the cable connecter. Shortening was required in order to meet the hardware modeling requirement and to adjust the length of the ethernet cable connection very small and convenient.
![Screenshot_20220318-221013_Snapchat](https://user-images.githubusercontent.com/92383132/159386021-ea0ecef6-c11d-4df2-83c2-d72cdad20153.jpg)





<br>

## **Machine Vision Target Aquisition** - [vinthund](https://www.github.com/vinthund)

Motion Detection in Python Version 1.0.0 
Written By Sian Pugh, sp3045k@gre.ac.uk, fleetfootgreyhound@gmail.com, vinthund @ github

Simple backend motion detector written in Python. Intended for use on a Jetson Nano 2gb to communicate with an Arduino system.

Part of a larger project used to control an autonomous nerf sentry - https://github.com/vinthund/UoGACE2022

Currently uses a subtractive method; it will take the first frame of input and store it as the "background", then compare all following frames to this. It has a threshold mechanism in place to ensure that small differences such as shadows are not seen as motion. When motion is detected using this subtractive method, a countour of the motion is drawn, as well as a retangle constructed of the area around it. This rectangle is used for specific targetting; the center is found and then compared to the center of the screen, with the results being sent to the arduino control unit using i2c.

Dependencies: This program requires the numpy and opencv packages to be installed to function. https://numpy.org/ https://opencv.org/

<br>

## **Web Server and Website** - [rowBoat](https://www.github.com/rowboat)

The website consist:
Was constructed using the php files in order to connection and retrieve data from the mysql server.
* The Login page - the first page that the user will see
* The Home page - once the user has logged in this is the next page that they will be greeted by. this page will consist of the window to the view the live video feed from the turret, a reading of how much bullets are left anda button for manual control. 
* A link to the github repository

Webserver
OS: Ubuntu
Wireless or wired: wireless
Configured in a lemp stack using nginx, mysql and php
Created to:
* Host the website
* Store login information
* Store and Retrieve video feed and bullet count from the camera and jetson so it could be displayed on the website and app
* Stores and sends button click to the jetson so when turret reacted to the corresponding button input.







<br>

## **Machine Vision Foam Dart Turret App** - [Kamyar Homampour-kamyar123](https://www.github.com/kamyar123)

This app has few pages which are:

* Login page

* Register page

![image](https://user-images.githubusercontent.com/91549986/158219660-c7ccc79a-a6d6-48ee-ba87-26c2d850fbc7.png)

* Main page

* Live page

Firebase has been used as a database for the login and register page to save all users' information and account date of the last login and created the account.
![image](https://user-images.githubusercontent.com/91549986/158220525-f49d3015-efca-4c4c-9624-0ee2122067cf.png)

The main page was supposed to count the bullets, but due to circumstances, it hasn't been completed.

The live page was supposed to show the live stream from the jetson nano camera, but now it shows the test stream for rtsp link to test  if the live stream is actually working.
(https://www.wowza.com/developer/rtsp-stream-test)

<br>

## **Project Management** - [Andrew Dean - ad4646d](https://www.github.com/ad4646d) & [Chris Halsall - ch6941r](https://www.github.com/ch6941r)

When the project was announced, we asked all team members to detail their prior skills and experience on a [shared word document](https://github.com/vinthund/UoGACE2022/blob/main/Project%20Management%20Documentation/ELEE1119%20Team%20List%20(AD%20created).docx).

This initial skillset list was used to inform the project specification and division of tasks, which can be found in [this word document](https://github.com/vinthund/UoGACE2022/blob/main/Project%20Management%20Documentation/Project%20Specification%20and%20Division%20of%20Work%20(rev3).docx).

After everyone had agreed to the contents of the specification and the division of work, a [Gantt chart was created in MS Project](https://github.com/vinthund/UoGACE2022/blob/main/Project%20Management%20Documentation/ELEE1119_Project_Plan_7th_Dec_2021.mpp) to give an overview of the project timeline and to clearly communicate deadlines.

After the Gantt was shared with the team, a [KANBAN was maintained within GitHub](https://github.com/vinthund/UoGACE2022/projects/1). This KANBAN would get updated every Tuesday during the weekly standup meeting to establish what tasks people had in-progress and what tasks they had outstanding.


<br>

## **GitHub Management** - [vinthund](https://www.github.com/vinthund)

***Add your summary of work here and delete this comment.***

<br>

# **License**

Due to the sensitive nature of projectile firing technology, much thought has gone into the ethics of releasing a publically available repository with details of how the project was created. Please note that the content contained within this repository should be used solely for EDUCATION purposes. For full details please see the [License](LICENSE) document found in the root of this repository. This license has been based upon the MIT license.
