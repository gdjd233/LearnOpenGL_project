#include <tool/headOpenGL.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <tool/shader.h>
#include <tool/camer.h>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
static void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
static void processInput(GLFWwindow* window);

// 设置窗户宽，高
static const int screenWidth = 800;
static const int screenHeight = 600;

// 判断鼠标是否第一次进入窗口
static bool firstMouse = true;
// 上一帧鼠标的位置
static float lastX = (float)screenWidth / 2.0f;
static float lastY = (float)screenHeight / 2.0f;

// 创建摄像机系统
static Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

// 获得摄像机时间差
static float deltaTime = 0.0f; // 当前帧与上一帧的时间差
static float lastFrame = 0.0f; // 上一帧的时间

// 设置灯光位置
static glm::vec3 lightPos(1.2f, 1.0f, 2.0f);


/*
尝试实现一个Gouraud着色（而不是风氏着色）。如果你做对了话，立方体的光照应该会看起来有些奇怪，尝试推理为什么它会看起来这么奇怪

那么我们看到了什么？
（你可以自行观察或查看提供的图像）立方体正面的两个三角形之间存在明显的分界线。这条"条纹"之所以可见，是由于片段插值造成的。
从示例图像中可以看到，立方体正面右上角顶点存在镜面高光。
由于右下三角形的右上顶点被照亮，而该三角形另外两个顶点未被照亮，明亮的光照值就会通过插值过渡到另外两个顶点。
左上角三角形也发生同样的情况。由于中间片段的颜色并非直接来自光源，而是插值计算的结果，导致中间片段的光照呈现错误状态，
使得左上三角形与右下三角形的亮度相互冲突，最终在两个三角形之间形成可见的明暗分界条纹。

当图形结构更为复杂时，这种效应会表现得更加明显。

*/

int work_09_04(int avgc, char* argv[])
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

	// 隐藏光标且不再显示
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// 鼠标回调, 当光标移动时调用此函数
	glfwSetCursorPosCallback(window, mouse_callback);
	// 鼠标滚动回调，当鼠标滚轮滚动时调用此函数
	glfwSetScrollCallback(window, scroll_callback);
	// 开启深度测试
	// 当片段想要输出它的颜色时，OpenGL会将它的深度值和z缓冲进行比较，如果当前的片段在其它片段之后，它将会被丢弃，否则将会覆盖。
	glEnable(GL_DEPTH_TEST);


	// 创建着色器类
	// 与光照计算相关的着色器类
	Shader lightingShader("./exercises/09_basic_lighting/shader4/cube.vs", "./exercises/09_basic_lighting/shader4/cube.fs");
	// 与“发光立方体”或“立方体光源”相关的着色器
	Shader lightCubeShader("./exercises/09_basic_lighting/shader4/light_cube.vs", "./exercises/09_basic_lighting/shader4/light_cube.fs");


	// 立体（位置3，纹理2, 法向量1）
	float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};

	// VBO
	// 顶点着色器会在GPU上创建内存用于储存我们的顶点数据，还要配置OpenGL如何解释这些内存，并且指定其如何发送给显卡。顶点着色器接着会处理我们在内存中指定数量的顶点。
	// 我们通过顶点缓冲对象(Vertex Buffer Objects, VBO)管理这个内存，它会在GPU内存（通常被称为显存）中储存大量顶点。使用这些缓冲对象的好处是我们可以一次性的发送一大批数据到显卡上，而不是每个顶点发送一次。

	// VAO
	// 顶点数组对象(Vertex Array Object, VAO)可以像顶点缓冲对象那样被绑定，任何随后的顶点属性调用都会储存在这个VAO中。这样的好处就是，当配置顶点属性指针时，你只需要将那些调用执行一次，之后再绘制物体的时候只需要绑定相应的VAO就行了。这使在不同顶点数据和属性配置之间切换变得非常简单，只需要绑定不同的VAO就行了。

	// 创建顶点数组对象（VAO）和顶点缓冲对象（VBO）
	unsigned int VBO, cubeVAO, lightCubeVAO;
	// 生成一个带有缓冲ID的VBO对象和VAO对象：
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &cubeVAO);
	// 绑定顶点数组对象和顶点缓冲对象
	// 我们使用的任何（在GL_ARRAY_BUFFER目标上的）缓冲调用都会用来配置当前绑定的缓冲(VBO)
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindVertexArray(cubeVAO);
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
	第二个参数: 顶点属性大小，这里顶点属性是vec3， 由3个值组成，大小是3
	第三个参数：数据类型，vec3都是float类型，这里用GL_FLOAT
	第四个参数：是否希望数据标准化————所有数据都会映射到0（有符号数据类型是-1）到1范围，这里选否（GL_FALSE)
	第五个参数：步长，当数据是紧密相连的也可以设置成0让OpenGL自己计算
	第六个参数：位置数据在缓冲中起始位置的偏移量，参数类型是(void*)，所以需要强制转换。这里是0
	*/
	// 设置位置属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	// 设置法向量属性
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	// 已经告诉OpenGL如何解释顶点数据了，现在需要启用顶点属性
	// 参数是指顶点属性位置值
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);


	// 设置光源VAO
	// 分配ID和绑定
	glGenVertexArrays(1, &lightCubeVAO);
	glBindVertexArray(lightCubeVAO);
	// 只需要绑定VBO不用再次设置VBO的数据，因为箱子的VBO数据中已经包含了正确的立方体顶点数据
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// 设置位置属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	// 已经告诉OpenGL如何解释顶点数据了，现在需要启用顶点属性
	// 参数是指顶点属性位置值
	glEnableVertexAttribArray(0);



	// 解绑定VAO，这样其他VAO调用就不会意外地修改这个VAO，但这种情况很少发生
	glBindVertexArray(0);
	// 解绑定VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	// 设置线框模式
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);




	// 函数在我们每次循环的开始前检查一次GLFW是否被要求退出，如果是的话，该函数返回true，渲染循环将停止运行，之后我们就可以关闭应用程序。
	while (!glfwWindowShouldClose(window))
	{
		// 在每一帧中我们计算出新的deltaTime以备后用
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// 每次循环开始检测用户是否按下键盘上的某些按键，执行对应功能
		processInput(window);

		// 渲染指令
		// 设置清空屏幕所用的颜色
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		// 清空屏幕的颜色缓冲和深度缓冲+
		// 调用glClear函数，清除颜色缓冲之后，整个颜色缓冲都会被填充为glClearColor里所设置的颜色。
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// 使用立体着色器
		lightingShader.use();
		// 设置 立体着色器 立体颜色
		lightingShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
		// 设置 立体着色器 物体被照光的颜色
		lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);

		// 设置光源在立体上方环绕移动
		lightPos = glm::vec3(2.0f * sin(glfwGetTime()), 1.0f, 2.0f * cos(glfwGetTime()));


		// 创建立体模型矩阵, 包含了位移、缩放与旋转操作
		glm::mat4 model = glm::mat4();
		// 创建投影矩阵
		// （视野，一般设置成45，宽高比，近平面（所有在近平面和远平面内且处于平截头体内的顶点都会被渲染），远平面）
		// 近平面若设置过大会将靠近摄像机的坐标（在0.0f和10.0f之间）都裁剪掉
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
		// 创建观察矩阵
		glm::mat4 view = camera.GetViewMatrix();



		// 把对应信息传递给立体着色器
		lightingShader.setMat4("model", model);
		lightingShader.setMat4("view", view);
		lightingShader.setMat4("projection", projection);
		lightingShader.setVec3("viewPos", camera.Position);
		lightingShader.setVec3("lightPos", lightPos);


		// 绑定顶点数组对象 
		glBindVertexArray(cubeVAO);
		// 绘制立体
		glDrawArrays(GL_TRIANGLES, 0, 36);


		// 使用光源着色器
		lightCubeShader.use();
		// 设置 光源 位置，大小
		model = glm::mat4();
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));
		// 把对应信息传递给光源着色器
		lightCubeShader.setMat4("model", model);
		lightCubeShader.setMat4("view", view);
		lightCubeShader.setMat4("projection", projection);

		// 绑定顶点数组对象 
		glBindVertexArray(lightCubeVAO);
		// 绘制光源
		glDrawArrays(GL_TRIANGLES, 0, 36);



		// 函数会交换颜色缓冲（它是一个储存着GLFW窗口每一个像素颜色值的大缓冲），它在这一迭代中被用来绘制，并且将会作为输出显示在屏幕上。
		glfwSwapBuffers(window);
		// 函数检查有没有触发什么事件（比如键盘输入、鼠标移动等）、更新窗口状态，并调用对应的回调函数（可以通过回调方法手动设置）。
		glfwPollEvents();
	}


	// 删除顶点数组对象 
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &lightCubeVAO);
	// 删除顶点缓冲对象 
	glDeleteBuffers(1, &VBO);


	// 终止GLFW库的运行，释放相关的系统资源。
	glfwTerminate();
	return 0;
}

// 每当窗口改变大小，GLFW会调用这个函数并填充相应的参数供你处理
static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}


/*这是一个函数声明，声明了一个名为processInput的函数，该函数用于处理用户输入。*/
static void processInput(GLFWwindow* window)
{
	// 当输入为Esc键时，关闭窗口
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	// 输入w,s,a,d对应摄像机移动方向
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

// 检测鼠标移动
static void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = (float)xposIn;
	float ypos = (float)yposIn;

	// 检测我们是不是第一次进入屏幕
	// 是的话强制将鼠标位置变为初始值
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	// 计算当前帧和上一帧鼠标位置的偏移量
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // 注意这里是相反的，因为y坐标是从底部往顶部依次增大的
	lastX = xpos;
	lastY = ypos;


	// 通过俯仰角和偏航角来计算以得到真正的方向向量
	camera.ProcessMouseMovement(xoffset, yoffset);
}

// 当鼠标滚轮滚动时进行缩放，对视野（FOV）进行修改
static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}