#include "lib\shader.h"
#include "lib\camera.h"
#include "lib\model.h"
#include "lib\light.h"
#include "lib\mouse.h"
#include "lib\render.h"
#include "lib\keyboard.h"
#include "lib\shader.h"
#include "lib\sphere.h"
#include "lib\collision.h"
#include <iostream>

smCamera*camera;
smMouse* mouse;
smKeyBoard* keyBoard;

smRender render;
world myworld("myworld");


int SCR_WIDTH;
int SCR_HEIGHT;
smShader *elementShader;
smShader *texShader;
smShader *shadowShader;
smShader * paticleShader;

vector<collosion> objV;
collosion fly;
vector<object> totalObj;
object * flyP;
object *obFly;

cloud testCloud;

glm::vec3 cameraPosition(0.f, 50.f, 30.f);
glm::vec3 cameraDir(0, 0, -30);



void initParticle(particle * m)
{
	if (m->sphere == nullptr)
	{
		//m.tex = new texture();
		//m.tex->load("fly//fly.obj");
		//m.which = IS_TEXTURE;
		m->sphere = new Sphere(0.05, 45, glm::vec4(1.f, 0.f, 0.f, 1.f));
		m->which = IS_SPHERE;
	}
	m->isDead = false;
	float x = rand() / (double(RAND_MAX)) - 0.5;
	float z = rand() / (double(RAND_MAX)) - 0.5;
	float pos[3] = { x,50,z };


	float speed[3] = { (rand() / (double(RAND_MAX)) - 0.5)*0.1,(rand() / (double(RAND_MAX)))*0.5,(rand() / (double(RAND_MAX)) - 0.5)*0.1 };
	float aspeed[3] = { 0,0.005,0 };
	m->setPosition(pos[0], pos[1], pos[2]);
	m->setSpeed(speed[0], speed[1], speed[2]);
	m->setAccerator(aspeed[0], aspeed[1], aspeed[2]);

	
	if (fabs(x) < fabs(z))
	{
		m->lifetime = 2.0*fabs(z) / 0.5;
	}
	else {
		m->lifetime = 2.0*fabs(x) / 0.5;
	}
	m->deci = 0.1 + (rand() / (double(RAND_MAX)))*0.1;
}

/*
@return true for dead
*/
bool isDead(particle * m)
{
	if (m->y < 50)
	{
		return true;
	}
	return false;
}


static void smInit()
{
	srand((unsigned int)time(NULL));
	glewInit();
	glEnable(GL_DEPTH_TEST);
	elementShader = new smShader("files//element.vert", "files//element.frag");
	texShader = new smShader("files//tex.vert", "files//tex.frag");
	shadowShader = new smShader("files//shadow.vert", "files//shadow.frag");
	camera = new smCamera(cameraPosition, cameraDir);
	mouse = new smMouse();
	keyBoard = new smKeyBoard();

	paticleShader = new smShader("files//paticle.vert", "files//paticle.frag");

	glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
}
unsigned int cnt = 0;

static void smTimer(int id)
{
	if (keyBoard->getKey((keyMap)'d') == true)
	{
		camera->moveCamera(-5, 0);
	}
	if (keyBoard->getKey((keyMap)'a') == true)
	{
		camera->moveCamera(5, 0);
	}
	if (keyBoard->getKey((keyMap)'s') == true)
	{
		camera->moveCamera(0, -5);
	}
	if (keyBoard->getKey((keyMap)'w') == true)
	{
		camera->moveCamera(0, 5);
	}

	if (keyBoard->getKey((keyMap)'q') == true)
	{
		camera->moveHCamera(10);
	}
	if (keyBoard->getKey((keyMap)'e') == true)
	{
		camera->moveHCamera(-10);
	}


	if (keyBoard->getKey((keyMap)'j') == true)
	{
		obFly->translate(-50, 0, 0);
	}
	if (keyBoard->getKey((keyMap)'l') == true)
	{
		obFly->translate(50, 0, 0);
	}
	if (keyBoard->getKey((keyMap)'i') == true)
	{
		obFly->translate(0, 0, -80);
	}
	if (keyBoard->getKey((keyMap)'k') == true)
	{
		obFly->translate(0, 0, 50);
	}
	if (keyBoard->getKey((keyMap)'i') == true)
	{
		obFly->translate(0, 50, 0);
	}
	if (keyBoard->getKey((keyMap)'k') == true)
	{
		obFly->translate(0, -50, 0);
	}


	glutTimerFunc(100, smTimer, id);
	glutPostRedisplay();
}

static void smDisplay() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	render.render(myworld, *camera);
	glutSwapBuffers();
}
static void smReshape(int w, int h) {
	SCR_WIDTH = w;
	SCR_HEIGHT = h;
}


std::vector<glm::vec3> posRe;
glm::mat4 model;
DWORD WINAPI ThreadMethod(LPVOID lpParameter)//执行线程任务的函数
{
	Sleep(10000);
	BOOL geifen = TRUE;
	while (geifen)
	{
		model = obFly->getModel();
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++)
				cout << model[j][i] << "\t";
			cout << endl;
		}
		posRe = fly.top;

		cout << "111: " << posRe.size() << "  " << fly.posRe.size() << endl;

		for (int i = 0; i < posRe.size(); i++) {
			int last = 1;
			glm::mat4 tem;
			double result[4];
			for (int j = 0; j < 4; j++) {
				result[j] = 0;
				for (int k = 0; k<3; k++)
					result[j] += ((double)(model[k][j])) * (double)(posRe[i][k]);
				result[j] += ((double)(model[3][j])) * last;
			}
			for (int j = 0; j < 3; j++) {
				fly.topR[i][j] = result[j] / result[3];
			}

		}

		fly.gen_obb_box(fly.topR);
		for (int i = 0; i < objV.size(); i++) {
			cout << fly.check_collision(objV[i]);
			cout << "the xyz of building" << i << " is = " << objV[i].a.center[0] << " " << objV[i].a.center[1] << " " << objV[i].a.center[2] << endl;
			cout << "the xyz of fly is " << " " << fly.a.center[0] << " " << fly.a.center[1] << " " << fly.a.center[2] << endl;
			if (fly.check_collision(objV[i]))
				cout << "碰撞了:" << i << endl;

		}
		Sleep(10);

	}
	return 0;
}

void dealBox() {
	char boxFileName[50];
	strcpy(boxFileName, "city//tem//6//test6.obj");
	for (int i = 1; i <= 8; i++) {
		object obj1;
		boxFileName[11] = i + '0';
		boxFileName[18] = i + '0';
		obj1.load(boxFileName);
		obj1.scale(0.05, 0.05, 0.05);
		collosion temC;
		temC.gen_obb_box(obj1.returnPos());
		temC.div20();
		objV.push_back(temC);
	}
	HANDLE hThread = NULL;
	DWORD dwThreadID = 0;//保存线程ID
	hThread = CreateThread(0, 0, ThreadMethod, NULL, 0, &dwThreadID);//创建线程
	CloseHandle(hThread);//关闭内核对象,不会停止线程
}

void build() {
	static scene tmp;

	tmp.push_back((new texture())->load("city//test3.obj")->scale(0.05, 0.05, 0.05));
	obFly = new object();
	obFly->load("fly//fly.obj");
	obFly->scale(0.05, 0.05, 0.05);
	fly.setPos(obFly->returnPos());
	fly.gen_obb_box(obFly->returnPos());
	tmp.push_back(obFly);
	fly.initTop();
	dealBox();

	
	testCloud.initParticle = initParticle;
	testCloud.isDead = isDead;
	testCloud.maxSize = 1000;
	testCloud.init();
	tmp.push_back(&testCloud);


	myworld.push_back(&tmp);
	myworld.push_back(new smLight(0, glm::vec3(55.f, 500.f, 23.f), glm::vec3(.7f, .7f, .7f), glm::vec3(.2f, .2f, .2f), 1.f));
	
}
static void smMouseFunc(int x, int y) {
	mouse->now.x = float(x);
	mouse->now.y = float(y);

	glutPostRedisplay();
}
static void smDrag(int x, int y) {
	float getx;
	float gety;
	if (abs(-mouse->pre.x + float(x))<100 && abs(-mouse->pre.y + float(y))<100)
		camera->rotateCamera((-mouse->pre.x + float(x)) / 40, (-mouse->pre.y + float(y)) / 40);
	mouse->pre.x = float(x);
	mouse->pre.y = float(y);
}
static void smClick(int button, int state, int x, int y) {
	mouse->state[button] = state;
	mouse->pre.x = float(x);
	mouse->pre.y = float(y);
}
static void smWheel(int wheel, int dir, int x, int y) {
	if (dir > 0)camera->zoomCamera(.95f);
	if (dir < 0)camera->zoomCamera(1.05f);

	glutPostRedisplay();
}

static void smKeyDown(unsigned char cAscii, int x, int y)
{
	static keyMap mykeys;
	if (cAscii == 'a' || cAscii == 's' || cAscii == 'd' || cAscii == 'w' || cAscii == 'q' || cAscii == 'e' || cAscii == 'j' || cAscii == 'k' || cAscii == 'l' || cAscii == 'i')
	{
		keyBoard->getKey((keyMap)cAscii) = true;
	}
}
static void smKeyUp(unsigned char cAscii, int x, int y)
{
	if (cAscii == 'a' || cAscii == 's' || cAscii == 'd' || cAscii == 'w' || cAscii == 'q' || cAscii == 'e' || cAscii == 'j' || cAscii == 'k' || cAscii == 'l' || cAscii == 'i')
	{
		keyBoard->getKey((keyMap)cAscii) = false;
	}
}

int main(int argc, char*argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);

	glutInitWindowSize(800, 600);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("shina mashiro");
	smInit();

	glutDisplayFunc(smDisplay);
	glutReshapeFunc(smReshape);


	glutPassiveMotionFunc(smMouseFunc);
	glutMotionFunc(smDrag);
	glutMouseFunc(smClick);
	glutMouseWheelFunc(smWheel);

	glutKeyboardFunc(smKeyDown);
	glutKeyboardUpFunc(smKeyUp);

	glutTimerFunc(100, smTimer, 1);

	build();
	glutMainLoop();
	return 0;


}
