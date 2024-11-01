# Recruits_task

1 Introduction

The primary goal of the first tasks is to validate the Low Voltage and Software presen-
tation and to create some initial functional and mind problems to combat this initial inertia
on the overall LV system functionality:

• Problem 1: Brake Pressure Sensor (BSPD)
• Problem 2: Current Sensor (BSPD)
• Problem 3: Accumulator Fan
• Problem 4: Accelerator Pedal Position Sensor (APPS)

The goal in each task is to create all the low voltage connections to give life to each
component. Always be careful to design a system that is rule-compliant with 2025 Formula
Student Germany (FSG). In each problem create code to extract the main information
relative to each low-voltage component in the individual task.
Main objectives in these tasks are the same for every problem created, each group
associated with a single problem has to create all the connections from the low voltage
battery of the car to the system created in order to extract the information detail in each
problem. With the information capture, the next step is to send that information well-
formatted to the CAN Bus line of the car.
The objectives are detailed below:
System Component allocated, Teensy 4.1, LV battery, logic, CAN Bus line;
Teensy 4.1 The micro-controller used in the car is the teensy 4.1, so that is the micro-
controller that everyone will be using when developing the LV task. Every 5 CAN messages
change the ”endianess” of the CAN message.
Diagram Create a diagram, mainly in draw.io which is the diagram working tool that
the department uses on a daily basis, with all the connections from the LV battery to your
system;
Git Hub All the code must be placed in the Git Hub of the department, at the end of
your code development the code must be sent to the main branch of the repository;

4 Problem 3

4.1 Accumulator Fan

The accumulator fan is one of the various fans used to cool the accumulator.
The fan has the ability to establish its working flow, create parameters for the Fan, and
send the percentage of usage from the Fan to the CAN Bus line.
Reference Number: FAN THA0412AD-TZW3
