#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

namespace Config {
	static constexpr float MovementSpeed{ 10.0f };
}

namespace CameraConfig {
	static constexpr float DefaultYaw{ -90.0f };
	static constexpr float DefaultPitch{ 0.0f };
	static constexpr float DefaultSensitivity{ 0.1f };
	// Zoom/FOV of the camera
	static constexpr float DefaultZoom{ 45.0f };
}

namespace FlyCamConfig {
	static constexpr float DefaultMovementSpeed{ 2.5f };
}

#endif
