#include <stdint.h>
#include "./src/handlers/co2Handler/interface/co2Handler.h"
#include "./src/handlers/HumidityHandler/humidityHandler.h"
#include "./src/handlers/temperturHandler/temperturHandler.h"
#include "./InterfaceWrapper/Wrapper.h"
#include <lora_driver.h>
#include <ATMEGA_FreeRTOS.h>
#include <semphr.h>
#include <status_leds.h>

static lora_driver_payload_t _uplink_payload;
QueueHandle_t xQueue;
void wrapper_init(){
	_uplink_payload.len = 6;
	_uplink_payload.portNo = 2;
	xQueue;
	if (xQueue != NULL)
	{
		return;
	}
	
}

 lora_driver_payload_t wrapperhandler()
{
uint16_t co2_ppm = co2_getMeasurement();
uint16_t temp = temp_getMeasurement();
uint16_t hum = hum_getMeasurement();

	_uplink_payload.bytes[0] = hum >> 8;
	_uplink_payload.bytes[1] = hum & 0xFF;
	_uplink_payload.bytes[2] = temp >> 8;
	_uplink_payload.bytes[3] = temp & 0xFF;
	_uplink_payload.bytes[4] = co2_ppm >> 8;
	_uplink_payload.bytes[5] = co2_ppm & 0xFF;
return _uplink_payload;
	
}

/*
#include <stdio.h>
#include <stdlib.h>

// Queue node structure
typedef struct QueueNode {
	int data;
	struct QueueNode* next;
} QueueNode;

// Queue structure
typedef struct Queue {
	QueueNode* front;
	QueueNode* rear;
} Queue;

// Function to create an empty queue
Queue* createQueue() {
	Queue* queue = (Queue*)malloc(sizeof(Queue));
	queue->front = queue->rear = NULL;
	return queue;
}

// Function to check if the queue is empty
int isEmpty(Queue* queue) {
	return (queue->front == NULL);
}

// Function to enqueue an element
void enqueue(Queue* queue, int data) {
	QueueNode* newNode = (QueueNode*)malloc(sizeof(QueueNode));
	newNode->data = data;
	newNode->next = NULL;

	if (isEmpty(queue)) {
		queue->front = queue->rear = newNode;
		} else {
		queue->rear->next = newNode;
		queue->rear = newNode;
	}

	printf("Enqueued: %d\n", data);
}

// Function to dequeue an element
int dequeue(Queue* queue) {
	if (isEmpty(queue)) {
		printf("Error: Queue is empty.\n");
		return -1; // Assuming -1 represents an error or invalid value
	}

	QueueNode* frontNode = queue->front;
	int data = frontNode->data;

	queue->front = queue->front->next;
	free(frontNode);

	if (queue->front == NULL) {
		queue->rear = NULL;
	}

	return data;
}

// Function to get the front element of the queue
int front(Queue* queue) {
	if (isEmpty(queue)) {
		printf("Error: Queue is empty.\n");
		return -1; // Assuming -1 represents an error or invalid value
	}

	return queue->front->data;
}

int main() {
	Queue* queue = createQueue();

	enqueue(queue, 10);
	enqueue(queue, 20);
	enqueue(queue, 30);

	printf("Front of the queue: %d\n", front(queue));

	int dequeued = dequeue(queue);
	if (dequeued != -1) {
		printf("Dequeued: %d\n", dequeued);
	}

	printf("Front of the queue after dequeue: %d\n", front(queue));

	return 0;
}
*/