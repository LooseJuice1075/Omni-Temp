#pragma once
#include <box2d/box2d.h>

namespace Omni
{
	//------------Bodies------------

	// TODO : Finish StaticBody Classes

	class StaticBody
	{
	public:
		void Create(glm::vec2 pos, float angle, const b2Vec2* points, int32 count);

		// TODO : Make GetPoints function that returns an array of all the points that m_Shape has

		void SetTransform(glm::vec2 pos) { m_Body->SetTransform({ pos.x, pos.y }, m_Body->GetAngle()); }
		glm::vec2 GetTransform() { return { m_Body->GetPosition().x, m_Body->GetPosition().y }; }

		void SetAngle(float angle) { m_Body->SetTransform(m_Body->GetPosition(), angle); }
		float GetAngle() { return m_Body->GetAngle(); }

		void SetAwake(bool flag) { m_Body->SetAwake(flag); }
		bool IsAwake() { return m_Body->IsAwake(); }

		void SetAllowSleep(bool sleep) { m_Body->SetSleepingAllowed(sleep); }
		bool GetAllowSleep() { return m_Body->IsSleepingAllowed(); }

		void SetFixedRot(bool fixedRot) { m_Body->SetFixedRotation(fixedRot); }
		bool GetFixedRot() { return m_Body->IsFixedRotation(); }

		glm::vec2 GetWorldCenter() { return { m_Body->GetWorldCenter().x, m_Body->GetWorldCenter().y }; }
		glm::vec2 GetLocalCenter() { return { m_Body->GetLocalCenter().x, m_Body->GetLocalCenter().y }; }

		glm::vec2 GetWorldPoint(glm::vec2 localPoint)
		{
			return { m_Body->GetWorldPoint({ localPoint.x, localPoint.y }).x, m_Body->GetWorldPoint({ localPoint.x, localPoint.y }).y };
		}
		glm::vec2 GetWorldVector(glm::vec2 localVector)
		{
			return { m_Body->GetWorldVector({localVector.x, localVector.y}).x, m_Body->GetWorldVector({localVector.x, localVector.y}).y };
		}

		glm::vec2 GetLocalPoint(glm::vec2 worldPoint)
		{
			return { m_Body->GetLocalPoint({worldPoint.x, worldPoint.y}).x, m_Body->GetLocalPoint({worldPoint.x, worldPoint.y}).y };
		}
		glm::vec2 GetLocalVector(glm::vec2 worldVector)
		{
			return { m_Body->GetLocalVector({worldVector.x, worldVector.y}).x, m_Body->GetLocalVector({worldVector.x, worldVector.y}).y };
		}

	private:
		b2BodyDef m_BodyDef;
		b2Body* m_Body;
		b2PolygonShape m_Shape;
	};

	class StaticBox
	{
	public:
		void Create(glm::vec2 pos, float angle, glm::vec2 size);

		void SetTransform(glm::vec2 pos) { m_Body->SetTransform({ pos.x, pos.y }, m_Body->GetAngle()); }
		glm::vec2 GetTransform() { return { m_Body->GetPosition().x, m_Body->GetPosition().y }; }

		void SetAngle(float angle) { m_Body->SetTransform(m_Body->GetPosition(), angle); }
		float GetAngle() { return m_Body->GetAngle(); }

		void SetSize(glm::vec2 size) { m_Shape.SetAsBox(size.x, size.y); }
		glm::vec2 GetSize() { return { m_BoxShapeSize.x, m_BoxShapeSize.y }; }

		void SetAwake(bool flag) { m_Body->SetAwake(flag); }
		bool IsAwake() { return m_Body->IsAwake(); }

		void SetAllowSleep(bool sleep) { m_Body->SetSleepingAllowed(sleep); }
		bool GetAllowSleep() { return m_Body->IsSleepingAllowed(); }

		void SetFixedRot(bool fixedRot) { m_Body->SetFixedRotation(fixedRot); }
		bool GetFixedRot() { return m_Body->IsFixedRotation(); }

		glm::vec2 GetWorldCenter() { return { m_Body->GetWorldCenter().x, m_Body->GetWorldCenter().y }; }
		glm::vec2 GetLocalCenter() { return { m_Body->GetLocalCenter().x, m_Body->GetLocalCenter().y }; }

		glm::vec2 GetWorldPoint(glm::vec2 localPoint)
		{
			return { m_Body->GetWorldPoint({ localPoint.x, localPoint.y }).x, m_Body->GetWorldPoint({ localPoint.x, localPoint.y }).y };
		}
		glm::vec2 GetWorldVector(glm::vec2 localVector)
		{
			return { m_Body->GetWorldVector({localVector.x, localVector.y}).x, m_Body->GetWorldVector({localVector.x, localVector.y}).y };
		}

		glm::vec2 GetLocalPoint(glm::vec2 worldPoint)
		{
			return { m_Body->GetLocalPoint({worldPoint.x, worldPoint.y}).x, m_Body->GetLocalPoint({worldPoint.x, worldPoint.y}).y };
		}
		glm::vec2 GetLocalVector(glm::vec2 worldVector)
		{
			return { m_Body->GetLocalVector({worldVector.x, worldVector.y}).x, m_Body->GetLocalVector({worldVector.x, worldVector.y}).y };
		}

	private:
		b2BodyDef m_BodyDef;
		b2Body* m_Body;
		b2PolygonShape m_Shape;
		b2Vec2 m_BoxShapeSize = { 0.0f, 0.0f };
	};

	class StaticCircle
	{
	public:
		void Create(glm::vec2 pos, float angle, float radius);

		void SetTransform(glm::vec2 pos) { m_Body->SetTransform({ pos.x, pos.y }, m_Body->GetAngle()); }
		glm::vec2 GetTransform() { return { m_Body->GetPosition().x, m_Body->GetPosition().y }; }

		void SetAngle(float angle) { m_Body->SetTransform(m_Body->GetPosition(), angle); }
		float GetAngle() { return m_Body->GetAngle(); }

		void SetRadius(float radius) { m_Shape.m_radius = radius; }
		float GetRadius() { return m_Shape.m_radius; }

		void SetAwake(bool flag) { m_Body->SetAwake(flag); }
		bool IsAwake() { return m_Body->IsAwake(); }

		void SetAllowSleep(bool sleep) { m_Body->SetSleepingAllowed(sleep); }
		bool GetAllowSleep() { return m_Body->IsSleepingAllowed(); }

		void SetFixedRot(bool fixedRot) { m_Body->SetFixedRotation(fixedRot); }
		bool GetFixedRot() { return m_Body->IsFixedRotation(); }

		glm::vec2 GetWorldCenter() { return { m_Body->GetWorldCenter().x, m_Body->GetWorldCenter().y }; }
		glm::vec2 GetLocalCenter() { return { m_Body->GetLocalCenter().x, m_Body->GetLocalCenter().y }; }

		glm::vec2 GetWorldPoint(glm::vec2 localPoint)
		{
			return { m_Body->GetWorldPoint({ localPoint.x, localPoint.y }).x, m_Body->GetWorldPoint({ localPoint.x, localPoint.y }).y };
		}
		glm::vec2 GetWorldVector(glm::vec2 localVector)
		{
			return { m_Body->GetWorldVector({localVector.x, localVector.y}).x, m_Body->GetWorldVector({localVector.x, localVector.y}).y };
		}

		glm::vec2 GetLocalPoint(glm::vec2 worldPoint)
		{
			return { m_Body->GetLocalPoint({worldPoint.x, worldPoint.y}).x, m_Body->GetLocalPoint({worldPoint.x, worldPoint.y}).y };
		}
		glm::vec2 GetLocalVector(glm::vec2 worldVector)
		{
			return { m_Body->GetLocalVector({worldVector.x, worldVector.y}).x, m_Body->GetLocalVector({worldVector.x, worldVector.y}).y };
		}

	private:
		b2BodyDef m_BodyDef;
		b2Body* m_Body;
		b2CircleShape m_Shape;
	};

	// TODO : Finish DynamicBody classes

	class DynamicBody
	{
	public:

		void Create(glm::vec2 pos, float angle, const b2Vec2* points, int32 count, float density, float friction);

		// TODO : Make GetPoints function that returns an array of all the points that m_Shape has

		glm::vec2 GetWorldCenter() { return { m_Body->GetWorldCenter().x, m_Body->GetWorldCenter().y }; }
		glm::vec2 GetLocalCenter() { return { m_Body->GetLocalCenter().x, m_Body->GetLocalCenter().y }; }

		void SetLinearVelocity(glm::vec2 v) { m_Body->SetLinearVelocity({ v.x, v.y }); }
		glm::vec2 GetLinearVelocity() { return { m_Body->GetLinearVelocity().x, m_Body->GetLinearVelocity().y }; }

		void SetAngularVelocity(float omega) { m_Body->SetAngularVelocity(omega); }
		float GetAngularVelocity() { return m_Body->GetAngularVelocity(); }

		void SetGravityScale(float scale) { m_Body->SetGravityScale(scale); }
		float GetGravityScale() { return m_Body->GetGravityScale(); }

		void ApplyForce(glm::vec2 force, glm::vec2 point, bool wake) { m_Body->ApplyForce({ force.x, force.y }, {point.x, point.y}, wake); }
		void ApplyForceToCenter(glm::vec2 force, bool wake) { m_Body->ApplyForceToCenter({ force.x, force.y }, wake); }

		void ApplyTorque(float torque, bool wake) { m_Body->ApplyTorque(torque, wake); }

		void ApplyLinearImpulse(glm::vec2 impulse, glm::vec2 point, bool wake)
		{ m_Body->ApplyLinearImpulse({ impulse.x, impulse.y }, { point.x, point.y }, wake); }
		void ApplyLinearImpulseToCenter(glm::vec2 impulse, bool wake)
		{ m_Body->ApplyLinearImpulseToCenter({ impulse.x, impulse.y }, wake); }
		void ApplyAngularImpulse(float impulse, bool wake) { m_Body->ApplyAngularImpulse(impulse, wake); }

		float GetDensity() { return m_Fixture.density; }
		float GetMass() { return m_Body->GetMass(); }
		float GetInertia() { return m_Body->GetInertia(); }

		// TODO : MassData functions

		glm::vec2 GetWorldPoint(glm::vec2 localPoint)
		{ return { m_Body->GetWorldPoint({ localPoint.x, localPoint.y }).x, m_Body->GetWorldPoint({ localPoint.x, localPoint.y }).y }; }
		glm::vec2 GetWorldVector(glm::vec2 localVector)
		{ return { m_Body->GetWorldVector({localVector.x, localVector.y}).x, m_Body->GetWorldVector({localVector.x, localVector.y}).y }; }

		glm::vec2 GetLocalPoint(glm::vec2 worldPoint)
		{ return { m_Body->GetLocalPoint({worldPoint.x, worldPoint.y}).x, m_Body->GetLocalPoint({worldPoint.x, worldPoint.y}).y }; }
		glm::vec2 GetLocalVector(glm::vec2 worldVector)
		{ return { m_Body->GetLocalVector({worldVector.x, worldVector.y}).x, m_Body->GetLocalVector({worldVector.x, worldVector.y}).y }; }

		// TODO: Implement the rest of the dynamic body functions https://box2d.org/documentation/classb2_body.html

		void SetTransform(glm::vec2 pos) { m_Body->SetTransform({ pos.x, pos.y }, m_Body->GetAngle()); }
		glm::vec2 GetTransform() { return { m_Body->GetPosition().x, m_Body->GetPosition().y }; }

		void SetAngle(float angle) { m_Body->SetTransform(m_Body->GetPosition(), angle); }
		float GetAngle() { return m_Body->GetAngle(); }

		void SetShape(const b2Vec2* points, int32 count) { m_Shape.Set(points, count); }

		void SetAwake(bool flag) { m_Body->SetAwake(flag); }
		bool IsAwake() { return m_Body->IsAwake(); }

		void SetAllowSleep(bool sleep) { m_Body->SetSleepingAllowed(sleep); }
		bool GetAllowSleep() { return m_Body->IsSleepingAllowed(); }
		void SetFixedRot(bool fixedRot) { m_Body->SetFixedRotation(fixedRot); }
		bool GetFixedRot() { return m_Body->IsFixedRotation(); }
	private:
		b2BodyDef m_BodyDef;
		b2Body* m_Body;
		b2PolygonShape m_Shape;
		b2FixtureDef m_Fixture;
	};

	class DynamicBox
	{
	public:

		void Create(glm::vec2 pos, float angle, glm::vec2 size, float density, float friction);

		glm::vec2 GetWorldCenter() { return { m_Body->GetWorldCenter().x, m_Body->GetWorldCenter().y }; }
		glm::vec2 GetLocalCenter() { return { m_Body->GetLocalCenter().x, m_Body->GetLocalCenter().y }; }

		void SetLinearVelocity(glm::vec2 v) { m_Body->SetLinearVelocity({ v.x, v.y }); }
		glm::vec2 GetLinearVelocity() { return { m_Body->GetLinearVelocity().x, m_Body->GetLinearVelocity().y }; }

		void SetAngularVelocity(float omega) { m_Body->SetAngularVelocity(omega); }
		float GetAngularVelocity() { return m_Body->GetAngularVelocity(); }

		void SetGravityScale(float scale) { m_Body->SetGravityScale(scale); }
		float GetGravityScale() { return m_Body->GetGravityScale(); }

		void ApplyForce(glm::vec2 force, glm::vec2 point, bool wake) { m_Body->ApplyForce({ force.x, force.y }, { point.x, point.y }, wake); }
		void ApplyForceToCenter(glm::vec2 force, bool wake) { m_Body->ApplyForceToCenter({ force.x, force.y }, wake); }

		void ApplyTorque(float torque, bool wake) { m_Body->ApplyTorque(torque, wake); }

		void ApplyLinearImpulse(glm::vec2 impulse, glm::vec2 point, bool wake)
		{
			m_Body->ApplyLinearImpulse({ impulse.x, impulse.y }, { point.x, point.y }, wake);
		}
		void ApplyLinearImpulseToCenter(glm::vec2 impulse, bool wake)
		{
			m_Body->ApplyLinearImpulseToCenter({ impulse.x, impulse.y }, wake);
		}
		void ApplyAngularImpulse(float impulse, bool wake) { m_Body->ApplyAngularImpulse(impulse, wake); }

		float GetDensity() { return m_Fixture.density; }
		float GetMass() { return m_Body->GetMass(); }
		float GetInertia() { return m_Body->GetInertia(); }

		// TODO : MassData functions

		glm::vec2 GetWorldPoint(glm::vec2 localPoint)
		{
			return { m_Body->GetWorldPoint({ localPoint.x, localPoint.y }).x, m_Body->GetWorldPoint({ localPoint.x, localPoint.y }).y };
		}
		glm::vec2 GetWorldVector(glm::vec2 localVector)
		{
			return { m_Body->GetWorldVector({localVector.x, localVector.y}).x, m_Body->GetWorldVector({localVector.x, localVector.y}).y };
		}

		glm::vec2 GetLocalPoint(glm::vec2 worldPoint)
		{
			return { m_Body->GetLocalPoint({worldPoint.x, worldPoint.y}).x, m_Body->GetLocalPoint({worldPoint.x, worldPoint.y}).y };
		}
		glm::vec2 GetLocalVector(glm::vec2 worldVector)
		{
			return { m_Body->GetLocalVector({worldVector.x, worldVector.y}).x, m_Body->GetLocalVector({worldVector.x, worldVector.y}).y };
		}

		// TODO: Implement the rest of the dynamic body functions https://box2d.org/documentation/classb2_body.html

		void SetTransform(glm::vec2 pos) { m_Body->SetTransform({ pos.x, pos.y }, m_Body->GetAngle()); }
		glm::vec2 GetTransform() { return { m_Body->GetPosition().x, m_Body->GetPosition().y }; }

		void SetAngle(float angle) { m_Body->SetTransform(m_Body->GetPosition(), angle); }
		float GetAngle() { return m_Body->GetAngle(); }

		void SetSize(glm::vec2 size) { m_Shape.SetAsBox(size.x, size.y); }
		glm::vec2 GetSize() { return { m_BoxShapeSize.x, m_BoxShapeSize.y }; }

		void SetAwake(bool flag) { m_Body->SetAwake(flag); }
		bool IsAwake() { return m_Body->IsAwake(); }

		void SetAllowSleep(bool sleep) { m_Body->SetSleepingAllowed(sleep); }
		bool GetAllowSleep() { return m_Body->IsSleepingAllowed(); }

		void SetFixedRot(bool fixedRot) { m_Body->SetFixedRotation(fixedRot); }
		bool GetFixedRot() { return m_Body->IsFixedRotation(); }
	private:
		b2BodyDef m_BodyDef;
		b2Body* m_Body;
		b2PolygonShape m_Shape;
		b2Vec2 m_BoxShapeSize = { 0.0f, 0.0f };
		b2FixtureDef m_Fixture;
	};

	class DynamicCircle
	{
	public:

		void Create(glm::vec2 pos, float angle, float radius, float density, float friction);

		glm::vec2 GetWorldCenter() { return { m_Body->GetWorldCenter().x, m_Body->GetWorldCenter().y }; }
		glm::vec2 GetLocalCenter() { return { m_Body->GetLocalCenter().x, m_Body->GetLocalCenter().y }; }

		void SetLinearVelocity(glm::vec2 v) { m_Body->SetLinearVelocity({ v.x, v.y }); }
		glm::vec2 GetLinearVelocity() { return { m_Body->GetLinearVelocity().x, m_Body->GetLinearVelocity().y }; }

		void SetAngularVelocity(float omega) { m_Body->SetAngularVelocity(omega); }
		float GetAngularVelocity() { return m_Body->GetAngularVelocity(); }

		void SetGravityScale(float scale) { m_Body->SetGravityScale(scale); }
		float GetGravityScale() { return m_Body->GetGravityScale(); }

		void ApplyForce(glm::vec2 force, glm::vec2 point, bool wake) { m_Body->ApplyForce({ force.x, force.y }, { point.x, point.y }, wake); }
		void ApplyForceToCenter(glm::vec2 force, bool wake) { m_Body->ApplyForceToCenter({ force.x, force.y }, wake); }

		void ApplyTorque(float torque, bool wake) { m_Body->ApplyTorque(torque, wake); }

		void ApplyLinearImpulse(glm::vec2 impulse, glm::vec2 point, bool wake)
		{
			m_Body->ApplyLinearImpulse({ impulse.x, impulse.y }, { point.x, point.y }, wake);
		}
		void ApplyLinearImpulseToCenter(glm::vec2 impulse, bool wake)
		{
			m_Body->ApplyLinearImpulseToCenter({ impulse.x, impulse.y }, wake);
		}
		void ApplyAngularImpulse(float impulse, bool wake) { m_Body->ApplyAngularImpulse(impulse, wake); }

		float GetDensity() { return m_Fixture.density; }
		float GetMass() { return m_Body->GetMass(); }
		float GetInertia() { return m_Body->GetInertia(); }

		// TODO : MassData functions

		glm::vec2 GetWorldPoint(glm::vec2 localPoint)
		{
			return { m_Body->GetWorldPoint({ localPoint.x, localPoint.y }).x, m_Body->GetWorldPoint({ localPoint.x, localPoint.y }).y };
		}
		glm::vec2 GetWorldVector(glm::vec2 localVector)
		{
			return { m_Body->GetWorldVector({localVector.x, localVector.y}).x, m_Body->GetWorldVector({localVector.x, localVector.y}).y };
		}

		glm::vec2 GetLocalPoint(glm::vec2 worldPoint)
		{
			return { m_Body->GetLocalPoint({worldPoint.x, worldPoint.y}).x, m_Body->GetLocalPoint({worldPoint.x, worldPoint.y}).y };
		}
		glm::vec2 GetLocalVector(glm::vec2 worldVector)
		{
			return { m_Body->GetLocalVector({worldVector.x, worldVector.y}).x, m_Body->GetLocalVector({worldVector.x, worldVector.y}).y };
		}

		// TODO: Implement the rest of the dynamic body functions https://box2d.org/documentation/classb2_body.html

		void SetTransform(glm::vec2 pos) { m_Body->SetTransform({ pos.x, pos.y }, m_Body->GetAngle()); }
		glm::vec2 GetTransform() { return { m_Body->GetPosition().x, m_Body->GetPosition().y }; }
		void SetAngle(float angle) { m_Body->SetTransform(m_Body->GetPosition(), angle); }
		float GetAngle() { return m_Body->GetAngle(); }
		void SetRadius(float radius) { m_Shape.m_radius = radius; }
		float GetRadius() { return m_Shape.m_radius; }
		void SetAwake(bool flag) { m_Body->SetAwake(flag); }
		bool IsAwake() { return m_Body->IsAwake(); }

		void SetAllowSleep(bool sleep) { m_Body->SetSleepingAllowed(sleep); }
		bool GetAllowSleep() { return m_Body->IsSleepingAllowed(); }
		void SetFixedRot(bool fixedRot) { m_Body->SetFixedRotation(fixedRot); }
		bool GetFixedRot() { return m_Body->IsFixedRotation(); }
	private:
		b2BodyDef m_BodyDef;
		b2Body* m_Body;
		b2CircleShape m_Shape;
		b2FixtureDef m_Fixture;
	};

	//------------Bodies------------

	enum DrawFlag
	{
		e_shapeBit = 0x0001,		///< draw shapes
		e_jointBit = 0x0002,		///< draw joint connections
		e_aabbBit = 0x0004,			///< draw axis aligned bounding boxes
		e_pairBit = 0x0008,			///< draw broad-phase pairs
		e_centerOfMassBit = 0x0010	///< draw center of mass frame
	};

	class DebugDraw : public b2Draw
	{
	public:
		friend class PhysicsManager;

		float SolidOpacity	= 35.0f;
		float LineOpacity	= 255.0f;
		float LineThickness = 2.0f;

		void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
		void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
		void DrawCircle(const b2Vec2& center, float radius, const b2Color& color);
		void DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color);
		void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color);
		void DrawTransform(const b2Transform& xf);
		void DrawPoint(const b2Vec2& p, float size, const b2Color& color);
	};

	class PhysicsManager
	{
	public:
		friend class StaticBody;
		friend class StaticBox;
		friend class StaticCircle;
		friend class DynamicBody;
		friend class DynamicBox;
		friend class DynamicCircle;
		friend class DebugDraw;

		static void InitDebugDraw(uint32 drawFlags);
		static void ShutdownDebugDraw();
		static void SetDebugDrawFlags(uint32 drawFlags);
		static void AppendDebugDrawFlags(uint32 drawFlags);
		static uint32 GetDebugDrawFlags();
		static void DrawDebug();

		static void Step(float timeStep);

		inline static int32 s_VelocityIterations = 8;
		inline static int32 s_PositionIterations = 3;
		inline static b2Vec2 s_Gravity = { 0.0f, -10.0f };
	private:
		inline static b2World s_World = b2World(s_Gravity);
		inline static DebugDraw s_DebugDraw;
	};
}
