#pragma once

typedef struct {
	void(*run)(void *params);		
} CThread;