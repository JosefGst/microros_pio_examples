#include <Arduino.h>
#include <micro_ros_platformio.h>
#include <stdio.h>
#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <std_srvs/srv/trigger.h>
#include <std_msgs/msg/int32.h>

#define RCCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){printf("Failed status on line %d: %d. Aborting.\n",__LINE__,(int)temp_rc); return 1;}}
#define RCSOFTCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){printf("Failed status on line %d: %d. Continuing.\n",__LINE__,(int)temp_rc);}}

rcl_subscription_t subscriber;
std_msgs__msg__Int32 msg;

rcl_service_t service;
std_srvs__srv__Trigger_Response res;
std_srvs__srv__Trigger_Request req;

rclc_executor_t executor;
rclc_support_t support;
rcl_allocator_t allocator;
rcl_node_t node;

static const char res_message = '\0';

void error_loop(){
  while(1){
    printf("error\n");
  }
}

void subscription_callback(const void * msgin){  
  const std_msgs__msg__Int32 * msg = (const std_msgs__msg__Int32 *)msgin;
  printf("received topic\n");
}


void service_callback(const void * req, void * res){
  std_srvs__srv__Trigger_Request * req_in = (std_srvs__srv__Trigger_Request *) req;
  std_srvs__srv__Trigger_Response * res_in = (std_srvs__srv__Trigger_Response *) res;
  printf("received service\n");

  res_in->success = true;
}


void setup() {

  allocator = rcl_get_default_allocator();

  //create init_options
  RCCHECK(rclc_support_init(&support, 0, NULL, &allocator));

  // create node
  RCCHECK(rclc_node_init_default(&node, "micro_ros_arduino_node", "", &support));

  // create subscriber
  RCCHECK(rclc_subscription_init_default(&subscriber, &node, ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32), "topic_int"));
    
  // create service
  RCCHECK(rclc_service_init_default(&service, &node,  ROSIDL_GET_SRV_TYPE_SUPPORT(std_srvs, srv, Trigger), "srv_trigger"));

  // create executor
  res.message.data = &res_message;
  res.message.size = 1;
  res.message.capacity = 1;  
  RCCHECK(rclc_executor_init(&executor, &support.context, 2, &allocator));
  RCCHECK(rclc_executor_add_service(&executor, &service, &req, &res, &service_callback));    // LINE A
  RCCHECK(rclc_executor_add_subscription(&executor, &subscriber, &msg, &subscription_callback, ON_NEW_DATA));    // LINE B
}

void loop() {
  RCCHECK(rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100)));
}