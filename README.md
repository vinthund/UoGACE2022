# **Machine Vision Foam Dart Turret**

### **UoG Advanced Computer Engineering Project 2021/22**
<br>


## **About the Project**

This project has been developed by the contributors below as a submission of coursework for the University of Greenwich Advanced Computer Engineering Module, academic year 2021/22.

The Machine Vision Foam Dart Turret is a fully automated object targetting system designed to identify, target, and shoot (with a foam dart to two) a person not recognised by the machine vision. It features a 12 dart magazine, 2 axis movement, a Jetson Nano controlled vision system, and two interfaces (one for LAN based web control and video feedback, and one for automatic downloading of this GitHub repo).

Mechanical parts of this project are mostly custom using standard industry parts for difficult to manufacture parts such as stepper motors and bearings. The software is written mostly from scratch, using part libraries where sensible, particularily for the arduino interfacing with hardware, and the Jetson managing its machine vision identification.

## **Contributors**
- [Andrew Dean - ad4646d](www.github.com/ad4646d)
- [Chris Halsall - ch6941r -](www.github.com/ch6941r)  [AfterEarthLtd](www.github.com/AfterEarthLTD)
- [vinthund](www.github.com/vinthund)
- [ht5640k](www.github.com/ht5640k)
- [JoelSmalls](www.github.com/JoelSmalls)
- [kamyar123](www.github.com/kamyar123)
- [Naojortsac](www.github.com/Naojortsac)
- [rowBoat](www.github.com/rowboat)
<br>
<br>

## **Firing Mechanism Mechanical Design** - [Chris Halsall - ch6941r](www.github.com/ch6941r)

The firing mechanism contains a few pre-made parts including a small stepper motor, the firing motors and barrel, a servo, and a bearing. All other parts have been designed in Fusion 360 and printed in orange PLA on an Ender 3 FDM 3D printer. It features a 12 dart rotary magazine driven by the stepper motor from one end and supported by a bearing at the other end. Through the bearing is mounted a magnet and a magnetic position sensor so that the control electronics can measure the angle of the magazine. A servo is mounted in the base and, with a custom arm, is capable of pushing a dart from the magazine into the firing motors and thus propelling it out of the barrel. A mounting plate is attached to the front of the design to securely mount the camera for machine vision. Finally, a hole and hex slot has been designed in to allow for easy mounting to the pan/tilt mehcanism.
<br>
<br>

## **Pan/Tilt Mechanism Mechanical Design** - [Andrew Dean - ad4646d](www.github.com/ad4646d)



<br>
<br>


## **Firing Mechanism Communications** - [JoelSmalls](www.github.com/JoelSmalls)



## **Pan/Tilt Mechanism Communicatons** - [ht5640k](www.github.com/ht5640k)

## **Machine Vision Target Aquisition** - [vinthund](www.github.com/vinthund)

@vinthund

## Web Server and Website - 

@Kamyar

#### Project and Github Management

@ad4646d
@vinthund

## License

Due to the sensitive nature of projectile firing technology, much thought has gone into the ethics of releasing a publically available repository with details of how the project was created. Please note that the content contained within this repository should be used solely for EDUCSTION purposes. For full details please see the [License](LICENSE) document found in the root of this repository. This license has been based upon the MIT license.