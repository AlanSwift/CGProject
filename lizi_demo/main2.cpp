#include "Particle.h"
#include "stdio.h"
#include "windows.h"

CParticle Snow;

/** �����������ӵ�����ֵ */
float x, y, z, vx, vy, vz, ax, ay, az, size, lifetime, dec;
int r, g, b;


unsigned char * LoadBitmapFile(char *filename, int * w, int * h)
{
	BITMAPINFOHEADER bitmapInfoHeader;
	FILE *filePtr;	// �ļ�ָ��
	BITMAPFILEHEADER bitmapFileHeader;	// bitmap�ļ�ͷ
	unsigned char	*bitmapImage;		// bitmapͼ������
	int	imageIdx = 0;		// ͼ��λ������
	unsigned char	tempRGB;	// ��������
	char temName[100] = { 0 };
//	strcat(temName, "bmp/");
	strcat(temName, filename);
	// �ԡ�������+����ģʽ���ļ�filename 
	filePtr = fopen(temName, "rb");
	if (filePtr == NULL) {
		printf("Open Error!\n");
		return NULL;
	}
	// ����bitmap�ļ�ͼ
	fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
	//// ��֤�Ƿ�Ϊbitmap�ļ�
	//if (bitmapFileHeader.bfType != BITMAP_ID) {
	//	fprintf(stderr, "Error in LoadBitmapFile: the file is not a bitmap file\n");
	//	return NULL;
	//}

	// ����bitmap��Ϣͷ
	fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
	// ���ļ�ָ������bitmap����
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);
	// Ϊװ��ͼ�����ݴ����㹻���ڴ�
	bitmapImage = new unsigned char[bitmapInfoHeader.biSizeImage];
	// ��֤�ڴ��Ƿ񴴽��ɹ�
	if (!bitmapImage) {
		fprintf(stderr, "Error in LoadBitmapFile: memory error\n");
		return NULL;
	}

	// ����bitmapͼ������
	fread(bitmapImage, 1, bitmapInfoHeader.biSizeImage, filePtr);
	// ȷ�϶���ɹ�
	if (bitmapImage == NULL) {
		fprintf(stderr, "Error in LoadBitmapFile: memory error\n");
		return NULL;
	}

	//����bitmap�б���ĸ�ʽ��BGR�����潻��R��B��ֵ���õ�RGB��ʽ
	for (imageIdx = 0;
		imageIdx < bitmapInfoHeader.biSizeImage; imageIdx += 3) {
		tempRGB = bitmapImage[imageIdx];
		bitmapImage[imageIdx] = bitmapImage[imageIdx + 2];
		bitmapImage[imageIdx + 2] = tempRGB;
	}
	// �ر�bitmapͼ���ļ�
	fclose(filePtr);
	*w = (int)(bitmapInfoHeader.biWidth);
	*h = (int)(bitmapInfoHeader.biHeight);
	return bitmapImage;
}

/** �������� */
GLuint  texName[2];
bool LoadTextures()
{
	GLbyte *pBits[2];
	int nWidth, nHeight;

	char* fileName[] = { "bmp/wall.bmp","bmp/snowball.bmp" };

	for (int i = 0; i<2; i++)
	{
		pBits[i] = (GLbyte*)LoadBitmapFile(fileName[i], &nWidth, &nHeight);
		if (pBits[i] == NULL)
		{
			printf("bmp load failed");
			exit(-1);
		}

		glGenTextures(1, &texName[i]);

		/** ����������� */
		glBindTexture(GL_TEXTURE_2D, texName[i]);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, nWidth, nHeight,
			GL_RGB, GL_UNSIGNED_BYTE, pBits[i]);
	}
	return true;
}


/** ���Ƶ��� */
float angle = 0;
void DrawGround()
{
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -6.0f);
	glRotatef(angle, 0.0f, 1.0f, 0.0f);

	/** ָ������ */
	glBindTexture(GL_TEXTURE_2D, texName[0]);
	glColor4ub(255, 255, 255, 255);
	glNormal3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-2.5f, -1.0f, 2.5f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-2.5f, -1.0f, -2.5f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(2.5f, -1.0f, -2.5f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(2.5f, -1.0f, 2.5f);
	glEnd();
	glPopMatrix();

	/** ����ת���ĽǶ� */
	angle += 0.02f;
}

/** ��ʼ��ѩ������ */
bool InitSnow()
{
	for (int i = 0; i < Snow.GetNumOfParticle(); ++i)
	{
		///��ʼ����ɫ����ɫ��
		r = 255;
		g = 255;
		b = 255;
		Snow.SetColor(i, r, g, b);

		///��ʼ������
		x = 0.1f * (rand() % 50) - 2.5f;
		y = 2 + 0.1f * (rand() % 2);
		if ((int)x % 2 == 0)
			z = rand() % 6;
		else
			x = -rand() % 3;
		Snow.SetPosition(i, x, y, z);

		///��ʼ���ٶ�
		vx = 0.00001 * (rand() % 100);
		vy = 0.0000002 * (rand() % 28000);
		vz = 0;
		Snow.SetVelocity(i, vx, vy, vz);

		///��ʼ�����ٶ�
		ax = 0;
		ay = 0.000005f;
		az = 0;
		Snow.SetAcceleration(i, ax, ay, az);

		///��ʼ����������
		lifetime = 100;
		Snow.SetLifeTime(i, lifetime);

		///��ʧ�ٶ�
		dec = 0.005 * (rand() % 50);
		Snow.SetDec(i, dec);

		///��ʼ����С
		Snow.SetSize(i, 0.03f);
	}
	return true;
}

/** �������� */
void UpdateSnow()
{
	/** ����λ�� */
	x += (vx * 5);
	y -= vy;

	/** �����ٶ� */
	vy += ay;

	/** ��������ʱ�� */
	lifetime -= dec;

	if (x > 3)
		x = -2;

	/** ���������ʧ���������� */
	if (y <= -1 || lifetime <= 0)
	{
		/** ��ʼ��λ�� */
		x = 0.1f * (rand() % 50) - 2.5f;
		y = 2 + 0.1f * (rand() % 2);
		if ((int)x % 2 == 0)
			z = rand() % 6;
		else
			z = -rand() % 3;

		/** ��ʼ���ٶ� */
		vx = (float)(0.00001 * (rand() % 100));
		vy = (float)(0.0000002 * (rand() % 28000));
		vz = 0;

		/** ��ʼ�����ٶ� */
		ax = 0;
		ay = 0.000005f;
		az = 0;
		lifetime = 100;
		dec = 0.005*(rand() % 50);
	}
}

/** �������� */
void DrawParticle()
{
	/** ������ */
	glBindTexture(GL_TEXTURE_2D, texName[1]);

	for (int i = 0; i<Snow.GetNumOfParticle(); ++i)
	{
		/** ������ӵ��������� */
		Snow.GetAll(i, r, g, b, x, y, z, vx, vy, vz, ax, ay, az, size, lifetime, dec);
		glLoadIdentity();
		glTranslatef(0.0f, 0.0f, -6.0f);
		glColor4ub(r, g, b, 255);
		glNormal3f(0.0f, 0.0f, 1.0f);   /**< ���巨�߷��� */
										/** �������� */
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x - size, y - size, z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x - size, y + size, z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x + size, y + size, z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x + size, y - size, z);
		glEnd();

		/** ������������ */
		UpdateSnow();
		Snow.SetAll(i, r, g, b, x, y, z, vx, vy, vz, ax, ay, az, size, lifetime, dec);
	}
	glutPostRedisplay();//������Ⱦ����
}

/////////////////////////////////////////////////////////////////
/** ��ʼ�� */
void init(void)
{
	/** �û��Զ���ĳ�ʼ������ */
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
	glClearDepth(1.0f);//ָ����Ȼ����������ֵ(������Ȼ��������ֵ����Ϊ1)
	glDepthFunc(GL_LEQUAL);//ָ��������Ȼ���Ƚ�ֵ(���½��������ֵ��ԭ����1�Ƚϣ�<=��ͨ����������)
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glEnable(GL_TEXTURE_2D);             /**< ��������ӳ�� */
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);    /**< ���û�����ӻ�ð�͸��Ч�� */
	glEnable(GL_BLEND);                  /**< ���û�� */

										 /**  �������� */
	if (!LoadTextures())
	{
		printf("bmp load failed");
		exit(-1);
	}

	/** ����500������ */
	Snow.Create(500);

	/** ��ʼ������ */
	InitSnow();
}

/** ��Ⱦ */
void display(void)
{
	/** �û��Զ���Ļ��ƹ��� */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	/** ���Ƶ��� */
	DrawGround();

	/** �������� */
	DrawParticle();

	glFlush(); /**<ǿ��ִ�����е�OpenGL���� */
}


void ChangeSize(int width, int height)
{
	glViewport(0, 0, width, height);    /**< ���������ӿ� */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 1700.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_RGB);
	glutInitWindowSize(400, 400);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("ѩ������");
	init();
	glutReshapeFunc(ChangeSize);
	glutDisplayFunc(display);
	glutMainLoop();
	return 0;
}