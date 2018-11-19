# odometry
Odometry -  building a line follower robot with arduino uno as a university group project (University of Southampton)


The Design Brief:

You are required to design, build and program a simple robot that can
drive a predetermined route (see course path). The vehicle must be able to drive to certain
waypoints and when there, it must stop and indicate that it has reached that waypoint and
then drive onto the next waypoint and repeat this action. At five special waypoints the system
must accurately deposit a single marker (marker maximum size should be 15 mm by 15 mm
by 5 mm and minimum size should be 8 mm by 8 mm by 3 mm, e.g. M&Ms and Smarties
could be used). Looking from above, at least some of the marker’s edge must intersect with
the disc of the waypoint.

Rules to follow:

• The controller must consist of one Arduino UNO, one MD25 and the RD02 drive unit.
• The robot must determine its position using the motor encoder outputs only, no
additional sensors are allowed.
• The system must be as accurate as possible in all its movements.
• It must indicate via sound and/or LED that it has reached the predetermined points.
• The system must have a simple switch to start it (not a wire).
• The use of a 2 A fuse between the battery and the hardware is mandatory.
• The system cannot exceed an A4 footprint and a height of 350 mm.
• All the electronics must be safely embedded into the prototype – no loose wires!
• Solder all connectors properly – Do NOT solder to battery terminals, use insulated
spade connectors.
• Protect the MD25 with a proper connector such as a Deans or XT60.


Deliverables:

• A functioning Mechatronic prototype with embedded Arduino software.
• Demonstration of the final prototype in a competition format.
• A print out of the Arduino sketch (A4 two pages per page, double sided) used for
the final demonstration with extensive comments and authorship and group name
(i.e. A2-G or M1-F) clearly marked in a header at the top.

The track looks like the one in the photo in file called: track.jpg

The order of the points are the following:

Course Path (All dimensions in mm):
Start at point 13 and are going in the order 1, 2, 11, 10, 9, 8, 5, 6, 7, 8, 4 ,3, 2,
13. The five special waypoints are denoted using an orange marker – Points 2, 4, 6, 8 &
10 (see below).

For the code, please make sure to use the correct pin numbers, I have changed a few during testing to make it nicer.

Hope you are all enjoying the challange!
