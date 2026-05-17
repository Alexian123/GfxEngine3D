#pragma once


class InputManager
{
public:
	InputManager(const InputManager&) = delete;
	InputManager& operator=(const InputManager&) = delete;

	static InputManager& getInstance();

	void processInput() const;

protected:
	InputManager();
};

