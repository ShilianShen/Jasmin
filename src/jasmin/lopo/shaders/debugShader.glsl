// 版本声明（如GLSL 330 core）
#version 330 core

// 输入：顶点属性（来自CPU或模型数据）
layout(location = 0) in vec3 aPos;      // 顶点位置
layout(location = 1) in vec2 aTexCoord;  // UV坐标

// 输出：传递给片元着色器的数据
out vec2 TexCoord;

// Uniform变量（由CPU传递的全局数据）
uniform mat4 model;      // 模型矩阵
uniform mat4 view;       // 视图矩阵
uniform mat4 projection; // 投影矩阵

void main() {
    // 坐标变换：模型 → 裁剪空间
    gl_Position = projection * view * model * vec4(aPos, 1.0);

    // 传递UV坐标给片元着色器
    TexCoord = aTexCoord;
}