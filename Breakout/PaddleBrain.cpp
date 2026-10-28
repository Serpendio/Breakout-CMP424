#include "PaddleBrain.h"
#include "json.hpp"
#include "httplib.h"
#include <string>
#include <chrono>
#include <format>
using namespace std::string_literals;

PaddleBrain::PaddleBrain(sf::RenderWindow* window)
	: Paddle(window)
{
	client = std::make_unique<httplib::Client>("http://localhost:11434/");
}

PaddleBrain::~PaddleBrain()
{

}

void PaddleBrain::SetGameState(nlohmann::json gameState)
{
	this->gameState = gameState;
}

void PaddleBrain::handleControls(float dt)
{
	if (dir < 0) moveLeft(dt);
	else if (dir > 0) moveRight(dt);

	if (thinking)
		return;

    std::string jsonData = std::format(R"(
        {{
			"model": "d1",
            "state": {{ 
						"info": "You are playing breakout",
						"boardstate": {}
					}},
            "questions": {{
              "q":  {{"type": "noul", "instructions": "should you go right?"}}
            }}
        }})", gameState.dump());

	thinking = true;
	std::thread worker([this, jsonData]()
	{
		auto start = std::chrono::steady_clock::now();
		auto res = client->Post("/v1/systemone", jsonData,
			"application/json");
		auto end = std::chrono::steady_clock::now();
		auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

		if (!res)
		{
			thinking = false;
			return;
		}

		auto j = nlohmann::json::parse(res->body);
		if (res->status == 200)
		{
			std::cout << j << std::endl;
			std::cout << j["answers"]["q"] << std::endl;
			std::cout << "Took " << ms.count() << " ms\n";
			this->dir = j["answers"]["q"]["noul"].get<float>() - .5f;
		}
		else
		{
			std::cout << "Error: " << j << std::endl;
			std::cout << "Input was: " << jsonData << std::endl;
		}

		thinking = false;
	});


	worker.detach();
}
