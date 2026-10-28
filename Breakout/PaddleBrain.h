#include "Paddle.h"
#include "json.hpp"

namespace httplib
{
	class Client;
};

#pragma once
class PaddleBrain : public Paddle
{
public:
	PaddleBrain(sf::RenderWindow* window);
	~PaddleBrain();
	void SetGameState(nlohmann::json gameState);

protected:
	void handleControls(float dt) override;

private:
	std::unique_ptr<httplib::Client> client;

	bool thinking;
	float dir = 0;
	nlohmann::json gameState;
};

