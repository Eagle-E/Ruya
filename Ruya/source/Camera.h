#ifndef CAMERA_H
#define CAMERA_H

#define _USE_MATH_DEFINES
#include <glm/glm.hpp>

#define M_PI       3.14159265358979323846   // pi

namespace ruya
{
    class Camera
    {
    public:
        Camera();

        void update_angle(float dx, float dy);

        glm::vec3 position() const;

        float get_horizontal_angle() const;
        float get_vertical_angle() const;
        glm::mat4 get_view_matrix() const;

        void go_forward();
        void go_backward();
        void go_right();
        void go_left();
        void go_up();
        void go_down();

        glm::vec3 get_cam_front() const;

    private:
        void calculate_movement(float& dx, float& dy);

        glm::vec3 mCamPos;  // cam position on world
        glm::vec3 mCamUp;   // necessary vector for the calculation of the camview
        glm::vec3 mCamFront; // vector pointing the direction the cam is faced
        float mHorizontalAngle, mVerticalAngle;
        float mCamSpeed = 0.01f;

        // camera constants
        const short CAM_START_HIGHT = 6;
        const double MAX_VERTICAL_ANGLE = M_PI/3, MIN_VERTICAL_ANGLE = -MAX_VERTICAL_ANGLE; //radians
        const double MAX_HORIZONTAL_ANGLE = 2*M_PI, MIN_HORIZONTAL_ANGLE = -MAX_HORIZONTAL_ANGLE; //radians
        const unsigned int SENSITIVITY = 2000; //higher number -> lower sensitivity
    };

}

#endif // CAMERA_H
