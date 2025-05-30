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

// ���ô�������
static const int screenWidth = 800;
static const int screenHeight = 600;

// �ж�����Ƿ��һ�ν��봰��
static bool firstMouse = true;
// ��һ֡����λ��
static float lastX = (float)screenWidth / 2.0f;
static float lastY = (float)screenHeight / 2.0f;

// ���������ϵͳ
static Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

// ��������ʱ���
static float deltaTime = 0.0f; // ��ǰ֡����һ֡��ʱ���
static float lastFrame = 0.0f; // ��һ֡��ʱ��

// ���õƹ�λ��
static glm::vec3 lightPos(1.2f, 1.0f, 2.0f);


/*
����ʵ��һ��Gouraud��ɫ�������Ƿ�����ɫ��������������˻���������Ĺ���Ӧ�ûῴ������Щ��֣���������Ϊʲô���ῴ������ô���

��ô���ǿ�����ʲô��
����������й۲��鿴�ṩ��ͼ�����������������������֮��������Եķֽ��ߡ�����"����"֮���Կɼ���������Ƭ�β�ֵ��ɵġ�
��ʾ��ͼ���п��Կ������������������ϽǶ�����ھ���߹⡣
�������������ε����϶��㱻����������������������������δ�������������Ĺ���ֵ�ͻ�ͨ����ֵ���ɵ������������㡣
���Ͻ�������Ҳ����ͬ��������������м�Ƭ�ε���ɫ����ֱ�����Թ�Դ�����ǲ�ֵ����Ľ���������м�Ƭ�εĹ��ճ��ִ���״̬��
ʹ�����������������������ε������໥��ͻ������������������֮���γɿɼ��������ֽ����ơ�

��ͼ�νṹ��Ϊ����ʱ������ЧӦ����ֵø������ԡ�

*/

int work_09_04(int avgc, char* argv[])
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
	// ������Ȳ���
	// ��Ƭ����Ҫ���������ɫʱ��OpenGL�Ὣ�������ֵ��z������бȽϣ������ǰ��Ƭ��������Ƭ��֮�������ᱻ���������򽫻Ḳ�ǡ�
	glEnable(GL_DEPTH_TEST);


	// ������ɫ����
	// ����ռ�����ص���ɫ����
	Shader lightingShader("./exercises/09_basic_lighting/shader4/cube.vs", "./exercises/09_basic_lighting/shader4/cube.fs");
	// �롰���������塱���������Դ����ص���ɫ��
	Shader lightCubeShader("./exercises/09_basic_lighting/shader4/light_cube.vs", "./exercises/09_basic_lighting/shader4/light_cube.fs");


	// ���壨λ��3������2, ������1��
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
	// ������ɫ������GPU�ϴ����ڴ����ڴ������ǵĶ������ݣ���Ҫ����OpenGL��ν�����Щ�ڴ棬����ָ������η��͸��Կ���������ɫ�����Żᴦ���������ڴ���ָ�������Ķ��㡣
	// ����ͨ�����㻺�����(Vertex Buffer Objects, VBO)��������ڴ棬������GPU�ڴ棨ͨ������Ϊ�Դ棩�д���������㡣ʹ����Щ�������ĺô������ǿ���һ���Եķ���һ�������ݵ��Կ��ϣ�������ÿ�����㷢��һ�Ρ�

	// VAO
	// �����������(Vertex Array Object, VAO)�����񶥵㻺������������󶨣��κ����Ķ������Ե��ö��ᴢ�������VAO�С������ĺô����ǣ������ö�������ָ��ʱ����ֻ��Ҫ����Щ����ִ��һ�Σ�֮���ٻ��������ʱ��ֻ��Ҫ����Ӧ��VAO�����ˡ���ʹ�ڲ�ͬ�������ݺ���������֮���л���÷ǳ��򵥣�ֻ��Ҫ�󶨲�ͬ��VAO�����ˡ�

	// ���������������VAO���Ͷ��㻺�����VBO��
	unsigned int VBO, cubeVAO, lightCubeVAO;
	// ����һ�����л���ID��VBO�����VAO����
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &cubeVAO);
	// �󶨶����������Ͷ��㻺�����
	// ����ʹ�õ��κΣ���GL_ARRAY_BUFFERĿ���ϵģ�������ö����������õ�ǰ�󶨵Ļ���(VBO)
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindVertexArray(cubeVAO);
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
	�ڶ�������: �������Դ�С�����ﶥ��������vec3�� ��3��ֵ��ɣ���С��3
	�������������������ͣ�vec3����float���ͣ�������GL_FLOAT
	���ĸ��������Ƿ�ϣ�����ݱ�׼�����������������ݶ���ӳ�䵽0���з�������������-1����1��Χ������ѡ��GL_FALSE)
	������������������������ǽ���������Ҳ�������ó�0��OpenGL�Լ�����
	������������λ�������ڻ�������ʼλ�õ�ƫ����������������(void*)��������Ҫǿ��ת����������0
	*/
	// ����λ������
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	// ���÷���������
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	// �Ѿ�����OpenGL��ν��Ͷ��������ˣ�������Ҫ���ö�������
	// ������ָ��������λ��ֵ
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);


	// ���ù�ԴVAO
	// ����ID�Ͱ�
	glGenVertexArrays(1, &lightCubeVAO);
	glBindVertexArray(lightCubeVAO);
	// ֻ��Ҫ��VBO�����ٴ�����VBO�����ݣ���Ϊ���ӵ�VBO�������Ѿ���������ȷ�������嶥������
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// ����λ������
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	// �Ѿ�����OpenGL��ν��Ͷ��������ˣ�������Ҫ���ö�������
	// ������ָ��������λ��ֵ
	glEnableVertexAttribArray(0);



	// ���VAO����������VAO���þͲ���������޸����VAO��������������ٷ���
	glBindVertexArray(0);
	// ���VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	// �����߿�ģʽ
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);




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
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		// �����Ļ����ɫ�������Ȼ���+
		// ����glClear�����������ɫ����֮��������ɫ���嶼�ᱻ���ΪglClearColor�������õ���ɫ��
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// ʹ��������ɫ��
		lightingShader.use();
		// ���� ������ɫ�� ������ɫ
		lightingShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
		// ���� ������ɫ�� ���屻�չ����ɫ
		lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);

		// ���ù�Դ�������Ϸ������ƶ�
		lightPos = glm::vec3(2.0f * sin(glfwGetTime()), 1.0f, 2.0f * cos(glfwGetTime()));


		// ��������ģ�;���, ������λ�ơ���������ת����
		glm::mat4 model = glm::mat4();
		// ����ͶӰ����
		// ����Ұ��һ�����ó�45����߱ȣ���ƽ�棨�����ڽ�ƽ���Զƽ�����Ҵ���ƽ��ͷ���ڵĶ��㶼�ᱻ��Ⱦ����Զƽ�棩
		// ��ƽ�������ù���Ὣ��������������꣨��0.0f��10.0f֮�䣩���ü���
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
		// �����۲����
		glm::mat4 view = camera.GetViewMatrix();



		// �Ѷ�Ӧ��Ϣ���ݸ�������ɫ��
		lightingShader.setMat4("model", model);
		lightingShader.setMat4("view", view);
		lightingShader.setMat4("projection", projection);
		lightingShader.setVec3("viewPos", camera.Position);
		lightingShader.setVec3("lightPos", lightPos);


		// �󶨶���������� 
		glBindVertexArray(cubeVAO);
		// ��������
		glDrawArrays(GL_TRIANGLES, 0, 36);


		// ʹ�ù�Դ��ɫ��
		lightCubeShader.use();
		// ���� ��Դ λ�ã���С
		model = glm::mat4();
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));
		// �Ѷ�Ӧ��Ϣ���ݸ���Դ��ɫ��
		lightCubeShader.setMat4("model", model);
		lightCubeShader.setMat4("view", view);
		lightCubeShader.setMat4("projection", projection);

		// �󶨶���������� 
		glBindVertexArray(lightCubeVAO);
		// ���ƹ�Դ
		glDrawArrays(GL_TRIANGLES, 0, 36);



		// �����ύ����ɫ���壨����һ��������GLFW����ÿһ��������ɫֵ�Ĵ󻺳壩��������һ�����б��������ƣ����ҽ�����Ϊ�����ʾ����Ļ�ϡ�
		glfwSwapBuffers(window);
		// ���������û�д���ʲô�¼�������������롢����ƶ��ȣ������´���״̬�������ö�Ӧ�Ļص�����������ͨ���ص������ֶ����ã���
		glfwPollEvents();
	}


	// ɾ������������� 
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &lightCubeVAO);
	// ɾ�����㻺����� 
	glDeleteBuffers(1, &VBO);


	// ��ֹGLFW������У��ͷ���ص�ϵͳ��Դ��
	glfwTerminate();
	return 0;
}

// ÿ�����ڸı��С��GLFW�������������������Ӧ�Ĳ������㴦��
static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}


/*����һ������������������һ����ΪprocessInput�ĺ������ú������ڴ����û����롣*/
static void processInput(GLFWwindow* window)
{
	// ������ΪEsc��ʱ���رմ���
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	// ����w,s,a,d��Ӧ������ƶ�����
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
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


	// ͨ�������Ǻ�ƫ�����������Եõ������ķ�������
	camera.ProcessMouseMovement(xoffset, yoffset);
}

// �������ֹ���ʱ�������ţ�����Ұ��FOV�������޸�
static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}