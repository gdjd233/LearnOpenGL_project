#include <tool/headOpenGL.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
static void processInput(GLFWwindow* window);

// ���ô�������
static const int screenWidth = 800;
static const int screenHeight = 600;

int solve_01(int avgc, char* argv[])
{
	// OpenGL
	// һ�������˺������ֺ������ͼ��API����ʽ�淶

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




		// �����ύ����ɫ���壨����һ��������GLFW����ÿһ��������ɫֵ�Ĵ󻺳壩��������һ�����б��������ƣ����ҽ�����Ϊ�����ʾ����Ļ�ϡ�
		glfwSwapBuffers(window);
		// ���������û�д���ʲô�¼�������������롢����ƶ��ȣ������´���״̬�������ö�Ӧ�Ļص�����������ͨ���ص������ֶ����ã���
		glfwPollEvents();
	}

	// ��ֹGLFW������У��ͷ���ص�ϵͳ��Դ
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