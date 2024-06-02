#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <GL/glew.h>

// Vertex shader source
char *vertex_shader_source = 
    "#version 330 core\n"
    "layout (location = 0) in vec3 position;\n"
    "layout (location = 1) in vec3 color;\n"
    "out vec3 vertex_color;\n"
    "void main() {\n"
    "    gl_Position = vec4(position, 1.0);\n"
    "    vertex_color = color;\n"
    "}\n";

// Fragment shader source
char *fragment_shader_source = 
    "#version 330 core\n"
    "in vec3 vertex_color;\n"
    "out vec4 frag_color;\n"
    "void main() {\n"
    "    frag_color = vec4(vertex_color, 1.0);\n"
    "}\n";

// Vertex data for a triangle
float vertex_data[] = {
    -0.5f, -0.5f, 1.0f,     1.0f, 0.0f, 0.0f, // Bottom-left vertex color (red)
    0.5f, -0.5f, 1.0f,      0.0f, 1.0f, 0.0f, // Bottom-right vertex color (green)
    0.0f, 0.5f, 1.0f,       0.0f, 0.0f, 1.0f  // Top vertex color (blue)
};

// Define struct named model
typedef struct {
    // Add members here
    GLuint vao;
    GLuint vbo;
    GLuint shader_program;
} model;

// create triangle model object
model triangle;

void initialize_sdl2() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        exit(1);
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
}

void create_window() {
    // Create an application window
    SDL_Window *win = SDL_CreateWindow("Hello World!", 100, 100, 640, 480, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
    if (win == NULL) {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        exit(1);
    }
    // Create an OpenGL context associated with the window.
    SDL_GLContext glcontext = SDL_GL_CreateContext(win);
    if (glcontext == NULL) {
        fprintf(stderr, "SDL_GL_CreateContext Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(win);
        SDL_Quit();
        exit(1);
    }

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    GLenum glewError = glewInit();
    if (glewError != GLEW_OK) {
        fprintf(stderr, "glewInit Error: %s\n", glewGetErrorString(glewError));
        SDL_GL_DeleteContext(glcontext);
        SDL_DestroyWindow(win);
        SDL_Quit();
        exit(1);
    }
}

GLuint compile_shader(char *shader_source, GLenum shader_type) {
    GLuint shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, (const GLchar**)&shader_source, NULL);
    glCompileShader(shader);
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar info_log[512];
        glGetShaderInfoLog(shader, 512, NULL, info_log);
        fprintf(stderr, "Shader compilation failed: %s\n", info_log);
        exit(1);
    }
    return shader;
}

GLuint create_program(GLuint vertex_shader, GLuint fragment_shader) {
    GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);
    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar info_log[512];
        glGetProgramInfoLog(program, 512, NULL, info_log);
        fprintf(stderr, "Shader linking failed: %s\n", info_log);
        exit(1);
    }
    glUseProgram(program);
    return program;

}

GLuint create_vertex_buffer() {
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);
    return vbo;
}

GLuint create_vertex_array() {
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    return vao;
}

void initialize()
{
    initialize_sdl2();
    create_window();

    // Compile and link shaders into a shader program
    GLuint vertex_shader = compile_shader(vertex_shader_source, GL_VERTEX_SHADER);
    GLuint fragment_shader = compile_shader(fragment_shader_source, GL_FRAGMENT_SHADER);

    GLuint shader_program = create_program(vertex_shader, fragment_shader);

    // Create a vertex buffer object and vertex array object
    GLuint vbo = create_vertex_buffer();
    GLuint vao = create_vertex_array();

    // set up model
    triangle.vao = vao;
    triangle.vbo = vbo;
    triangle.shader_program = shader_program;

    
}

void draw() {
    // use models vao, vbo, and shader program
    glBindVertexArray(triangle.vao);
    glBindBuffer(GL_ARRAY_BUFFER, triangle.vbo);
    glUseProgram(triangle.shader_program);

    glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    SDL_GL_SwapWindow(SDL_GL_GetCurrentWindow());
}

void main_loop() {
    // Event loop
    SDL_Event e;
    while (1) {
        // Poll for events from SDL
        if (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                break;
            }

            // Handle key presses
            else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_ESCAPE) {
                    break;
                }
            }

        draw();
        }
    }
}

int main() {
    initialize();
    main_loop();
    return 0;
}