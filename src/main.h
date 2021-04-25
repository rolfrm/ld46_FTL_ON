
//test

typedef struct _context context;

bool context_running(context * ctx);

void render_update(context * ctx, float dt);
context * context_init(gl_window * win);

void context_load_lisp(context * ctx, const char * file);
void context_load_lisp_string(context * ctx, const char * string, size_t length);

// distance fields

typedef struct{
  void * userdata;
  f32 (* f)(vec3 pt, void * userdata);
}distance_field;
distance_field * distance_field_load(scheme * sc, pointer obj);
vec3 trace_closest_point(vec3 p, mat4 m1, mat4 m2, distance_field *d1, distance_field * d2, f32 * d);

// scheme extra
bool symbol_eq(pointer sym, const char * str);
float * pointer_to_floats(scheme * sc, pointer data, size_t * cnt);
int list_len(scheme * sc, pointer data);
void sc_print(scheme * sc, pointer item);
