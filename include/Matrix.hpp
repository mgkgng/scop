#pragma once

#include <iostream>
#include <cmath>

class Matrix {
private:
	float data[16];

public:
	Matrix() {
		this->setToIdentity();
	}
	~Matrix() {}

	void setProjection(float fov, float aspect_ratio, float near_plane, float far_plane) {
		this->setToIdentity();

		const float radian_fov = fov * static_cast<float>(M_PI) / 180.0f;
		const float tan_half_fov = tan(radian_fov / 2.0f);

		const float y_scale = 1.0f / tan_half_fov;
		const float x_scale = y_scale / aspect_ratio;
		const float frustum_length = far_plane - near_plane;

		data[0] = x_scale;
		data[5] = y_scale;
		data[10] = -((far_plane + near_plane) / frustum_length);
		data[11] = -1.0f;
		data[14] = -((2 * near_plane * far_plane) / frustum_length);
	}

	void setModel(float pos_x, float pos_y, float pos_z, /* Translation components */
        float rot_x, float rot_y, float rot_z, /* Rotation components (in radians) */ 
    	float sca_x, float sca_y, float sca_z) /* Scaling components */ 
	{
		data[0] = sca_x;
		data[5] = sca_y;
		data[10] = sca_z;

		data[0] *= cos(rot_z);
		data[1] = -sin(rot_z);
		data[4] = sin(rot_z);
		data[5] *= cos(rot_z);

		data[0] *= cos(rot_y);
		data[2] = sin(rot_y);
		data[8] = -sin(rot_y);
		data[10] *= cos(rot_y);

		data[5] *= cos(rot_x);
		data[6] = sin(rot_x);
		data[9] = -sin(rot_x);
		data[10] *= cos(rot_x);

		data[12] = pos_x;
		data[13] = pos_y;
		data[14] = pos_z;
	}

	void setView(const float *camera_pos, const float *target_pos, const float *up_vector) {
		float forward[3] = {
			target_pos[0] - camera_pos[0],
			target_pos[1] - camera_pos[1],
			target_pos[2] - camera_pos[2]
		};
		float forward_norm = sqrt(forward[0] * forward[0] + forward[1] * forward[1] + forward[2] * forward[2]);
		forward[0] /= forward_norm;
		forward[1] /= forward_norm;
		forward[2] /= forward_norm;

		float right[3] = {
			forward[1] * up_vector[2] - forward[2] * up_vector[1],
			forward[2] * up_vector[0] - forward[0] * up_vector[2],
			forward[0] * up_vector[1] - forward[1] * up_vector[0]
		};
		float right_norm = sqrt(right[0] * right[0] + right[1] * right[1] + right[2] * right[2]);
		right[0] /= right_norm;
		right[1] /= right_norm;
		right[2] /= right_norm;

		float up[3] = {
			right[1] * forward[2] - right[2] * forward[1],
			right[2] * forward[0] - right[0] * forward[2],
			right[0] * forward[1] - right[1] * forward[0]
		};

		setToIdentity();

		data[0] = right[0];
		data[4] = right[1];
		data[8] = right[2];

		data[1] = up[0];
		data[5] = up[1];
		data[9] = up[2];

		data[2] = -forward[0];
		data[6] = -forward[1];
		data[10] = -forward[2];

		data[12] = -right[0] * camera_pos[0] - right[1] * camera_pos[1] - right[2] * camera_pos[2];
		data[13] = -up[0] * camera_pos[0] - up[1] * camera_pos[1] - up[2] * camera_pos[2];
		data[14] = forward[0] * camera_pos[0] + forward[1] * camera_pos[1] + forward[2] * camera_pos[2];
	}

	void setToIdentity() {
		std::fill(std::begin(data), std::end(data), 0.0f);
		data[0] = data[5] = data[10] = data[15] = 1.0f;
	}

	void rotate(float angle, float x, float y, float z) {
		/* Rotation matrix components */ 
		float c = cos(-angle);
		float s = sin(-angle);
		float t = 1.0f - c;
		
		/* Normalize the rotation axis */
		float magnitude = sqrt(x * x + y * y + z * z);
		x /= magnitude;
		y /= magnitude;
		z /= magnitude;

		Matrix rotationMatrix;

		rotationMatrix.data[0] = c + x * x * t;
		rotationMatrix.data[1] = x * y * t - z * s;
		rotationMatrix.data[2] = x * z * t + y * s;

		rotationMatrix.data[4] = y * x * t + z * s;
		rotationMatrix.data[5] = c + y * y * t;
		rotationMatrix.data[6] = y * z * t - x * s;

		rotationMatrix.data[8] = z * x * t - y * s;
		rotationMatrix.data[9] = z * y * t + x * s;
		rotationMatrix.data[10] = c + z * z * t;

		/* Multiply current matrix by rotation matrix */
		*this = *this * rotationMatrix;
	}

	void translate(float x, float y, float z) {
		Matrix translationMatrix;

		/* Set the translation components of the translation matrix */ 
		translationMatrix.data[12] = x;
		translationMatrix.data[13] = y;
		translationMatrix.data[14] = z;

		/* Multiply current matrix by translation matrix */
		*this = *this * translationMatrix;
	}

	void move(float x, float y, float z) {
		this->data[12] += x;
		this->data[13] += y;
		this->data[14] += z;
	}

	void print() {
		for (size_t column = 0; column < 4; column++) {
			std::cout << "[ ";
			for (size_t row = 0; row < 4; row++) {
				std::cout << data[row + column * 4] << " ";
			}
			std::cout << "]\n";
		}
	}

	Matrix operator*(const Matrix& rhs) const {
        Matrix result;
        for (int row = 0; row < 4; ++row) {
            for (int col = 0; col < 4; ++col) {
                result.data[row + col * 4] = 0.0f;
                for (int k = 0; k < 4; ++k) {
                    result.data[row + col * 4] += this->data[row + k * 4] * rhs.data[k + col * 4];
                }
            }
        }
        return result;
    }

	const float *get_data() {
		return data;
	}
};
