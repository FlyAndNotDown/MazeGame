#include <GL\glut.h>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
using namespace cv;
#include "define.h"

// ������ɫ
Color white, gray, green;
// �����ͼ
Map map;
// ����ƹ�
Light sunLight;
// ���巽��Ĳ���
Material cubeMaterial, playerCubeMaterial;
// ��ϲͼ���Ĳ���
Material completeMaterial;
// ���
Player player;
// �����
Camare fristPersonCamare, globalCamare;
// �����յ�
MapPosition startPosition, endPosition;
// ��ͼģʽ
ViewMode viewMode;
// ����ʼλ��
Vector2f mousePosition;
// ������﷽��
Cube playerCube;
// ǽ����ͼ
Texture wall, complete;

// xyz ���붥��ṹ��
void xyzToVertex(Vertex &vertex, float x, float y, float z) {
	vertex.x = x;
	vertex.y = y;
	vertex.z = z;
}

// ��������ṹ��
void vertexToVertex(Vertex &det, Vertex src) {
	det.x = src.x;
	det.y = src.y;
	det.z = src.z;
}

// �ĸ����������ṹ��
void vertexesToQuad(Quad4 &quad, Vertex vertex0, Vertex vertex1, Vertex vertex2, Vertex vertex3) {
	vertexToVertex(quad.vertexes[0], vertex0);
	vertexToVertex(quad.vertexes[1], vertex1);
	vertexToVertex(quad.vertexes[2], vertex2);
	vertexToVertex(quad.vertexes[3], vertex3);
}

// ���������λ��
void setCamarePosition(Camare &camare, GLfloat x, GLfloat y, GLfloat z) {
	camare.position[0] = x;
	camare.position[1] = y;
	camare.position[2] = z;
}

// �������������
void setCamareLookAt(Camare &camare, GLfloat x, GLfloat y, GLfloat z) {
	camare.lookAt[0] = x;
	camare.lookAt[1] = y;
	camare.lookAt[2] = z;
}

// �����������ת
void setCamareDirection(Camare &camare, GLfloat x, GLfloat y, GLfloat z) {
	camare.direction[0] = x;
	camare.direction[1] = y;
	camare.direction[2] = z;
}

// ���ݼ�����Ϣ�޸ĵ�һ�˳������
void setFristPersonCamareByPlayer() {
	// ���������λ��
	setCamarePosition(
		fristPersonCamare,
		player.y * MAP_BLOCK_LENGTH + MAP_BLOCK_LENGTH / 2,
		map.height * MAP_BLOCK_LENGTH - player.x * MAP_BLOCK_LENGTH - MAP_BLOCK_LENGTH / 2,
		MAP_BLOCK_LENGTH / 2
	);
	// ������������������������������
	switch (player.face) {
		case PLAYER_FACE_UP:
			setCamareLookAt(
				fristPersonCamare,
				fristPersonCamare.position[0],
				fristPersonCamare.position[1] + MAP_BLOCK_LENGTH * VIEW_STEP_LENGTH_XY,
				fristPersonCamare.position[2] - MAP_BLOCK_LENGTH * VIEW_STEP_LENGTH_Z * 2
			);
			break;
		case PLAYER_FACE_DOWN:
			setCamareLookAt(
				fristPersonCamare,
				fristPersonCamare.position[0],
				fristPersonCamare.position[1] - MAP_BLOCK_LENGTH * VIEW_STEP_LENGTH_XY,
				fristPersonCamare.position[2] - MAP_BLOCK_LENGTH * VIEW_STEP_LENGTH_Z * 2
			);
			break;
		case PLAYER_FACE_LEFT:
			setCamareLookAt(
				fristPersonCamare,
				fristPersonCamare.position[0] - MAP_BLOCK_LENGTH * VIEW_STEP_LENGTH_XY,
				fristPersonCamare.position[1],
				fristPersonCamare.position[2] - MAP_BLOCK_LENGTH * VIEW_STEP_LENGTH_Z * 2
			);
			break;
		case PLAYER_FACE_RIGHT:
			setCamareLookAt(
				fristPersonCamare,
				fristPersonCamare.position[0] + MAP_BLOCK_LENGTH * VIEW_STEP_LENGTH_XY,
				fristPersonCamare.position[1],
				fristPersonCamare.position[2] - MAP_BLOCK_LENGTH * VIEW_STEP_LENGTH_Z * 2
			);
			break;
	}
	// ����������Ϸ�������
	setCamareDirection(fristPersonCamare, 0, 0, 1);
}

// ��ͼ������ת����������ͼ��Ϣ
void loadTexture(Texture &texture) {
	// �� textureId ����Ϊ 2d ������Ϣ
	glGenTextures(1, &texture.id);
	glBindTexture(GL_TEXTURE_2D, texture.id);

	//����Ŵ���Сʹ�����Բ�ֵ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// ��ͼ���ڴ�����������Ϣ
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.width, texture.height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, texture.img.data);
}

// ��ʼ��
void init() {
	// ��ɫ
	white.r = 1.0f;
	white.g = 1.0f;
	white.b = 1.0f;
	// ��ɫ
	gray.r = 0.3f;
	gray.g = 0.3f;
	gray.b = 0.3f;
	// ��ɫ
	green.r = 0.4f;
	green.g = 1.0f;
	green.b = 0.6f;

	// ���صڶ��ŵ�ͼ
	map.width = MAP2_WIDTH;
	map.height = MAP2_HEIGHT;
	for (int i = 0; i < map.width; i++)
		for (int j = 0; j < map.height; j++)
			map.blocks[i][j] = MAP2_BLOCKS[i][j];

	// �����������
	sunLight.position[0] = 0.0f;
	sunLight.position[1] = 0.0f;
	sunLight.position[2] = 0.0f;
	sunLight.position[3] = 1.0f;
	sunLight.ambient[0] = 0.0f;
	sunLight.ambient[1] = 0.0f;
	sunLight.ambient[2] = 0.0f;
	sunLight.ambient[3] = 1.0f;
	sunLight.diffuse[0] = 1.0f;
	sunLight.diffuse[1] = 1.0f;
	sunLight.diffuse[2] = 1.0f;
	sunLight.diffuse[3] = 1.0f;
	sunLight.specular[0] = 1.0f;
	sunLight.specular[1] = 1.0f;
	sunLight.specular[2] = 1.0f;
	sunLight.specular[3] = 1.0f;

	// ���÷���Ĳ���
	cubeMaterial.ambient[0] = 0.0f;
	cubeMaterial.ambient[1] = 0.0f;
	cubeMaterial.ambient[2] = 0.5f;
	cubeMaterial.ambient[3] = 1.0f;
	cubeMaterial.diffuse[0] = 0.0f;
	cubeMaterial.diffuse[1] = 0.0f;
	cubeMaterial.diffuse[2] = 0.5f;
	cubeMaterial.diffuse[3] = 1.0f;
	cubeMaterial.specular[0] = 0.0f;
	cubeMaterial.specular[1] = 0.0f;
	cubeMaterial.specular[2] = 0.8f;
	cubeMaterial.specular[3] = 1.0f;
	cubeMaterial.emission[0] = white.r;
	cubeMaterial.emission[1] = white.g;
	cubeMaterial.emission[2] = white.b;
	cubeMaterial.emission[3] = 0;
	cubeMaterial.shininess = 20;
	playerCubeMaterial.ambient[0] = 0.0f;
	playerCubeMaterial.ambient[1] = 0.0f;
	playerCubeMaterial.ambient[2] = 0.5f;
	playerCubeMaterial.ambient[3] = 1.0f;
	playerCubeMaterial.diffuse[0] = 0.0f;
	playerCubeMaterial.diffuse[1] = 0.0f;
	playerCubeMaterial.diffuse[2] = 0.5f;
	playerCubeMaterial.diffuse[3] = 1.0f;
	playerCubeMaterial.specular[0] = 0.0f;
	playerCubeMaterial.specular[1] = 0.0f;
	playerCubeMaterial.specular[2] = 0.8f;
	playerCubeMaterial.specular[3] = 1.0f;
	playerCubeMaterial.emission[0] = green.r;
	playerCubeMaterial.emission[1] = green.g;
	playerCubeMaterial.emission[2] = green.b;
	playerCubeMaterial.emission[3] = 0;
	playerCubeMaterial.shininess = 20;
	// ���ͼ������
	completeMaterial.ambient[0] = 0.0f;
	completeMaterial.ambient[1] = 0.0f;
	completeMaterial.ambient[2] = 0.5f;
	completeMaterial.ambient[3] = 1.0f;
	completeMaterial.diffuse[0] = 0.0f;
	completeMaterial.diffuse[1] = 0.0f;
	completeMaterial.diffuse[2] = 0.5f;
	completeMaterial.diffuse[3] = 1.0f;
	completeMaterial.specular[0] = 0.0f;
	completeMaterial.specular[1] = 0.0f;
	completeMaterial.specular[2] = 0.8f;
	completeMaterial.specular[3] = 1.0f;
	completeMaterial.emission[0] = white.r;
	completeMaterial.emission[1] = white.g;
	completeMaterial.emission[2] = white.b;
	completeMaterial.emission[3] = 0;
	completeMaterial.shininess = 20;

	// ��ʼ����ͼ
	// ����ǽ����ͼͼƬ
	wall.img = imread(".\\texture.bmp");
	// ���ó���
	wall.width = wall.img.cols;
	wall.height = wall.img.rows;
	// ����ͨ��ͼƬ
	complete.img = imread(".\\complete.jpg");
	complete.width = complete.img.cols;
	complete.height = complete.img.rows;

	// Ѱ�������յ�
	for (int i = 0; i < map.width; i++) {
		for (int j = 0; j < map.height; j++) {
			if (map.blocks[i][j] == MAP_BLOCK_START) {
				startPosition.x = i;
				startPosition.y = j;
			}

			if (map.blocks[i][j] == MAP_BLOCK_END) {
				endPosition.x = i;
				endPosition.y = j;
			}
		}
	}

	// �������λ��Ϊ���
	player.x = startPosition.x;
	player.y = startPosition.y;
	// �������������������
	player.face = PLAYER_FACE_UP;
	
	// ������ͼģʽ
	viewMode = VIEW_MODE_FRIST_PERSON;

	// ��ʼ��ȫ�������
	setCamarePosition(globalCamare, GLOBAL_CAMARE_POSITION_X, GLOBAL_CAMARE_POSITION_Y, GLOBAL_CAMARE_POSITION_Z);
	setCamareLookAt(globalCamare, GLOBAL_CAMARE_LOOKAT_X, GLOBAL_CAMARE_LOOKAT_Y, GLOBAL_CAMARE_LOOKAT_Z);
	setCamareDirection(globalCamare, GLOBAL_CAMARE_DIRECTION_X, GLOBAL_CAMARE_DIRECTION_Y, GLOBAL_CAMARE_DIRECTION_Z);

	// ��ʼ����һ�˳������
	setFristPersonCamareByPlayer();

	// ��ʼ�����λ��
	mousePosition.x = 0.0f;
	mousePosition.y = 0.0f;
}

// ���Ʒ���
void drawCube(Cube cube, bool isPlayer) {
	// ���������İ˸���������
	Vertex vertexes[8];
	xyzToVertex(vertexes[0], cube.x, cube.y, cube.z);
	xyzToVertex(vertexes[1], cube.x + cube.size, cube.y, cube.z);
	xyzToVertex(vertexes[2], cube.x + cube.size, cube.y + cube.size, cube.z);
	xyzToVertex(vertexes[3], cube.x, cube.y + cube.size, cube.z);
	xyzToVertex(vertexes[4], cube.x, cube.y, cube.z + cube.size);
	xyzToVertex(vertexes[5], cube.x + cube.size, cube.y, cube.z + cube.size);
	xyzToVertex(vertexes[6], cube.x + cube.size, cube.y + cube.size, cube.z + cube.size);
	xyzToVertex(vertexes[7], cube.x, cube.y + cube.size, cube.z + cube.size);

	// ����������������
	Quad4 quads[6];
	vertexesToQuad(quads[0], vertexes[0], vertexes[1], vertexes[2], vertexes[3]);
	vertexesToQuad(quads[1], vertexes[0], vertexes[1], vertexes[5], vertexes[4]);
	vertexesToQuad(quads[2], vertexes[2], vertexes[3], vertexes[7], vertexes[6]);
	vertexesToQuad(quads[3], vertexes[1], vertexes[2], vertexes[6], vertexes[5]);
	vertexesToQuad(quads[4], vertexes[0], vertexes[3], vertexes[7], vertexes[4]);
	vertexesToQuad(quads[5], vertexes[4], vertexes[5], vertexes[6], vertexes[7]);

	//// ʹ����ɫ
	//glColor3f(cubeColor.r, cubeColor.g, cubeColor.b);

	//// ʹ�ò���
	//glMaterialfv(GL_FRONT, GL_AMBIENT, material.ambient);
	//glMaterialfv(GL_FRONT, GL_DIFFUSE, material.diffuse);
	//glMaterialfv(GL_FRONT, GL_SPECULAR, material.specular);
	//glMaterialfv(GL_FRONT, GL_EMISSION, material.emission);
	//glMaterialfv(GL_FRONT, GL_SHININESS, &material.shininess);

	
	// ��������
	if (isPlayer) {
		// ʹ����ɫ
		glColor3f(green.r, green.g, green.b);

		// ʹ����Ҳ���
		glMaterialfv(GL_FRONT, GL_AMBIENT, playerCubeMaterial.ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, playerCubeMaterial.diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, playerCubeMaterial.specular);
		glMaterialfv(GL_FRONT, GL_EMISSION, playerCubeMaterial.emission);
		glMaterialfv(GL_FRONT, GL_SHININESS, &playerCubeMaterial.shininess);

		for (int i = 0; i < 6; i++) {
			glBegin(GL_POLYGON);
			for (int j = 0; j < 4; j++) {
				glVertex3f(quads[i].vertexes[j].x, quads[i].vertexes[j].y, quads[i].vertexes[j].z);
			}
			glEnd();
		}
	} else {
		// ʹ�ð�ɫ
		glColor3f(white.r, white.g, white.b);

		// ʹ��ǽ�����
		glMaterialfv(GL_FRONT, GL_AMBIENT, cubeMaterial.ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, cubeMaterial.diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, cubeMaterial.specular);
		glMaterialfv(GL_FRONT, GL_EMISSION, cubeMaterial.emission);
		glMaterialfv(GL_FRONT, GL_SHININESS, &cubeMaterial.shininess);

		// ʹ������
		glEnable(GL_TEXTURE_2D);
		// ��ʼ����
		for (int i = 0; i < 6; i++) {
			// ʹ������
			glBindTexture(GL_TEXTURE_2D, wall.id);
			glBegin(GL_POLYGON);
			/* for (int j = 0; j < 4; j++) {
			glVertex3f(quads[i].vertexes[j].x, quads[i].vertexes[j].y, quads[i].vertexes[j].z);
			}*/

			// �������½�
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(quads[i].vertexes[0].x, quads[i].vertexes[0].y, quads[i].vertexes[0].z);
			// �������½�
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(quads[i].vertexes[1].x, quads[i].vertexes[1].y, quads[i].vertexes[1].z);
			// �������Ͻ�
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(quads[i].vertexes[2].x, quads[i].vertexes[2].y, quads[i].vertexes[2].z);
			// �������Ͻ�
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(quads[i].vertexes[3].x, quads[i].vertexes[3].y, quads[i].vertexes[3].z);

			glEnd();
		}
		// �ر�����
		glDisable(GL_TEXTURE_2D);
	}
}

// �����Թ�
void drawMaze(Map map) {
	Cube cube;
	cube.size = MAP_BLOCK_LENGTH;
	for (int i = 0; i < map.width; i++) {
		for (int j = 0; j < map.height; j++) {
			if (map.blocks[i][j] == MAP_BLOCK_CUBE) {
				cube.x = j * cube.size;
				cube.y = map.height * cube.size - (i + 1) * cube.size;
				cube.z = 0;
				drawCube(cube, false);
			}
		}
	}
}

// ���ƹ�ϲͼ��
void drawCongratulation() {
	// �Ӵ�
	GLint viewPort[4];
	// modelView ����
	GLdouble modelView[16];
	// projection ����
	GLdouble projection[16];

	// ���л�ȡ
	glGetIntegerv(GL_VIEWPORT, viewPort);
	glGetDoublev(GL_MODELVIEW_MATRIX, modelView);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);

	// ���ڴ洢ת��ǰ��ά����ı���
	Vector2d pointOld[4];
	// ת��������
	Vector3d pointNew[4];

	// ����Ҫչʾͨ��ͼƬ��λ��
	pointOld[0].x = WINDOW_SIZE_WIDTH / 4;
	pointOld[0].y = WINDOW_SIZE_HEIGHT / 4;
	pointOld[1].x = WINDOW_SIZE_WIDTH / 4;
	pointOld[1].y = WINDOW_SIZE_HEIGHT / 4 * 3;
	pointOld[2].x = WINDOW_SIZE_WIDTH / 4 * 3;
	pointOld[2].y = WINDOW_SIZE_HEIGHT / 4 * 3;
	pointOld[3].x = WINDOW_SIZE_WIDTH / 4 * 3;
	pointOld[3].y = WINDOW_SIZE_HEIGHT / 4;

	// ��ÿһ���������ת��
	for (int i = 0; i < 4; i++) {
		gluUnProject(pointOld[i].x, pointOld[i].y, 0.1, modelView, projection, viewPort, &pointNew[i].x, &pointNew[i].y, &pointNew[i].z);
	}

	// ʹ�ð�ɫ
	glColor3f(white.r, white.g, white.b);
	// ʹ���ض�����
	glMaterialfv(GL_FRONT, GL_AMBIENT, completeMaterial.ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, completeMaterial.diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, completeMaterial.specular);
	glMaterialfv(GL_FRONT, GL_EMISSION, completeMaterial.emission);
	glMaterialfv(GL_FRONT, GL_SHININESS, &completeMaterial.shininess);
	
	// ��ʼ����
	// ������ͼ
	glEnable(GL_TEXTURE_2D);
	// ����ͼ
	glBindTexture(GL_TEXTURE_2D, complete.id);

	// ��ʼ����
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(pointNew[0].x, pointNew[0].y, pointNew[0].z);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(pointNew[1].x, pointNew[1].y, pointNew[1].z);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(pointNew[2].x, pointNew[2].y, pointNew[2].z);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(pointNew[3].x, pointNew[3].y, pointNew[3].z);
	glEnd();

	// �ر���ͼ
	glDisable(GL_TEXTURE_2D);
}

// ��Ⱦ����
void render() {
	// �����ɫ�������Ȼ���
	glClearColor(gray.r, gray.g, gray.b, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// ����ͶӰ����
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, 1, 1, 10000);
	// ����ģ���Ӿ�����
	switch (viewMode) {
		// �����ȫ���ӽǵĻ�
		case VIEW_MODE_GLOBAL:
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			gluLookAt(
				globalCamare.position[0], globalCamare.position[1], globalCamare.position[2],
				globalCamare.lookAt[0], globalCamare.lookAt[1], globalCamare.lookAt[2],
				globalCamare.direction[0], globalCamare.direction[1], globalCamare.direction[2]);
			break;
		// ����ǵ�һ�˳��ӽǵĻ�
		case VIEW_MODE_FRIST_PERSON:
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			gluLookAt(
				fristPersonCamare.position[0], fristPersonCamare.position[1], fristPersonCamare.position[2],
				fristPersonCamare.lookAt[0], fristPersonCamare.lookAt[1], fristPersonCamare.lookAt[2],
				fristPersonCamare.direction[0], fristPersonCamare.direction[1], fristPersonCamare.direction[2]);
			break;
	}

	loadTexture(wall);
	loadTexture(complete);

	// ���ù���
	glLightfv(GL_LIGHT0, GL_POSITION, sunLight.position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, sunLight.ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, sunLight.diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, sunLight.specular);
	// ʹ�� 0 �Ź�Դ
	glEnable(GL_LIGHT0);
	// ��֮�����Ⱦ��ʹ�øù���
	glEnable(GL_LIGHTING);
	
	// ������Ȳ���
	glEnable(GL_DEPTH_TEST);

	// �����Թ�
	drawMaze(map);
	
	// �������λ����������ҷ���
	playerCube.x = player.y * MAP_BLOCK_LENGTH + MAP_BLOCK_LENGTH / 2 - PLAYER_CUBE_SIZE / 2;
	playerCube.y = map.height * MAP_BLOCK_LENGTH - player.x * MAP_BLOCK_LENGTH - MAP_BLOCK_LENGTH / 2 - PLAYER_CUBE_SIZE / 2;
	playerCube.z = 0;
	playerCube.size = PLAYER_CUBE_SIZE;
	drawCube(playerCube, true);

	// �����ҵ����յ�
	if (player.x == endPosition.x && player.y == endPosition.y) {
		drawCongratulation();
	}

	// ����ǰ��̨����
	glutSwapBuffers();
}

// ���̰�������
void onSpecialKeyDown(int key, int x, int y) {
	switch (key) {
		case GLUT_KEY_F1:
			viewMode = VIEW_MODE_FRIST_PERSON;
			glutPostRedisplay();
			break;
		case GLUT_KEY_F2:
			viewMode = VIEW_MODE_GLOBAL;
			glutPostRedisplay();
			break;
	}

	// �ڵ�һ�˳�ģʽ�²���Ӧ���°���
	if (viewMode == VIEW_MODE_FRIST_PERSON) {
		switch (key) {
			// ǰ��
			case GLUT_KEY_UP:
				switch (player.face) {
					case PLAYER_FACE_UP:
						// ������򲻿�����������
						if (player.x > 0) {
							// ���ǰ�治�� cube �����ǰ��
							if (map.blocks[player.x - 1][player.y] != MAP_BLOCK_CUBE) {
								player.x -= 1;
							}
						}
						break;
					case PLAYER_FACE_DOWN:
						// ����Ͷ��򲻿�����������
						if (player.x < map.height - 1) {
							// ���ǰ�治�� cube �����ǰ��
							if (map.blocks[player.x + 1][player.y] != MAP_BLOCK_CUBE) {
								player.x += 1;
							}
						}
						break;
					case PLAYER_FACE_LEFT:
						// ��������򲻿�����������
						if (player.y > 0) {
							// ������� cube ����Լ���ǰ��
							if (map.blocks[player.x][player.y - 1] != MAP_BLOCK_CUBE) {
								player.y -= 1;
							}
						}
						break;
					case PLAYER_FACE_RIGHT:
						// �����ұ��򲻿�����������
						if (player.y < map.width - 1) {
							// ������� cube ����Լ���ǰ��
							if (map.blocks[player.x][player.y + 1] != MAP_BLOCK_CUBE) {
								player.y += 1;
							}
						}
						break;
				}
				break;
			// ����
			case GLUT_KEY_DOWN:
				switch (player.face) {
					case PLAYER_FACE_UP:
						// ����׶�������������
						if (player.x < map.height - 1) {
							// ����󷽲��� cube ����Ժ���
							if (map.blocks[player.x + 1][player.y] != MAP_BLOCK_CUBE) {
								player.x += 1;
							}
						}
						break;
					case PLAYER_FACE_DOWN:
						// �����������������
						if (player.x > 0) {
							// ����󷽲��� cube ����Ժ���
							if (map.blocks[player.x - 1][player.y] != MAP_BLOCK_CUBE) {
								player.x -= 1;
							}
						}
						break;
					case PLAYER_FACE_LEFT:
						// �����ұ�������������
						if (player.y < map.width - 1) {
							// ����󷽲��� cube ����Ժ���
							if (map.blocks[player.x][player.y + 1] != MAP_BLOCK_CUBE) {
								player.y += 1;
							}
						}
						break;
					case PLAYER_FACE_RIGHT:
						// �������������������
						if (player.y > 0) {
							// ����󷽲��� cube ����Ժ���
							if (map.blocks[player.x][player.y - 1] != MAP_BLOCK_CUBE) {
								player.y -= 1;
							}
						}
						break;
				}
				break;
			// ��ת
			case GLUT_KEY_LEFT:
				switch (player.face) {
					case PLAYER_FACE_UP:
						player.face = PLAYER_FACE_LEFT;
						break;
					case PLAYER_FACE_DOWN:
						player.face = PLAYER_FACE_RIGHT;
						break;
					case PLAYER_FACE_LEFT:
						player.face = PLAYER_FACE_DOWN;
						break;
					case PLAYER_FACE_RIGHT:
						player.face = PLAYER_FACE_UP;
						break;
				}
				break;
			// ��ת
			case GLUT_KEY_RIGHT:
				switch (player.face) {
					case PLAYER_FACE_UP:
						player.face = PLAYER_FACE_RIGHT;
						break;
					case PLAYER_FACE_DOWN:
						player.face = PLAYER_FACE_LEFT;
						break;
					case PLAYER_FACE_LEFT:
						player.face = PLAYER_FACE_UP;
						break;
					case PLAYER_FACE_RIGHT:
						player.face = PLAYER_FACE_DOWN;
						break;
				}
				break;
		}

		// λ�ø��º�ʱ���µ�һ�˳��ӽǵ������
		setFristPersonCamareByPlayer();
		glutPostRedisplay();
	}
}

int main(int argc, char *argv[]) {
	init();
	
	// ��ʼ�� glut
	glutInit(&argc, argv);

	// ������ʾ��ʽΪ RGB + ˫����
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

	// ��ʼ������
	glutInitWindowPosition(WINDOW_POSITION_X, WINDOW_POSITION_Y);
	glutInitWindowSize(WINDOW_SIZE_WIDTH, WINDOW_SIZE_HEIGHT);
	glutCreateWindow("�Թ���Ϸ");

	// ������Ⱦ����
	glutDisplayFunc(render);
	
	// ��������Ӧ
	glutSpecialFunc(onSpecialKeyDown);

	// ������ѭ��
	glutMainLoop();

	return 0;
}