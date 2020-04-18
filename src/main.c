#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <iron/full.h>
#include <iron/gl.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <GL/gl.h>
#include "scheme.h"
#include "scheme-private.h"
#include "u32_to_u32_table.h"
#include "main.h"

typedef struct{
  blit3d_polygon * verts;
  vec4 color;
  vec3 offset;
  vec3 rotation;
}polygon;

typedef struct{
  u32 sub_object;
  mat4 transform;
}sub_object;


struct _context{
  gl_window * win;
  blit3d_context * blit3d;
  int running;
  scheme * sc;

  u32_to_u32_table * sym_to_object;
  polygon * polygons;
  u32 polygon_count;

  sub_object * sub_objects;
  u32 sub_object_count;

  u32_to_u32_table * sub_object_list;
  u32_to_u32_table * poly_to_sub_object;
  
  
  u32_to_u32_table * shown_objects;
  u32 current_symbol;
  u32 current_sub_object;
  mat4 view_matrix;
  mat4 camera_matrix;
};

context * current_context;

bool context_running(context * ctx){
  current_context = ctx;
  return ctx->running;
}
vec4 bg_color;
mat4 mat4_translate_vec3(vec3 v){
  return mat4_translate(v.x, v.y, v.z);
}

void render_polygon(context * ctx, mat4 transform, u32 id){
  var blit3d = ctx->blit3d;
  var object =  ctx->polygons + id;
  
  mat4 O = mat4_translate_vec3(object->offset);
  mat4 C = mat4_invert(ctx->camera_matrix);
  
  O = mat4_mul(O, transform);
  
  if(object->verts != NULL){
    mat4 V = ctx->view_matrix;
    // World coord: Object * Vertex
    // Camera coord: InvCamera * World
    // View coord: View * Camera(ve
    // Vertx * Object * InvCamera * View
    
    blit3d_view(blit3d, mat4_mul(V, mat4_mul(C, O)));
    blit3d_color(blit3d, object->color);
    blit3d_polygon_blit(blit3d, object->verts);
  }
  int err = glGetError();
  if(err != 0)
    printf("ERR? %i\n", err);
  
  size_t indexes[10];
  size_t cnt;
  size_t index = 0;
  while((cnt = u32_to_u32_table_iter(ctx->poly_to_sub_object, &id, 1, NULL, indexes, array_count(indexes), &index))){
    for(size_t i = 0 ; i < cnt; i++){
      u32 ref = ctx->sub_object_list->value[indexes[i]];
      render_polygon(ctx, mat4_mul(O, ctx->sub_objects[ref].transform), ctx->sub_objects[ref].sub_object);
    }
  }
  
}

void render_update(context * ctx){
  scheme_load_string(ctx->sc, "(update)");
  current_context = ctx;
  gl_window_make_current(ctx->win);
  
  blit_begin(BLIT_MODE_UNIT);
  var blit3d = ctx->blit3d;
  glClearColor(bg_color.x,bg_color.y,bg_color.z,bg_color.w);
  glClear(GL_COLOR_BUFFER_BIT);
  blit3d_context_load(blit3d);
  mat4 C = mat4_invert(ctx->camera_matrix);

  for(u32 i = 0; i < ctx->shown_objects->count; i++){
    render_polygon(ctx, mat4_identity(), ctx->shown_objects->key[i + 1]);  
  }
}

int list_len(scheme * sc, pointer data){
  int cnt = 0;
  while(data != sc->NIL){
    cnt += 1;
    data = pair_cdr(data);
  }
  return cnt;
}

float * pointer_to_floats(scheme * sc, pointer data, size_t * cnt){
  var orig_data = data;
  int l = list_len(sc, data);
  f32 * array = alloc0(l * sizeof(data[0]));
  int offset = 0;
  while(data != sc->NIL){
    
    var val = pair_car(data);
    if(is_integer(val)){
      array[offset] = (float)ivalue(val);
    }else if(is_number(val)){
      array[offset] = (float)rvalue(val);
    }
    data = pair_cdr(data);
    offset += 1;
  }
  *cnt = l;
  return array;
}

pointer load_poly(scheme * sc, pointer data){
  size_t c = 0;
  f32 * fs = pointer_to_floats(sc, data, &c);
  for(size_t i = 0; i < c; i++){
    printf(" %f", fs[i]);
  }
  printf("\n");

  var object =  current_context->polygons + current_context->current_symbol;
  if(object->verts == NULL)
    object->verts = blit3d_polygon_new();
  blit3d_polygon_load_data(object->verts, fs, c * sizeof(fs[0]));
  blit3d_polygon_configure(object->verts, 3);
  //free(fs);
  return sc->NIL;
}

pointer set_bg_color(scheme * sc, pointer args){
  size_t count;
  f32 * colors = pointer_to_floats(sc, args, &count);
  if(count == 4)
    bg_color = vec4_new(colors[0] ,colors[1], colors[2], colors[3]);
  free(colors);
  return sc->NIL;
}


pointer print_result(scheme * sc, pointer data){
  printf("\nhej?? %i %i \n", pair_cdr(data) == sc->NIL, 0);
  return data;
}

u32 pointer_to_polygon(pointer arg){
  u32 id;
  u32 ptr = (u32)arg;
  if(u32_to_u32_table_try_get(current_context->sym_to_object, &ptr, &id)){
    return id;
  }else{
    ERROR("INVALID ID");
    return 0;
  }
}

pointer define_model(scheme * sc, pointer args){
  printf("define model %i\n", pair_car(args));
  u32 ptr = (u32)pair_car(args);
  // ptr -> symbol pointer
  u32 id;
  if(!u32_to_u32_table_try_get(current_context->sym_to_object, &ptr, &id)){
    u32 newid = current_context->polygon_count;
    current_context->polygon_count += 1;
    current_context->polygons = realloc(current_context->polygons, sizeof(current_context->polygons[0]) * current_context->polygon_count);
    current_context->polygons[newid] = (polygon){0};
    id = newid;
    u32_to_u32_table_set(current_context->sym_to_object, ptr, id);
    
  }else{
    printf("Model was already defined %i\n", id);
  }
  current_context->current_symbol = id;
  return sc->NIL;
}


pointer show_model(scheme * sc, pointer args){
  printf("show model %i\n", pair_car(args));
  u32 ptr = (u32)pair_car(args);
  // ptr -> symbol pointer
  u32 id;
  if(u32_to_u32_table_try_get(current_context->sym_to_object, &ptr, &id)){
    u32_to_u32_table_set(current_context->shown_objects, id, 0);
  }
  return sc->NIL;
}

pointer unshow_model(scheme * sc, pointer args){
  u32 ptr = (u32)pair_car(args);
  // ptr -> symbol pointer
  u32 id;
  if(u32_to_u32_table_try_get(current_context->sym_to_object, &ptr, &id)){
    u32_to_u32_table_unset(current_context->shown_objects, id);
  }
  printf("show model %i\n", pair_car(args));
  return sc->NIL; 

}

pointer offset_model(scheme * sc, pointer args){
  size_t count;
  f32 * f = pointer_to_floats(sc, args, &count);
  vec3 offset;
  if(count == 2){
    offset = vec3_new(f[0], f[1], 0);
  }else if(count == 3){
    offset = vec3_new(f[0], f[1], f[2]);
  }else{
    goto end;
  }
  var object =  current_context->polygons + current_context->current_symbol;
  object->offset = offset;
 end:;
  free(f);
  return sc->NIL; 
      
}

pointer set_color(scheme * sc, pointer args){
  printf("set color\n");
  size_t count;
  f32 * colors = pointer_to_floats(sc, args, &count);
  if(count == 4){
    var object =  current_context->polygons + current_context->current_symbol;
    object->color = vec4_new(colors[0],colors[1],colors[2],colors[3]);
  }
  free(colors);
  return sc->NIL; 
}

pointer set_perspective(scheme * sc, pointer args){
  size_t count;
  f32 * fargs = pointer_to_floats(sc, args, &count);
  // y_fov, aspect, near, far
  if(count == 4){
    current_context->view_matrix = mat4_perspective(fargs[0], fargs[1], fargs[2], fargs[3]);
  }
  free(fargs);
  return sc->NIL; 
  
  
}


pointer set_orthographic(scheme * sc, pointer args){
  size_t count;
  f32 * fargs = pointer_to_floats(sc, args, &count);
  // y_fov, aspect, near, far
  if(count == 6)
    current_context->view_matrix = mat4_ortho(fargs[0], fargs[1], fargs[2], fargs[3], fargs[4], fargs[5]);
  if(count == 3)
    current_context->view_matrix = mat4_ortho(- 0.5 * fargs[0], 0.5 * fargs[0], -0.5 * fargs[1], 0.5 * fargs[1], -0.5 * fargs[2], 0.5 * fargs[2]);  
  free(fargs);
  return sc->NIL; 
}


pointer set_camera(scheme * sc, pointer args){
  size_t count;
  f32 * fargs = pointer_to_floats(sc, args, &count);
  // pos:x y z rot: x y z
  if(count == 6){
    
    var m = mat4_identity();//translate(fargs[0],fargs[1],fargs[2]);
    m.data[3][0] = fargs[0];
    m.data[3][1] = fargs[1];
    m.data[3][2] = fargs[2];
    m = mat4_rotate_X(m, fargs[3]);
    m = mat4_rotate_Y(m, fargs[4]);
    m = mat4_rotate_Z(m, fargs[5]);

    current_context->camera_matrix = m;
    //ctx->camera_rot = vec3_new(fargs[3],fargs[4],fargs[5]);
  }
  free(fargs);
  return sc->NIL;
}

u32 polygon_sym_to_id(pointer arg){
  ASSERT(is_symbol(arg));
  u32 id;
  u32 sym = (u32) arg;
  if(!u32_to_u32_table_try_get(current_context->sym_to_object, &sym, &id))
    ERROR("Unknown id");
  return id;
}

u32 sym_to_sub_object(pointer arg){
  ASSERT(is_symbol(arg));
  u32 id;
  u32 sym = (u32) arg;
  if(!u32_to_u32_table_try_get(current_context->sub_object_list, &sym, &id))
    ERROR("Unknown sub object");

  return id;
}
 

pointer define_sub_object(scheme * sc, pointer args){
  pointer x[3];
  x[0] = pair_car(args);
  args = pair_cdr(args);
  x[1] = pair_car(args);
  args = pair_cdr(args);
  x[2] = pair_car(args);
  for(int i = 0 ;i < 3; i++){
    ASSERT(is_symbol(x[i]));
  }

  u32 id;  
  u32 ptr = (u32) x[0];
  u32 a = polygon_sym_to_id(x[1]);
  u32 b = polygon_sym_to_id(x[2]);
  var ctx = current_context;
  if(!u32_to_u32_table_try_get(current_context->sub_object_list, &ptr, &id)){
    u32 newid = ctx->sub_object_count;
    ctx->sub_object_count += 1;
    ctx->sub_objects = realloc(current_context->sub_objects, sizeof(current_context->sub_objects[0]) * current_context->sub_object_count);
    ctx->sub_objects[newid] = (sub_object){0};
    ctx->sub_objects[newid].transform = mat4_identity();
    id = newid;
    u32_to_u32_table_set(current_context->sub_object_list, ptr, id);
  }else{
    
  }

  size_t indexes[10];
  size_t cnt;
  size_t index = 0;
  while((cnt = u32_to_u32_table_iter(ctx->poly_to_sub_object, &a, 1, NULL, indexes, array_count(indexes), &index))){
    for(size_t i = 0 ; i < cnt; i++){
      u32 ref = ctx->sub_object_list->value[indexes[i]];
      if(ref == id)
	goto end;
    }
  }
  u32_to_u32_table_set(ctx->poly_to_sub_object, a, id);

  ctx->sub_objects[id].sub_object = b;

 end:
  
  return sc->NIL;
}

pointer unset_sub_object(scheme * sc, pointer args){
  var ctx = current_context;
  pointer x[2];
  x[0] = pair_car(args);
  args = pair_cdr(args);
  x[1] = pair_car(args);
  for(int i = 0 ;i < 2; i++){
    ASSERT(is_symbol(x[i]));
  }

  u32 sub_object_id = sym_to_sub_object(x[0]);
  u32 a = polygon_sym_to_id(x[1]);
  
  size_t indexes[10];
  size_t cnt;
  size_t index = 0;
  while((cnt = u32_to_u32_table_iter(ctx->poly_to_sub_object, &a, 1, NULL, indexes, array_count(indexes), &index))){
    for(size_t i = 0 ; i < cnt; i++){
      u32 ref = ctx->sub_object_list->value[indexes[i]];
      if(ref == sub_object_id){
	icy_table_remove_indexes((icy_table *)ctx->sub_object_list, indexes + i, 1);
	goto end;
      }
    }
  }
 end:
  
  return sc->NIL;
}


pointer set_sub_object_transform(scheme * sc, pointer args){
  var name = pair_car(args);
  u32 id = sym_to_sub_object(name);
  args = pair_cdr(args);
  size_t count;
  f32 * fargs = pointer_to_floats(sc, args, &count);
  // pos:x y z rot: x y z
  if(count == 6){
    
    var m = mat4_identity();//translate(fargs[0],fargs[1],fargs[2]);
    m.data[3][0] = fargs[0];
    m.data[3][1] = fargs[1];
    m.data[3][2] = fargs[2];
    m = mat4_rotate_X(m, fargs[3]);
    m = mat4_rotate_Y(m, fargs[4]);
    m = mat4_rotate_Z(m, fargs[5]);

    current_context->sub_objects[id].transform = m;
  }
  free(fargs);  
  
  return sc->NIL;
}



context * context_init(gl_window * win){
   scheme_registerable reg[] = {
    {.f = print_result, .name = "print2"},
    {.f = load_poly, .name = "load-poly"},
    {.f = set_bg_color, .name = "set-bg-color"},
    {.f = define_model, .name = "define-model"},
    {.f = show_model, .name = "show-model"},
    {.f = unshow_model, .name = "unshow-model"},
    {.f = set_color, .name = "set-color"},
    {.f = offset_model, .name= "offset-model"},
    {.f = set_perspective, .name="perspective"},
    {.f = set_orthographic, .name="orthographic"},
    {.f = set_camera, .name="set-camera"},
    {.f = define_sub_object, .name="define-sub-object"},
    {.f = set_sub_object_transform, .name="set-sub-object-transform"},
    {.f = unset_sub_object, .name="unset-sub-object"}
    
    

  };
   context * ctx = alloc0(sizeof(ctx[0]));
   ctx->blit3d  = blit3d_context_new();
   var sc = scheme_init_new();
   ctx->sc = sc;
   scheme_set_output_port_file(sc, stdout);
   scheme_register_foreign_func_list(sc, reg, array_count(reg));
   current_context = ctx;
   ctx->sym_to_object = u32_to_u32_table_create(NULL);
   ctx->shown_objects = u32_to_u32_table_create(NULL);
   ctx->sub_object_list = u32_to_u32_table_create(NULL);
   ctx->poly_to_sub_object = u32_to_u32_table_create(NULL);
   ((bool *) &ctx->poly_to_sub_object->is_multi_table)[0] = true;
   ctx->running = 1;
   ctx->win = win;
   ctx->view_matrix = mat4_identity();
   ctx->camera_matrix = mat4_identity();
   return ctx;
}


void context_load_lisp(context * ctx, const char * file){
  var fin =fopen(file, "r");
  scheme_load_named_file(ctx->sc, fin, file);
  fclose(fin); 
}
