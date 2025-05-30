#include <tool/headOpenGL.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
static void processInput(GLFWwindow* window);

// 设置窗户宽，高
static const int screenWidth = 800;
static const int screenHeight = 600;

// 顶点着色器
static const char* vertexShaderSource =
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

// 片段着色器
static const char* fragmentShaderSource =
"#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";

/*
问题1
添加更多顶点到数据中，使用glDrawArrays，尝试绘制两个彼此相连的三角形：
*/

int work_02_01(int avgc, char* argv[])
{
	// OpenGL
	// 一个定义了函数布局和输出的图形API的正式规范。

	// OpenGL上下文
	// OpenGL的状态通常被称为OpenGL上下文(Context)。我们通常使用如下途径去更改OpenGL状态：设置选项，操作缓冲。最后，我们使用当前OpenGL上下文来渲染。
	// OpenGL 是一个图形库，它本身并不直接与屏幕交互，而是通过一个“上下文”与 GPU 交互。这个上下文包含了所有必要的状态、缓存、程序和资源，它定义了 OpenGL 操作的环境。
	// 每个应用程序可能有多个窗口，每个窗口可能有不同的 OpenGL 上下文。在一个多窗口的应用中，每个窗口都可能有一个独立的 OpenGL 上下文。

	// 在我们画出出色的效果之前，首先要做的就是创建一个OpenGL上下文(Context)和一个用于显示的窗口。然而，这些操作在每个系统上都是不一样的，OpenGL有意将这些操作抽象(Abstract)出去。这意味着我们不得不自己处理创建窗口，定义OpenGL上下文以及处理用户输入。

	// GLFW
	// GLFW是一个专门针对OpenGL的C语言库，它提供了一些渲染物体所需的最低限度的接口。它允许用户创建OpenGL上下文、定义窗口参数以及处理用户输入

	// 调用glfwInit函数来初始化GLFW
	glfwInit();
	// 在开始前，我们需要告诉GLFW我们要使用的OpenGL版本是3.3
	// 使用glfwWindowHint函数来配置GLFW("选项的名称,以GLFW_开头的枚举值选取", “版本号”)
	// 设置主要的版本，版本号为3（3.x版本)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	// 设置次要的版本，版本号为3（x.3版本)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// 设置OpenGL的模式：核心模式
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// 创建窗口对象(宽， 高， 窗口名称， NULL, NULL)
	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "LearnOpenGL", NULL, NULL);
	// 检测是否创建成功
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		// 终止GLFW库的运行，释放相关的系统资源
		glfwTerminate();
		return -1;
	}
	// 在一个 OpenGL 应用中，可能会有多个窗口或者多个上下文。例如，一个游戏可能有一个主窗口用于渲染场景，另一个窗口用于渲染 UI 或调试信息。每个窗口都有可能有自己的 OpenGL 上下文，而 OpenGL 本身并不知道当前活跃的上下文。
	// glfwMakeContextCurrent(window) 的作用就是告诉 GLFW 和 OpenGL 哪个窗口的上下文是当前线程中的活动上下文。因为 OpenGL 是基于上下文的，只有在当前上下文下才能执行渲染操作或者进行状态设置。
	glfwMakeContextCurrent(window);

	// GLAD
	// 由于OpenGL驱动版本众多，它大多数函数的位置都无法在编译时确定下来，需要在运行时查询。所以任务就落在了开发者身上，开发者需要在运行时获取函数地址并将其保存在一个函数指针中供以后使用。
	// 一个扩展加载库，用来为我们加载并设定所有OpenGL函数指针，从而让我们能够使用所有（现代）OpenGL函数。
	// 在这里我们能够告诉GLAD需要定义的OpenGL版本，并且根据这个版本加载所有相关的OpenGL函数。

	// 我们给GLAD传入了用来加载系统相关的OpenGL函数指针地址的函数。GLFW给我们的是glfwGetProcAddress，它根据我们编译的系统定义了正确的函数。
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// 渲染开始之前必须告诉OpenGL渲染窗口的尺寸大小，即视口(Viewport)，这样OpenGL才只能知道怎样根据窗口大小显示数据和坐标。
	// OpenGL幕后使用glViewport中定义的位置和宽高进行2D坐标的转换，将OpenGL
	// glViewport函数前两个参数控制窗口左下角的位置。第三个和第四个参数控制渲染窗口的宽度和高度（像素）。
	glViewport(0, 0, screenWidth, screenHeight);

	// 这行代码设置了窗口大小变化时的回调函数，即当窗口大小发生变化时，framebuffer_size_callback函数会被调用。
	// 当窗口被第一次显示的时候framebuffer_size_callback也会被调用。
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);



	// 两个彼此相连的三角形顶点坐标
	float vertices[] = {
		// first triangle
		-0.9f, -0.5f, 0.0f,  // left 
		-0.0f, -0.5f, 0.0f,  // right
		-0.45f, 0.5f, 0.0f,  // top 

		// second triangle
		 0.0f, -0.5f, 0.0f,  // left
		 0.9f, -0.5f, 0.0f,  // right
		 0.45f, 0.5f, 0.0f   // top 
	};


	// VBO
	// 顶点着色器会在GPU上创建内存用于储存我们的顶点数据，还要配置OpenGL如何解释这些内存，并且指定其如何发送给显卡。顶点着色器接着会处理我们在内存中指定数量的顶点。
	// 我们通过顶点缓冲对象(Vertex Buffer Objects, VBO)管理这个内存，它会在GPU内存（通常被称为显存）中储存大量顶点。使用这些缓冲对象的好处是我们可以一次性的发送一大批数据到显卡上，而不是每个顶点发送一次。

	// VAO
	// 顶点数组对象(Vertex Array Object, VAO)可以像顶点缓冲对象那样被绑定，任何随后的顶点属性调用都会储存在这个VAO中。这样的好处就是，当配置顶点属性指针时，你只需要将那些调用执行一次，之后再绘制物体的时候只需要绑定相应的VAO就行了。这使在不同顶点数据和属性配置之间切换变得非常简单，只需要绑定不同的VAO就行了。

	// 创建顶点数组对象（VAO）和顶点缓冲对象（VBO）
	unsigned int VBO, VAO;
	// 生成一个带有缓冲ID的VBO对象和VAO对象：
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	// 绑定顶点数组对象和顶点缓冲对象
	// 我们使用的任何（在GL_ARRAY_BUFFER目标上的）缓冲调用都会用来配置当前绑定的缓冲(VBO)
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindVertexArray(VAO);
	// glBufferData是一个专门用来把用户定义的数据复制到当前绑定缓冲的函数
	// (目标缓冲的类型(顶点缓冲对象当前绑定到GL_ARRAY_BUFFER目标上), 指定传输数据的大小(以字节为单位), 我们希望发送的实际数据,我们希望显卡如何管理给定的数据)
	// 第4个参数有3种形式:
	// GL_STATIC_DRAW ：数据不会或几乎不会改变。
	// GL_DYNAMIC_DRAW：数据会被改变很多。
	// GL_STREAM_DRAW ：数据每次绘制时都会改变。
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// 告诉OpenGL该如何解析顶点数据
	/*
	第一个参数：配置的顶点属性，在顶点着色器中已经使用layout(location = 0)定义了position顶点属性
	第二个参数: 顶点属性大小，这里顶点属性是vec3，由3个值组成，大小是3
	第三个参数：数据类型，vec3都是float类型，这里用GL_FLOAT
	第四个参数：是否希望数据标准化————所有数据都会映射到0（有符号数据类型是-1）到1范围，这里选否（GL_FALSE)
	第五个参数：步长，这里一个数据（vec3）长度是3 * sizeof(float)，当数据是紧密相连的也可以设置成0让OpenGL自己计算
	第六个参数：位置数据在缓冲中起始位置的偏移量，参数类型是(void*)，所以需要强制转换。这里是0
	*/
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	// 已经告诉OpenGL如何解释顶点数据了，现在需要启用顶点属性
	// 参数是指顶点属性位置值
	glEnableVertexAttribArray(0);

	// 解绑定VAO，这样其他VAO调用就不会意外地修改这个VAO，但这种情况很少发生
	glBindVertexArray(0);
	// 解绑定VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	// 创建顶点着色器对象和片段着色器对象（用ID来引用）
	unsigned int vertexShader;
	unsigned int fragmentShader;
	// 创建的着色器类型以参数形式提供给glCreateShader
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// 将顶点着色器源代码附加到顶点着色器对象上，将片段着色器源代码附加到片段着色器对象上 
	// (要编译的着色器对象, 传递的源码字符串数量, 顶点着色器真正的源码,指定字符串长度的数组)
	// 第4个参数输入NULL会自动计算着色器源码每一行字符串长度
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);

	// 编译顶点着色器片段着色器 
	glCompileShader(vertexShader);
	glCompileShader(fragmentShader);


	// 用于检查着色器编译和链接状态以及存储信息日志的变量
	int success;
	char infoLog[512];
	// 获取顶点着色器的编译状态
	// (要查询的着色器对象，着色器对象的参数，返回请求的参数结果值）
	// GL_COMPILE_STATUS：对于支持着色器编译器的实现，如果着色器上的最后一次编译操作成功，则返回GL_TRUE，否则返回GL_FALSE
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	// 检查顶点着色器是否编译成功，如果失败则输出错误信息日志
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// 获取片段着色器的编译状态 
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	// 检查片段着色器是否编译成功，如果失败则输出错误信息日志 
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// 创建着色器程序，函数创建一个程序，并返回新创建程序对象的ID引用
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	// 将顶点着色器和片段着色器附加到着色器程序上
	// (接受附加着色器的程序对象, 需要被附加到程序对象的 shader 对象)
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	// 链接着色器程序
	glLinkProgram(shaderProgram);

	// 获取着色器程序的链接状态
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	// 检查着色器程序是否链接成功，如果失败则输出错误信息日志
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::PROGRAM::SHADER::LINKING_FAILED\n" << infoLog << std::endl;
	}

	// 着色器对象链接到程序对象以后，需删除着色器对象，我们不再需要它们了：
	// 删除已经链接到着色器程序的顶点着色器和片段着色器对象 
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// 设置线框模式
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);



	// 函数在我们每次循环的开始前检查一次GLFW是否被要求退出，如果是的话，该函数返回true，渲染循环将停止运行，之后我们就可以关闭应用程序。
	while (!glfwWindowShouldClose(window))
	{
		// 每次循环开始检测用户是否按下键盘上的某些按键，执行对应功能
		processInput(window);

		// 渲染指令
		// 设置清空屏幕所用的颜色
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		// 清空颜色缓冲区，以准备进行下一帧的渲染
		// 调用glClear函数，清除颜色缓冲之后，整个颜色缓冲都会被填充为glClearColor里所设置的颜色。
		glClear(GL_COLOR_BUFFER_BIT);


		// 使用着色器程序 
		glUseProgram(shaderProgram);
		// 绑定顶点数组对象 
		glBindVertexArray(VAO);
		// 绘制三角形 
		// (打算绘制的OpenGL图元的类型，顶点数组的起始索引，打算绘制多少个顶点)
		glDrawArrays(GL_TRIANGLES, 0, 6);


		// 函数会交换颜色缓冲（它是一个储存着GLFW窗口每一个像素颜色值的大缓冲），它在这一迭代中被用来绘制，并且将会作为输出显示在屏幕上。
		glfwSwapBuffers(window);
		// 函数检查有没有触发什么事件（比如键盘输入、鼠标移动等）、更新窗口状态，并调用对应的回调函数（可以通过回调方法手动设置）。
		glfwPollEvents();
	}



	// 删除顶点数组对象 
	glDeleteVertexArrays(1, &VAO);
	// 删除顶点缓冲对象 
	glDeleteBuffers(1, &VBO);
	// 删除着色器程序 
	glDeleteProgram(shaderProgram);


	// 终止GLFW库的运行，释放相关的系统资源。
	glfwTerminate();
	return 0;
}

// 每当窗口改变大小，GLFW会调用这个函数并填充相应的参数供你处理
static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// 检测用户键盘输入
static void processInput(GLFWwindow* window)
{
	// 当输入为Esc键时，关闭窗口
	// 这个函数将会返回这个按键是否正在被按下
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		// 这个函数会把WindowShouldClose属性设置为 true来关闭GLFW。
		glfwSetWindowShouldClose(window, true);
	}
}