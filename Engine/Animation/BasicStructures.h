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
			//Skeleton
			Skeleton* skeleton;
			//Local joint poses
			JointPose* jointPoses;
			//Global joint poses
			glm::mat4* globalPoses;
		};

		struct AnimationSample {
			JointPose* jointPoses;
		};

		struct AnimationClip {
			Skeleton* skeleton;
			float framesPerSecond;
			uint32_t frameCount;
			AnimationSample* samples;
			bool isLooping;
		};

		struct SkinnedVertex {
			glm::vec3 position;
			glm::vec3 normal;
			float u, v;
			uint8_t jointIndex[4];
			float jointWeight[3];
		};
	}
}