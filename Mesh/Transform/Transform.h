#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include "../../Core/Maths/Vectors/Vec4/Vec4.h"
#include "../../Core/Maths/Matrices/Mat4.h"
#include "../../Core/Maths/Quaternions/Quaternion.h"
#include <vector>

#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG

using namespace engine::core::maths;

class Transform
{
	public:
		Transform();
		Transform(const Transform& copy);
		Transform(Transform&& copy) noexcept;
		~Transform();

		void Translate(const Vec3& translation);
		void Rotate(const Quaternion& rotation);
		void Scale(const Vec3& scale);

		Vec3 GetPosition() const;
		Quaternion GetRotation() const;
		Vec3 GetScale() const;
		Mat4 GetWorldMat() const;
		Vec3 GetRight() const;
		Vec3 GetUp() const;
		Vec3 GetForward() const;

		void SetPosition(const Vec3& position);
		void SetRotation(const Quaternion& rotation);
		void SetScale(const Vec3& scale);
		void SetTRS(const Vec3& pos, const Quaternion& rot, const Vec3& scale);

		Transform& operator=(const Transform& other);
		Transform& operator=(Transform&& other) noexcept;
		bool operator==(const Transform& other) const;
		bool operator!=(const Transform& other) const;

	private:

		Vec3 m_pos;
		Quaternion m_rot;
		Vec3 m_scale;

		Mat4 m_worldMat;
};
#endif