/*
 * GLAD - OpenGL Loader-Generator
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glad/glad.h>

#ifdef _WIN32
#include <windows.h>
static HMODULE libgl;

static void open_gl(void) {
    libgl = LoadLibraryA("opengl32.dll");
}

static void close_gl(void) {
    FreeLibrary(libgl);
}

static void *get_proc(const char *namez) {
    void *p = NULL;
    if(libgl != NULL) {
        p = (void *)wglGetProcAddress(namez);
    }
    if(p == NULL) {
        p = (void *)GetProcAddress(libgl, namez);
    }
    return p;
}
#else
#include <dlfcn.h>
static void *libgl;

static void open_gl(void) {
#ifdef __APPLE__
    static const char *NAMES[] = {
        "/System/Library/Frameworks/OpenGL.framework/OpenGL",
        "/System/Library/Frameworks/OpenGL.framework/Versions/Current/OpenGL"
    };
#else
    static const char *NAMES[] = {"libGL.so.1", "libGL.so"};
#endif

    unsigned int index = 0;
    for(index = 0; index < (sizeof(NAMES) / sizeof(NAMES[0])); index++) {
        libgl = dlopen(NAMES[index], RTLD_NOW | RTLD_GLOBAL);
        if(libgl != NULL) {
            return;
        }
    }
}

static void close_gl(void) {
    if(libgl != NULL) {
        dlclose(libgl);
    }
}

static void *get_proc(const char *namez) {
    void *p;
    if(libgl != NULL) {
        p = dlsym(libgl, namez);
        if(p != NULL) {
            return p;
        }
    }
    return NULL;
}
#endif

static void* load_gl_func(const char *name) {
    void *proc = get_proc(name);
    if(proc == NULL) {
        fprintf(stderr, "Failed to load function: %s\n", name);
    }
    return proc;
}

/* Function pointer definitions */
PFNGLCLEARPROC glad_glClear = NULL;
PFNGLCLEARCOLORPROC glad_glClearColor = NULL;
PFNGLVIEWPORTPROC glad_glViewport = NULL;
PFNGLENABLEPROC glad_glEnable = NULL;
PFNGLDISABLEPROC glad_glDisable = NULL;
PFNGLBLENDFUNCPROC glad_glBlendFunc = NULL;
PFNGLCULLFACEPROC glad_glCullFace = NULL;
PFNGLFRONTFACEPROC glad_glFrontFace = NULL;
PFNGLGETSTRINGPROC glad_glGetString = NULL;
PFNGLGETERRORPROC glad_glGetError = NULL;
PFNGLGETINTEGERVPROC glad_glGetIntegerv = NULL;

PFNGLGENBUFFERSPROC glad_glGenBuffers = NULL;
PFNGLDELETEBUFFERSPROC glad_glDeleteBuffers = NULL;
PFNGLBINDBUFFERPROC glad_glBindBuffer = NULL;
PFNGLBUFFERDATAPROC glad_glBufferData = NULL;
PFNGLBUFFERSUBDATAPROC glad_glBufferSubData = NULL;

PFNGLGENVERTEXARRAYSPROC glad_glGenVertexArrays = NULL;
PFNGLDELETEVERTEXARRAYSPROC glad_glDeleteVertexArrays = NULL;
PFNGLBINDVERTEXARRAYPROC glad_glBindVertexArray = NULL;
PFNGLVERTEXATTRIBPOINTERPROC glad_glVertexAttribPointer = NULL;
PFNGLENABLEVERTEXATTRIBARRAYPROC glad_glEnableVertexAttribArray = NULL;
PFNGLDISABLEVERTEXATTRIBARRAYPROC glad_glDisableVertexAttribArray = NULL;

PFNGLCREATESHADERPROC glad_glCreateShader = NULL;
PFNGLDELETESHADERPROC glad_glDeleteShader = NULL;
PFNGLSHADERSOURCEPROC glad_glShaderSource = NULL;
PFNGLCOMPILESHADERPROC glad_glCompileShader = NULL;
PFNGLGETSHADERIVPROC glad_glGetShaderiv = NULL;
PFNGLGETSHADERINFOLOGPROC glad_glGetShaderInfoLog = NULL;

PFNGLCREATEPROGRAMPROC glad_glCreateProgram = NULL;
PFNGLDELETEPROGRAMPROC glad_glDeleteProgram = NULL;
PFNGLATTACHSHADERPROC glad_glAttachShader = NULL;
PFNGLDETACHSHADERPROC glad_glDetachShader = NULL;
PFNGLLINKPROGRAMPROC glad_glLinkProgram = NULL;
PFNGLUSEPROGRAMPROC glad_glUseProgram = NULL;
PFNGLGETPROGRAMIVPROC glad_glGetProgramiv = NULL;
PFNGLGETPROGRAMINFOLOGPROC glad_glGetProgramInfoLog = NULL;
PFNGLGETUNIFORMLOCATIONPROC glad_glGetUniformLocation = NULL;
PFNGLUNIFORM1FPROC glad_glUniform1f = NULL;
PFNGLUNIFORM2FPROC glad_glUniform2f = NULL;
PFNGLUNIFORM3FPROC glad_glUniform3f = NULL;
PFNGLUNIFORM4FPROC glad_glUniform4f = NULL;
PFNGLUNIFORM1IPROC glad_glUniform1i = NULL;
PFNGLUNIFORMMATRIX4FVPROC glad_glUniformMatrix4fv = NULL;

PFNGLDRAWARRAYSPROC glad_glDrawArrays = NULL;
PFNGLDRAWELEMENTSPROC glad_glDrawElements = NULL;

PFNGLGENTEXTURESPROC glad_glGenTextures = NULL;
PFNGLDELETETEXTURESPROC glad_glDeleteTextures = NULL;
PFNGLBINDTEXTUREPROC glad_glBindTexture = NULL;
PFNGLTEXPARAMETERIPROC glad_glTexParameteri = NULL;
PFNGLTEXIMAGE2DPROC glad_glTexImage2D = NULL;
PFNGLGENERATEMIPMAPPROC glad_glGenerateMipmap = NULL;
PFNGLACTIVETEXTUREPROC glad_glActiveTexture = NULL;

PFNGLGENFRAMEBUFFERSPROC glad_glGenFramebuffers = NULL;
PFNGLDELETEFRAMEBUFFERSPROC glad_glDeleteFramebuffers = NULL;
PFNGLBINDFRAMEBUFFERPROC glad_glBindFramebuffer = NULL;
PFNGLFRAMEBUFFERTEXTURE2DPROC glad_glFramebufferTexture2D = NULL;
PFNGLCHECKFRAMEBUFFERSTATUSPROC glad_glCheckFramebufferStatus = NULL;

PFNGLGENRENDERBUFFERSPROC glad_glGenRenderbuffers = NULL;
PFNGLDELETERENDERBUFFERSPROC glad_glDeleteRenderbuffers = NULL;
PFNGLBINDRENDERBUFFERPROC glad_glBindRenderbuffer = NULL;
PFNGLRENDERBUFFERSTORAGEPROC glad_glRenderbufferStorage = NULL;
PFNGLFRAMEBUFFERRENDERBUFFERPROC glad_glFramebufferRenderbuffer = NULL;

static void load_GL_VERSION_1_0(void* (*load)(const char *name)) {
    if(!glad_glClear) glad_glClear = (PFNGLCLEARPROC)load("glClear");
    if(!glad_glClearColor) glad_glClearColor = (PFNGLCLEARCOLORPROC)load("glClearColor");
    if(!glad_glViewport) glad_glViewport = (PFNGLVIEWPORTPROC)load("glViewport");
    if(!glad_glEnable) glad_glEnable = (PFNGLENABLEPROC)load("glEnable");
    if(!glad_glDisable) glad_glDisable = (PFNGLDISABLEPROC)load("glDisable");
    if(!glad_glBlendFunc) glad_glBlendFunc = (PFNGLBLENDFUNCPROC)load("glBlendFunc");
    if(!glad_glCullFace) glad_glCullFace = (PFNGLCULLFACEPROC)load("glCullFace");
    if(!glad_glFrontFace) glad_glFrontFace = (PFNGLFRONTFACEPROC)load("glFrontFace");
    if(!glad_glGetString) glad_glGetString = (PFNGLGETSTRINGPROC)load("glGetString");
    if(!glad_glGetError) glad_glGetError = (PFNGLGETERRORPROC)load("glGetError");
    if(!glad_glGetIntegerv) glad_glGetIntegerv = (PFNGLGETINTEGERVPROC)load("glGetIntegerv");
}

static void load_GL_VERSION_1_5(void* (*load)(const char *name)) {
    if(!glad_glGenBuffers) glad_glGenBuffers = (PFNGLGENBUFFERSPROC)load("glGenBuffers");
    if(!glad_glDeleteBuffers) glad_glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)load("glDeleteBuffers");
    if(!glad_glBindBuffer) glad_glBindBuffer = (PFNGLBINDBUFFERPROC)load("glBindBuffer");
    if(!glad_glBufferData) glad_glBufferData = (PFNGLBUFFERDATAPROC)load("glBufferData");
    if(!glad_glBufferSubData) glad_glBufferSubData = (PFNGLBUFFERSUBDATAPROC)load("glBufferSubData");
}

static void load_GL_VERSION_2_0(void* (*load)(const char *name)) {
    if(!glad_glCreateShader) glad_glCreateShader = (PFNGLCREATESHADERPROC)load("glCreateShader");
    if(!glad_glDeleteShader) glad_glDeleteShader = (PFNGLDELETESHADERPROC)load("glDeleteShader");
    if(!glad_glShaderSource) glad_glShaderSource = (PFNGLSHADERSOURCEPROC)load("glShaderSource");
    if(!glad_glCompileShader) glad_glCompileShader = (PFNGLCOMPILESHADERPROC)load("glCompileShader");
    if(!glad_glGetShaderiv) glad_glGetShaderiv = (PFNGLGETSHADERIVPROC)load("glGetShaderiv");
    if(!glad_glGetShaderInfoLog) glad_glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)load("glGetShaderInfoLog");
    if(!glad_glCreateProgram) glad_glCreateProgram = (PFNGLCREATEPROGRAMPROC)load("glCreateProgram");
    if(!glad_glDeleteProgram) glad_glDeleteProgram = (PFNGLDELETEPROGRAMPROC)load("glDeleteProgram");
    if(!glad_glAttachShader) glad_glAttachShader = (PFNGLATTACHSHADERPROC)load("glAttachShader");
    if(!glad_glDetachShader) glad_glDetachShader = (PFNGLDETACHSHADERPROC)load("glDetachShader");
    if(!glad_glLinkProgram) glad_glLinkProgram = (PFNGLLINKPROGRAMPROC)load("glLinkProgram");
    if(!glad_glUseProgram) glad_glUseProgram = (PFNGLUSEPROGRAMPROC)load("glUseProgram");
    if(!glad_glGetProgramiv) glad_glGetProgramiv = (PFNGLGETPROGRAMIVPROC)load("glGetProgramiv");
    if(!glad_glGetProgramInfoLog) glad_glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)load("glGetProgramInfoLog");
    if(!glad_glGetUniformLocation) glad_glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)load("glGetUniformLocation");
    if(!glad_glUniform1f) glad_glUniform1f = (PFNGLUNIFORM1FPROC)load("glUniform1f");
    if(!glad_glUniform2f) glad_glUniform2f = (PFNGLUNIFORM2FPROC)load("glUniform2f");
    if(!glad_glUniform3f) glad_glUniform3f = (PFNGLUNIFORM3FPROC)load("glUniform3f");
    if(!glad_glUniform4f) glad_glUniform4f = (PFNGLUNIFORM4FPROC)load("glUniform4f");
    if(!glad_glUniform1i) glad_glUniform1i = (PFNGLUNIFORM1IPROC)load("glUniform1i");
    if(!glad_glUniformMatrix4fv) glad_glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)load("glUniformMatrix4fv");
    if(!glad_glDrawArrays) glad_glDrawArrays = (PFNGLDRAWARRAYSPROC)load("glDrawArrays");
    if(!glad_glDrawElements) glad_glDrawElements = (PFNGLDRAWELEMENTSPROC)load("glDrawElements");
    if(!glad_glVertexAttribPointer) glad_glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)load("glVertexAttribPointer");
    if(!glad_glEnableVertexAttribArray) glad_glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)load("glEnableVertexAttribArray");
    if(!glad_glDisableVertexAttribArray) glad_glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)load("glDisableVertexAttribArray");
}

static void load_GL_VERSION_3_0(void* (*load)(const char *name)) {
    if(!glad_glGenVertexArrays) glad_glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)load("glGenVertexArrays");
    if(!glad_glDeleteVertexArrays) glad_glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)load("glDeleteVertexArrays");
    if(!glad_glBindVertexArray) glad_glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)load("glBindVertexArray");
    if(!glad_glGenFramebuffers) glad_glGenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC)load("glGenFramebuffers");
    if(!glad_glDeleteFramebuffers) glad_glDeleteFramebuffers = (PFNGLDELETEFRAMEBUFFERSPROC)load("glDeleteFramebuffers");
    if(!glad_glBindFramebuffer) glad_glBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC)load("glBindFramebuffer");
    if(!glad_glFramebufferTexture2D) glad_glFramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DPROC)load("glFramebufferTexture2D");
    if(!glad_glCheckFramebufferStatus) glad_glCheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUSPROC)load("glCheckFramebufferStatus");
    if(!glad_glGenRenderbuffers) glad_glGenRenderbuffers = (PFNGLGENRENDERBUFFERSPROC)load("glGenRenderbuffers");
    if(!glad_glDeleteRenderbuffers) glad_glDeleteRenderbuffers = (PFNGLDELETERENDERBUFFERSPROC)load("glDeleteRenderbuffers");
    if(!glad_glBindRenderbuffer) glad_glBindRenderbuffer = (PFNGLBINDRENDERBUFFERPROC)load("glBindRenderbuffer");
    if(!glad_glRenderbufferStorage) glad_glRenderbufferStorage = (PFNGLRENDERBUFFERSTORAGEPROC)load("glRenderbufferStorage");
    if(!glad_glFramebufferRenderbuffer) glad_glFramebufferRenderbuffer = (PFNGLFRAMEBUFFERRENDERBUFFERPROC)load("glFramebufferRenderbuffer");
    if(!glad_glGenerateMipmap) glad_glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC)load("glGenerateMipmap");
    if(!glad_glGenTextures) glad_glGenTextures = (PFNGLGENTEXTURESPROC)load("glGenTextures");
    if(!glad_glDeleteTextures) glad_glDeleteTextures = (PFNGLDELETETEXTURESPROC)load("glDeleteTextures");
    if(!glad_glBindTexture) glad_glBindTexture = (PFNGLBINDTEXTUREPROC)load("glBindTexture");
    if(!glad_glTexParameteri) glad_glTexParameteri = (PFNGLTEXPARAMETERIPROC)load("glTexParameteri");
    if(!glad_glTexImage2D) glad_glTexImage2D = (PFNGLTEXIMAGE2DPROC)load("glTexImage2D");
    if(!glad_glActiveTexture) glad_glActiveTexture = (PFNGLACTIVETEXTUREPROC)load("glActiveTexture");
}

int gladLoadGLLoader(void* (*load)(const char *name)) {
    if(load == NULL) return 0;

    load_GL_VERSION_1_0(load);
    load_GL_VERSION_1_5(load);
    load_GL_VERSION_2_0(load);
    load_GL_VERSION_3_0(load);

    return 1;
}
