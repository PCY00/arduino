/*
 * FND.c
 *
 *  Created on: Dec 22, 2024
 *      Author: kksp1
 */

#include "FND.h"

void send(uint8_t X){
	  for (int i = 8; i >= 1; i--)
	  {
	    if (X & 0x80)
	    {
	    	HAL_GPIO_WritePin(GPIOA, GPIO_PA10_DIO_Pin, HIGH);
	    }
	    else
	    {
	    	HAL_GPIO_WritePin(GPIOA, GPIO_PA10_DIO_Pin, LOW);
	    }
	    X <<= 1;

	    // make CLK
	    HAL_GPIO_WritePin(GPIOA, GPIO_PA8_SCLK_Pin, LOW);
	    HAL_GPIO_WritePin(GPIOA, GPIO_PA8_SCLK_Pin, HIGH);
	  }
}

void send_port(uint8_t X, uint8_t port){
	  send(X);
	  send(port);
	  HAL_GPIO_WritePin(GPIOA, GPIO_PA9_RCLK_Pin, LOW);
	  HAL_GPIO_WritePin(GPIOA, GPIO_PA9_RCLK_Pin, HIGH);
}

void display_number(int number, int repetitions){
	int n1, n2, n3, n4;

	uint8_t numArr[10] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90};
	uint8_t numPosition[4] = {0b0001, 0b0010, 0b0100, 0b1000};

	n1 = number % 10;
	n2 = (number / 10) % 10;
	n3 = (number / 100) % 10;
	n4 = (number / 1000) % 10;

	for(int i = 0; i < repetitions; i++){
		send_port(numArr[n1], numPosition[0]);
		send_port(numArr[n2], numPosition[1]);
		send_port(numArr[n3], numPosition[2]);
		send_port(numArr[n4], numPosition[3]);
	}
}

