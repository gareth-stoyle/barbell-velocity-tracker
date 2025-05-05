# Barbell velocity tracker
A system that uses an ESP32 Arduino dev board + Ultrasonic sensor to track the velocity of barbell in various gym exercises.

# Why track barbell velocity?
This is called [Velocity Based Training (BVT)](https://www.scienceforsport.com/velocity-based-training/) and is proposed to be a more rigorous alternative to using % of one rep max (1RM), or subjective measures such as rate of perceived exertion (RPE).

# Demo
![VBT](examples/demo.gif)


# Improvements required
- The overall rig could be put together a lot better.
- Combining more than one sensor for better noise detection and accuracy would be good. I would like to try two ultrasonic sensors (but they may interfere with eachother).