#pragma once

#include "Window.h"

// To think about for the future, could implement some sort of per frame key caching to avoid redundant keydown polls to the input source
// Not sure about this yet, but usuability of the system should remain the same as this simple approach

class InputListener
{
public:
	virtual void OnKeyPress(int key, int scancode, int mods) {}
	virtual void OnKeyRepeat(int key, int scancode, int mods) {}
	virtual void OnKeyRelease(int key, int scancode, int mods) {}
	virtual void OnCharInput(unsigned int codepoint) {}
	virtual void OnMouseMove(double xpos, double ypos) {}
	virtual void OnMousePress(int button, int mods) {}
	virtual void OnMouseRelease(int button, int mods) {}
	virtual void OnMouseWheel(double x_offset, double y_offset) {}
};

class IInputSource
{
public:
	virtual bool GetKey(int keycode) = 0;
	virtual bool GetMouseButton(int button) = 0;
	virtual void GetMouse(double* xpos, double* ypos) = 0;
};

class WindowInputSource : public IInputSource
{
public:
	WindowInputSource(Window* window);

	bool GetKey(int keycode) override;
	bool GetMouseButton(int button) override;
	void GetMouse(double* xpos, double* ypos) override;
private:
	Window* m_Window;
};

class InputManager
{
public:
	InputManager(Window* w);
	~InputManager();

	void Update();

	bool GetKey(int keycode);
	bool GetMouseButton(int button);
	std::pair<double, double> GetMouse();
	double GetMouseX();
	double GetMouseY();

	template<typename T>
	void RegisterListener(InputListener* listener)
	{
		uint32_t id = TypeIdentifier::GetId<T>();
		m_Listeners[id].push_back(listener);
	}

	template<typename T>
	void DeregisterListener(InputListener* listener)
	{
		uint32_t id = TypeIdentifier::GetId<T>();
		m_Listeners[id].erase(remove(m_Listeners[id].begin(), m_Listeners[id].end(), listener), m_Listeners[id].end());
	}

	template<typename T>
	void RegisterDebugListener(InputListener* listener)
	{
		uint32_t id = TypeIdentifier::GetId<T>();
		m_DebugListeners[id].push_back(listener);
	}

	template<typename T>
	void DeregisterDebugListener(InputListener* listener)
	{
		uint32_t id = TypeIdentifier::GetId<T>();
		m_DebugListeners[id].erase(remove(m_Listeners[id].begin(), m_Listeners[id].end(), listener), m_Listeners[id].end());
	}

private:
	IInputSource* m_InputSource;

	double m_MouseX;
	double m_MouseY;

	bool m_IsDebugMode = false;
	
	// Replace with custom allocater eventually to control heap allocations
	// Listeners are not modified every frame so this is alright for now
	std::unordered_map<uint32_t, std::vector<InputListener*>> m_Listeners;
	std::unordered_map<uint32_t, std::vector<InputListener*>> m_DebugListeners;
};