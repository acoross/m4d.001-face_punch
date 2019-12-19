#pragma once

#include "entityx/System.h"
#include "SharedContext.h"

struct MoveRpc;

// windows 의 raw input 을 확인해서 게임 월드에 맞는 명령으로 변환.
  // 명령 종류가 매우 많아질 경우 input 을 event 로 다루어,
  // 각 system 이 알아서 subscribe 하여 처리하도록 한다.
  // -> 의존성 분리
class InputHandleSystem : public entityx::System<InputHandleSystem>
{
public:
	void update(entityx::EntityManager& es, entityx::EventManager& events, entityx::TimeDelta dt) override;

	SharedContext* m_sharedContext = nullptr;
};