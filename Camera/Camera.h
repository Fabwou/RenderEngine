#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "../Core/Maths/Vectors/Vec3/Vec3.h"
#include "../Core/Maths/Vectors/Vec2/Vec2.h"
#include "../Mesh/Transform/Transform.h"

#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG

namespace engine
{
	namespace camera
	{
		enum class EProjectType
		{
			PERSPECTIVE,
			ORTHOGRAPHIC
		};

		class Camera
		{
		public:
			Camera(EProjectType type, float width, float height, float _near, float _far, float fov, engine::core::maths::Vec3 pos);
			~Camera();

			EProjectType GetType() const;
			engine::core::maths::Mat4 GetViewMat() const;
			engine::core::maths::Mat4 GetProjectMat() const;
			engine::core::maths::Vec3 GetPosition() const;
			engine::core::maths::Quaternion GetRotation() const;
			engine::core::maths::Vec3 GetForward() const;
			engine::core::maths::Vec3 GetRight() const;
			engine::core::maths::Vec3 GetUp() const;

			void SetType(EProjectType type);
			void SetFov(float fov);
			void SetViewSize(float width, float height, float _near, float _far);
			void SetPosition(engine::core::maths::Vec3 position);
			void SetRotation(engine::core::maths::Quaternion rot);

			void Translate(engine::core::maths::Vec3 position);
			void Rotate(engine::core::maths::Quaternion rot);
			void LookAt(engine::core::maths::Vec3 pos);
			void Check(); // breakpoint debug camera

		protected:
			EProjectType m_type;

			float m_viewWidth;
			float m_viewHeight;
			float m_fov;
			float m_near;
			float m_far;

			Transform m_transform;
			engine::core::maths::Mat4 m_viewMat;
			engine::core::maths::Mat4 m_projectMat;

			void InitProjectionMat();
			void ComputeViewMatrix();
		};
	}
}
#endif