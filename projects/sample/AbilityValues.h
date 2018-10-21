#pragma once

namespace AbilityValues {
	namespace Pyromancer {
		const float IgniteCooldown = 5.f;
		const float FirestreamChargeSeconds = 3.f;
		const float FirestreamRechargeRate = 0.4f;

		// TODO: Handle this over lower frame rates better
		const float FirestreamCooldown = 0.02f;
		const int FirestreamsPerShot = 2;
		const float FirestreamSpeed = 6.f;
		const float FirestreamSpreadRadians = .5f;
	}
}
