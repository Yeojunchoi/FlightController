/*
 * algorithmCollection.h
 *
 *  Created on: Jul 13, 2020
 *      Author: cjb88
 */

#ifndef INC_ALGORITHMCOLLECTION_H_
#define INC_ALGORITHMCOLLECTION_H_

#ifdef __cplusplus
extern "C" {
#endif

// Function declarations
#include "main.h"

#define DEG2PI 0.01745329251

float invSqrt(float x);

struct junTimer{
   uint32_t last_time;
   uint8_t start; // 0 not start, 1 start
};

void junTimer_tic(struct junTimer* t);

uint32_t junTimer_toc(struct junTimer* t);

#ifdef __cplusplus
}
#endif

#endif /* INC_ALGORITHMCOLLECTION_H_ */
