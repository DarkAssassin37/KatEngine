#version 430 core

#define MAX_SCENE_BOUNDS 1000.0
#define NUM_BOXES 2

layout(binding = 0, rgba32f) uniform image2D framebuffer;

// The camera specification
uniform vec3 eye;
uniform vec3 ray00;
uniform vec3 ray10;
uniform vec3 ray01;
uniform vec3 ray11;

struct box {
  vec3 min;
  vec3 max;
};


const box boxes[] = {
  /* The ground */
  {vec3(-5.0, -0.1, -5.0), vec3(5.0, 0.0, 5.0)},
  /* Box in the middle */
  {vec3(-0.5, 0.0, -0.5), vec3(0.5, 1.0, 0.5)}
};

vec2 intersectBox(vec3 origin, vec3 dir, const box b) {
  vec3 tMin = (b.min - origin) / dir;
  vec3 tMax = (b.max - origin) / dir;
  vec3 t1 = min(tMin, tMax);
  vec3 t2 = max(tMin, tMax);
  float tNear = max(max(t1.x, t1.y), t1.z);
  float tFar = min(min(t2.x, t2.y), t2.z);
  return vec2(tNear, tFar);
}

struct hitinfo {
  vec2 lambda;
  int bi;
};

bool intersectBoxes(vec3 origin, vec3 dir, out hitinfo info) {
  float smallest = MAX_SCENE_BOUNDS;
  bool found = false;
  for (int i = 0; i < NUM_BOXES; i++) {
    vec2 lambda = intersectBox(origin, dir, boxes[i]);
    if (lambda.x > 0.0 && lambda.x < lambda.y && lambda.x < smallest) {
      info.lambda = lambda;
      info.bi = i;
      smallest = lambda.x;
      found = true;
    }
  }
  return found;
}