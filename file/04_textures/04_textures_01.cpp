#include <tool/headOpenGL.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <tool/shader.h>
#include <iostream>

#include <tool/stb_image.h>

static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
static void processInput(GLFWwindow* window);

// 设置窗户宽，高
static const int screenWidth = 800;
static const int screenHeight = 600;

int solve_04_01(int avgc, char* argv[])
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


	// 翻转y轴
	stbi_set_flip_vertically_on_load(true);
	// 创建纹理
	int width, height, nrChannels;
	// （文件位置，图像宽度，图像高度，图像颜色通道个数，指定加载后的目标通道数（填0使用图像原始通道数））
	unsigned char* data = stbi_load("./static/texture/container.jpg", &width, &height, &nrChannels, 0);

	// 生成纹理
	unsigned int texture1, texture2;
	// （纹理数量， 纹理）
	glGenTextures(1, &texture1);
	glGenTextures(1, &texture2);

	/* 下面这行代码使用OpenGL函数glActiveTexture来激活纹理单元。GL_TEXTURE0表示纹理单元0，通过在其上加上偏移量0，
    即GL_TEXTURE0 + 0，可以选择激活纹理单元0。这意味着在之后的纹理操作中，将会针对纹理单元1进行设置和使用。
    例如，如果你在之后绑定纹理对象，它将会与激活的纹理单元1相关联。这样的机制允许你在单个渲染过程中使用多个纹理。
    通过激活不同的纹理单元，你可以在每个纹理单元上绑定不同的纹理，并在渲染过程中同时使用它们。
    当然也可以写成是加上变量的形式，更方便，也推荐这么做，如“GL_TEXTURE0 + index”，这里index等于0。 */
	glActiveTexture(GL_TEXTURE0 + 0);
	// 绑定纹理1 
	glBindTexture(GL_TEXTURE_2D, texture1);
	// 设置纹理的环绕方式参数 
	// 设置纹理的S轴环绕方式为GL_REPEAT（默认环绕方法） 
	// (纹理目标(我们使用的是2D纹理，因此纹理目标是GL_TEXTURE_2D), 设置的选项与应用的纹理轴, 环绕方式)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// 设置纹理的过滤方式参数（邻近过滤）
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


	if (data)
	{
		/* 下面一行代码使用OpenGL函数glTexImage2D来将当前绑定的纹理对象附加上纹理图像。
		参数解释如下：
			第1个参数 GL_TEXTURE_2D 表示目标纹理类型，这里是二维纹理。
			第2个参数 0 表示纹理的细节级别，这里只有一个级别。
			第3个参数 GL_RGB 表示纹理的内部存储格式，这里使用RGB颜色空间。
			第4个参数 width 表示纹理图像的宽度。
			第5个参数 height 表示纹理图像的高度。
			第6个参数 0 表示边框的大小，一般用0表示无边框。
			第7个参数 GL_RGB 表示源图像数据的格式，这里使用RGB颜色空间。
			第8个参数 GL_UNSIGNED_BYTE 表示源图像数据的数据类型，这里使用无符号字节。
			第9个参数 data 是指向源图像数据的指针，它包含了实际的图像数据。
		通过调用glTexImage2D函数，你将纹理数据加载到OpenGL中，并为之后的纹理操作做准备。
		这个函数会将指定的图像数据复制到纹理对象中，并为其分配内存空间。
		在之后的渲染过程中，你可以使用这个纹理进行贴图操作。*/
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

		// 为当前绑定的纹理自动生成所有需要的多级渐远纹理
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	// 释放图像的内存
	stbi_image_free(data);

	// 创建第二个纹理
	data = stbi_load("./static/texture/awesomeface.png", &width, &height, &nrChannels, 0);

	glActiveTexture(GL_TEXTURE0 + 1);
	// 绑定纹理2
	glBindTexture(GL_TEXTURE_2D, texture2);
	// 设置纹理的环绕方式参数 
	// 设置纹理的S轴环绕方式为GL_REPEAT（默认环绕方法） 
	// (纹理目标(我们使用的是2D纹理，因此纹理目标是GL_TEXTURE_2D), 设置的选项与应用的纹理轴, 环绕方式) 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// 设置纹理的过滤方式参数（邻近过滤）
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
	if (data)
	{
		// 注意！这行代码是对png格式的图片进行操作，png格式图片除了RGB之外还有另外的透明度分量A(Alpha)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

		// 为当前绑定的纹理自动生成所有需要的多级渐远纹理
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	// 释放图像的内存
	stbi_image_free(data);


	// 创建着色器类
	Shader ourShader("./file/04_textures/shader2/vertex.vs", "./file/04_textures/shader2/fragment.fs");

	// 创建矩形
	float vertices[] = {
		//     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
			 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
			 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
			-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
			-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上
	};

	// 绘制出矩形所需的索引
	unsigned int indices[] = {
		// 注意索引从0开始! 
		// 此例的索引(0,1,2,3)就是顶点数组vertices的下标，
		// 这样可以由下标代表顶点组合成矩形

		0, 1, 3, // 第一个三角形
		1, 2, 3  // 第二个三角形
	};

	// VBO
	// 顶点着色器会在GPU上创建内存用于储存我们的顶点数据，还要配置OpenGL如何解释这些内存，并且指定其如何发送给显卡。顶点着色器接着会处理我们在内存中指定数量的顶点。
	// 我们通过顶点缓冲对象(Vertex Buffer Objects, VBO)管理这个内存，它会在GPU内存（通常被称为显存）中储存大量顶点。使用这些缓冲对象的好处是我们可以一次性的发送一大批数据到显卡上，而不是每个顶点发送一次。

	// VAO
	// 顶点数组对象(Vertex Array Object, VAO)可以像顶点缓冲对象那样被绑定，任何随后的顶点属性调用都会储存在这个VAO中。这样的好处就是，当配置顶点属性指针时，你只需要将那些调用执行一次，之后再绘制物体的时候只需要绑定相应的VAO就行了。这使在不同顶点数据和属性配置之间切换变得非常简单，只需要绑定不同的VAO就行了。

	// EBO
	// EBO是一个缓冲区，就像一个顶点缓冲区对象一样，它存储 OpenGL 用来决定要绘制哪些顶点的索引。这种所谓的索引绘制(Indexed Drawing)正是我们问题的解决方案。

	// 创建顶点数组对象（VAO），顶点缓冲对象（VBO）和元素缓冲对象（EBO）
	unsigned int VBO, VAO, EBO;
	// 生成一个带有缓冲ID的VBO对象，VAO对象，EBO对象：
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &EBO);
	// 绑定顶点数组对象和顶点缓冲对象
	// 我们使用的任何（在GL_ARRAY_BUFFER目标上的）缓冲调用都会用来配置当前绑定的缓冲(VBO)
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	// glBufferData是一个专门用来把用户定义的数据复制到当前绑定缓冲的函数
	// (目标缓冲的类型(顶点缓冲对象当前绑定到GL_ARRAY_BUFFER目标上), 指定传输数据的大小(以字节为单位), 我们希望发送的实际数据,我们希望显卡如何管理给定的数据)
	// 第4个参数有3种形式:
	// GL_STATIC_DRAW ：数据不会或几乎不会改变。
	// GL_DYNAMIC_DRAW：数据会被改变很多。
	// GL_STREAM_DRAW ：数据每次绘制时都会改变。
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	// 设置顶点属性指针，指定如何解释顶点数据 
	/*
	第一个参数：配置的顶点属性，在顶点着色器中已经使用layout(location = 0)定义了position顶点属性
	第二个参数: 顶点属性大小，这里顶点属性是vec3， 由3个值组成，大小是3
	第三个参数：数据类型，vec3都是float类型，这里用GL_FLOAT
	第四个参数：是否希望数据标准化————所有数据都会映射到0（有符号数据类型是-1）到1范围，这里选否（GL_FALSE)
	第五个参数：步长，这里有两个vec3数据和一个vec2数据，长度是8 * sizeof(float)，当数据是紧密相连的也可以设置成0让OpenGL自己计算
	第六个参数：位置数据在缓冲中起始位置的偏移量，参数类型是(void*)，所以需要强制转换。这里是0
	*/
	// 设置位置属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	// 设置颜色属性，用的是后3个数据，偏移3个单位
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	// 设置纹理属性，
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	// 已经告诉OpenGL如何解释顶点数据了，现在需要启用顶点属性
	// 参数是指顶点属性位置值
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	// 解绑定VAO，这样其他VAO调用就不会意外地修改这个VAO，但这种情况很少发生
	glBindVertexArray(0);
	// 解绑定VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// 解绑定EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// 设置线框模式
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// 告诉OpenGL每个采样器属于哪个纹理单元（只需执行一次
	// 在设置uniform之前激活着色器 
	ourShader.use();
	// 设置纹理采样器1 
	ourShader.setInt("texture1", 0);
	// 设置纹理采样器2 
	ourShader.setInt("texture2", 1);


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

		// 使用着色器
		ourShader.use();
		// 绑定顶点数组对象 
		glBindVertexArray(VAO);
		// 图案 
		// (打算绘制的OpenGL图元的类型, 打算绘制顶点的个数, 索引的类型, EBO中的偏移量)
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


		// 函数会交换颜色缓冲（它是一个储存着GLFW窗口每一个像素颜色值的大缓冲），它在这一迭代中被用来绘制，并且将会作为输出显示在屏幕上。
		glfwSwapBuffers(window);
		// 函数检查有没有触发什么事件（比如键盘输入、鼠标移动等）、更新窗口状态，并调用对应的回调函数（可以通过回调方法手动设置）。
		glfwPollEvents();
	}


	// 删除顶点数组对象 
	glDeleteVertexArrays(1, &VAO);
	// 删除顶点缓冲对象 
	glDeleteBuffers(1, &VBO);
	// 删除元素缓冲对象 
	glDeleteBuffers(1, &EBO);

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