#pragma once 

#include <Arduino.h>
#include <types.h>

//adopted from https://fjrg76.wordpress.com/2018/05/20/objectifying-task-creation-in-freertos/

class Thread {
 public:
  Thread();
  Thread(const uint32_t _stackDepth, UBaseType_t _priority, const char* const _name);
  TaskHandle_t getHandle();
  virtual void run() = 0;

 protected:
  static void task(void* _params);
  TaskHandle_t taskHandle;
};