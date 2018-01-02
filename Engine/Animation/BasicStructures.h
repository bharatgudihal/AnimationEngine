#pragma once

#include<Externals/glm/Includes.h>

namespace Engine {
	namespace Animation {
		
		struct Joint {
			//Inverse bind transform
			glm::mat4x3 invBindPose;
			//Joint name
			const char* name;
			//Parent index
			uint8_t parent;
		};

		struct Skeleton {
			uint32_t jointCount;
			Joint* joints;
		};

		struct JointPose {
			glm::quat rotation;
			glm::vec3 position;
			float scale;
		};

		struct SkeletonPose {
			Skeleton* skeleton;
			JointPose* jointPoses;
		};
	}
}