#pragma once

#include <array>
#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <functional>
#include <map>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

using vec2 = glm::vec2; //Format ix x,y
using vec3 = glm::vec3; //Format is x,y,z
using vec4 = glm::vec4; //Format is x,y,z,w

#define se_bit_left(x) 1 << x
#include <glm/mat4x4.hpp>

#ifdef se_DEBUG //Debug only macros
#define se_log(x) std::cout << x << std::endl;
#else
#define se_Log(x) 
#endif

using vec2 = glm::vec2; //Format ix x,y
using vec3 = glm::vec3; //Format is x,y,z
using vec4 = glm::vec4; //Format is x,y,z,w
using mat3 = glm::mat3x3;
using mat4 = glm::mat4x4;

//Macros for singletons
#define se_layerStack LayerStack::get()
#define se_application ShmingoApp::get()
#define se_masterRenderer MasterRenderer::get()
#define se_uniformBuffer UniformBuffer::get()

//Useful macros
#define se_bit_left(x) 1 << x

struct se_bitfield { //Bit field of 8 bits
	unsigned int bits : 8; //Access using .bits
};

void clearOpenGLError();
void checkOpenGLError();


//When we do premake, we need to implement a ifdef for a windows platform here

#define se_KEY_SPACE	GLFW_KEY_SPACE
#define se_KEY_APOSTROPHE	GLFW_KEY_APOSTROPHE /* ' */
#define se_KEY_COMMA	GLFW_KEY_COMMA/* , */
#define se_KEY_MINUS	GLFW_KEY_MINUS /* - */
#define se_KEY_PERIOD	GLFW_KEY_PERIOD /* . */
#define se_KEY_SLASH	GLFW_KEY_SLASH /* / */
#define se_KEY_0	GLFW_KEY_0
#define se_KEY_1	GLFW_KEY_1
#define se_KEY_2	GLFW_KEY_2
#define se_KEY_3	GLFW_KEY_3
#define se_KEY_4	GLFW_KEY_4
#define se_KEY_5	GLFW_KEY_5
#define se_KEY_6	GLFW_KEY_6
#define se_KEY_7	GLFW_KEY_7
#define se_KEY_8	GLFW_KEY_8
#define se_KEY_9	GLFW_KEY_9
#define se_KEY_SEMICOLON	GLFW_KEY_SEMICOLON /* ; */
#define se_KEY_EQUAL	GLFW_KEY_EQUAL /* = */
#define se_KEY_A	GLFW_KEY_A
#define se_KEY_B	GLFW_KEY_B
#define se_KEY_C	GLFW_KEY_C
#define se_KEY_D	GLFW_KEY_D
#define se_KEY_E	GLFW_KEY_E
#define se_KEY_F	GLFW_KEY_F
#define se_KEY_G	GLFW_KEY_G
#define se_KEY_H	GLFW_KEY_H
#define se_KEY_I	GLFW_KEY_I
#define se_KEY_J	GLFW_KEY_J
#define se_KEY_K	GLFW_KEY_K
#define se_KEY_L	GLFW_KEY_L
#define se_KEY_M	GLFW_KEY_M
#define se_KEY_N	GLFW_KEY_N
#define se_KEY_O	GLFW_KEY_O
#define se_KEY_P	GLFW_KEY_P
#define se_KEY_Q	GLFW_KEY_Q
#define se_KEY_R	GLFW_KEY_R
#define se_KEY_S	GLFW_KEY_S
#define se_KEY_T	GLFW_KEY_T
#define se_KEY_U	GLFW_KEY_U
#define se_KEY_V	GLFW_KEY_V
#define se_KEY_W	GLFW_KEY_W
#define se_KEY_X	GLFW_KEY_X
#define se_KEY_Y	GLFW_KEY_Y
#define se_KEY_Z	GLFW_KEY_Z
#define se_KEY_BRACKET	GLFW_KEY_LEFT_BRACKET /* [ */
#define se_KEY_BACKSLASH	GLFW_KEY_BACKSLASH /* \ */
#define se_KEY_RIGHT_BRACKET	GLFW_KEY_RIGHT_BRACKET /* ] */
#define se_KEY_GRAVE_ACCENT	GLFW_KEY_GRAVE_ACCENT /* ` */
#define se_KEY_WORLD_1	GLFW_KEY_WORLD_1  /* non-US #1 */
#define se_KEY_WORLD_2	GLFW_KEY_WORLD_2  /* non-US #2 */
#define se_KEY_ESCAPE	GLFW_KEY_ESCAPE 
#define se_KEY_ENTER	GLFW_KEY_ENTER 
#define se_KEY_TAB	GLFW_KEY_TAB 
#define se_KEY_BACKSPACE	GLFW_KEY_BACKSPACE 
#define se_KEY_INSERT	GLFW_KEY_INSERT 
#define se_KEY_DELETE	GLFW_KEY_DELETE 
#define se_KEY_RIGHT	GLFW_KEY_RIGHT 
#define se_KEY_LEFT	GLFW_KEY_LEFT 
#define se_KEY_DOWN	GLFW_KEY_DOWN 
#define se_KEY_UP	GLFW_KEY_UP 
#define se_KEY_PAGE_UP	GLFW_KEY_PAGE_UP 
#define se_KEY_PAGE_DOWN	GLFW_KEY_PAGE_DOWN 
#define se_KEY_HOME 	GLFW_KEY_HOME 
#define se_KEY_AND 	GLFW_KEY_END 
#define se_KEY_CAPS_LOCK	GLFW_KEY_CAPS_LOCK 
#define se_KEY_SCROLL_LOCK	GLFW_KEY_SCROLL_LOCK
#define se_KEY_NUYM_LOCK	GLFW_KEY_NUM_LOCK 
#define se_KEY_PRINT_SCREEN	GLFW_KEY_PRINT_SCREEN 
#define se_KEY_PAUSE	GLFW_KEY_PAUSE 
#define se_KEY_F1	GLFW_KEY_F1
#define se_KEY_F2	GLFW_KEY_F2
#define se_KEY_F3	GLFW_KEY_F3
#define se_KEY_F4	GLFW_KEY_F4
#define se_KEY_F5	GLFW_KEY_F5
#define se_KEY_F6	GLFW_KEY_F6
#define se_KEY_F7	GLFW_KEY_F7
#define se_KEY_F8	GLFW_KEY_F8
#define se_KEY_F9	GLFW_KEY_F9
#define se_KEY_F10	GLFW_KEY_F10
#define se_KEY_F11	GLFW_KEY_F11
#define se_KEY_F12	GLFW_KEY_F12
#define se_KEY_F13	GLFW_KEY_F13
#define se_KEY_F14	GLFW_KEY_F14
#define se_KEY_F15	GLFW_KEY_F15
#define se_KEY_F16	GLFW_KEY_F16
#define se_KEY_F17	GLFW_KEY_F17
#define se_KEY_F18	GLFW_KEY_F18
#define se_KEY_F19	GLFW_KEY_F19
#define se_KEY_F20	GLFW_KEY_F20
#define se_KEY_F21	GLFW_KEY_F21
#define se_KEY_F22	GLFW_KEY_F22
#define se_KEY_F23	GLFW_KEY_F23
#define se_KEY_F24	GLFW_KEY_F24
#define se_KEY_F25	GLFW_KEY_F25
#define se_KEY_KP_0	GLFW_KEY_KP_0
#define se_KEY_KP_1	GLFW_KEY_KP_1
#define se_KEY_KP_2	GLFW_KEY_KP_2
#define se_KEY_KP_3	GLFW_KEY_KP_3
#define se_KEY_KP_4	GLFW_KEY_KP_4
#define se_KEY_KP_5	GLFW_KEY_KP_5
#define se_KEY_KP_6	GLFW_KEY_KP_6
#define se_KEY_KP_7	GLFW_KEY_KP_7
#define se_KEY_KP_8	GLFW_KEY_KP_8
#define se_KEY_KP_9	GLFW_KEY_KP_9
#define se_KEY_KP_DECIMAL	GLFW_KEY_KP_DECIMAL
#define se_KEY_KP_DIVIDE	GLFW_KEY_KP_DIVIDE
#define se_KEY_KP_MULTIPLY	GLFW_KEY_KP_MULTIPLY
#define se_KEY_KP_SUBTRACT	GLFW_KEY_KP_SUBTRACT
#define se_KEY_KP_ADD	GLFW_KEY_KP_ADD
#define se_KEY_KP_ENTER	GLFW_KEY_KP_ENTER
#define se_KEY_KP_EQUAL	GLFW_KEY_KP_EQUAL
#define se_KEY_LEFT_SHIFT	GLFW_KEY_LEFT_SHIFT
#define se_KEY_LEFT_CONTROL	 GLFW_KEY_LEFT_CONTROL
#define se_KEY_LEFT_ALT	 GLFW_KEY_LEFT_ALT
#define se_KEY_LEFT_SUPER	GLFW_KEY_LEFT_SUPER
#define se_KEY_RIGHT_SHIFT 	GLFW_KEY_RIGHT_SHIFT
#define se_KEY_RIGHT_CONTROL	GLFW_KEY_RIGHT_CONTROL
#define se_KEY_RIGHT_ALT	GLFW_KEY_RIGHT_ALT
#define se_KEY_RIGHT_SUPER	GLFW_KEY_RIGHT_SUPER
#define se_KEY_MENU 	GLFW_KEY_MENU
#define se_KEY_LAST GLFW_KEY_MENU