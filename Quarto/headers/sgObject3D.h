#pragma once
#include <sgModel.h>
#include <sgTextureManager.h>
#include <sgTransform.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtx/euler_angles.hpp>
#include <glm/glm/gtx/vector_angle.hpp>

namespace sg {
	class Object3D {
	private:
		static unsigned int nextId;
		int _id;
		glm::mat4 _modelMatrix;
		Transform _transform;
		Model* _model3D;
		Material* _materials;
		unsigned int _nMaterials;
		int _patches;
		//std::vector<Object3D*> _children;

		glm::mat4 BuildRotationMatrix() {
			return glm::inverse(glm::lookAt(glm::vec3(0), _transform.forward, _transform.up));
		}

		void BuildModelMatrix() {
			_modelMatrix = glm::translate(_transform.localPosition)
				* BuildRotationMatrix()
				* glm::scale(_transform.localScale);
		}

		bool FrustumCheck(glm::mat4 mvp) {
			glm::vec3 points[8];
			points[0] = _model3D->GetBoundingBoxLower();
			points[7] = _model3D->GetBoundingBoxUpper();
			glm::vec3 lower = glm::vec3(2, 2, 2);
			glm::vec3 upper = glm::vec3(-2, -2, -2);
			for (int i = 1; i < 7; i++) {
				points[i] = glm::vec3(points[7 * ((i / 4) % 2)].x, points[7 * ((i / 2) % 2)].y, points[7 * (i % 2)].z);
			}
			for (int i = 0; i < 8; i++) {
				glm::vec4 v = mvp * glm::vec4(points[i], 1);
				v.x = v.x / v.w; v.y = v.y / v.w; v.z = v.z / v.w;
				if (v.x < lower.x) lower.x = v.x;
				if (v.y < lower.y) lower.y = v.y;
				if (v.z < lower.z) lower.z = v.z;
				if (v.x > upper.x) upper.x = v.x;
				if (v.y > upper.y) upper.y = v.y;
				if (v.z > upper.z) upper.z = v.z;
			}
			if (lower.x > 1 || lower.y > 1 || lower.z > 1 || upper.x < -1 || upper.y < -1 || upper.z < -1) {
				//printf("Model not rendered: %s\n", _model3D->GetMeshAt(0).name);
				return false;
			}
			return true;
		}

		void CopyMaterialsFromModel() {
			_nMaterials = _model3D->GetNMaterials();
			_materials = (Material*)malloc(sizeof(Material) * _nMaterials);
			for (int i = 0; i < _nMaterials; i++) {
				_materials[i] = _model3D->GetMaterialAt(i);
			}
		}

	public:
		bool CastsShadows;
		bool ReceivesShadows;
		bool Lit;

		Object3D() : _id(nextId++) {
			_modelMatrix = glm::mat4(1);
			_transform = Transform();
			_model3D = NULL;
			_patches = 0;
			CastsShadows = false;
			ReceivesShadows = false;
			Lit = false;
		}

		bool LoadModelFromObj(const char* path) {
			_model3D = (Model*)malloc(sizeof(Model));
			*_model3D = Model();
			if (_model3D->LoadFromObj(path)) {
				CopyMaterialsFromModel();
				return true;
			}
			return false;

		}

		void LoadModelFromData(sg::Vertex vertices[], int nVertices, sg::Triangle triangles[], int nTriangles) {
			_model3D = (Model*)malloc(sizeof(Model));
			*_model3D = Model();
			_model3D->InitFromVerticesAndTriangles(vertices, nVertices, triangles, nTriangles);
			CopyMaterialsFromModel();
		}

		void LoadModelFromData(sg::Vertex vertices[], int nVertices, sg::Material materials[], int nMaterials, sg::Mesh meshes[], int nMeshes) {
			_model3D = (Model*)malloc(sizeof(Model));
			*_model3D = Model();
			_model3D->InitFromVerticesMaterialsAndMeshes(vertices, nVertices, materials, nMaterials, meshes, nMeshes);
			CopyMaterialsFromModel();
		}

		void SetModel(sg::Model* model) {
			_model3D = model;
			CopyMaterialsFromModel();
		}

		Material GetMaterialAt(unsigned int index) { return _materials[index]; }

		Material* GetMaterialReferenceAt(unsigned int index) { return &_materials[index]; }

		Material* GetMaterialByName(const char* name) {
			for (int i = 0; i < _nMaterials; i++) {
				if (strcmp(name, _materials[i].name) == 0) {
					return &_materials[i];
				}
			}
			return &_materials[0];
		}

		void ChangeMaterial(const char* name, Material newMat) {
			Material* old = GetMaterialByName(name);
			old->Ka[0] = newMat.Ka[0]; old->Ka[1] = newMat.Ka[1];  old->Ka[2] = newMat.Ka[2];
			old->Kd[0] = newMat.Kd[0]; old->Kd[1] = newMat.Kd[1];  old->Kd[2] = newMat.Kd[2];
			old->Ks[0] = newMat.Ks[0]; old->Ks[1] = newMat.Ks[1];  old->Ks[2] = newMat.Ks[2];
			old->Ke[0] = newMat.Ke[0]; old->Ke[1] = newMat.Ke[1];  old->Ke[2] = newMat.Ke[2];
			old->Tf[0] = newMat.Tf[0]; old->Tf[1] = newMat.Tf[1];  old->Tf[2] = newMat.Tf[2];
			old->Ns = newMat.Ns;
			old->Ni = newMat.Ni;
			old->illum = newMat.illum;
			old->d = newMat.d;
			old->Tr = newMat.Tr;
			old->texture_Ka = newMat.texture_Ka;
			old->texture_Kd = newMat.texture_Kd;
			old->texture_Ks = newMat.texture_Ks;
			old->texture_Ns = newMat.texture_Ns;
			old->texture_d = newMat.texture_d;
			old->texture_bump = newMat.texture_bump;
			old->texture_disp = newMat.texture_disp;
		}

		void SetPatches(int p) {
			_patches = p;
		}

		Model* GetModel() {
			return _model3D;
		}

		glm::mat4 GetModelMatrix() {
			BuildModelMatrix();
			return _modelMatrix;
		}

		void Translate(float x, float y, float z) { _transform.Translate(x, y, z); }
		void Translate(glm::vec3 vec) { _transform.Translate(vec); }
		void SetLocalPosition(float x, float y, float z) { _transform.localPosition = glm::vec3(x, y, z); }
		void SetLocalPosition(glm::vec3 pos) { _transform.localPosition = pos; }
		glm::vec3 GetLocalPosition() { return _transform.localPosition; }

		void Rotate(float x, float y, float z) { _transform.Rotate(x, y, z); }
		void Rotate(glm::vec3 axis, float angle) { _transform.Rotate(axis, angle); }
		void RotateAround(glm::vec3 axis, glm::vec3 point, float angle) { _transform.RotateAround(axis, point, angle); }
		void SetLocalRotation(float x, float y, float z) { _transform.ResetRotation();  _transform.Rotate(x, y, z); }
		void ResetRotation() { _transform.ResetRotation(); }

		void Scale(float x, float y, float z) { _transform.Scale(x, y, z); }
		void Scale(glm::vec3 scale) { _transform.Scale(scale); }
		void SetLocalScale(float x, float y, float z) { _transform.localScale = glm::vec3(x, y, z); }
		void SetLocalScale(glm::vec3 scale) { _transform.localScale = scale; }
		void SetLocalUniformScale(float s) { _transform.localScale = glm::vec3(s, s, s); }

		glm::vec3 Forward() { return _transform.forward; }
		glm::vec3 Up() { return _transform.up; }
		glm::vec3 Right() { return _transform.right; }

		//void AddChild(Object3D* child) {
			//_children.push_back(child);
		//}

		//void RemoveChild(Object3D* child) {
			//_children.erase(std::remove(_children.begin(), _children.end(), child), _children.end());
		//}

		void Draw(glm::mat4 vp, GLuint program) {
			BuildModelMatrix();
			glm::mat4 mvp = vp * _modelMatrix;
			if (FrustumCheck(mvp)) {
				glUseProgram(program);
				glUniformMatrix4fv(glGetUniformLocation(program, "mvp"), 1, false, glm::value_ptr(mvp));

				glBindBuffer(GL_ARRAY_BUFFER, _model3D->GetVBO());
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(sg::Vertex), (GLvoid*)0);
				glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(sg::Vertex), (GLvoid*)(sizeof(float) * 3));
				glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(sg::Vertex), (GLvoid*)(sizeof(float) * 5));
				for (int i = 0; i < _model3D->GetNMeshes(); i++) {
					sg::Mesh m = _model3D->GetMeshAt(i);
					sg::TextureManager::Instance()->SetMaterialData(program, GetMaterialByName(m.materialName));
					if (_patches > 0) {
						glDrawArrays(GL_PATCHES, 0, _patches);
					} else {
						glDrawElements(GL_TRIANGLES, m.nTriangles * 3, GL_UNSIGNED_INT, m.triangles);
					}
				}
			}

			//for (int i = 0; i < _children.size(); i++) {
			//	_children[i]->Draw(mvp, program);
			//}
		}
	};
	unsigned int Object3D::nextId;

}
