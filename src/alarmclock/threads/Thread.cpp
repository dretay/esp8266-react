#include "Thread.h"

Thread::Thread(const uint32_t _stackDepth, UBaseType_t _priority, const char* const _name){
  xTaskCreate(task, _name, _stackDepth, this, _priority, &this->taskHandle);
}
TaskHandle_t* Thread::getHandle(){
  return &taskHandle;
}
void Thread::task(void* _params) {
  Thread* p = static_cast<Thread*>(_params);
  p->run();
}
