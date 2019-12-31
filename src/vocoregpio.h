#ifndef __VOCOREGPIO_H__
#define __VOCOREGPIO_H__

void setDirection(uint8_t pin, int direction);
void setPolarity(uint8_t pin, int polarity);
void setData(uint8_t, int data);
int getData(uint8_t pin);
void clearData(uint8_t pin);

#endif
