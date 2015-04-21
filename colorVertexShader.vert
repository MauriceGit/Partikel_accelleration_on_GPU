#version 430 core

layout (location = 0) in vec3 vertPos; 
uniform mat4 viewMatrix, projMatrix; 

void main(){ 
	gl_Position =  projMatrix * viewMatrix * vec4(vertPos, 1.0); 
}
