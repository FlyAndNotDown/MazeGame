#pragma once

// ���ڳ���
#define WINDOW_POSITION_X 100
#define WINDOW_POSITION_Y 100
#define WINDOW_SIZE_WIDTH 600
#define WINDOW_SIZE_HEIGHT 600


// ��ɫ
struct Color {
	GLfloat r;
	GLfloat g;
	GLfloat b;
};
// �ƹ�
struct Light {
	GLfloat position[4];
	GLfloat ambient[4];
	GLfloat diffuse[4];
	GLfloat specular[4];
};
// ����
struct Material {
	GLfloat ambient[4];
	GLfloat diffuse[4];
	GLfloat specular[4];
	GLfloat emission[4];
	GLfloat shininess;
};


// ��ͼ
struct Map {
	GLint width;
	GLint height;
	GLint blocks[100][100];
};
// ��ͼ block ����
#define MAP_BLOCK_EMPTY 0
#define MAP_BLOCK_CUBE 1
#define MAP_BLOCK_START 2
#define MAP_BLOCK_END 3
// ��ͼ����
const GLint MAP1_WIDTH = 20;
const GLint MAP1_HEIGHT = 20;
const GLint MAP1_BLOCKS[MAP1_WIDTH][MAP1_HEIGHT] = {
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1,
	1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1,
	1, 0, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1,
	1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
};
// ��ͼλ��
struct MapPosition {
	GLint x;
	GLint y;
};
// ��ͼ���鳤��
#define MAP_BLOCK_LENGTH 30


// �ӽ�ģʽ
typedef GLint ViewMode;
#define VIEW_MODE_GLOBAL 0
#define VIEW_MODE_FRIST_PERSON 1


// �����Ϣ
struct Player {
	GLint x;
	GLint y;
	GLint face;
};
// ���������
#define PLAYER_FACE_UP 0
#define PLAYER_FACE_DOWN 1
#define PLAYER_FACE_LEFT 2
#define PLAYER_FACE_RIGHT 3
// ��ҷ����С
#define PLAYER_CUBE_SIZE MAP_BLOCK_LENGTH / 4


// ����
struct Cube {
	// ���½Ƕ������ά����
	GLfloat x;
	GLfloat y;
	GLfloat z;
	// �߳�
	GLfloat size;
};
// ����
struct Vertex {
	GLfloat x;
	GLfloat y;
	GLfloat z;
};
// ��
struct Quad4 {
	Vertex vertexes[4];
};

// �����
struct Camare {
	GLfloat position[3];
	GLfloat lookAt[3];
	GLfloat direction[3];
};
// �ӽǲ���
#define VIEW_STEP_LENGTH_XY 40 / 81
#define VIEW_STEP_LENGTH_Z 1 / 15
// ȫ�����������
#define GLOBAL_CAMARE_POSITION_X 200
#define GLOBAL_CAMARE_POSITION_Y 0
#define GLOBAL_CAMARE_POSITION_Z 800
#define GLOBAL_CAMARE_LOOKAT_X 200
#define GLOBAL_CAMARE_LOOKAT_Y 100
#define GLOBAL_CAMARE_LOOKAT_Z 0
#define GLOBAL_CAMARE_DIRECTION_X 0
#define GLOBAL_CAMARE_DIRECTION_Y 0
#define GLOBAL_CAMARE_DIRECTION_Z 1

// ��ά����
struct Vector2f {
	GLfloat x;
	GLfloat y;
};

// ��ͼ
struct Texture {
	Mat img;
	GLint width;
	GLint height;
	// ͼ��ָ��
	GLubyte *pixels;
	// id
	GLuint id;
};