# NodeMCU_WebSocket_Server

## Table of Contents
- [Abstract](#abstract)
- [NodeMCU Pin Diagram](#nodemcu-pin-diagram)
- [User Interface](#user-interface)
- [Conclusion](#conclusion)
- [Future Scope](#future-scope)


## Abstract

Utrasonic sensor data processing is a simple yet powerful implementation of NodeMCU WebSocket server. There is a huge demand of a smart and IoT devices in the upcoming decade. But there is no promising solution for real time connectivity of  such  smart devices on a large scale with  feasible processing power and efficient power consumption. It provides a solution that do not require user to learn anything new and hence can be used by anyone. It requires a simple connection at the circuit side with minimum power consumption and no need of peripheral along with providing a robust user interface that can be used by any age group to access the devices at fingertips. The project has been designed with functioning of  ultrasonic sensor at the core. Here ultrasonic sensor sends a burst of 8 pulses at 40 khz frequency for accurate positioning of the object near it. This data is processed by NodeMCU and depending on the situation it fires a signal if object is detected. The second half of this project is to bring the data captured by the ultrasonic sensor to the client at the far end of the world.Asynchronous webserver over TCP protocol is used to establish client server connection. Here we are using the websockets protocol to connect the client device to the server. Then the data is sent with the help of socket to the client where the predesigned interface takes in this data and present in the user friendly format. Also some features are given to the user to control the NodeMCU via webserver. This whole system works as one unit and is a reliable solution  backed by 5V power supply. Hence this circuit provides a practical, feasible and easily implementable solution with minimal cost and zero maintenance.  

## NodeMCU Pin Diagram
<img src="https://github.com/Pratham23052002/NodeMCU_WebSocket_Server/blob/main/NodeMCU-Pinout-Image.jpg"/>

## User Interface
<img src="https://github.com/Pratham23052002/NodeMCU_WebSocket_Server/blob/main/UI%20Interface.png"/>

## Conclusion
Hereby we conclude that, this project aims to be one of the solutions to solve this problem and cater a large number of devices with maximum possible compatibility. This project focuses mainly on two core functions, to add a wireless capability to any sensor or device and providing a robust user interface to control it. Hence this circuit provides a practical, feasible and easily implementable solution with minimal cost and zero maintenance.

## Future Scope
We are planning to solve a next generation issue that hasn’t been raised yet so thare are quite possible chances to update and  upgrade the project to make it relevant to the current scenario. This will also required a sincere amount of time and resources. Hence this project has been hosted and actively maintained on the Github and can be found on the following [link](https://github.com/PatelVatsalB21/NodeMCU_WebSocket_Server).
