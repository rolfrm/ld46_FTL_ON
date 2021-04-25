#include <iron/full.h>
#include <iron/gl.h>

#include "scheme.h"
#include "scheme-private.h"
#include "main.h"
f32 distance_field_distance(vec3 p, distance_field * field){
  return field->f(p, field->userdata);
}

f32 circle_distance(vec3 pt, void * _v){
  var v = (vec4 *) _v;
  //vec3_print(pt); vec4_print(*v); logd(" <=\n");
  return vec3_len(vec3_sub(pt, v->xyz)) - v->w;
}

f32 square2d(vec2 p, vec2 center, vec2 radius){
  p = vec2_abs(vec2_sub(p, center));
  if(p.x > radius.x){
    if(p.y > radius.y)
      return vec2_len(vec2_sub(p, radius));
    return p.x - radius.x;
  }else if(p.y > radius.y)
    return p.y - radius.y;
  // inside the square.
  return fmax(p.y- radius.y, p.x - radius.x);
}
 

f32 square_distance(vec3 p, void * _v){
  f32 * v = _v;
  vec3 a = vec3_new(v[0], v[1], v[2]);
  vec3 r = vec3_new(v[3], v[4], v[5]);
  return square2d(p.xy, a.xy, r.xy);
}

distance_field * circle_df(f32 x, f32 y, f32 z, f32 r){
  distance_field * df = alloc0(sizeof(distance_field));
  vec4 * v4 = alloc0(sizeof(v4[0]));

  *v4 = vec4_new(x,y,z,r);
  df->userdata = v4;
  df->f = circle_distance;
  return df;

}

distance_field * square_df(f32 x1, f32 y1, f32 z1, f32 x2, f32 y2, f32 z2 ){
  distance_field * df = alloc0(sizeof(distance_field));
  f32 * v4 = alloc0(sizeof(v4[0]) * 6);
  v4[0] = x1;
  v4[1] = y1;
  v4[2] = z1;
  v4[3] = x2;
  v4[4] = y2;
  v4[5] = z2;

  df->userdata = v4;
  df->f = square_distance;
  return df;
}


distance_field * distance_field_load(scheme * sc, pointer obj){
  var name = pair_car(obj);
  distance_field * df = NULL;
  if(!is_symbol(name)) return df;
  if(symbol_eq(name, "circle")){
    size_t c = 0;
    logd("Load Circle %i\n", c);
    f32 * fs = pointer_to_floats(sc, pair_cdr(obj), &c);
    if(c == 4){
      df = circle_df(fs[0], fs[1], fs[2], fs[3]);
    }else if(c == 1){
      df = circle_df(0,0,0, fs[0]);
    }
    dealloc(fs);
  }else if(symbol_eq(name, "rectangle")){
    size_t c = 0;
    logd("Load Rect %i\n", c);
    f32 * fs = pointer_to_floats(sc, pair_cdr(obj), &c);
    if(c == 6){
      df = square_df(fs[0], fs[1], fs[2], fs[3], fs[4], fs[5]);
    }else if(c == 3){
      df = square_df(0,0,0, fs[0], fs[1], fs[2]);
    }
    dealloc(fs);
  }
  return df;
}

vec3 gradient_vector(vec3 p, distance_field * field, bool half){
  f32 a = distance_field_distance(p, field);
  float fac = 0.1;
  vec3 px = p;
  px.x += a * fac;
  vec3 py = p;
  py.y += a * fac;
  vec3 pz = p;
  pz.z += a * fac;
  
  vec3 px2 = p;
  px2.x -= a * fac;
  vec3 py2 = p;
  py2.y -= a * fac;
  vec3 pz2 = p;
  pz2.z -= a * fac;

  
  f32 dy = distance_field_distance(py, field)
    - distance_field_distance(py2, field);

  f32 dx1 = distance_field_distance(px, field);
  f32 dx2 = distance_field_distance(px2, field);
  f32 dx = dx1 - dx2;
  f32 dz1 =distance_field_distance(pz, field);
  f32 dz2 = distance_field_distance(pz2, field);
  f32 dz = dz1 - dz2;
  var d =vec3_new(dx, dy, dz);
  return vec3_scale(vec3_normalize(d), a);
}


vec3 trace_closest_point(vec3 p, mat4 m1, mat4 m2, distance_field *d1, distance_field * d2, f32 * d){
  //vec3 o1 = mat4_mul_vec3(m1, vec3_new(0,0,0));
  //vec3 o2 = mat4_mul_vec3(m2, vec3_new(0,0,0));
  //vec3_print(o1); vec3_print(o2);logd(" center \n");
  var inv1 = mat4_invert(m1);
  var inv2 = mat4_invert(m2);
  f32 mind = 100000;
  vec3 half2 = p;
  for(int i = 0; i < 15; i++){
   
   vec3 p1 = mat4_mul_vec3(inv1, p);
   vec3 p2 = mat4_mul_vec3(inv2, p);
   f32 a1 = distance_field_distance(p1, d1);
   f32 b1 = distance_field_distance(p2, d2);

   vec3 a = gradient_vector(p1, d1, true);
   var p3 = vec3_sub(p1, vec3_scale(a, 0.95));
   p3 = mat4_mul_vec3(m1, p3);
   var a2 = vec3_sub(p, p3);
    vec3 p0 = p;
    p = vec3_sub(p, a2);

    var half = vec3_scale(vec3_add(p0, p), 0.5);
    mind = MIN(mind, a1 + b1);
    f32 d = vec3_sqlen(vec3_sub(half2, half));
    if(d < 0.001){
      break;
    }
    half2 = half;
    SWAP(d1, d2);
    SWAP(m1, m2);
    SWAP(inv1, inv2);
  }
  *d = mind;
  return p;
}
