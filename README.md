# microros_pio_examples

# UDPv4 micro-ROS Agent
ros2 run micro_ros_agent micro_ros_agent udp4 --port 8888 -v6

# Serial micro-ROS Agent
ros2 run micro_ros_agent micro_ros_agent serial --dev /dev/ttyACM0 -v6

# TCPv4 micro-ROS Agent
ros2 run micro_ros_agent micro_ros_agent tcp4 --port 8888 -v6

# CAN-FD micro-ROS Agent
ros2 run micro_ros_agent micro_ros_agent --dev [YOUR CAN INTERFACE] -v6