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

	       			     

distance_field * circle_df(f32 x, f32 y, f32 z, f32 r){
  distance_field * df = alloc0(sizeof(distance_field));
  vec4 * v4 = alloc0(sizeof(v4[0]));

  *v4 = vec4_new(x,y,z,r);
  df->userdata = v4;
  df->f = circle_distance;
  return df;

}

distance_field * distance_field_load(scheme * sc, pointer obj){
  var name = pair_car(obj);
  if(!is_symbol(name)) return NULL;
  if(symbol_eq(name, "circle")){
    size_t c = 0;
    f32 * fs = pointer_to_floats(sc, pair_cdr(obj), &c);
    logd("LOAD CIRCLE %i\n", c);
    if(c == 4){
      return circle_df(fs[0], fs[1], fs[2], fs[3]);
    }
  }
  return NULL;
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
  for(int i = 0; i < 15; i++){
   
   vec3 p1 = mat4_mul_vec3(inv1, p);
   vec3 p2 = mat4_mul_vec3(inv2, p);
   f32 a1 = distance_field_distance(p1, d1);
   f32 b1 = distance_field_distance(p2, d2);

   vec3 a = gradient_vector(p1, d1, true);
   var p3 = vec3_sub(p1, vec3_scale(a, 0.8));
   f32 a3 = distance_field_distance(p3, d1);
   p3 = mat4_mul_vec3(m1, p3);
   var a2 = vec3_sub(p, p3);
    vec3 p0 = p;
    p = vec3_sub(p, a2);

    var half = vec3_scale(vec3_add(p0, p), 0.5);
    mind = MIN(mind, a1 + b1);
    SWAP(d1, d2);
    SWAP(m1, m2);
    SWAP(inv1, inv2);
  }
  *d = mind;
  return p;
}
