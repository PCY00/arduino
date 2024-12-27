/*
 * FND.h
 *
 *  Created on: Dec 22, 2024
 *      Author: kksp1
 */

#ifndef INC_FND_H_
#define INC_FND_H_

#include "main.h"

#define HIGH 1
#define LOW 0

void send(uint8_t X);
void send_port(uint8_t X, uint8_t port);
void display_number(int number, int repetitions);


#endif /* INC_FND_H_ */
