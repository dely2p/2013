#include<stdio.h>
#include "glut.h"
#include <math.h>
#include <iostream>
#include <stdlib.h>
#include "bitmap.h"

#define PI 3.1415926
#define WIDTH 600
#define HEIGHT 600

struct VERTEX_C {
	int index;
	float x;
	float y;
	float z;
};
VERTEX_C *cvertex;
struct FACE_C {
	int index;
	int v1;
	int v2;
	int v3;
};
FACE_C *cface;

struct VERTEX_T {
	int index;
	float x;
	float y;
	float z;
};
VERTEX_T *tvertex;
struct FACE_T1 {
	int dotnum;
	int v1;
	int v2;
	int v3;
};
FACE_T1 *tface;
struct FACE_T2 {
	int dotnum;
	int v1;
	int v2;
	int v3;
	int v4;
};
FACE_T2 *tface2;

struct ADDFACE_T {
	
	int v1;
	int v2;
	int v3;
};
ADDFACE_T *addtface;
struct UV_C {
	float x;
	float y;
};
UV_C *cuv;
struct ID_C {
	int v1;
	int v2;
	int v3;
};
ID_C *cid;

struct UV_T {
	float x;
	float y;
};
UV_T *tuv;
struct ID_T1 {
	int dotnum;
	int v1;
	int v2;
	int v3;
};
ID_T1 *tid;
struct ID_T2 {
	int dotnum;
	int v1;
	int v2;
	int v3;
	int v4;
};
ID_T2 *tid2;

int temp;
int temp2;

float cameraX = 0;
float cameraY = 0;
float cameraZ = 0;
float radius = 200;
float phi = 0;
float theta = 0;
int cvertexmount;
int cfacemount;
int tvertexmount;
int tfacemount;
int cuvmount;
int cidmount;
int tuvmount;
int tidmount;
float cr=1.0,cg=1.0,cb=1.0;
float tr=1.0,tg=1.0,tb=1.0;
float cx=0,cy=90,cz=1;
float tx=-90,ty=90,tz=1;

unsigned int MyTextureObject[1];
BITMAPINFO *bmpcow; 
GLubyte    *Cowbit; 
BITMAPINFO *bmptiger; 
GLubyte    *Tigerbit;
BITMAPINFO *bmpland; 
GLubyte    *Landbit; 
using namespace std;

int vertexNumber;
int faceNumber;


int indexarr[3];
float cfn[5804][3];
float cvtn[2904][3];
float tfn[303][3];
float tvtn[599][3];
float tfn2[303][3];
float tvtn2[599][3];
float ctxr[3045][3];
float ttxr[303][3];
void calcNormal(float v[3][3], float out[3]);
void fileimport(void) {
	char *filename;
	FILE *fp;
	char *filename2;
	FILE *fp2;

	int i;

	filename = "Vertex_Face.txt";
	fp = fopen(filename,"r");

	fscanf(fp,"%d",&cvertexmount);
	cvertex = new VERTEX_C[cvertexmount];
	for(i=0; i<cvertexmount; i++)
	{
		fscanf(fp,"%d %f %f %f",&cvertex[i].index,&cvertex[i].x,&cvertex[i].y,&cvertex[i].z);
	//	printf("%d %f %f %f",cvertex[i].index,cvertex[i].x,cvertex[i].y,cvertex[i].z);
	//	printf("\n");
	}

	fscanf(fp,"%d",&cfacemount);
	cface = new FACE_C[cfacemount];
	for(i=0; i<cfacemount; i++)
	{	
		fscanf(fp,"%d %d %d %d",&cface[i].index,&cface[i].v1,&cface[i].v2,&cface[i].v3);
	//	printf("%d %d %d %d",cface[i].index,cface[i].v1,cface[i].v2,cface[i].v3);
	//	printf("\n");
	}

	for(i=0; i<cfacemount; i++) {

	
		float cv[3][3]={{cvertex[cface[i].v1-1].x,cvertex[cface[i].v1-1].y,cvertex[cface[i].v1-1].z},
					{cvertex[cface[i].v2-1].x,cvertex[cface[i].v2-1].y,cvertex[cface[i].v2-1].z},
					{cvertex[cface[i].v3-1].x,cvertex[cface[i].v3-1].y,cvertex[cface[i].v3-1].z}}; 

		calcNormal(cv,cfn[i]);
	}
	for(int i=0; i<cvertexmount; i++) {
		float tmpnomal[3] = {0, };
		int cnt = 0;
		for(int j=0; j<cfacemount; j++) {
			if(cface[j].v1 == i+1 || cface[j].v2 == i+1 || cface[j].v3 == i+1) {
				tmpnomal[0] += cfn[j][0];
				tmpnomal[1] += cfn[j][1];
				tmpnomal[2] += cfn[j][2];
				cnt++;
			}
		}
		cvtn[i][0] = tmpnomal[0] / cnt;
		cvtn[i][1] = tmpnomal[1] / cnt;
		cvtn[i][2] = tmpnomal[2] / cnt;
	}

	fscanf(fp,"%d",&tvertexmount);
	tvertex = new VERTEX_T[tvertexmount];
	for(i=0; i<tvertexmount; i++)
	{
		fscanf(fp,"%d %f %f %f",&tvertex[i].index,&tvertex[i].x,&tvertex[i].y,&tvertex[i].z);
		printf("%d %f %f %f",tvertex[i].index,tvertex[i].x,tvertex[i].y,tvertex[i].z);
		printf("\n");
	}

	fscanf(fp,"%d",&tfacemount);
	tface = new FACE_T1[tfacemount];
	tface2 = new FACE_T2[tfacemount];
	addtface = new ADDFACE_T[tfacemount+3];
	int j=0;
	for(i=0; i<tfacemount; i++)
	{	
		fscanf(fp,"%d",&temp);
			tface[i].dotnum=temp;
			fscanf(fp,"%d %d %d",&tface[i].v1,&tface[i].v2,&tface[i].v3);
			printf("%d %d %d %d",tface[i].dotnum,tface[i].v1,tface[i].v2,tface[i].v3);
			printf("\n");

			addtface[i].v1=tface[i].v1;
			addtface[i].v2=tface[i].v2;
			addtface[i].v3=tface[i].v3;
			

		//if(temp==3){
		//	tface[i].dotnum=temp;
		//	fscanf(fp,"%d %d %d",&tface[i].v1,&tface[i].v2,&tface[i].v3);
		//	printf("%d %d %d %d",tface[i].dotnum,tface[i].v1,tface[i].v2,tface[i].v3);
		//	printf("\n");
		//}
		if(temp==4){
			tface2[i].dotnum=temp;
			tface2[i].v1=tface[i].v1;
			tface2[i].v2=tface[i].v2;
			tface2[i].v3=tface[i].v3;
			
			fscanf(fp,"%d",&tface2[i].v4);
			printf("%d %d %d %d %d",tface2[i].dotnum,tface2[i].v1,tface2[i].v2,tface2[i].v3,tface2[i].v4);
			printf("\n");
			indexarr[j]=i;
			j++;

		}	
	}
	for(i=0;i<3;i++){
		addtface[i+599].v1=tface2[i].v3;
		addtface[i+599].v2=tface2[i].v4;
		addtface[i+599].v3=tface2[i].v1;
	}

	for(i=0; i<tfacemount; i++) {

	
		float tv[3][3]={{tvertex[addtface[i].v1].x,tvertex[addtface[i].v1].y,tvertex[addtface[i].v1].z},
				{tvertex[addtface[i].v2].x,tvertex[addtface[i].v2].y,tvertex[addtface[i].v2].z},
				{tvertex[addtface[i].v3].x,tvertex[addtface[i].v3].y,tvertex[addtface[i].v3].z}};  

		calcNormal(tv,tfn[i]);
	}
	for(int i=0; i<tvertexmount; i++) {
		float tmpnomal[3] = {0, };
		int cnt = 0;
		for(int j=0; j<tfacemount; j++) {
			if(addtface[j].v1 == i+1 || addtface[j].v2 == i+1 || addtface[j].v3 == i+1) {
				tmpnomal[0] += cfn[j][0];
				tmpnomal[1] += cfn[j][1];
				tmpnomal[2] += cfn[j][2];
				cnt++;
			}
		}
		cvtn[i][0] = tmpnomal[0] / cnt;
		cvtn[i][1] = tmpnomal[1] / cnt;
		cvtn[i][2] = tmpnomal[2] / cnt;
	}


	filename2 = "Texture_UV_ID.txt";
	fp2 = fopen(filename2,"r");

	fscanf(fp2,"%d",&cuvmount);
	cuv = new UV_C[cuvmount];
	for(i=0; i<cuvmount; i++)
	{
		fscanf(fp2,"%f %f",&cuv[i].x,&cuv[i].y);
		printf("%f %f",cuv[i].x,cuv[i].y);
		printf("\n");
	}

	fscanf(fp2,"%d",&cidmount);
	cid = new ID_C[cidmount];
	for(i=0; i<cidmount; i++)
	{	
		fscanf(fp2,"%d %d %d",&cid[i].v1,&cid[i].v2,&cid[i].v3);
		printf("%d %d %d",cid[i].v1,cid[i].v2,cid[i].v3);
		printf("\n");
	}
	fscanf(fp2,"%d",&tuvmount);
	tuv = new UV_T[tuvmount];
	for(i=0; i<tuvmount; i++)
	{
		fscanf(fp2,"%f %f",&tuv[i].x,&tuv[i].y);
		printf("%f %f",tuv[i].x,tuv[i].y);
		printf("\n");
	}

	fscanf(fp2,"%d",&tidmount);
	tid = new ID_T1[tidmount];
	tid2 = new ID_T2[tidmount];
	for(i=0; i<tidmount; i++)
	{	
		fscanf(fp2,"%d",&temp2);
		tid[i].dotnum=temp2;
		fscanf(fp2,"%d %d %d",&tid[i].v1,&tid[i].v2,&tid[i].v3);
		printf("%d %d %d %d",tid[i].dotnum,tid[i].v1,tid[i].v2,tid[i].v3);
		printf("\n");

		if(temp2==4){
			tid2[i].dotnum=temp2;
			tid2[i].v1=tid[i].v1;
			tid2[i].v2=tid[i].v2;
			tid2[i].v3=tid[i].v3;
			
			fscanf(fp2,"%d",&tid2[i].v4);
			printf("%d %d %d %d %d",tid2[i].dotnum,tid2[i].v1,tid2[i].v2,tid2[i].v3,tid2[i].v4);
			printf("\n");
		}	
	}	
}
void ReduceToUnit(float vector[3])
{
	float length;
	
	// Calculate the length of the vector		
	length = (float)sqrt((vector[0]*vector[0]) + 
						(vector[1]*vector[1]) +
						(vector[2]*vector[2]));

	// Keep the program from blowing up by providing an exceptable
	// value for vectors that may calculated too close to zero.
	if(length == 0.0f)
		length = 1.0f;

	// Dividing each element by the length will result in a
	// unit normal vector.
	vector[0] /= length;
	vector[1] /= length;
	vector[2] /= length;


}
void calcNormal(float v[3][3], float out[3])
{
	float v1[3],v2[3];
	static const int x = 0;
	static const int y = 1;
	static const int z = 2;

	// Calculate two vectors from the three points
	v1[x] = v[0][x] - v[1][x];
	v1[y] = v[0][y] - v[1][y];
	v1[z] = v[0][z] - v[1][z];

	v2[x] = v[1][x] - v[2][x];
	v2[y] = v[1][y] - v[2][y];
	v2[z] = v[1][z] - v[2][z];

	// Take the cross product of the two vectors to get
	// the normal vector which will be stored in out
	out[x] = v1[y]*v2[z] - v1[z]*v2[y];
	out[y] = v1[z]*v2[x] - v1[x]*v2[z];
	out[z] = v1[x]*v2[y] - v1[y]*v2[x];

	// Normalize the vector (shorten length to one)
	ReduceToUnit(out);
}
void SetupRC()
	{
	// Light values and coordinates
	GLfloat  ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat  diffuseLight[] = { 0.9f, 0.9f, 0.9f, 1.0f };
	GLfloat  specular[] = { 1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat  specref[] =  { 1.0f, 1.0f, 1.0f, 1.0f };

	glEnable(GL_DEPTH_TEST);	// Hidden surface removal
	glFrontFace(GL_CCW);		// Counter clock-wise polygons face out
	glEnable(GL_CULL_FACE);		// Do not calculate inside of jet
	glCullFace(GL_BACK);
	// Enable lighting
	glEnable(GL_LIGHTING);

	// Setup and enable light 0
	
	glLightfv(GL_LIGHT0,GL_AMBIENT,ambientLight);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuseLight);
	glLightfv(GL_LIGHT0,GL_SPECULAR,specular);
	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHT0);
	glColor3f(0.0,0.0,0.0);
	// Enable color tracking
	glEnable(GL_COLOR_MATERIAL);
	
	// Set Material properties to follow glColor values
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	// All materials hereafter have full specular reflectivity
	// with a high shine
	glMaterialfv(GL_FRONT, GL_SPECULAR,specref);
	glMateriali(GL_FRONT,GL_SHININESS,128);

	// Light blue background
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f );
	}

void drawCow(){
	int i;
	float cnormal[3];
	for(i=0; i<cfacemount; i++)
	{/*
		glBegin(GL_POLYGON);
			glColor3f(1.0,0.5,0.0);
			glVertex3f(cvertex[cface[i].v1-1].x,cvertex[cface[i].v1-1].y,cvertex[cface[i].v1-1].z);
			glVertex3f(cvertex[cface[i].v2-1].x,cvertex[cface[i].v2-1].y,cvertex[cface[i].v2-1].z);
			glVertex3f(cvertex[cface[i].v3-1].x,cvertex[cface[i].v3-1].y,cvertex[cface[i].v3-1].z); 
		glEnd();
		*/
		glBegin(GL_TRIANGLES);
			float cv[3][3]={{cvertex[cface[i].v1-1].x,cvertex[cface[i].v1-1].y,cvertex[cface[i].v1-1].z},
				{cvertex[cface[i].v2-1].x,cvertex[cface[i].v2-1].y,cvertex[cface[i].v2-1].z},
				{cvertex[cface[i].v3-1].x,cvertex[cface[i].v3-1].y,cvertex[cface[i].v3-1].z}}; 
			float ctxr[3][3]={{cuv[cid[i].v1-1].x,cuv[cid[i].v1-1].y},
					{cuv[cid[i].v2-1].x,cuv[cid[i].v2-1].y},
					{cuv[cid[i].v3-1].x,cuv[cid[i].v3-1].y}};
			
			glColor3f(cr,cg,cb);
			glNormal3fv(cvtn[cface[i].v1-1]);
			glTexCoord2f(cuv[cid[i].v1].x,cuv[cid[i].v1].y);
			glVertex3fv(cv[0]);
			glNormal3fv(cvtn[cface[i].v2-1]);
			glTexCoord2f(cuv[cid[i].v2].x,cuv[cid[i].v2].y);
			glVertex3fv(cv[1]);
			glNormal3fv(cvtn[cface[i].v3-1]);
			glTexCoord2f(cuv[cid[i].v3].x,cuv[cid[i].v3].y);
			glVertex3fv(cv[2]);

		glEnd();
	
		/*
		glBegin(GL_LINE_LOOP);
			glColor3f(1.0,0.0,0.5);
			glVertex3f(cvertex[cface[i].v1-1].x,cvertex[cface[i].v1-1].y,cvertex[cface[i].v1-1].z);
			glVertex3f(cvertex[cface[i].v2-1].x,cvertex[cface[i].v2-1].y,cvertex[cface[i].v2-1].z);
			glVertex3f(cvertex[cface[i].v3-1].x,cvertex[cface[i].v3-1].y,cvertex[cface[i].v3-1].z);
		glEnd();
		*/
	}
	/*	glBindTexture(GL_TEXTURE_2D,MyTextureObject[0]);

		for(i=0;i<cuvmount;i++){
			glBegin(GL_QUADS);
					float ctxr[3][3]={{cuv[cid[i].v1-1].x,cuv[cid[i].v1-1].y},
					{cuv[cid[i].v2-1].x,cuv[cid[i].v2-1].y},
					{cuv[cid[i].v3-1].x,cuv[cid[i].v3-1].y}};

					glTexCoord2d(cuv[i].x,cuv[i].y);
					glVertex3fv(ctxr[0]);

			glEnd();
			glutSwapBuffers();
		}*/
}
void drawTiger(){
	int i;
	float tnormal[3];
	float tnormal2[3];
	for(i=0; i<tfacemount; i++)
	{
		//glBegin(GL_POLYGON);
		//	//glColor3f(1.0,0.5,0.0);
		//	glVertex3f(tvertex[tface[i].v1].x,tvertex[tface[i].v1].y,tvertex[tface[i].v1].z);
		//	glVertex3f(tvertex[tface[i].v2].x,tvertex[tface[i].v2].y,tvertex[tface[i].v2].z);
		//	glVertex3f(tvertex[tface[i].v3].x,tvertex[tface[i].v3].y,tvertex[tface[i].v3].z); 
		//glEnd();

		glBegin(GL_TRIANGLES);
			float tv[3][3]={{tvertex[tface[i].v1].x,tvertex[tface[i].v1].y,tvertex[tface[i].v1].z},
				{tvertex[tface[i].v2].x,tvertex[tface[i].v2].y,tvertex[tface[i].v2].z},
				{tvertex[tface[i].v3].x,tvertex[tface[i].v3].y,tvertex[tface[i].v3].z}}; 
			float ttxr[3][3]={{tuv[tid[i].v1].x,tuv[tid[i].v1].y},
					{tuv[tid[i].v2].x,tuv[tid[i].v2].y},
					{tuv[tid[i].v3].x,tuv[tid[i].v3].y}};

		//	calcNormal(tv,tnormal);
				glColor3f(tr,tg,tb);
			glNormal3fv(tvtn[tface[i].v1]);
				glTexCoord2f(tuv[tid[i].v1].x,tuv[tid[i].v1].y);
				glVertex3fv(tv[0]);
				glNormal3fv(tvtn[tface[i].v2]);
				glTexCoord2f(tuv[tid[i].v2].x,tuv[tid[i].v2].y);
				glVertex3fv(tv[1]);
				glNormal3fv(tvtn[tface[i].v3]);
				glTexCoord2f(tuv[tid[i].v3].x,tuv[tid[i].v3].y);
				glVertex3fv(tv[2]);
		glEnd();


	/*	glBegin(GL_LINE_LOOP);
			glColor3f(1.0,0.0,0.5);
			glVertex3f(tvertex[tface[i].v1].x,tvertex[tface[i].v1].y,tvertex[tface[i].v1].z);
			glVertex3f(tvertex[tface[i].v2].x,tvertex[tface[i].v2].y,tvertex[tface[i].v2].z);
			glVertex3f(tvertex[tface[i].v3].x,tvertex[tface[i].v3].y,tvertex[tface[i].v3].z);
		glEnd();*/

		if(tface[i].dotnum==4){
			
		/*	glBegin(GL_POLYGON);
				glColor3f(1.0,0.5,0.0);
				glVertex3f(tvertex[tface2[i].v3].x,tvertex[tface2[i].v3].y,tvertex[tface2[i].v3].z);
				glVertex3f(tvertex[tface2[i].v4].x,tvertex[tface2[i].v4].y,tvertex[tface2[i].v4].z);
				glVertex3f(tvertex[tface2[i].v1].x,tvertex[tface2[i].v1].y,tvertex[tface2[i].v1].z); 
			glEnd();*/

			glBegin(GL_TRIANGLES);
				float tv2[3][3]={{tvertex[tface2[i].v1].x,tvertex[tface2[i].v1].y,tvertex[tface2[i].v1].z},
					{tvertex[tface2[i].v2].x,tvertex[tface2[i].v2].y,tvertex[tface2[i].v2].z},
					{tvertex[tface2[i].v3].x,tvertex[tface2[i].v3].y,tvertex[tface2[i].v3].z}}; 

				//calcNormal(tv2,tnormal2);
				glColor3f(tr,tg,tb);
				glNormal3fv(tvtn[tface2[i].v1]);
				glTexCoord2f(tuv[tid2[i].v1].x,tuv[tid2[i].v1].y);
				glVertex3fv(tv[0]);
				glNormal3fv(tvtn[tface2[i].v2]);
				glTexCoord2f(tuv[tid2[i].v2].x,tuv[tid2[i].v2].y);
				glVertex3fv(tv[1]);
				glNormal3fv(tvtn[tface2[i].v3]);
				glTexCoord2f(tuv[tid2[i].v3].x,tuv[tid2[i].v3].y);
				glVertex3fv(tv[2]);
			glEnd();

			/*glBegin(GL_LINE_LOOP);
				glColor3f(1.0,0.0,0.5);
				glVertex3f(tvertex[tface2[i].v3].x,tvertex[tface2[i].v3].y,tvertex[tface2[i].v3].z);
				glVertex3f(tvertex[tface2[i].v4].x,tvertex[tface2[i].v4].y,tvertex[tface2[i].v4].z);
				glVertex3f(tvertex[tface2[i].v1].x,tvertex[tface2[i].v1].y,tvertex[tface2[i].v1].z);
			glEnd();*/
		}
	}

}
//AUX_RGBImageRec * LoadBMP(char *szFilename){
//	FILE *pFile = NULL;
//	
//	if(!szFilename){
//		return NULL;
//	}
//	pFile = fopen(szFilename,"r");
//	if(pFile){
//		fclose(pFile);
//		return auxRGBImageLoad(szFilename);
//	}
//	return NULL;
//}
//int LoadGLTextures(char * szFilePath){
//	int Status = FALSE;
//	glClearColor(0.0,0.0,0.0,0.5);
//	memset(pTextureImage,0,sizeof(void *)*1);
//
//	if(pTextureImage[0] = LoadBMP(szFilePath)){
//		Status=TRUE;
//		glGenTextures(1,&MyTextureObject[0]);
//		glBindTexture(GL_TEXTURE_2D,MyTextureObject[0]);
//		glTexImage2D(GL_TEXTURE_2D,0,3,pTextureImage[0]->sizeX,
//			pTextureImage[0]->sizeY,0,GL_RGB,GL_UNSIGNED_BYTE,
//			pTextureImage[0]->data);
//		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
//		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
//		glEnable(GL_TEXTURE_2D);
//	}
//	if(pTextureImage[0]){
//		if(pTextureImage[0]->data){
//			free(pTextureImage[0]->data);
//		}
//		free(pTextureImage[0]);
//	}
//	return Status;
//}
void init(void) {
	glClearColor(1.0,1.0,1.0,0.0);
	//glColor3f(0.0,0.0,0.0);
	glEnable(GL_DEPTH_TEST);

	fileimport();
}

void reshape(int w, int h)
{
	glViewport(0,0,w,h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0,WIDTH/HEIGHT,1.0,500.0);
}

void DrawBackgound()
{
	glPushMatrix();	//�ٴ�
		glColor3f(0.5,0.2,0.5);
		glTranslatef(0.0f, -11.5f, -18.0f);
		glBegin(GL_POLYGON);
			glVertex3f(-18.0,0.0,0.0);
			glVertex3f(18.0,0.0,0.0);
			glVertex3f(18.0,0.0,-19.0);
			glVertex3f(-18.0,0.0,-19.0);
		glEnd();
	glPopMatrix();
}
void display(void)
{
	DrawBackgound();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	cameraX = radius * cos(phi) * cos(theta);
	cameraY = radius * cos(phi) * cos(theta);
	cameraZ = radius * sin(phi);

	GLfloat LightPosition0[] = {0.0f,0.0f,0.0f,1.0f};

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();
	//glTranslatef(0.5,0.5,1.0);
	glPushMatrix();
	glLightfv(GL_LIGHT0,GL_POSITION,LightPosition0);
	glPopMatrix();
	glPopMatrix();

	gluLookAt(cameraX,cameraY,cameraZ,0.0,0.0,0.0,0.0,0.0,1.0);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, bmpcow->bmiHeader.biWidth,
		bmpcow->bmiHeader.biHeight, 0, GL_BGR_EXT,
		GL_UNSIGNED_BYTE, Cowbit);

	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
		glRotatef(cy,1.0,0.0,0.0);
		glRotatef(cx,0.0,1.0,0.0);
		glScalef(100.0,100.0,100.0);
		glColor3ub(128, 128, 128);
		drawCow();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

	glTexImage2D(GL_TEXTURE_2D, 0, 3, bmptiger->bmiHeader.biWidth,
		bmptiger->bmiHeader.biHeight, 0, GL_BGR_EXT,
		GL_UNSIGNED_BYTE, Tigerbit);

	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
		glTranslated(0.0,-80.0,0.0);
		glRotatef(ty,1.0,0.0,0.0);
		glRotatef(tx,0.0,1.0,0.0);
		
		glScalef(50.0,50.0,50.0);
		glColor3ub(128, 128, 128);
		drawTiger();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,MyTextureObject[0]);

	glShadeModel(GL_SMOOTH);



	glFlush();
	glutSwapBuffers();
}
void c_color(int n) {
	switch(n) {
		case 1:
			cr=1.0;
			cg=1.0;
			cb=1.0;
			break;
		case 2:
			cr=1.0;
			cg=0.0;
			cb=0.0;
			break;
		case 3:
			cr=0.5;
			cg=0.5;
			cb=0.0;
			break;
		case 4:
			cr=0.0;
			cg=0.5;
			cb=0.5;
			break;
	}
	glutPostRedisplay();
}
void t_color(int n) {
	switch(n) {
		case 1:
			tr=1.0;
			tg=1.0;
			tb=1.0;
			break;
		case 2:
			tr=1.0;
			tg=0.0;
			tb=0.0;
			break;
		case 3:
			tr=0.5;
			tg=0.5;
			tb=0.0;
			break;
		case 4:
			tr=0.0;
			tg=0.5;
			tb=0.5;
			break;
	}
	glutPostRedisplay();
}
void main_menu(int n) {

	glutPostRedisplay();
}
void Key(unsigned char key,int x,int y)
{
	switch(key)
	{
		case 27:
			exit(0);
			break;
	}
}

void specialkey(int key,int x,int y)
{
	switch(key)
	{
		case GLUT_KEY_LEFT:
			theta-=0.1;
			break;
		case GLUT_KEY_RIGHT:
			theta+=0.1;
			break;		
		case GLUT_KEY_UP:
			phi+=0.1;
			break;
		case GLUT_KEY_DOWN:
			phi-=0.1;
			break;
		case GLUT_KEY_F1:
			cx+=10.0;
			break;
		case GLUT_KEY_F2:		
			cy+=10.0;		
			break;
		case GLUT_KEY_F3:
			cz+=10.0;
			break;
		case GLUT_KEY_F4:
			tx+=10.0;
			break;
		case GLUT_KEY_F5:
			ty+=10.0;
			break;
		case GLUT_KEY_F6:
			tz+=10.0;
			break;
	}
}
int main(int argc, char** argv){
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(WIDTH,HEIGHT);
	glutCreateWindow("CownTiger");
	Cowbit = LoadDIBitmap("cow.bmp", &bmpcow);
	Tigerbit = LoadDIBitmap("tiger.bmp", &bmptiger);
	Landbit = LoadDIBitmap("land.bmp", &bmpland);
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
//	glColor3f(0.0,0.0,0.0);

	GLint CowColorID = glutCreateMenu(c_color);	
    glutAddMenuEntry("�׳� ��", 1);	
    glutAddMenuEntry("���� ��", 2);
	glutAddMenuEntry("��� ��", 3);
    glutAddMenuEntry("�ʷ� ��", 4);

	GLint subTigerColorID = glutCreateMenu(t_color);	
    glutAddMenuEntry("�׳� ȣ����", 1);	
    glutAddMenuEntry("���� ȣ����", 2);
	glutAddMenuEntry("��� ȣ����", 3);
    glutAddMenuEntry("�ʷ� ȣ����", 4);

	glutCreateMenu(main_menu);
	glutAddSubMenu("���� ����", 1);
	glutAddSubMenu("ȣ������ ����", 2);

	glutAttachMenu(GLUT_RIGHT_BUTTON); 

	glutKeyboardFunc(Key);
	glutSpecialFunc(specialkey);
	glutIdleFunc(display);
	SetupRC();
	glutMainLoop();
	
	return 0;
}