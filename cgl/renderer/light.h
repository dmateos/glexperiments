#ifndef _LIGHT_H
#define _LIGHT_H

typedef struct {
  float position[3];
  float color[3];
  float intensity;
} Light;

#endif