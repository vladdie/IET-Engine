#ifndef PhysicsController_h__
#define PhysicsController_h__

#include "AbstractController.h"
#include "Point.h" 
#include "GLParticleRenderer.h"
#include "ParticleSystem.h"
#include "BoxGenerator.h"
#include "EulerUpdater.h"
#include "Line.h"

namespace Controller
{

	using namespace std::placeholders;
	using namespace Physics::Particles::Renderer;
	using namespace Physics::Particles;

	class PhysicsController : public AbstractController {
	public:
		void PhysicsController::setupCurrentInstance();
		void Init(int argc, char* argv[]);
		void Draw();
		void Run();
		~PhysicsController();
		PhysicsController();
	private:
		Camera* d_camera;
		Shader* d_shader;
		GLParticleRenderer* d_particle_renderer;
		ParticleSystem* d_particle_system; 
		std::shared_ptr<BoxGenerator> d_box_generator;
		std::shared_ptr<ParticleEmitter> d_particle_emitter;
		std::shared_ptr<EulerUpdater> d_particle_updater;
	};

 
	void PhysicsController::Init(int argc, char* argv[])
	{
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
		glutInitWindowSize(VIEWPORT_WIDTH, VIEWPORT_HEIGHT);
		glutCreateWindow("Particle System"); 

		glewExperimental = GL_TRUE;
		glewInit();

		glutDisplayFunc(drawCallback);
		glutIdleFunc(drawCallback);

		this->d_camera = new Camera(glm::vec3(0.0f,0.0f,4.0f));
		d_camera->CameraType = FREE_FLY;
		d_camera->MovementSpeed = 2.0f;
		this->d_particle_renderer = new GLParticleRenderer();
		this->d_particle_system = new ParticleSystem(1000);
		
		d_box_generator = std::make_shared<BoxGenerator>(glm::vec4(0.0f,0.0f,0.0f,1.0f));
		d_particle_emitter = std::make_shared<ParticleEmitter>();
		d_particle_emitter->m_emit_rate = 100;
		d_particle_emitter->addGenerator(d_box_generator);

		d_particle_updater = std::make_shared<EulerUpdater>();
		d_particle_updater->m_globalAcceleration = glm::vec4(0.0f,-9.8f,0.0f,0.0f);

		d_particle_system->addUpdater(d_particle_updater);
		d_particle_system->addEmitter(d_particle_emitter);

		d_particle_renderer->generate(d_particle_system, false);

		//I know it may sound strange but new lambdas in C++ 11 are like this :-) I miss C# a bit :P
		UserMouseCallback = std::bind(&Camera::ProcessMouseMovement,d_camera, _1, _2);
		UserMouseScrollCallback = std::bind(&Camera::ProcessMouseScroll,d_camera,_1);


		glutKeyboardFunc(Callbacks::keyboardCallback);
		glutKeyboardUpFunc(Callbacks::keyboardUpCallback);
		glutPassiveMotionFunc(Callbacks::mouseCallback);

		glutSetCursor(GLUT_CURSOR_NONE); 

		glutWarpPointer(VIEWPORT_WIDTH/2, VIEWPORT_HEIGHT/2);

		glViewport(0, 0, VIEWPORT_WIDTH, VIEWPORT_HEIGHT); 

		d_shader = new Shader("particle.vert","particle.frag"); 

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_PROGRAM_POINT_SIZE); 
		//glEnable(GL_BLEND);
		
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE);

		d_time_at_reset = glutGet(GLUT_ELAPSED_TIME);
	}

	void PhysicsController::Run(){
		glutMainLoop();
	}

	void PhysicsController::Draw()
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

		update_timer(); 

		d_projection_matrix = glm::perspective(d_camera->Zoom, VIEWPORT_RATIO, 0.1f, 1000.0f);  
		
		d_camera->MoveCamera();

		d_view_matrix = d_camera->GetViewMatrix();

		d_shader->Use();
		
		d_shader->SetModelViewProjection(glm::mat4(),d_view_matrix,d_projection_matrix);
		Vertex vertex;
		vertex.Position = glm::vec3(0.5f,0.5f,0.5f);
		vertex.Color = glm::vec3(1.0f,1.0f,1.0f);

		Vertex vertex1;
		vertex1.Position = glm::vec3(1.0f,1.0f,1.0f);
		vertex.Color = glm::vec3(1.0f,1.0f,1.0f);

	/*	Point p(vertex);
		Point p2(vertex1);
		Line(vertex,vertex1).Draw();

		p.Draw();
		p2.Draw();*/
		d_particle_renderer->update();
		
		d_particle_system->update(d_delta_time_secs);

		d_particle_renderer->render();



		glutSwapBuffers();
	}

	 
	void PhysicsController::setupCurrentInstance(){
		Controller::g_CurrentInstance = this; 
	}

	PhysicsController::~PhysicsController()
	{
		free(d_camera);
		free(d_shader);
		free(d_particle_renderer);
		free(d_particle_system); 
	}

	PhysicsController::PhysicsController() 	
		: d_camera(nullptr),
		d_shader(nullptr)
	{
		setupCurrentInstance();
	}





}

#endif // PhysicsController_h__
