#include <iron/full.h>

typedef struct{
  i64 sg_us_pi;
  i64 sg_time_us;
  i64 sg_us;
}sound_ctx;

float sg_phase(sound_ctx * ctx, float frequency){
  i64 pf = (i64)(frequency * ctx->sg_time_us * 2 * M_PI);
  i64 p =  pf % ctx->sg_us_pi;
  float r = (float)p / ctx->sg_us;
  return r;
}

float sg_sine(sound_ctx * ctx, float frequency){
  return sin(sg_phase(ctx, frequency));
}
static inline float saw(float rads, float shape)
{
  float t = rads / (float)(M_PI * 2);
  float a = (shape + 1.0f) / 2.0f;
  
  if (t < a / 2)
    return 2 * t / a;
  
  if (t > (1 - (a / 2)))
    return (2 * t - 2) / a;
  
  return (1 - 2 * t) / (1 - a);
}

float sg_saw(sound_ctx * ctx, float frequency, float shape){
  return saw(sg_phase(ctx, frequency), shape);
}

static inline float square(float rads){
  if(rads > M_PI)
    return 1.0f;
  else return -1.0f;
}
float sg_square(sound_ctx * ctx, float frequency){
  return square(sg_phase(ctx, frequency));
}

