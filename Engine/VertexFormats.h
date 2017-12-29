#pragma once

namespace Engine {
	namespace Graphics {
		namespace VertexFormat {
			
			struct Color {
				float r, g, b, a;
			};

			struct Sprite {
				
				struct {
					float x, y, z;
				}position;

				struct{
					float r, g, b;
				}color;

				struct {
					float u, v;
				}UV;

			};

			struct Mesh {

				struct {
					float x, y, z;
				}position;

				struct {
					float r, g, b;
				}color;

				struct {
					float u, v;
				}UV;

				struct {
					float x, y, z;
				}normal;

			};

		}
	}
 }