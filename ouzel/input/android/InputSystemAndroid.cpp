// Copyright 2015-2018 Elviss Strazdins. All rights reserved.

#include <unordered_map>
#include <android/keycodes.h>
#include <android/input.h>
#include "InputSystemAndroid.hpp"
#include "GamepadDeviceAndroid.hpp"

namespace ouzel
{
    namespace input
    {
        static const std::unordered_map<jint, Keyboard::Key> keyMap = {
            {AKEYCODE_0, Keyboard::Key::NUM_0},
            {AKEYCODE_1, Keyboard::Key::NUM_1},
            {AKEYCODE_2, Keyboard::Key::NUM_2},
            {AKEYCODE_3, Keyboard::Key::NUM_3},
            {AKEYCODE_4, Keyboard::Key::NUM_4},
            {AKEYCODE_5, Keyboard::Key::NUM_5},
            {AKEYCODE_6, Keyboard::Key::NUM_6},
            {AKEYCODE_7, Keyboard::Key::NUM_7},
            {AKEYCODE_8, Keyboard::Key::NUM_8},
            {AKEYCODE_9, Keyboard::Key::NUM_9},

            {AKEYCODE_DPAD_UP, Keyboard::Key::UP},
            {AKEYCODE_DPAD_DOWN, Keyboard::Key::DOWN},
            {AKEYCODE_DPAD_LEFT, Keyboard::Key::LEFT},
            {AKEYCODE_DPAD_RIGHT, Keyboard::Key::RIGHT},

            {AKEYCODE_A, Keyboard::Key::A},
            {AKEYCODE_B, Keyboard::Key::B},
            {AKEYCODE_C, Keyboard::Key::C},
            {AKEYCODE_D, Keyboard::Key::D},
            {AKEYCODE_E, Keyboard::Key::E},
            {AKEYCODE_F, Keyboard::Key::F},
            {AKEYCODE_G, Keyboard::Key::G},
            {AKEYCODE_H, Keyboard::Key::H},
            {AKEYCODE_I, Keyboard::Key::I},
            {AKEYCODE_J, Keyboard::Key::J},
            {AKEYCODE_K, Keyboard::Key::K},
            {AKEYCODE_L, Keyboard::Key::L},
            {AKEYCODE_M, Keyboard::Key::M},
            {AKEYCODE_N, Keyboard::Key::N},
            {AKEYCODE_O, Keyboard::Key::O},
            {AKEYCODE_P, Keyboard::Key::P},
            {AKEYCODE_Q, Keyboard::Key::Q},
            {AKEYCODE_R, Keyboard::Key::R},
            {AKEYCODE_S, Keyboard::Key::S},
            {AKEYCODE_T, Keyboard::Key::T},
            {AKEYCODE_U, Keyboard::Key::U},
            {AKEYCODE_V, Keyboard::Key::V},
            {AKEYCODE_W, Keyboard::Key::W},
            {AKEYCODE_X, Keyboard::Key::X},
            {AKEYCODE_Y, Keyboard::Key::Y},
            {AKEYCODE_Z, Keyboard::Key::Z},

            {AKEYCODE_SEMICOLON, Keyboard::Key::SEMICOLON},
            {AKEYCODE_EQUALS, Keyboard::Key::PLUS},
            {AKEYCODE_COMMA, Keyboard::Key::COMMA},
            {AKEYCODE_PERIOD, Keyboard::Key::PERIOD},
            {AKEYCODE_SLASH, Keyboard::Key::SLASH},
            {AKEYCODE_GRAVE, Keyboard::Key::GRAVE},
            {AKEYCODE_LEFT_BRACKET, Keyboard::Key::LEFT_BRACKET},
            {AKEYCODE_BACKSLASH, Keyboard::Key::BACKSLASH},
            {AKEYCODE_RIGHT_BRACKET, Keyboard::Key::RIGHT_BRACKET},
            {AKEYCODE_APOSTROPHE, Keyboard::Key::QUOTE},
            {AKEYCODE_SPACE, Keyboard::Key::SPACE},

            {AKEYCODE_DEL, Keyboard::Key::BACKSPACE},
            {AKEYCODE_TAB, Keyboard::Key::TAB},
            {AKEYCODE_ENTER, Keyboard::Key::RETURN},
            {AKEYCODE_BREAK, Keyboard::Key::PAUSE},

            {AKEYCODE_BACK, Keyboard::Key::ESCAPE},
            {AKEYCODE_PAGE_UP, Keyboard::Key::PRIOR},
            {AKEYCODE_PAGE_DOWN, Keyboard::Key::NEXT},

            {AKEYCODE_FORWARD_DEL, Keyboard::Key::DEL},

            {AKEYCODE_STAR, Keyboard::Key::MULTIPLY},
            {AKEYCODE_PLUS, Keyboard::Key::PLUS},
            {AKEYCODE_MINUS, Keyboard::Key::MINUS},

            {AKEYCODE_F1, Keyboard::Key::F1},
            {AKEYCODE_F2, Keyboard::Key::F2},
            {AKEYCODE_F3, Keyboard::Key::F3},
            {AKEYCODE_F4, Keyboard::Key::F4},
            {AKEYCODE_F5, Keyboard::Key::F5},
            {AKEYCODE_F6, Keyboard::Key::F6},
            {AKEYCODE_F7, Keyboard::Key::F7},
            {AKEYCODE_F8, Keyboard::Key::F8},
            {AKEYCODE_F9, Keyboard::Key::F9},
            {AKEYCODE_F10, Keyboard::Key::F10},
            {AKEYCODE_F11, Keyboard::Key::F11},
            {AKEYCODE_F12, Keyboard::Key::F12},

            {AKEYCODE_SHIFT_LEFT, Keyboard::Key::LEFT_SHIFT},
            {AKEYCODE_SHIFT_RIGHT, Keyboard::Key::RIGHT_SHIFT},
            {AKEYCODE_CTRL_LEFT, Keyboard::Key::LEFT_CONTROL},
            {AKEYCODE_CTRL_RIGHT, Keyboard::Key::RIGHT_CONTROL},
            {AKEYCODE_ALT_LEFT, Keyboard::Key::LEFT_ALT},
            {AKEYCODE_ALT_RIGHT, Keyboard::Key::RIGHT_ALT},
            {AKEYCODE_META_LEFT, Keyboard::Key::LEFT_SUPER},
            {AKEYCODE_META_RIGHT, Keyboard::Key::RIGHT_SUPER}
        };

        Keyboard::Key InputSystemAndroid::convertKeyCode(jint keyCode)
        {
            auto i = keyMap.find(keyCode);

            if (i != keyMap.end())
                return i->second;
            else
                return Keyboard::Key::NONE;
        }

        InputSystemAndroid::InputSystemAndroid()
        {
            Event keyboardConnectEvent;
            keyboardConnectEvent.type = Event::Type::DEVICE_CONNECT;
            std::unique_ptr<KeyboardDevice> keyboard(new KeyboardDevice(*this, ++lastDeviceId));
            keyboardConnectEvent.deviceId = keyboard->getId();
            keyboardConnectEvent.deviceType = Controller::Type::KEYBOARD;
            keyboardDevice = keyboard.get();
            inputDevices.insert(std::make_pair(keyboard->getId(), std::move(keyboard)));
            addEvent(keyboardConnectEvent);

            Event mouseConnectEvent;
            mouseConnectEvent.type = Event::Type::DEVICE_CONNECT;
            std::unique_ptr<MouseDeviceAndroid> mouse(new MouseDeviceAndroid(*this, ++lastDeviceId));
            mouseConnectEvent.deviceId = mouse->getId();
            mouseConnectEvent.deviceType = Controller::Type::MOUSE;
            mouseDevice = mouse.get();
            inputDevices.insert(std::make_pair(mouse->getId(), std::move(mouse)));
            addEvent(mouseConnectEvent);

            Event touchpadConnectEvent;
            touchpadConnectEvent.type = Event::Type::DEVICE_CONNECT;
            std::unique_ptr<TouchpadDevice> touchpad(new TouchpadDevice(*this, ++lastDeviceId));
            touchpadConnectEvent.deviceId = touchpad->getId();
            touchpadConnectEvent.deviceType = Controller::Type::TOUCHPAD;
            touchpadDevice = touchpad.get();
            inputDevices.insert(std::make_pair(touchpad->getId(), std::move(touchpad)));
            addEvent(touchpadConnectEvent);
        }

        void InputSystemAndroid::executeCommand(Command command)
        {
            switch (command.type)
            {
            case Command::Type::START_DEVICE_DISCOVERY:
                break;
            case Command::Type::STOP_DEVICE_DISCOVERY:
                break;
            case Command::Type::SET_ABSOLUTE_DPAD_VALUES:
            {
                break;
            }
            case Command::Type::SET_PLAYER_INDEX:
            {
                break;
            }
            case Command::Type::SET_VIBRATION:
            {
                break;
            }
            case Command::Type::SET_POSITON:
            {
                break;
            }
            case Command::Type::SET_CURSOR:
            {
                break;
            }
            case Command::Type::SET_CURSOR_VISIBLE:
            {
                break;
            }
            case Command::Type::SHOW_VIRTUAL_KEYBOARD:
                break;
            case Command::Type::HIDE_VIRTUAL_KEYBOARD:
                break;
            }
        }
    } // namespace input
} // namespace ouzel
