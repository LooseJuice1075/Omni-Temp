#include <ompch.h>
#include "Physics.h"
#include "core/Application.h"
#include "core/Omni_Utils.h"

namespace Omni
{
	#pragma region Bodies

	void StaticBody::Create(glm::vec2 pos, float angle, const b2Vec2* points, int32 count)
	{
		m_BodyDef.position.Set(pos.x, pos.y);
		m_BodyDef.angle = angle;
		m_Body = PhysicsManager::s_World.CreateBody(&m_BodyDef);
		m_Shape.Set(points, count);
		m_Body->CreateFixture(&m_Shape, 0.0f);
	}

	void StaticBox::Create(glm::vec2 pos, float angle, glm::vec2 size)
	{
		m_BodyDef.position.Set(pos.x, pos.y);
		m_BodyDef.angle = angle;
		m_Body = PhysicsManager::s_World.CreateBody(&m_BodyDef);
		m_Shape.SetAsBox(size.x, size.y);
		m_Body->CreateFixture(&m_Shape, 0.0f);
	}

	void StaticCircle::Create(glm::vec2 pos, float angle, float radius)
	{
		m_BodyDef.position.Set(pos.x, pos.y);
		m_BodyDef.angle = angle;
		m_Body = PhysicsManager::s_World.CreateBody(&m_BodyDef);
		m_Shape.m_radius = radius;
		m_Body->CreateFixture(&m_Shape, 0.0f);
	}

	void DynamicBody::Create(glm::vec2 pos, float angle, const b2Vec2* points, int32 count, float density, float friction)
	{
		m_BodyDef.position.Set(pos.x, pos.y);
		m_BodyDef.type = b2_dynamicBody;
		m_Body = PhysicsManager::s_World.CreateBody(&m_BodyDef);
		m_Shape.Set(points, count);
		m_Fixture.shape = &m_Shape;
		m_Fixture.density = density;
		m_Fixture.friction = friction;
		m_Body->CreateFixture(&m_Fixture);
	}

	void DynamicBox::Create(glm::vec2 pos, float angle, glm::vec2 size, float density, float friction)
	{
		m_BodyDef.position.Set(pos.x, pos.y);
		m_BodyDef.type = b2_dynamicBody;
		m_Body = PhysicsManager::s_World.CreateBody(&m_BodyDef);
		m_Shape.SetAsBox(size.x, size.y);
		m_Fixture.shape = &m_Shape;
		m_Fixture.density = density;
		m_Fixture.friction = friction;
		m_Body->CreateFixture(&m_Fixture);
	}

	void DynamicCircle::Create(glm::vec2 pos, float angle, float radius, float density, float friction)
	{
		m_BodyDef.position.Set(pos.x, pos.y);
		m_BodyDef.type = b2_dynamicBody;
		m_Body = PhysicsManager::s_World.CreateBody(&m_BodyDef);
		m_Shape.m_radius = radius;
		m_Fixture.shape = &m_Shape;
		m_Fixture.density = density;
		m_Fixture.friction = friction;
		m_Body->CreateFixture(&m_Fixture);
	}

	#pragma endregion

	void PhysicsManager::InitDebugDraw(uint32 drawFlags)
	{
		s_World.SetDebugDraw(&s_DebugDraw);
		s_DebugDraw.SetFlags(drawFlags);

		//DebugDraw::s_nvg = nvgCreate(1, 0);
	}

	void PhysicsManager::ShutdownDebugDraw()
	{
		//nvgDelete(DebugDraw::s_nvg);
	}

	void PhysicsManager::SetDebugDrawFlags(uint32 drawFlags)
	{
		s_DebugDraw.SetFlags(drawFlags);
	}

	void PhysicsManager::AppendDebugDrawFlags(uint32 drawFlags)
	{
		s_DebugDraw.AppendFlags(drawFlags);
	}

	uint32 PhysicsManager::GetDebugDrawFlags()
	{
		return s_DebugDraw.GetFlags();
	}

	void PhysicsManager::DrawDebug()
	{
		//nvgBeginFrame(DebugDraw::s_nvg, Application::Get().GetWindow().GetWidth(), Application::Get().GetWindow().GetHeight(), 1.0f);
		s_World.DebugDraw();
		//nvgEndFrame(DebugDraw::s_nvg);
	}

	void PhysicsManager::Step(float timeStep)
	{
		s_World.Step(timeStep, s_VelocityIterations, s_PositionIterations);
	}

	//--------Debug-Drawing--------

	void DebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
	{
		//if (CameraManager::GetMainCamera()->m_CamType == EditorCameraType::Perspective) { return; }

		std::vector<glm::vec2> polyVerts;
		for (size_t i = 0; i < vertexCount; i++)
		{
			polyVerts.push_back({ vertices[i].x, vertices[i].y });
		}

		//nvgBeginPath(s_nvg);
		for (size_t i = 0; i < polyVerts.size(); i++)
		{
			if (i == 0)
			{
				//glm::vec3 projCoords =
				//	CameraManager::GetMainCamera()->Project({ polyVerts.at(i).x, -polyVerts.at(i).y, 0 });

				//nvgMoveTo(s_nvg, (projCoords.x), (projCoords.y));
			}
			else if (i == polyVerts.size() - 1)
			{
				//glm::vec3 projCoords =
				//	CameraManager::GetMainCamera()->Project({ polyVerts.at(i).x, -polyVerts.at(i).y, 0 });

				//nvgLineTo(s_nvg, (projCoords.x), (projCoords.y));

				//glm::vec3 zeroProjCoords =
				//	CameraManager::GetMainCamera()->Project({ polyVerts.at(0).x, -polyVerts.at(0).y, 0 });

				//nvgLineTo(s_nvg, (zeroProjCoords.x), (zeroProjCoords.y));
			}
			else
			{
				//glm::vec3 projCoords =
				//	CameraManager::GetMainCamera()->Project({ polyVerts.at(i).x, -polyVerts.at(i).y, 0 });

				//nvgLineTo(s_nvg, (projCoords.x), (projCoords.y));
			}
		}
		//nvgStrokeColor(s_nvg, nvgRGBA(color.r * 255, color.g * 255, color.b * 255, LineOpacity));
		//nvgStrokeWidth(s_nvg, LineThickness);
		//nvgStroke(s_nvg);
	}

	void DebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
	{
		//if (CameraManager::GetMainCamera()->m_CamType == EditorCameraType::Perspective) { return; }

		std::vector<glm::vec2> polyVerts;
		for (size_t i = 0; i < vertexCount; i++)
		{
			polyVerts.push_back({ vertices[i].x, vertices[i].y });
		}

		//nvgBeginPath(s_nvg);
		for (size_t i = 0; i < polyVerts.size(); i++)
		{
			if (i == 0)
			{
				//glm::vec3 projCoords =
				//	CameraManager::GetMainCamera()->Project({ polyVerts.at(i).x, -polyVerts.at(i).y, 0});

				//nvgMoveTo(s_nvg,(projCoords.x),(projCoords.y));
			}
			else if (i == polyVerts.size() - 1)
			{
				//glm::vec3 projCoords =
				//	CameraManager::GetMainCamera()->Project({ polyVerts.at(i).x, -polyVerts.at(i).y, 0 });

				//nvgLineTo(s_nvg, (projCoords.x), (projCoords.y));

				//glm::vec3 zeroProjCoords =
				//	CameraManager::GetMainCamera()->Project({ polyVerts.at(0).x, -polyVerts.at(0).y, 0 });

				//nvgLineTo(s_nvg, (zeroProjCoords.x), (zeroProjCoords.y));
			}
			else
			{
				//glm::vec3 projCoords =
				//	CameraManager::GetMainCamera()->Project({ polyVerts.at(i).x, -polyVerts.at(i).y, 0 });

				//nvgLineTo(s_nvg,(projCoords.x),(projCoords.y));
			}
		}
		//nvgFillColor(s_nvg, nvgRGBA(color.r * 255, color.g * 255, color.b * 255, SolidOpacity));
		//nvgFill(s_nvg);

		//nvgBeginPath(s_nvg);
		for (size_t i = 0; i < polyVerts.size(); i++)
		{
			if (i == 0)
			{
				//glm::vec3 projCoords =
				//	CameraManager::GetMainCamera()->Project({ polyVerts.at(i).x, -polyVerts.at(i).y, 0 });

				//nvgMoveTo(s_nvg, (projCoords.x), (projCoords.y));
			}
			else if (i == polyVerts.size() - 1)
			{
				//glm::vec3 projCoords =
				//	CameraManager::GetMainCamera()->Project({ polyVerts.at(i).x, -polyVerts.at(i).y, 0 });

				//nvgLineTo(s_nvg, (projCoords.x), (projCoords.y));

				//glm::vec3 zeroProjCoords =
				//	CameraManager::GetMainCamera()->Project({ polyVerts.at(0).x, -polyVerts.at(0).y, 0 });

				//nvgLineTo(s_nvg, (zeroProjCoords.x), (zeroProjCoords.y));
			}
			else
			{
				//glm::vec3 projCoords =
				//	CameraManager::GetMainCamera()->Project({ polyVerts.at(i).x, -polyVerts.at(i).y, 0 });

				//nvgLineTo(s_nvg, (projCoords.x), (projCoords.y));
			}
		}
		//nvgStrokeColor(s_nvg, nvgRGBA(color.r * 255, color.g * 255, color.b * 255, LineOpacity));
		//nvgStrokeWidth(s_nvg, LineThickness);
		//nvgStroke(s_nvg);
	}

	void DebugDraw::DrawCircle(const b2Vec2& center, float radius, const b2Color& color)
	{
		//if (CameraManager::GetMainCamera()->m_CamType == EditorCameraType::Perspective) { return; }
		//if (CameraManager::GetMainCamera()->m_FOV == 0.0f){ return; }

		//glm::vec3 projCoord =
		//	CameraManager::GetMainCamera()->Project({ center.x, -center.y, 0.0f });

		//nvgBeginPath(s_nvg);
		//nvgCircle(s_nvg, projCoord.x, projCoord.y, radius * (60.0f / CameraManager::GetMainCamera()->m_FOV));
		//nvgStrokeColor(s_nvg, nvgRGBA(color.r * 255, color.g * 255, color.b * 255, LineOpacity));
		//nvgStrokeWidth(s_nvg, LineThickness);
		//nvgStroke(s_nvg);
	}

	void DebugDraw::DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color)
	{
		//if (CameraManager::GetMainCamera()->m_CamType == EditorCameraType::Perspective) { return; }
		//if (CameraManager::GetMainCamera()->m_FOV == 0.0f) { return; }

		//glm::vec3 projCoord =
		//	CameraManager::GetMainCamera()->Project({ center.x, -center.y, 0.0f });

		//nvgBeginPath(s_nvg);
		//nvgCircle(s_nvg, projCoord.x, projCoord.y, radius * (60.0f / CameraManager::GetMainCamera()->m_FOV));
		//nvgFillColor(s_nvg, nvgRGBA(color.r * 255, color.g * 255, color.b * 255, SolidOpacity));
		//nvgFill(s_nvg);

		//nvgBeginPath(s_nvg);
		//nvgCircle(s_nvg, projCoord.x, projCoord.y, radius * (60.0f / CameraManager::GetMainCamera()->m_FOV));
		//nvgStrokeColor(s_nvg, nvgRGBA(color.r * 255, color.g * 255, color.b * 255, LineOpacity));
		//nvgStrokeWidth(s_nvg, LineThickness);
		//nvgStroke(s_nvg);
	}

	void DebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
	{
		//if (CameraManager::GetMainCamera()->m_CamType == EditorCameraType::Perspective) { return; }

		//glm::vec3 p1Proj =
		//	CameraManager::GetMainCamera()->Project({ p1.x, -p1.y, 0.0f });

		//glm::vec3 p2Proj =
		//	CameraManager::GetMainCamera()->Project({ p2.x, -p2.y, 0.0f });

		//nvgBeginPath(s_nvg);
		//nvgMoveTo(s_nvg, p1Proj.x, p1Proj.y);
		//nvgLineTo(s_nvg, p2Proj.x, p2Proj.y);
		//nvgStrokeColor(s_nvg, nvgRGBA(color.r * 255, color.g * 255, color.b * 255, LineOpacity));
		//nvgStrokeWidth(s_nvg, LineThickness);
		//nvgStroke(s_nvg);
	}

	// TODO : Write DrawTransform Function

	void DebugDraw::DrawTransform(const b2Transform& xf)
	{
	}

	void DebugDraw::DrawPoint(const b2Vec2& p, float size, const b2Color& color)
	{
		//if (CameraManager::GetMainCamera()->m_CamType == EditorCameraType::Perspective) { return; }
		//if (CameraManager::GetMainCamera()->m_FOV == 0.0f) { return; }

		//glm::vec3 projCoord =
		//	CameraManager::GetMainCamera()->Project({ p.x, -p.y, 0.0f });

		//nvgBeginPath(s_nvg);
		//nvgCircle(s_nvg, projCoord.x, projCoord.y, size * (60.0f / CameraManager::GetMainCamera()->m_FOV));
		//nvgStrokeColor(s_nvg, nvgRGBA(color.r * 255, color.g * 255, color.b * 255, LineOpacity));
		//nvgStrokeWidth(s_nvg, LineThickness);
		//nvgStroke(s_nvg);
	}

	//--------Debug-Drawing--------
}
