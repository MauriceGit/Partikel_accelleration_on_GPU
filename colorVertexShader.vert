#version 430 core

layout (location = 0) in vec3 vertPos;
layout (location = 1) in float life;
uniform mat4 viewMatrix, projMatrix; 

out float color;

void main(){ 
	color = life;
	gl_Position =  projMatrix * viewMatrix * vec4(vertPos, 1.0); 
}
