#include <tool/headOpenGL.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
static void processInput(GLFWwindow* window);

// ���ô�������
static const int screenWidth = 800;
static const int screenHeight = 600;

// ������ɫ��
static const char* vertexShaderSource =
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

// Ƭ����ɫ��
static const char* fragmentShaderSource =
"#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";

/*
����1
��Ӹ��ඥ�㵽�����У�ʹ��glDrawArrays�����Ի��������˴������������Σ�
*/

int work_02_01(int avgc, char* argv[])
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



	// �����˴������������ζ�������
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
	// ������ɫ������GPU�ϴ����ڴ����ڴ������ǵĶ������ݣ���Ҫ����OpenGL��ν�����Щ�ڴ棬����ָ������η��͸��Կ���������ɫ�����Żᴦ���������ڴ���ָ�������Ķ��㡣
	// ����ͨ�����㻺�����(Vertex Buffer Objects, VBO)��������ڴ棬������GPU�ڴ棨ͨ������Ϊ�Դ棩�д���������㡣ʹ����Щ�������ĺô������ǿ���һ���Եķ���һ�������ݵ��Կ��ϣ�������ÿ�����㷢��һ�Ρ�

	// VAO
	// �����������(Vertex Array Object, VAO)�����񶥵㻺������������󶨣��κ����Ķ������Ե��ö��ᴢ�������VAO�С������ĺô����ǣ������ö�������ָ��ʱ����ֻ��Ҫ����Щ����ִ��һ�Σ�֮���ٻ��������ʱ��ֻ��Ҫ����Ӧ��VAO�����ˡ���ʹ�ڲ�ͬ�������ݺ���������֮���л���÷ǳ��򵥣�ֻ��Ҫ�󶨲�ͬ��VAO�����ˡ�

	// ���������������VAO���Ͷ��㻺�����VBO��
	unsigned int VBO, VAO;
	// ����һ�����л���ID��VBO�����VAO����
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	// �󶨶����������Ͷ��㻺�����
	// ����ʹ�õ��κΣ���GL_ARRAY_BUFFERĿ���ϵģ�������ö����������õ�ǰ�󶨵Ļ���(VBO)
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindVertexArray(VAO);
	// glBufferData��һ��ר���������û���������ݸ��Ƶ���ǰ�󶨻���ĺ���
	// (Ŀ�껺�������(���㻺�����ǰ�󶨵�GL_ARRAY_BUFFERĿ����), ָ���������ݵĴ�С(���ֽ�Ϊ��λ), ����ϣ�����͵�ʵ������,����ϣ���Կ���ι������������)
	// ��4��������3����ʽ:
	// GL_STATIC_DRAW �����ݲ���򼸺�����ı䡣
	// GL_DYNAMIC_DRAW�����ݻᱻ�ı�ܶࡣ
	// GL_STREAM_DRAW ������ÿ�λ���ʱ����ı䡣
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// ����OpenGL����ν�����������
	/*
	��һ�����������õĶ������ԣ��ڶ�����ɫ�����Ѿ�ʹ��layout(location = 0)������position��������
	�ڶ�������: �������Դ�С�����ﶥ��������vec3����3��ֵ��ɣ���С��3
	�������������������ͣ�vec3����float���ͣ�������GL_FLOAT
	���ĸ��������Ƿ�ϣ�����ݱ�׼�����������������ݶ���ӳ�䵽0���з�������������-1����1��Χ������ѡ��GL_FALSE)
	���������������������һ�����ݣ�vec3��������3 * sizeof(float)���������ǽ���������Ҳ�������ó�0��OpenGL�Լ�����
	������������λ�������ڻ�������ʼλ�õ�ƫ����������������(void*)��������Ҫǿ��ת����������0
	*/
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	// �Ѿ�����OpenGL��ν��Ͷ��������ˣ�������Ҫ���ö�������
	// ������ָ��������λ��ֵ
	glEnableVertexAttribArray(0);

	// ���VAO����������VAO���þͲ���������޸����VAO��������������ٷ���
	glBindVertexArray(0);
	// ���VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	// ����������ɫ�������Ƭ����ɫ��������ID�����ã�
	unsigned int vertexShader;
	unsigned int fragmentShader;
	// ��������ɫ�������Բ�����ʽ�ṩ��glCreateShader
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// ��������ɫ��Դ���븽�ӵ�������ɫ�������ϣ���Ƭ����ɫ��Դ���븽�ӵ�Ƭ����ɫ�������� 
	// (Ҫ�������ɫ������, ���ݵ�Դ���ַ�������, ������ɫ��������Դ��,ָ���ַ������ȵ�����)
	// ��4����������NULL���Զ�������ɫ��Դ��ÿһ���ַ�������
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);

	// ���붥����ɫ��Ƭ����ɫ�� 
	glCompileShader(vertexShader);
	glCompileShader(fragmentShader);


	// ���ڼ����ɫ�����������״̬�Լ��洢��Ϣ��־�ı���
	int success;
	char infoLog[512];
	// ��ȡ������ɫ���ı���״̬
	// (Ҫ��ѯ����ɫ��������ɫ������Ĳ�������������Ĳ������ֵ��
	// GL_COMPILE_STATUS������֧����ɫ����������ʵ�֣������ɫ���ϵ����һ�α�������ɹ����򷵻�GL_TRUE�����򷵻�GL_FALSE
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	// ��鶥����ɫ���Ƿ����ɹ������ʧ�������������Ϣ��־
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// ��ȡƬ����ɫ���ı���״̬ 
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	// ���Ƭ����ɫ���Ƿ����ɹ������ʧ�������������Ϣ��־ 
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// ������ɫ�����򣬺�������һ�����򣬲������´�����������ID����
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	// ��������ɫ����Ƭ����ɫ�����ӵ���ɫ��������
	// (���ܸ�����ɫ���ĳ������, ��Ҫ�����ӵ��������� shader ����)
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	// ������ɫ������
	glLinkProgram(shaderProgram);

	// ��ȡ��ɫ�����������״̬
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	// �����ɫ�������Ƿ����ӳɹ������ʧ�������������Ϣ��־
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::PROGRAM::SHADER::LINKING_FAILED\n" << infoLog << std::endl;
	}

	// ��ɫ���������ӵ���������Ժ���ɾ����ɫ���������ǲ�����Ҫ�����ˣ�
	// ɾ���Ѿ����ӵ���ɫ������Ķ�����ɫ����Ƭ����ɫ������ 
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// �����߿�ģʽ
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);



	// ����������ÿ��ѭ���Ŀ�ʼǰ���һ��GLFW�Ƿ�Ҫ���˳�������ǵĻ����ú�������true����Ⱦѭ����ֹͣ���У�֮�����ǾͿ��Թر�Ӧ�ó���
	while (!glfwWindowShouldClose(window))
	{
		// ÿ��ѭ����ʼ����û��Ƿ��¼����ϵ�ĳЩ������ִ�ж�Ӧ����
		processInput(window);

		// ��Ⱦָ��
		// ���������Ļ���õ���ɫ
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		// �����ɫ����������׼��������һ֡����Ⱦ
		// ����glClear�����������ɫ����֮��������ɫ���嶼�ᱻ���ΪglClearColor�������õ���ɫ��
		glClear(GL_COLOR_BUFFER_BIT);


		// ʹ����ɫ������ 
		glUseProgram(shaderProgram);
		// �󶨶���������� 
		glBindVertexArray(VAO);
		// ���������� 
		// (������Ƶ�OpenGLͼԪ�����ͣ������������ʼ������������ƶ��ٸ�����)
		glDrawArrays(GL_TRIANGLES, 0, 6);


		// �����ύ����ɫ���壨����һ��������GLFW����ÿһ��������ɫֵ�Ĵ󻺳壩��������һ�����б��������ƣ����ҽ�����Ϊ�����ʾ����Ļ�ϡ�
		glfwSwapBuffers(window);
		// ���������û�д���ʲô�¼�������������롢����ƶ��ȣ������´���״̬�������ö�Ӧ�Ļص�����������ͨ���ص������ֶ����ã���
		glfwPollEvents();
	}



	// ɾ������������� 
	glDeleteVertexArrays(1, &VAO);
	// ɾ�����㻺����� 
	glDeleteBuffers(1, &VBO);
	// ɾ����ɫ������ 
	glDeleteProgram(shaderProgram);


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
}