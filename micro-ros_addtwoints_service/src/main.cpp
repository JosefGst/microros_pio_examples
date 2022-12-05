// TODO no service reply

#include <Arduino.h>
#include <micro_ros_platformio.h>
#include <example_interfaces/srv/add_two_ints.h>
#include <stdio.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <std_msgs/msg/int64.h>

#if !defined(MICRO_ROS_TRANSPORT_ARDUINO_SERIAL)
#error This example is only avaliable for Arduino framework with serial transport.
#endif

#define LED PA5


rcl_node_t node;
rclc_support_t support;
rcl_allocator_t allocator;
rclc_executor_t executor;

rcl_service_t service;

example_interfaces__srv__AddTwoInts_Response res;
example_interfaces__srv__AddTwoInts_Request req;

#define RCCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){while(1){};}}

#define RCSOFTCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){}}

void service_callback(const void * req, void * res){
  example_interfaces__srv__AddTwoInts_Request * req_in = (example_interfaces__srv__AddTwoInts_Request *) req;
  example_interfaces__srv__AddTwoInts_Response * res_in = (example_interfaces__srv__AddTwoInts_Response *) res;

  printf("Service request value: %d + %d.\n", (int) req_in->a, (int) req_in->b);
  digitalWrite(LED, !digitalRead(LED)); 
  res_in->sum = req_in->a + req_in->b;
}

void setup() {

  pinMode(LED, OUTPUT);

  //set_microros_transports();
  Serial.begin(115200);
  set_microros_serial_transports(Serial);
  delay(2000); 

  allocator = rcl_get_default_allocator();

  // Define DOMAIN_ID to 31
  rcl_init_options_t init_options = rcl_get_zero_initialized_init_options();
  RCCHECK(rcl_init_options_init(&init_options, allocator));
  RCCHECK(rcl_init_options_set_domain_id(&init_options, 31));

  //create init_options
  RCCHECK(rclc_support_init_with_options(&support, 0, NULL, &init_options, &allocator));

  // create init_options
  // RCCHECK(rclc_support_init(&support, 0, NULL, &allocator));

  // create node
  RCCHECK(rclc_node_init_default(&node, "micro_ros_platformio_node", "", &support));

  // create service
  RCCHECK(rclc_service_init_default(
    &service, 
    &node, 
    ROSIDL_GET_SRV_TYPE_SUPPORT(example_interfaces, srv, AddTwoInts), 
    "micro_ros_platformio_node_service"));

  // create executor
  RCCHECK(rclc_executor_init(&executor, &support.context, 1, &allocator));
  RCCHECK(rclc_executor_add_service(
    &executor, 
    &service, 
    &req, 
    &res, 
    &service_callback));
}


void loop() {
  
  delay(100);
  RCSOFTCHECK(rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100)));
}
