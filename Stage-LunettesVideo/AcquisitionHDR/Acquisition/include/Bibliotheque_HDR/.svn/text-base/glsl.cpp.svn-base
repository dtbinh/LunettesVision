#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>

#if WIN32
#include <windows.h>
#include <time.h>
#else
#include <sys/time.h>
#include <unistd.h>
#endif
#include <GL/glew.h>


#define VERBOSE 0 
#define VERBOSE_DEBUG 0


void extFailed(char *message)
{
    printf("%-32s  [FAILED]\n", message);
    fflush(stdout);
}

void extSupported(char *message)
{
#if VERBOSE
    printf("%-32s  [  OK  ]\n", message);
    fflush(stdout);
#endif
}


int extIsSupported(char *ext, char *message, int verbose)
{
    if(glewIsSupported(ext))
    {
        if(verbose > 0)
            extSupported(message);
        
        return 0;
    }
    else
    {
#if VERBOSE_DEBUG	// n'affiche pas les tentatives
        if(verbose > 0)
            extFailed(message);
#endif
        return -1;	
    }
}

#ifdef WIN32
char *strsep(char **str, char *delim)
{
    char *tmp;
    char *s;
    int len;

    if(str==NULL || *str==NULL)
        return NULL;

    //	
    tmp=*str;
    if(tmp[0]==0)
    {
        *str= NULL;
        return NULL;
    }

    len= strcspn(tmp, delim);
    if(len==0)
    {
        *str= NULL;
        return NULL;
    }
    
    //
    s= tmp;
    
    if(tmp[len]==0)
        tmp= NULL;
    
    else
    {
        tmp[len]= 0;
        tmp= &tmp[len +1];
    }
    
    //
    *str= tmp;
    return s;
}

#endif

int get_libdir(char *dest, int dest_len, char *argv0, char **env)	
{
    char pathtmp[1024];
    struct stat statentry;
    char pathstr[]= "PATH";
    char *path;
    char *tmp, *s;
    int pathstr_len= strlen(pathstr);
    int i;
    

#ifdef WIN32
    s= strrchr(argv0, '\\');
#else
    s= strrchr(argv0, '/');
#endif
    
    if(s != NULL)
        argv0= s+1;
            
    #if VERBOSE_DEBUG
        printf("%s\n", argv0);
    #endif

    // par defaut renvoie le repertoire courant
    strcpy(dest, ".");
    
    //
    for(i= 0; env[i]!=NULL; i++)
    {
        if(strncmp(pathstr, env[i], pathstr_len)!=0)
            continue;

        tmp= strchr(env[i], '=');
        if(tmp != NULL)
        {
            path= strdup(tmp +1);
            
    #if VERBOSE_DEBUG
            printf("%s\n", path);
    #endif
            //
            tmp= path; 
            #ifdef WIN32
                s= strsep(&tmp, ";");
            #else
                s= strsep(&tmp, ":");
            #endif
            while(s != NULL)
            {
                snprintf(pathtmp, sizeof(pathtmp), "%s/%s", s, argv0);

                if(stat(pathtmp, &statentry) == 0)
                {
            #if VERBOSE
                    printf("program search path :\n    %s\n", s);
            #endif
                    
                    if(dest != NULL)
                        strncpy(dest, s, dest_len);
                    
                    free(path);
                    return 0;
                }
        #if VERBOSE_DEBUG
                else
                    printf("%s ...\n", s);
        #endif
                
                //
        #ifdef WIN32
                s= strsep(&tmp, ";");
        #else
                s= strsep(&tmp, ":");
        #endif
            }
            
            //
            free(path);
            return -1;
        }
    }
    
    return -1;
}


int glsl_loadSource(char **str, int *n, char *fname)
{
    FILE *in;
    int size;
    
    *n= 0;
    *str= NULL;
    size= 0;		

    in= fopen(fname, "r");
    if(in==NULL)
        return -1;

    do
    {		
        size+= 4096;
        *str= (char *) realloc(*str, size);
        assert(*str != NULL);

        (*n)+= (int) fread(*str + *n, 1, 4096, in);
    }
    while(!feof(in));
    
    fclose(in);

    //
    (*str)[*n]= 0;
    
    return 0;
}


void glsl_infoLog(GLhandleARB object)
{
    char *log;
    int length= 0;

    glGetObjectParameterivARB(object, GL_OBJECT_INFO_LOG_LENGTH_ARB, &length);

    log= (char *)malloc(length);
    assert(log != NULL);
    
    glGetInfoLogARB(object, length, &length, log);
    
    printf("\n%s", log);
    
    free(log);
}


/* affiche les uniforms 
 */
void glsl_programUniforms(GLhandleARB program)
{
    GLenum type;
    GLint n;
    GLint maxLength, length, size;
    int i;

    glGetObjectParameterivARB(program, GL_OBJECT_ACTIVE_UNIFORMS_ARB, &n);
    glGetObjectParameterivARB(program, GL_OBJECT_ACTIVE_UNIFORM_MAX_LENGTH_ARB, &maxLength);
    
    char *name = (char *) malloc(maxLength);
    
    printf("uniforms (%d)\n", n);
    for(i = 0; i < n; i++)
    {
        glGetActiveUniformARB(program, i, maxLength, &length, &size, &type, name);
        switch(type)
        {
            /*
            GL_FLOAT, GL_FLOAT_VEC2_ARB, GL_FLOAT_VEC3_ARB, GL_FLOAT_VEC4_ARB, 
            GL_INT, GL_INT_VEC2_ARB, GL_INT_VEC3_ARB, GL_INT_VEC4_ARB, 
            GL_BOOL_ARB, GL_BOOL_VEC2_ARB, GL_BOOL_VEC3_ARB, GL_BOOL_VEC4_ARB, 
            GL_FLOAT_MAT2_ARB, 
            GL_FLOAT_MAT3_ARB, 
            GL_FLOAT_MAT4_ARB 
            
            texture
            GL_TEXTURE_2D
            GL_TEXTURE_RECTANGLE_ARB
            GL_TEXTURE_DEPTH ?
            */
            
            case GL_FLOAT:
                printf("  float %s\n", name);
            break;
            
            case GL_FLOAT_VEC2_ARB:
                printf("  vec2 %s\n", name);
            break;
            
            case GL_FLOAT_VEC3_ARB:
                printf("  vec3 %s\n", name);
            break;
            
            case GL_FLOAT_VEC4_ARB:
                printf("  vec4 %s\n", name);
            break;

            case GL_FLOAT_MAT4_ARB:
                printf("  mat4 %s\n", name);
            break;
            
            case GL_TEXTURE_2D:
                printf("  sampler2D %s\n", name);
            break;	
            
            case GL_TEXTURE_RECTANGLE_ARB:
                printf("  sampler2DRect %s\n", name);
            break;	
            
            default:
                printf("  type (0x%X) %s\n", type, name);
            break;
        }
    }
    
    fflush(stdout);
    free(name);
}


void glsl_addShader(GLhandleARB program, const GLcharARB *shaderSource, GLenum shaderType)
{
    GLenum err;
    GLhandleARB shader;
    GLint compiled;
    
    assert(program != 0);
    assert(shaderSource != NULL);
    assert(shaderType != 0);
    
    shader= glCreateShaderObjectARB(shaderType);
    assert(shader != 0);
    
    glShaderSourceARB(shader, 1, &shaderSource, NULL);
    
    // compile shader object
    glCompileShaderARB(shader);
    
    // check if shader compiled
    compiled= 0;
    glGetObjectParameterivARB(shader, GL_OBJECT_COMPILE_STATUS_ARB, &compiled);
    if(compiled==0)
    {
        glsl_infoLog(shader);
        extFailed("compile");
        exit(1);
    }
#if VERBOSE_DEBUG
    else
        extSupported("compile");
#endif
    
    // attach shader to program object
    glAttachObjectARB(program, shader);
    
    // delete object, no longer needed
    glDeleteObjectARB(shader);
    
    err= glGetError();
    if(err != GL_NO_ERROR)
        fprintf(stderr, "\nopenGL Error  %s:%d %s()\n%s\n", __FILE__, __LINE__, __FUNCTION__, gluErrorString(err));
}


GLhandleARB initglsl(char *argv0, char **env, char *vfname, char *ffname)
{
    char libdir[1024];
    char tmpfname[1024];
    GLcharARB *shaderData;
    GLhandleARB program;
    GLint linked;
    GLint validated;
    int shaderData_length;
    
    // . glsl
    if(extIsSupported("GL_ARB_shader_objects GL_ARB_vertex_shader GL_ARB_fragment_shader", 
        "openGL Shading Language", 1) < 0)
            exit(0);

    program= glCreateProgramObjectARB();
    
    //
    get_libdir(libdir, sizeof(libdir), argv0, env);
    
    // vertex
    snprintf(tmpfname, sizeof(tmpfname), "%s/%s", libdir, vfname);
    if(glsl_loadSource(&shaderData, &shaderData_length, tmpfname) == 0)
    {
        glsl_addShader(program, shaderData, GL_VERTEX_SHADER_ARB);
        
        free(shaderData);
        extSupported("vertex program");
    }
    else
    {
        extFailed("vertex program");
        exit(1);
    }
    
    // fragment
    snprintf(tmpfname, sizeof(tmpfname), "%s/%s", libdir, ffname);
    if(glsl_loadSource(&shaderData, &shaderData_length, tmpfname) == 0)
    {
        glsl_addShader(program, shaderData, GL_FRAGMENT_SHADER_ARB);
        
        free(shaderData);
        extSupported("fragment program");
    }
    else
    {
        extFailed("fragment program");
        exit(1);
    }
    
    // link
    glLinkProgramARB(program);
    
    linked= 0;
    glGetObjectParameterivARB(program, GL_OBJECT_LINK_STATUS_ARB, &linked);
    if(linked==0)
    {
        glsl_infoLog(program);
        extFailed("link");
        exit(1);
    }
#if VERBOSE_DEBUG
    else
        extSupported("link");
#endif
    
    // validate
    glValidateProgramARB(program);
    
    validated= 0;
    glGetObjectParameterivARB(program, GL_OBJECT_VALIDATE_STATUS_ARB, &validated);
    if(validated == 0)
    {
        glsl_infoLog(program);
        extFailed("validation");
        exit(1);
    }
#if VERBOSE_DEBUG
    else
        extSupported("validation");
#endif
    
    glUseProgramObjectARB(program);

#if VERBOSE_DEBUG
    glsl_programUniforms(program);
#endif
    
    return program;
}
 
