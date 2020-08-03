#include "ButtonThread.h"
#include "driver/uart.h"

using namespace std;

static CThread thread;
#define SERIAL_PORT_NUMBER UART_NUM_2
#define BUF_SIZE (1024 * 2)
#define RD_BUF_SIZE (1024)
static QueueHandle_t uart2_queue;

#define UART_RX 18
#define UART_TX 19

QueueHandle_t keypadQueue = NULL;
EventGroupHandle_t keypadEventGroup = NULL;
static uint16_t result;

static void run(void* params) {
  int command = -1;
  Serial.printf_P(PSTR("ButtonThread running...\n"));

  uart_event_t event;
  size_t buffered_size;
  bool exit_condition = false;

  // Infinite loop to run main bulk of task
  while (1) {
    // Loop will continually block (i.e. wait) on event messages from the event queue
    if (xQueueReceive(uart2_queue, (void*)&event, (portTickType)portMAX_DELAY)) {
      // Handle received event
      if (event.type == UART_DATA) {
        uint8_t UART_data[128];
        int UART_data_length = 0;
        ESP_ERROR_CHECK(uart_get_buffered_data_len(SERIAL_PORT_NUMBER, (size_t*)&UART_data_length));
        UART_data_length = uart_read_bytes(SERIAL_PORT_NUMBER, UART_data, UART_data_length, 100);

        result = UART_data[0] << 8 | UART_data[1];

        Serial.printf_P(PSTR("Received : %04x\n"), result);

        // Broadcast event to notify any waiters.
        xEventGroupClearBits(keypadEventGroup, KEY_EVENTBITMASK);
        xEventGroupSetBits(keypadEventGroup, (result & KEY_EVENTBITMASK));

        // Send to queue to notify any receivers.
        xQueueOverwrite(keypadQueue, &result);

        // Serial.println("LEN= ");
        // Serial.println(UART_data_length);

        // Serial.print("DATA= ");
        // for (byte i = 0; i < UART_data_length; i++)
        //   Serial.print((char)UART_data[i]);
        // Serial.println("");

      }

      // Handle frame error event
      else if (event.type == UART_FRAME_ERR) {
        // TODO...
      }

      // Final else statement to act as a default case
      else {
        // TODO...
      }
    }

    if (exit_condition) {
      break;
    }
  }

  // Out side of loop now. Task needs to clean up and self terminate before returning
  vTaskDelete(NULL);
}

static CThread* initialize(u8 priority) {
  uart_config_t UART_CONFIG = {.baud_rate = 4800,
                               .data_bits = UART_DATA_8_BITS,
                               .parity = UART_PARITY_DISABLE,
                               .stop_bits = UART_STOP_BITS_1,
                               .flow_ctrl = UART_HW_FLOWCTRL_DISABLE};
  uart_param_config(SERIAL_PORT_NUMBER, &UART_CONFIG);
  uart_set_pin(SERIAL_PORT_NUMBER, UART_TX, UART_RX, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
  uart_driver_install(SERIAL_PORT_NUMBER, BUF_SIZE, BUF_SIZE, 20, &uart2_queue, 0);

  // Create Event Group.
  keypadEventGroup = xEventGroupCreate();
  xEventGroupClearBits(keypadEventGroup, KEY_EVENTBITMASK);

  // Create Queue.
  keypadQueue = xQueueCreate(1, sizeof(uint32_t));

  thread.run = run;
  return CThread_super(&thread, 2048, "buttonThread", (tskIDLE_PRIORITY+priority));
}
const struct buttonThread ButtonThread = {
    .initialize = initialize,
};
