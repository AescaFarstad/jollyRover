#include <GameView.h>


GameView::GameView(SDL_Window* window, SDL_Renderer* renderer, Prototypes* prototypes)
{
	this->window = window;
	this->renderer = renderer;
	this->prototypes = prototypes;

	isInitialized = false;
}


GameView::~GameView()
{
}

void GameView::render(GameState* state, RouteInput* routeInput)
{
	if (!isInitialized)
		init();

	this->state = state;
	this->routeInput = routeInput;

	drawPlayers();
	drawObstacles();
	drawInput();
	drawCars();
}

void GameView::init()
{
	SDL_SetWindowSize(window, prototypes->variables.fieldWidth, prototypes->variables.fieldHeight);
	isInitialized = true;
}

void GameView::setColor(uint32_t color)
{
	SDL_SetRenderDrawColor(renderer, (color >> 16) & 0xff, (color >> 8) & 0xff, color & 0xff, 0xFF);
}

void GameView::drawPlayers()
{
	uint32_t colors[10] = {
		0x000000,
		0xff0000,
		0x00ff00,
		0x0000ff,
		0xffff00,
		0x00ffff,
		0xff00ff,
		0x777777,
		0x0077ff,
		0xff7700
	};

	for (size_t i = 0; i < state->players.size(); i++)
	{
		setColor(colors[i]);

		SDL_Rect rect;
		rect.x = state->players[i].x + 200;
		rect.y = state->players[i].y + 200;
		rect.w = 5;
		rect.h = 5;
		SDL_RenderFillRect(renderer, &rect);
	}
}

void GameView::drawObstacles()
{
	uint32_t obstacleColor = 0x004477;

	setColor(obstacleColor);

	for (auto &obstacle : prototypes->obstacles)
	{
		size_t count = obstacle.vertices.size() + 1;
		SDL_Point* points = new SDL_Point[count];
		for (size_t i = 0; i < count - 1; i++)
		{
			points[i].x = obstacle.vertices[i].x;
			points[i].y = obstacle.vertices[i].y;
		}
		points[count - 1].x = obstacle.vertices[0].x;
		points[count - 1].y = obstacle.vertices[0].y;
		SDL_RenderDrawLines(renderer, points, count);
		delete[] points;
	}
}

void GameView::drawInput()
{
	uint32_t validColor = 0x00ff00;
	uint32_t invalidColor = 0xff0000;
	uint32_t lastColor = 0;
	int rectSize = 6;

	for (size_t i = 1; i < routeInput->route.size(); i++)
	{
		bool isValidEdge = routeInput->route[i].isValid;
		uint32_t newColor = isValidEdge ? validColor : invalidColor;
		if (lastColor != newColor)
		{
			setColor(newColor);
			lastColor = newColor;
		}
		SDL_RenderDrawLine(renderer,
			(int)routeInput->route[i - 1].x,
			(int)routeInput->route[i - 1].y,
			(int)routeInput->route[i].x,
			(int)routeInput->route[i].y
		);

		//S::log.add(std::to_string(i) + " draw " + routeInput->route[i - 1].toString() + " -> " + routeInput->route[i].toString());

		newColor = routeInput->route[i].isValid ? validColor : invalidColor;
		if (lastColor != newColor)
		{
			setColor(newColor);
			lastColor = newColor;
		}

		SDL_Rect rect;
		rect.x = routeInput->route[i].x - rectSize / 2;
		rect.y = routeInput->route[i].y - rectSize / 2;
		rect.w = rectSize;
		rect.h = rectSize;
		SDL_RenderDrawRect(renderer, &rect);
	}
}

void GameView::drawCars()
{
	uint32_t color = 0x0000ff;
	int rectSize = 4;
	int carSize = 8;
	setColor(color);

	for (PlayerTest &player : state->players)
	{
		for (CarRide &ride : player.activeCars)
		{
			for (size_t i = ride.routeIndex + 1; i < ride.route.size(); i++)
			{
				SDL_RenderDrawLine(renderer,
					(int)ride.route[i - 1].x,
					(int)ride.route[i - 1].y,
					(int)ride.route[i].x,
					(int)ride.route[i].y
				);

				SDL_Rect rect;
				rect.x = ride.route[i].x - rectSize / 2;
				rect.y = ride.route[i].y - rectSize / 2;
				rect.w = rectSize;
				rect.h = rectSize;
				SDL_RenderDrawRect(renderer, &rect);
			}

			Point carLocation = ride.route[ride.routeIndex + 1].subtract(ride.route[ride.routeIndex]);
			carLocation.scaleTo(ride.progress * carLocation.getLength());
			carLocation.add(ride.route[ride.routeIndex]);

			SDL_Rect carRect;
			carRect.x = carLocation.x - carSize / 2;
			carRect.y = carLocation.y - carSize / 2;
			carRect.w = carSize;
			carRect.h = carSize;
			SDL_RenderFillRect(renderer, &carRect);
		}
	}
}
