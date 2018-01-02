#ifndef COLLISION_H
#define COLLISION_H
#include"model.h"

#ifndef  M_PI
#define M_PI
const float PI = 3.14159;
#endif // ! M_PI


struct obb_box {
	glm::vec3 center;//���ĵ�
	glm::vec3 half;//�볤
	glm::vec3 x_axis, y_axis, z_axis;//������
};

class collosion {
private:
	/*
	���ܣ��Ÿ��ȼ�������ֵ 3*3
	����: void
	����:
	a: ԭʼ���� ����Ϊ����ֵ
	p: ��������
	eps: ����
	T: ��������
	*/
	void jacobi(glm::mat3 &a, glm::mat3 &p, double eps, int t);
	/*
	���ܣ�ʩ���������� 3*3
	���أ�void
	������
	p:������������
	a:����ֵ
	*/
	void schmidtOrthogonal(glm::mat3 &p, glm::vec3 &a);
	/*
	���ܣ�����Э������� 3*3
	���أ�Э�������
	������
	vertexs����������
	*/
	glm::mat3 cal_cov_mat(const std::vector<glm::vec3>& vertexs);
	/*
	���ܣ�����obb�� private
	���أ�obb�нṹ
	������
	vertexs����������
	p����һ��������ϵ
	*/
	obb_box __gen_obb_box(const std::vector<glm::vec3> vertexs, const glm::mat3 &p);
public:
	obb_box a;
	obb_box a2;
	std::vector<glm::vec3> posRe;
	std::vector<glm::vec3> topR;
	std::vector<glm::vec3> topR2;
	std::vector<glm::vec3> top;
	std::vector<glm::vec3> top2;
	collosion() {};
	collosion(char* filePath) {
		object obj1;
		obj1.load(filePath);
		obj1.scale(0.05, 0.05, 0.05);
		gen_obb_box(obj1.returnPos());
	}
	void setPos(const std::vector<glm::vec3>& vertexs) {
		posRe = vertexs;
	}
	void div20() {
		a.center /= 20;
		a.half /= 20;
	}

	void initTopP() {
		float temV[3];
		float temV1[3] = { -5689.73,895, 310.76 };
		top.push_back(glm::make_vec3(temV1));
		float temV2[3] = { -2287.73,895, 310.76 };
		top.push_back(glm::make_vec3(temV2));
		float temV3[3] = { -2287.73,895, 11066.8 };
		top.push_back(glm::make_vec3(temV3));
		float temV8[3] = { -5689.73,895, 11066.8 };
		top.push_back(glm::make_vec3(temV8));
		float temV4[3] = { -5689.73,2541,310.76 };
		top.push_back(glm::make_vec3(temV4));

		float temV5[3] = { -2287.73,2541,310.76 };
		top.push_back(glm::make_vec3(temV5));
		float temV6[3] = { -5689.73, 2541, 11066.8 };
		top.push_back(glm::make_vec3(temV6));
		float temV7[3] = { -2287.73,2541,11066.8 };
		top.push_back(glm::make_vec3(temV7));
		topR = top;

		float temV11[3] = { -7804.73,895,6165.76 };
		top2.push_back(glm::make_vec3(temV1));
		float temV21[3] = { -96.7308,895,6165.76 };
		top2.push_back(glm::make_vec3(temV2));
		float temV31[3] = { -96.7308,895,8575.76 };
		top2.push_back(glm::make_vec3(temV3));
		float temV81[3] = { -7804.73,895,8575.76 };
		top2.push_back(glm::make_vec3(temV8));
		float temV41[3] = { -7804.73,2108,6165.76 };
		top2.push_back(glm::make_vec3(temV4));

		float temV51[3] = { -96.7308,2108,6165.76 };
		top2.push_back(glm::make_vec3(temV5));
		float temV61[3] = { -7804.73,2108,8575.76 };
		top2.push_back(glm::make_vec3(temV6));
		float temV71[3] = { -96.7308,2108,8575.76 };
		top2.push_back(glm::make_vec3(temV7));
		topR2 = top2;
	}

	void initTop() {
		float temV[3];
		for(int i=0; i<3; i++)
			temV[i] = a.center[i] + a.half[i];
		top.push_back(glm::make_vec3(temV));
		temV[0] = a.center[0] - a.half[0];
		temV[1] = a.center[1] + a.half[1];
		temV[2] = a.center[2] + a.half[2];
		top.push_back(glm::make_vec3(temV));

		temV[0] = a.center[0] + a.half[0];
		temV[1] = a.center[1] - a.half[1];
		temV[2] = a.center[2] + a.half[2];
		top.push_back(glm::make_vec3(temV));

		temV[0] = a.center[0] + a.half[0];
		temV[1] = a.center[1] + a.half[1];
		temV[2] = a.center[2] - a.half[2];
		top.push_back(glm::make_vec3(temV));

		temV[0] = a.center[0] - a.half[0];
		temV[1] = a.center[1] - a.half[1];
		temV[2] = a.center[2] + a.half[2];
		top.push_back(glm::make_vec3(temV));

		temV[0] = a.center[0] + a.half[0];
		temV[1] = a.center[1] - a.half[1];
		temV[2] = a.center[2] - a.half[2];
		top.push_back(glm::make_vec3(temV));

		temV[0] = a.center[0] - a.half[0];
		temV[1] = a.center[1] + a.half[1];
		temV[2] = a.center[2] - a.half[2];
		top.push_back(glm::make_vec3(temV));

		temV[0] = a.center[0] - a.half[0];
		temV[1] = a.center[1] - a.half[1];
		temV[2] = a.center[2] - a.half[2];
		top.push_back(glm::make_vec3(temV));
		topR = top;
	}
	/*
	����: ����obb���� public
	���أ�obb�нṹ
	������
	vertexs : ��������
	*/
	obb_box gen_obb_box(const std::vector<glm::vec3>& vertexs, int type = 0);
	/*
	���ܣ���������Ƿ���ײ
	���أ�����ֵ
	������
	����obb�нṹ
	*/
	bool check_collision(collosion obj2, int type = 0);
};
#endif // !COLLISION_H

