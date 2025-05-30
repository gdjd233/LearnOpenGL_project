#include <tool/headOpenGL.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <tool/shader.h>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <tool/stb_image.h>

static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
static void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
static void processInput(GLFWwindow* window);

// ���ô�������
static const int screenWidth = 800;
static const int screenHeight = 600;

// �ж�����Ƿ��һ�ν��봰��
static bool firstMouse = true;
// ƫ����
static float yaw = -90.0f;
// ������
static float pitch = 0.0f;
// ��һ֡����λ��
static float lastX = (float)screenWidth / 2.0f;
static float lastY = (float)screenHeight / 2.0f;
static float fov = 45.0f;

// ���������ϵͳ
// ���λ��
static glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
static glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
static glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

// ��������ʱ���
static float deltaTime = 0.0f; // ��ǰ֡����һ֡��ʱ���
static float lastFrame = 0.0f; // ��һ֡��ʱ��

int solve_07_01(int avgc, char* argv[])
{
	// OpenGL
	// һ�������˺������ֺ������ͼ��API����ʽ�淶��

	// OpenGL������
	// OpenGL��״̬ͨ������ΪOpenGL������(Context)������ͨ��ʹ������;��ȥ����OpenGL״̬������ѡ��������塣�������ʹ�õ�ǰOpenGL����������Ⱦ��
	// OpenGL ��һ��ͼ�ο⣬��������ֱ������Ļ����������ͨ��һ���������ġ��� GPU ��������������İ��������б�Ҫ��״̬�����桢�������Դ���������� OpenGL �����Ļ�����
	// ÿ��Ӧ�ó�������ж�����ڣ�ÿ�����ڿ����в�ͬ�� OpenGL �����ġ���һ���ര�ڵ�Ӧ���У�ÿ�����ڶ�������һ�������� OpenGL �����ġ�

	// �����ǻ�����ɫ��Ч��֮ǰ������Ҫ���ľ��Ǵ���һ��OpenGL������(Context)��һ��������ʾ�Ĵ��ڡ�Ȼ������Щ������ÿ��ϵͳ�϶��ǲ�һ���ģ�OpenGL���⽫��Щ��������(Abstract)��ȥ������ζ�����ǲ��ò��Լ����������ڣ�����OpenGL�������Լ������û����롣

	// GLFW
	// GLFW��һ��ר�����OpenGL��C���Կ⣬���ṩ��һЩ��Ⱦ�������������޶ȵĽӿڡ��������û�����OpenGL�����ġ����崰�ڲ����Լ������û�����

	// ����glfwInit��������ʼ��GLFW
	glfwInit();
	// �ڿ�ʼǰ��������Ҫ����GLFW����Ҫʹ�õ�OpenGL�汾��3.3
	// ʹ��glfwWindowHint����������GLFW("ѡ�������,��GLFW_��ͷ��ö��ֵѡȡ", ���汾�š�)
	// ������Ҫ�İ汾���汾��Ϊ3��3.x�汾)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	// ���ô�Ҫ�İ汾���汾��Ϊ3��x.3�汾)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// ����OpenGL��ģʽ������ģʽ
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// �������ڶ���(�� �ߣ� �������ƣ� NULL, NULL)
	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "LearnOpenGL", NULL, NULL);
	// ����Ƿ񴴽��ɹ�
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		// ��ֹGLFW������У��ͷ���ص�ϵͳ��Դ
		glfwTerminate();
		return -1;
	}
	// ��һ�� OpenGL Ӧ���У����ܻ��ж�����ڻ��߶�������ġ����磬һ����Ϸ������һ��������������Ⱦ��������һ������������Ⱦ UI �������Ϣ��ÿ�����ڶ��п������Լ��� OpenGL �����ģ��� OpenGL ������֪����ǰ��Ծ�������ġ�
	// glfwMakeContextCurrent(window) �����þ��Ǹ��� GLFW �� OpenGL �ĸ����ڵ��������ǵ�ǰ�߳��еĻ�����ġ���Ϊ OpenGL �ǻ��������ĵģ�ֻ���ڵ�ǰ�������²���ִ����Ⱦ�������߽���״̬���á�
	glfwMakeContextCurrent(window);

	// GLAD
	// ����OpenGL�����汾�ڶ࣬�������������λ�ö��޷��ڱ���ʱȷ����������Ҫ������ʱ��ѯ����������������˿��������ϣ���������Ҫ������ʱ��ȡ������ַ�����䱣����һ������ָ���й��Ժ�ʹ�á�
	// һ����չ���ؿ⣬����Ϊ���Ǽ��ز��趨����OpenGL����ָ�룬�Ӷ��������ܹ�ʹ�����У��ִ���OpenGL������
	// �����������ܹ�����GLAD��Ҫ�����OpenGL�汾�����Ҹ�������汾����������ص�OpenGL������

	// ���Ǹ�GLAD��������������ϵͳ��ص�OpenGL����ָ���ַ�ĺ�����GLFW�����ǵ���glfwGetProcAddress�����������Ǳ����ϵͳ��������ȷ�ĺ�����
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// ��Ⱦ��ʼ֮ǰ�������OpenGL��Ⱦ���ڵĳߴ��С�����ӿ�(Viewport)������OpenGL��ֻ��֪���������ݴ��ڴ�С��ʾ���ݺ����ꡣ
	// OpenGLĻ��ʹ��glViewport�ж����λ�úͿ�߽���2D�����ת������OpenGL
	// glViewport����ǰ�����������ƴ������½ǵ�λ�á��������͵��ĸ�����������Ⱦ���ڵĿ�Ⱥ͸߶ȣ����أ���
	glViewport(0, 0, screenWidth, screenHeight);

	// ���д��������˴��ڴ�С�仯ʱ�Ļص��������������ڴ�С�����仯ʱ��framebuffer_size_callback�����ᱻ���á�
	// �����ڱ���һ����ʾ��ʱ��framebuffer_size_callbackҲ�ᱻ���á�
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// ���ع���Ҳ�����ʾ
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// ���ص�, ������ƶ�ʱ���ô˺���
	glfwSetCursorPosCallback(window, mouse_callback);
	// �������ص����������ֹ���ʱ���ô˺���
	glfwSetScrollCallback(window, scroll_callback);



	// ��תy��
	stbi_set_flip_vertically_on_load(true);
	// ��������
	int width, height, nrChannels;
	// ���ļ�λ�ã�ͼ���ȣ�ͼ��߶ȣ�ͼ����ɫͨ��������ָ�����غ��Ŀ��ͨ��������0ʹ��ͼ��ԭʼͨ��������
	unsigned char* data = stbi_load("./static/texture/container.jpg", &width, &height, &nrChannels, 0);

	// ��������
	unsigned int texture1, texture2;
	// ������������ ����
	glGenTextures(1, &texture1);
	glGenTextures(1, &texture2);

	/* �������д���ʹ��OpenGL����glActiveTexture����������Ԫ��GL_TEXTURE0��ʾ����Ԫ0��ͨ�������ϼ���ƫ����0��
	��GL_TEXTURE0 + 0������ѡ�񼤻�����Ԫ0������ζ����֮�����������У������������Ԫ1�������ú�ʹ�á�
	���磬�������֮�����������������뼤�������Ԫ1������������Ļ����������ڵ�����Ⱦ������ʹ�ö������
	ͨ�����ͬ������Ԫ���������ÿ������Ԫ�ϰ󶨲�ͬ������������Ⱦ������ͬʱʹ�����ǡ�
	��ȻҲ����д���Ǽ��ϱ�������ʽ�������㣬Ҳ�Ƽ���ô�����硰GL_TEXTURE0 + index��������index����0�� */
	glActiveTexture(GL_TEXTURE0 + 0);
	// ������1 
	glBindTexture(GL_TEXTURE_2D, texture1);
	// ��������Ļ��Ʒ�ʽ���� 
	// ���������S�ỷ�Ʒ�ʽΪGL_REPEAT��Ĭ�ϻ��Ʒ����� 
	// (����Ŀ��(����ʹ�õ���2D�����������Ŀ����GL_TEXTURE_2D), ���õ�ѡ����Ӧ�õ�������, ���Ʒ�ʽ)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// ��������Ĺ��˷�ʽ�������ڽ����ˣ�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


	if (data)
	{
		/* ����һ�д���ʹ��OpenGL����glTexImage2D������ǰ�󶨵�������󸽼�������ͼ��
		�����������£�
			��1������ GL_TEXTURE_2D ��ʾĿ���������ͣ������Ƕ�ά����
			��2������ 0 ��ʾ�����ϸ�ڼ�������ֻ��һ������
			��3������ GL_RGB ��ʾ������ڲ��洢��ʽ������ʹ��RGB��ɫ�ռ䡣
			��4������ width ��ʾ����ͼ��Ŀ�ȡ�
			��5������ height ��ʾ����ͼ��ĸ߶ȡ�
			��6������ 0 ��ʾ�߿�Ĵ�С��һ����0��ʾ�ޱ߿�
			��7������ GL_RGB ��ʾԴͼ�����ݵĸ�ʽ������ʹ��RGB��ɫ�ռ䡣
			��8������ GL_UNSIGNED_BYTE ��ʾԴͼ�����ݵ��������ͣ�����ʹ���޷����ֽڡ�
			��9������ data ��ָ��Դͼ�����ݵ�ָ�룬��������ʵ�ʵ�ͼ�����ݡ�
		ͨ������glTexImage2D�������㽫�������ݼ��ص�OpenGL�У���Ϊ֮������������׼����
		��������Ὣָ����ͼ�����ݸ��Ƶ���������У���Ϊ������ڴ�ռ䡣
		��֮�����Ⱦ�����У������ʹ��������������ͼ������*/
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

		// Ϊ��ǰ�󶨵������Զ�����������Ҫ�Ķ༶��Զ����
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	// �ͷ�ͼ����ڴ�
	stbi_image_free(data);

	// �����ڶ�������
	data = stbi_load("./static/texture/awesomeface.png", &width, &height, &nrChannels, 0);

	glActiveTexture(GL_TEXTURE0 + 1);
	// ������2
	glBindTexture(GL_TEXTURE_2D, texture2);
	// ��������Ļ��Ʒ�ʽ���� 
	// ���������S�ỷ�Ʒ�ʽΪGL_REPEAT��Ĭ�ϻ��Ʒ����� 
	// (����Ŀ��(����ʹ�õ���2D�����������Ŀ����GL_TEXTURE_2D), ���õ�ѡ����Ӧ�õ�������, ���Ʒ�ʽ) 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// ��������Ĺ��˷�ʽ�������ڽ����ˣ�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	if (data)
	{
		// ע�⣡���д����Ƕ�png��ʽ��ͼƬ���в�����png��ʽͼƬ����RGB֮�⻹�������͸���ȷ���A(Alpha)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

		// Ϊ��ǰ�󶨵������Զ�����������Ҫ�Ķ༶��Զ����
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	// �ͷ�ͼ����ڴ�
	stbi_image_free(data);

	// ������ɫ����
	Shader ourShader("./file/07_camera/shader/vertex.vs", "./file/07_camera/shader/fragment.fs");

	// ����
	float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	// ���Ƴ��������������
	unsigned int indices[] = {
		// ע��������0��ʼ! 
		// ����������(0,1,2,3)���Ƕ�������vertices���±꣬
		// �����������±��������ϳɾ���

		0, 1, 3, // ��һ��������
		1, 2, 3  // �ڶ���������
	};

	// ����������� 
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	// VBO
	// ������ɫ������GPU�ϴ����ڴ����ڴ������ǵĶ������ݣ���Ҫ����OpenGL��ν�����Щ�ڴ棬����ָ������η��͸��Կ���������ɫ�����Żᴦ���������ڴ���ָ�������Ķ��㡣
	// ����ͨ�����㻺�����(Vertex Buffer Objects, VBO)��������ڴ棬������GPU�ڴ棨ͨ������Ϊ�Դ棩�д���������㡣ʹ����Щ�������ĺô������ǿ���һ���Եķ���һ�������ݵ��Կ��ϣ�������ÿ�����㷢��һ�Ρ�

	// VAO
	// �����������(Vertex Array Object, VAO)�����񶥵㻺������������󶨣��κ����Ķ������Ե��ö��ᴢ�������VAO�С������ĺô����ǣ������ö�������ָ��ʱ����ֻ��Ҫ����Щ����ִ��һ�Σ�֮���ٻ��������ʱ��ֻ��Ҫ����Ӧ��VAO�����ˡ���ʹ�ڲ�ͬ�������ݺ���������֮���л���÷ǳ��򵥣�ֻ��Ҫ�󶨲�ͬ��VAO�����ˡ�

	// EBO
	// EBO��һ��������������һ�����㻺��������һ�������洢 OpenGL ��������Ҫ������Щ�����������������ν����������(Indexed Drawing)������������Ľ��������

	// ���������������VAO�������㻺�����VBO����Ԫ�ػ������EBO��
	unsigned int VBO, VAO, EBO;
	// ����һ�����л���ID��VBO����VAO����EBO����
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &EBO);
	// �󶨶����������Ͷ��㻺�����
	// ����ʹ�õ��κΣ���GL_ARRAY_BUFFERĿ���ϵģ�������ö����������õ�ǰ�󶨵Ļ���(VBO)
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	// glBufferData��һ��ר���������û���������ݸ��Ƶ���ǰ�󶨻���ĺ���
	// (Ŀ�껺�������(���㻺�����ǰ�󶨵�GL_ARRAY_BUFFERĿ����), ָ���������ݵĴ�С(���ֽ�Ϊ��λ), ����ϣ�����͵�ʵ������,����ϣ���Կ���ι������������)
	// ��4��������3����ʽ:
	// GL_STATIC_DRAW �����ݲ���򼸺�����ı䡣
	// GL_DYNAMIC_DRAW�����ݻᱻ�ı�ܶࡣ
	// GL_STREAM_DRAW ������ÿ�λ���ʱ����ı䡣
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	// ���ö�������ָ�룬ָ����ν��Ͷ������� 
	/*
	��һ�����������õĶ������ԣ��ڶ�����ɫ�����Ѿ�ʹ��layout(location = 0)������position��������
	�ڶ�������: �������Դ�С�����ﶥ��������vec3�� ��3��ֵ��ɣ���С��3
	�������������������ͣ�vec3����float���ͣ�������GL_FLOAT
	���ĸ��������Ƿ�ϣ�����ݱ�׼�����������������ݶ���ӳ�䵽0���з�������������-1����1��Χ������ѡ��GL_FALSE)
	�����������������������һ��vec3���ݺ�һ��vec2���ݣ�������5 * sizeof(float)���������ǽ���������Ҳ�������ó�0��OpenGL�Լ�����
	������������λ�������ڻ�������ʼλ�õ�ƫ����������������(void*)��������Ҫǿ��ת����������0
	*/
	// ����λ������
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	// �����������ԣ��õ��Ǻ�2�����ݣ�ƫ��3����λ
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	// �Ѿ�����OpenGL��ν��Ͷ��������ˣ�������Ҫ���ö�������
	// ������ָ��������λ��ֵ
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);


	// ���VAO����������VAO���þͲ���������޸����VAO��������������ٷ���
	glBindVertexArray(0);
	// ���VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// ���EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// �����߿�ģʽ
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// ����OpenGLÿ�������������ĸ�����Ԫ��ֻ��ִ��һ��
	// ������uniform֮ǰ������ɫ�� 
	ourShader.use();
	// �������������1 
	ourShader.setInt("texture1", 0);
	// �������������2 
	ourShader.setInt("texture2", 1);

	// ������Ȳ���
	// ��Ƭ����Ҫ���������ɫʱ��OpenGL�Ὣ�������ֵ��z������бȽϣ������ǰ��Ƭ��������Ƭ��֮�������ᱻ���������򽫻Ḳ�ǡ�
	glEnable(GL_DEPTH_TEST);




	// ����������ÿ��ѭ���Ŀ�ʼǰ���һ��GLFW�Ƿ�Ҫ���˳�������ǵĻ����ú�������true����Ⱦѭ����ֹͣ���У�֮�����ǾͿ��Թر�Ӧ�ó���
	while (!glfwWindowShouldClose(window))
	{
		// ��ÿһ֡�����Ǽ�����µ�deltaTime�Ա�����
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// ÿ��ѭ����ʼ����û��Ƿ��¼����ϵ�ĳЩ������ִ�ж�Ӧ����
		processInput(window);

		// ��Ⱦָ��
		// ���������Ļ���õ���ɫ
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		// �����Ļ����ɫ�������Ȼ���
		// ����glClear�����������ɫ����֮��������ɫ���嶼�ᱻ���ΪglClearColor�������õ���ɫ��
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// ʹ����ɫ��
		ourShader.use();

		// �����۲����
		// ������ӽ��������Ŀռ�
		glm::mat4 view;
		// ����һ������(Look at)����Ŀ��Ĺ۲����
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		// ����ͶӰ����
		glm::mat4 projection;
		// ����Ұ��һ�����ó�45����߱ȣ���ƽ�棨�����ڽ�ƽ���Զƽ�����Ҵ���ƽ��ͷ���ڵĶ��㶼�ᱻ��Ⱦ����Զƽ�棩
		// ��ƽ�������ù���Ὣ��������������꣨��0.0f��10.0f֮�䣩���ü���
		projection = glm::perspective(glm::radians(fov), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);

		ourShader.setMat4("view", view);
		ourShader.setMat4("projection", projection);

		// �󶨶���������� 
		glBindVertexArray(VAO);

		// ����������� 
		for (int i = 0; i < 10; ++i) {
			glm::mat4 model;
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			ourShader.setMat4("model", model);
			// ������ 
			// (������Ƶ�OpenGLͼԪ�����ͣ������������ʼ������������ƶ��ٸ�����)
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// ������ 
		// glDrawArrays(GL_TRIANGLES, 0, 36);
		// �����ύ����ɫ���壨����һ��������GLFW����ÿһ��������ɫֵ�Ĵ󻺳壩��������һ�����б��������ƣ����ҽ�����Ϊ�����ʾ����Ļ�ϡ�
		glfwSwapBuffers(window);
		// ���������û�д���ʲô�¼�������������롢����ƶ��ȣ������´���״̬�������ö�Ӧ�Ļص�����������ͨ���ص������ֶ����ã���
		glfwPollEvents();
	}


	// ɾ������������� 
	glDeleteVertexArrays(1, &VAO);
	// ɾ�����㻺����� 
	glDeleteBuffers(1, &VBO);
	// ɾ��Ԫ�ػ������ 
	glDeleteBuffers(1, &EBO);

	// ��ֹGLFW������У��ͷ���ص�ϵͳ��Դ��
	glfwTerminate();
	return 0;
}

// ÿ�����ڸı��С��GLFW�������������������Ӧ�Ĳ������㴦��
static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}


// ����û���������
static void processInput(GLFWwindow* window)
{
	// ������ΪEsc��ʱ���رմ���
		// ����������᷵����������Ƿ����ڱ�����
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		// ����������WindowShouldClose��������Ϊ true���ر�GLFW��
		glfwSetWindowShouldClose(window, true);
	}

	// ��������ٶ�
	float cameraSpeed = 2.5f * deltaTime;
	// ����w,s,a,d��Ӧ������ƶ�����
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

// �������ƶ�
static void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = (float)xposIn;
	float ypos = (float)yposIn;

	// ��������ǲ��ǵ�һ�ν�����Ļ
	// �ǵĻ�ǿ�ƽ����λ�ñ�Ϊ��ʼֵ
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	// ���㵱ǰ֡����һ֡���λ�õ�ƫ����
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // ע���������෴�ģ���Ϊy�����Ǵӵײ����������������
	lastX = xpos;
	lastY = ypos;

	// ������
	float sensitivity = 0.1f; 
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	// ���Ƹ����Ƕ�
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	// ͨ�������Ǻ�ƫ�����������Եõ������ķ�������
	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
}

// �������ֹ���ʱ�������ţ�����Ұ��FOV�������޸�
static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (fov >= 1.0f && fov <= 45.0f)
		fov -= yoffset;
	if (fov <= 1.0f)
		fov = 1.0f;
	if (fov >= 45.0f)
		fov = 45.0f;
}