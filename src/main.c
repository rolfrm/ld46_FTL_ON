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
#include "ptr_to_u32_table.h"
#include "main.h"

typedef enum{
  MODEL_TYPE_POLYGONAL

}model_type;

typedef struct{
  model_type type;
  //union{
  blit3d_polygon * verts;
  //distance_field_model * dist;
  //};
  vec4 color;
  vec3 offset;
  vec3 rotation;
  vec3 scale;
}polygon; // rename to 'model'.

struct _context{
  gl_window * win;
  blit3d_context * blit3d;
  int running;
  scheme * sc;

  polygon * polygons;
  u32 polygon_count;

  u32_to_u32_table * poly_to_sub_object;
  
  u32_to_u32_table * shown_objects;

  u32 current_symbol;
  u32 current_sub_object;
  mat4 view_matrix;
  mat4 camera_matrix;

  vec4 bg_color;

  float dt;

  u32 object_count;
};

context * current_context;

mat4 rotate_offset_3d_transform(float x, float y, float z, float rx, float ry, float rz){
  var m = mat4_identity();
  m.data[3][0] = x;
  m.data[3][1] = y;
  m.data[3][2] = z;
  m = mat4_rotate_X(m, rx);
  m = mat4_rotate_Y(m, ry);
  m = mat4_rotate_Z(m, rz);
  return m;
}

bool context_running(context * ctx){
  current_context = ctx;
  return ctx->running;
}

mat4 mat4_translate_vec3(vec3 v){
  return mat4_translate(v.x, v.y, v.z);
}

void print_object_sub_objects(){
  var ctx = current_context;
  for(u32 i = 0; i < ctx->poly_to_sub_object->count; i++){
    int j = i + 1;
    var k = ctx->poly_to_sub_object->key[j];
    var v = ctx->poly_to_sub_object->value[j];

    printf("%i -> %i\n", k, v);

  }

}

int render_it = 0;
vec3 v3_one = {.x = 1, .y = 1, .z = 1};
void render_polygon(context * ctx, mat4 transform, u32 id, vec4 color){
  //print_object_sub_objects();
  //ERROR("!!");
  if(render_it > 5) ERROR("Stack\n");
  render_it += 1;
  var blit3d = ctx->blit3d;
  var object =  ctx->polygons + id;
  
  var o = object->offset.data;
  var r = object->rotation.data;
  var s = object->scale;
  var O = rotate_offset_3d_transform(o[0], o[1], o[2], r[0], r[1], r[2]);
  if(vec3_compare(v3_one, s, 0.0001) == false){
    O = mat4_mul(O, mat4_scaled(s.x, s.y, s.z));

  }
  color = vec4_mul(object->color, color);
  mat4 C = mat4_invert(ctx->camera_matrix);
  
  O = mat4_mul(transform, O);
  
  if(object->verts != NULL){
    mat4 V = ctx->view_matrix;
    // World coord: Object * Vertex
    // Camera coord: InvCamera * World
    // View coord: View * Camera(ve
    // Vertx * Object * InvCamera * View
    
    blit3d_view(blit3d, mat4_mul(V, mat4_mul(C, O)));
    blit3d_color(blit3d, color);
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
      var sub = ctx->poly_to_sub_object->value[indexes[i]];
      //printf("render sub\n");

      if(sub == id){
	print_object_sub_objects();
	ERROR("LOOP\n");
      }
      render_polygon(ctx, O, sub,  color);
      //printf("render done\n");
    }
  }

  render_it -= 1;
}

void render_update(context * ctx, float dt){
  { // handle events
    gl_window_event evt[10];
    size_t cnt = gl_get_events(evt, array_count(evt));
    for(size_t i = 0; i < cnt; i++){
      if(evt[i].type == EVT_KEY_DOWN){
	//printf("%i \n", evt[i].key.key);
      }
    }
  }


  scheme_load_string(ctx->sc, "(update)");
  current_context = ctx;
  current_context->dt = dt;
  gl_window_make_current(ctx->win);
  
  blit_begin(BLIT_MODE_UNIT);
  var blit3d = ctx->blit3d;
  var bg_color = ctx->bg_color;
  glClearColor(bg_color.x,bg_color.y,bg_color.z,bg_color.w);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  blit3d_context_load(blit3d);
  
  for(u32 i = 0; i < ctx->shown_objects->count; i++){
    render_it = 0;
    render_polygon(ctx, mat4_identity(), ctx->shown_objects->key[i + 1], vec4_new(1,1,1,1));  
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
  //var orig_data = data;
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

pointer load_poly(scheme * sc, pointer args){
  size_t c = 0;
  f32 * fs = pointer_to_floats(sc, args, &c);

  var object =  current_context->polygons + current_context->current_symbol;
  if(object->verts == NULL)
    object->verts = blit3d_polygon_new();
  blit3d_polygon_load_data(object->verts, fs, c * sizeof(f32));
  blit3d_polygon_configure(object->verts, 3);
  free(fs);
  return sc->NIL;
}

pointer set_bg_color(scheme * sc, pointer args){
  size_t count;
  f32 * colors = pointer_to_floats(sc, args, &count);
  if(count == 4)
    current_context->bg_color = vec4_new(colors[0] ,colors[1], colors[2], colors[3]);
  free(colors);
  return sc->NIL;
}


pointer print_result(scheme * sc, pointer data){
  printf("\nhej?? %i %i \n", pair_cdr(data) == sc->NIL, 0);
  return data;
}

u32 polygon_new(){
  u32 newid = current_context->polygon_count;
  current_context->polygon_count += 1;
  current_context->polygons = realloc(current_context->polygons, sizeof(current_context->polygons[0]) * current_context->polygon_count);
  current_context->polygons[newid] = (polygon){0};
  current_context->polygons[newid].scale = vec3_new(1,1,1);
  current_context->polygons[newid].color = vec4_new(1,1,1,1);
  return newid;
}

u32 get_model_id_from_pair(pointer pair){
  var car = pair_car(pair);
  ASSERT(is_symbol(car) && strcmp(symname(car), "model") == 0);
  var modelid = pair_cdr(pair);
  ASSERT(is_integer(modelid));
  var id = (u32)ivalue(modelid);
  return id;
}

u32 get_model_id_from_args(pointer args){
  var ptr = pair_car(args);
  // (model model-id)
  return get_model_id_from_pair(ptr);
}

pointer show_model(scheme * sc, pointer args){

  u32 id = get_model_id_from_args(args);
  u32_to_u32_table_set(current_context->shown_objects, id, 0);
  printf("show model %i\n", id);
  return sc->NIL;
}

pointer unshow_model(scheme * sc, pointer args){
  u32 id = get_model_id_from_args(args);
  u32_to_u32_table_unset(current_context->shown_objects, id);
  printf("show model %i\n", id);
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

pointer scale_model(scheme * sc, pointer args){
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
  object->scale = offset;
 end:;
  free(f);
  return sc->NIL; 
      
}


pointer rotate_model(scheme * sc, pointer args){
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
  object->rotation = offset;
 end:;
  free(f);
  return sc->NIL; 
      
}

pointer set_color(scheme * sc, pointer args){
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
    
    var m = rotate_offset_3d_transform(fargs[0], fargs[1], fargs[2],
				       fargs[3], fargs[4], fargs[5]);
    current_context->camera_matrix = m;
  }
  free(fargs);
  return sc->NIL;
}

void create_sub_object(u32 polygon, u32 sub_polygon){
  size_t indexes[10];
  size_t cnt;
  size_t index = 0;
  var ctx = current_context;
  while((cnt = u32_to_u32_table_iter(ctx->poly_to_sub_object, &polygon, 1, NULL, indexes, array_count(indexes), &index))){
    for(size_t i = 0 ; i < cnt; i++){
      u32 ref = ctx->poly_to_sub_object->value[indexes[i]];
      if(ref == sub_polygon)
	return;
    }
  }
  u32_to_u32_table_set(ctx->poly_to_sub_object, polygon, sub_polygon);
}


/*
u32 sym_to_sub_object(pointer arg){
  ASSERT(is_symbol(arg));
  u32 id;
  var sym =  arg;
  if(!ptr_to_u32_table_try_get(current_context->sub_object_list, &sym, &id))
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
  pointer ptr =  x[0];
  u32 a = polygon_sym_to_id(x[1]);
  u32 b = polygon_sym_to_id(x[2]);
  var ctx = current_context;
  if(!ptr_to_u32_table_try_get(current_context->sub_object_list, &ptr, &id)){
    u32 newid = ctx->sub_object_count;
    ctx->sub_object_count += 1;
    ctx->sub_objects = realloc(current_context->sub_objects, sizeof(current_context->sub_objects[0]) * current_context->sub_object_count);
    ctx->sub_objects[newid] = (sub_object){0};
    ctx->sub_objects[newid].transform = mat4_identity();
    ctx->sub_objects[newid].color = vec4_new(1,1,1,1);
    
    id = newid;
    ptr_to_u32_table_set(current_context->sub_object_list, ptr, id);
  }

  printf("Link %i %i %i\n", id, a ,b);
  
  size_t indexes[10];
  size_t cnt;
  size_t index = 0;
  while((cnt = u32_to_u32_table_iter(ctx->poly_to_sub_object, &a, 1, NULL, indexes, array_count(indexes), &index))){
    for(size_t i = 0 ; i < cnt; i++){
      u32 ref = ctx->poly_to_sub_object->value[indexes[i]];
      if(ref == id){
	goto end;
      }
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
*/
pointer key_is_down(scheme * sc, pointer args){
  if(is_integer(pair_car(args)) == false)
    ERROR("Key is not integer");
  int keyid = ivalue(pair_car(args));
  bool isdown = gl_window_get_key_state(current_context->win, keyid);
  if(isdown)
    return sc->T;
  return sc->F;
}

pointer vec3_to_scheme(scheme * sc, vec3 v){
  pointer s_vec = sc->vptr->mk_vector(sc, 3);
  for(int i = 0; i < 3; i++){
    sc->vptr->set_vector_elem(s_vec, i, sc->vptr->mk_real(sc, v.data[i]));
  }
  return s_vec;
}

pointer camera_direction(scheme * sc, pointer args){
  var m = current_context->camera_matrix;
  for(int i = 0; i < 3; i++)
    m.data[3][i] = 0;
  vec3 v = mat4_mul_vec3(m, vec3_new(0,0,1));
  return vec3_to_scheme(sc, v);
}

pointer camera_right(scheme * sc, pointer args){
  var m = current_context->camera_matrix;
  for(int i = 0; i < 3; i++)
    m.data[3][i] = 0;
  m = mat4_rotate_Y(m, PI / 2.0);
  vec3 v = mat4_mul_vec3(m, vec3_new(0,0,1));
  return vec3_to_scheme(sc, v);
}

pointer get_dt(scheme * sc, pointer args){
  return sc->vptr->mk_real(sc, current_context->dt);
}
/*
pointer hide_sub_object(scheme * sc, pointer args){
  u32 id;  
  pointer ptr = pair_car(args);
  if(ptr_to_u32_table_try_get(current_context->sub_object_list, &ptr, &id)){
    var obj = current_context->sub_objects + id;
    obj->hidden = true;
  }
  return sc->NIL;
}

pointer show_sub_object(scheme * sc, pointer args){
  u32 id;  
  pointer ptr = pair_car(args);
  if(ptr_to_u32_table_try_get(current_context->sub_object_list, &ptr, &id)){
    var obj = current_context->sub_objects + id;
    obj->hidden = false;
  }
  return sc->NIL;
}

pointer sub_object_color(scheme * sc, pointer args){
  u32 id;  
  pointer ptr = pair_car(args);
  args = pair_cdr(args);
  
  size_t c = 0;
  f32 * fs = pointer_to_floats(sc, args, &c);
  
  if(c == 4 && ptr_to_u32_table_try_get(current_context->sub_object_list, &ptr, &id)){
    var obj = current_context->sub_objects + id;
    obj->color = vec4_new(fs[0], fs[1], fs[2], fs[3]);
  }
  return sc->NIL;  
  }*/

pointer load_model2(scheme * sc, pointer args){
  printf("Load model!\n");
  return sc->NIL;
}

pointer config_model(scheme * sc, pointer args){
  pointer ptr = pair_car(args);
  u32 id = get_model_id_from_pair(ptr);
  ptr = pair_cdr(args);
  while(ptr != sc->NIL){

    pointer pt2 = pair_car(ptr); //(poly 1 2 3)
    pointer sym = pair_car(pt2);
    if(is_symbol(sym)){
      char * symchars = symname(sym);
      bool ispoly = strcmp(symchars, "poly") == 0;
      bool iscolor = !ispoly && strcmp(symchars, "color") == 0;
      bool isscale = !iscolor && !ispoly && strcmp(symchars, "scale") == 0;
      bool isoffset = strcmp(symchars, "offset") == 0;
      bool ismodel = strcmp(symchars, "model") == 0;
      if(ispoly || iscolor || isscale || isoffset){
	// this is for model data that is only floats.
	size_t c = 0;
	f32 * fs = pointer_to_floats(sc, pair_cdr(pt2), &c);
	var object =  current_context->polygons + id;
	if(ispoly){
	  if(object->verts == NULL)
	    object->verts = blit3d_polygon_new();
	  blit3d_polygon_load_data(object->verts, fs, c * sizeof(f32));
	  blit3d_polygon_configure(object->verts, 3);
	}else if(iscolor){
	  memcpy(object->color.data, fs, MIN(4, c) * sizeof(fs[0]));
	  printf("COLOR: ");vec4_print(object->color);printf("\n");
	}else if(isscale){
	  memcpy(object->scale.data, fs, MIN(3, c) * sizeof(fs[0]));
	  printf("SCALE: ");vec3_print(object->scale);printf("\n");
	}else if(isoffset){
	  memcpy(object->offset.data, fs, MIN(3, c) * sizeof(fs[0]));
	  printf("OFFSET: ");vec3_print(object->scale);printf("\n");
	}
	free(fs);
      }
      if(ismodel){
	u32 sub_id = get_model_id_from_pair(pt2);
	create_sub_object(id, sub_id);
	printf("creating sub model: %i -> %i\n", id, sub_id);
      }

      
      printf("jaaa %i %s\n", sym, symchars);
    }
    ptr = pair_cdr(ptr);
  }
  //ASSERT(is_list(ptr));
  printf("Config model! %i\n", id);
  return sc->NIL;
}

pointer object_id_new(scheme * sc, pointer args){
  u32 objectid = polygon_new();  
  return mk_integer(sc, (long)objectid);
}

context * context_init(gl_window * win){
  static scheme_registerable reg[] = {
    {.f = print_result, .name = "print2"},
    {.f = load_poly, .name = "load-poly"},
    {.f = set_bg_color, .name = "set-bg-color"},
    {.f = show_model, .name = "show-model"},
    {.f = unshow_model, .name = "unshow-model"},
    {.f = set_color, .name = "set-color"},
    {.f = scale_model, .name= "scale-model"},
    {.f = offset_model, .name= "offset-model"},
    {.f = rotate_model, .name="rotate-model"},
    {.f = set_perspective, .name="perspective"},
    {.f = set_orthographic, .name="orthographic"},
    {.f = set_camera, .name="set-camera"},
    {.f = key_is_down, .name="key-is-down"},
    {.f = camera_direction, .name="camera-direction"},
    {.f = camera_right, .name="camera-right"},
    {.f = get_dt, .name="get-dt"}
    ,{.f = config_model, .name="config-model"}
    ,{.f = object_id_new, .name = "object-new"}
  };
   context * ctx = alloc0(sizeof(ctx[0]));
   ctx->blit3d  = blit3d_context_new();
   var sc = scheme_init_new();
   ctx->sc = sc;
   scheme_set_output_port_file(sc, stdout);
   scheme_register_foreign_func_list(sc, reg, array_count(reg));
   current_context = ctx;
   ctx->shown_objects = u32_to_u32_table_create(NULL);
   ctx->poly_to_sub_object = u32_to_u32_table_create(NULL);
   ((bool *) &ctx->poly_to_sub_object->is_multi_table)[0] = true;
   ctx->running = 1;
   ctx->win = win;
   ctx->view_matrix = mat4_identity();
   ctx->camera_matrix = mat4_identity();
   return ctx;
}


void context_load_lisp(context * ctx, const char * file){
  u32_to_u32_table_clear(ctx->shown_objects);
  u32_to_u32_table_clear(ctx->poly_to_sub_object);
  ctx->polygon_count = 0;
  var fin = fopen(file, "r");
  scheme_load_named_file(ctx->sc, fin, file);
  fclose(fin);  
}


void context_load_lisp_string(context * ctx, const char * string, size_t length){
  scheme_load_string(ctx->sc, string);
}
