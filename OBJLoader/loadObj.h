#ifndef Load_OBJ
#define Load_OBJ

#include <GL/glu.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include "../TGALoader/targa.h"

GLboolean load_obj( const char * filename, GLuint & object_list );

#endif
