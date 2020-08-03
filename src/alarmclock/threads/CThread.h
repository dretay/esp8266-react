#pragma once

#include <Arduino.h>

typedef struct CThread {
  // void(*stop)(void) //TODO: implement me!
  void (*run)(void* params);
  float (*memoryFree)(CThread*); 
  TaskHandle_t thread;
  uint32_t stack_allotment;
  char taskName[configMAX_TASK_NAME_LEN];

} CThread;

static float CThread_memoryFree(CThread* thread) {
  
  UBaseType_t uxHighWaterMark;
  uint32_t diff;
  float result;

  uxHighWaterMark = uxTaskGetStackHighWaterMark(thread->thread);

  diff = thread->stack_allotment - uxHighWaterMark;

  result = ((float)diff / (float)thread->stack_allotment) * 100.0;

  Serial.printf_P(PSTR("%s max usage: %.0f%%\n"), thread->taskName, result);
  
}

static CThread* CThread_super(CThread* child, uint32_t stack_allotment, char* taskName, UBaseType_t priority) {
  child->memoryFree = CThread_memoryFree;
  child->stack_allotment = stack_allotment;
  strncpy(child->taskName, taskName,configMAX_TASK_NAME_LEN);

  Serial.printf_P(PSTR("Starting %s..."), taskName);
  xTaskCreate(child->run, taskName, stack_allotment, NULL, priority, &child->thread);

  return child;
}
