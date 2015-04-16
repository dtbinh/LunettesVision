#ifndef _GLSL_H
#define _GLSL_H

#include <GL/glew.h>

extern void extFailed(char *message);
extern void extSupported(char *message);
extern int extIsSupported(char *ext, char *message, int verbose);

// extern GLhandleARB initglsl(char *vfname, char *ffname);
extern GLhandleARB initglsl(char *argv0, char **env, char *vfname, char *ffname);

extern void glsl_programUniforms(GLhandleARB program);

#endif

