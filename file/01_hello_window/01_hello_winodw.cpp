#include <tool/headOpenGL.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
static void processInput(GLFWwindow* window);

// 设置窗户宽，高
static const int screenWidth = 800;
static const int screenHeight = 600;

int solve_01(int avgc, char* argv[])
{
	// OpenGL
	// 一个定义了函数布局和输出的图形API的正式规范

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




		// 函数会交换颜色缓冲（它是一个储存着GLFW窗口每一个像素颜色值的大缓冲），它在这一迭代中被用来绘制，并且将会作为输出显示在屏幕上。
		glfwSwapBuffers(window);
		// 函数检查有没有触发什么事件（比如键盘输入、鼠标移动等）、更新窗口状态，并调用对应的回调函数（可以通过回调方法手动设置）。
		glfwPollEvents();
	}

	// 终止GLFW库的运行，释放相关的系统资源
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