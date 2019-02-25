#include <RouteInput.h>


RouteInput::RouteInput()
{
	isInputActive = false;
	isLoaded = false;
}

RouteInput::~RouteInput()
{
}

void RouteInput::load(GameState* state, Prototypes* prototypes, std::function<void(std::unique_ptr<InputMessage>)> sendInputFunc)
{
	this->state = state;
	this->prototypes = prototypes;
	this->sendInputFunc = sendInputFunc;
	isLoaded = true;
}

void RouteInput::onMouseDown(SDL_MouseButtonEvent* event)
{
	if (!isLoaded)
		return;
	if (event->x < prototypes->variables.fieldWidth && event->x > 0 &&
		event->y < prototypes->variables.fieldHeight && event->y > 0)
	{
		Point touch(event->x, event->y);

		route.push_back(RoutePoint(event->x, prototypes->variables.fieldHeight));

		buildPath(touch);

		isInputActive = true;
	}
}

void RouteInput::onMouseUp(SDL_MouseButtonEvent* event)
{
	if (!isLoaded)
		return;

	isInputActive = false;
	if (route.size() < (size_t)prototypes->variables.minRouteSteps)
	{
		reset();
		return;
	}

	RoutePoint finish(event->x, prototypes->variables.fieldHeight);
	if (!isAnglePositive(finish))
	{
		reset();
		return;
	}

	buildPath(finish);
	route.push_back(finish);

	bool isCompletelyValid = true;

	for (RoutePoint &p : route)
	{
		if (!p.isValid)
		{
			isCompletelyValid = false;
			break;
		}
	}

	if (isCompletelyValid)
	{
		std::vector<Point> routeAsPoints;
		for (RoutePoint &rp : route)
		{
			routeAsPoints.emplace_back(rp.x, rp.y);
		}
		sendInputFunc(std::make_unique<InputRouteMessage>(routeAsPoints));
		S::log.add("Route input is valid!", { LOG_TAGS::INPUT_ });
	}
	reset();

}

void RouteInput::onMouseMove(SDL_MouseMotionEvent* event)
{
	if (!isInputActive || !isLoaded)
		return;

	if (event->x < prototypes->variables.fieldWidth && event->x > 0 &&
		event->y < prototypes->variables.fieldHeight && event->y > 0)
	{
		Point touch(event->x, event->y);

		while (route.size() > 1 && !isAnglePositive(touch))
		{
			//S::log.add("-point:" + route.back().toString(), { LOG_TAGS::INPUT_ });
			route.pop_back();
		}

		buildPath(touch);
	}
}

void RouteInput::buildPath(Point &target)
{
	RoutePoint last = route.back();
	Point step = target.subtract(last);
	step.scaleTo(prototypes->variables.routeStepSize);

	while (last.distanceTo(target) > prototypes->variables.routeStepSize)
	{
		route.emplace_back();
		RoutePoint &newPoint = route.back();

		newPoint.x = last.x + step.x;
		newPoint.y = last.y + step.y;

		newPoint.isValid = GameLogic::testEdgeIsValid(last, newPoint, prototypes->obstacles);
		std::cout << "+point:" + newPoint.toString() + " " + (newPoint.isValid ? "true" : "false") << "\n";
		//S::log.add("+point:" + newPoint.toString() + " " + (newPoint.isValid ? "true" : "false"), { LOG_TAGS::INPUT_ });

		last = newPoint;
	}
}


bool RouteInput::isAnglePositive(Point &target)
{
	auto iter = --route.end();

	Point ongoingVector = (*iter).subtract(*(iter - 1));
	Point finishingVector = (target).subtract(*iter);

	float angleDelta = FMath::angleDelta(ongoingVector.asAngle(), finishingVector.asAngle());
	return fabsf(angleDelta) < prototypes->variables.stepAngleWindow / 2;
}



void RouteInput::reset()
{
	route.clear();
	isInputActive = false;
}


RoutePoint::RoutePoint()
{
}

RoutePoint::RoutePoint(float x, float y) : Point(x, y) { isValid = true; }

RoutePoint::~RoutePoint()
{
}
