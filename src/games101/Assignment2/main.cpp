// clang-format off
#include <iostream>
#include <opencv2/opencv.hpp>
#include "rasterizer.hpp"
#include "global.hpp"
#include "Triangle.hpp"

constexpr double MY_PI = 3.1415926;

Eigen::Matrix4f get_view_matrix(Eigen::Vector3f eye_pos)
{
    Eigen::Matrix4f view = Eigen::Matrix4f::Identity();

    Eigen::Matrix4f translate;
    translate << 1,0,0,-eye_pos[0],
                 0,1,0,-eye_pos[1],
                 0,0,1,-eye_pos[2],
                 0,0,0,1;

    view = translate*view;

    return view;
}

inline double DEG2RAD(double deg) { return deg * MY_PI / 180; }

Eigen::Matrix4f get_model_matrix(float rotation_angle)
{
    Eigen::Matrix4f model = Eigen::Matrix4f::Identity();
    // return model;
    
    Eigen::Matrix4f rotate;
    float radian = rotation_angle/180*MY_PI;
    rotate << cos(radian), -sin(radian), 0, 0,
              sin(radian), cos(radian), 0, 0,
              0, 0, 1, 0,
              0, 0, 0, 1;

    
    model = rotate * model;
    return model;
        // TODO: Implement this function
    // Create the model matrix for rotating the triangle around the Z axis.
    // Eigen::Matrix4f trans;
    // float radian = DEG2RAD(rotation_angle);
    // trans << std::cos(radian), -std::sin(radian), 0, 0,
    //     std::sin(radian), std::cos(radian), 0, 0,
    //     0, 0, 1, 0,
    //     0, 0, 0, 1;
    // // Then return it.
    // model = trans * model;

    return model;
}

Eigen::Matrix4f get_projection_matrix(float eye_fov, float aspect_ratio, float zNear, float zFar)
{
    Eigen::Matrix4f projection;
    // Eigen::Matrix4f projection = Eigen::Matrix4f::Identity();
    // float t = tan(eye_fov/180*MY_PI)*abs(zNear);
    float h = tan(eye_fov/2)*abs(zNear);
    float w = aspect_ratio*h;
    Eigen::Matrix4f persp;
    persp << -2*zNear/w, 0, 0, 0,
             0, -2*zNear/h, 0, 0,
             0, 0, (zFar+zNear)/(zFar-zNear), 2*zFar*zNear/(zFar-zNear), 
             0, 0, 1, 0;
    projection = persp * projection;
    return projection;
}
/*Eigen::Matrix4f get_projection_matrix(float eye_fov, float aspect_ratio, float zNear, float zFar)
{
    // TODO: Copy-paste your implementation from the previous assignment.
    Eigen::Matrix4f projection;

    Eigen::Matrix4f proj, ortho;
    proj << zNear, 0, 0, 0,
            0, zNear, 0, 0,
            0, 0, zNear + zFar, -zNear * zFar,
            0, 0, 1, 0;//透视投影矩阵

    double w, h, z;
    h = zNear * tan(eye_fov / 2);
    w = h * aspect_ratio;
    z = zFar - zNear;

    ortho << 2 / w, 0, 0, 0,
             0, 2 / h, 0, 0,
             0, 0, 2 / z, - (zFar + zNear) / 2,
             0, 0, 0, 1;//正交投影矩阵
    projection = ortho * proj * projection;

    return projection;
}*/

int main(int argc, const char** argv)
{
    float angle = 0;
    bool command_line = false;
    std::string filename = "output.png";

    if (argc == 2)
    {
        command_line = true;
        filename = std::string(argv[1]);
    }

    rst::rasterizer r(700, 700);

    Eigen::Vector3f eye_pos = {0,0,5};


    std::vector<Eigen::Vector3f> pos
            {
                    {2, 0, -2},
                    {0, 2, -2},
                    {-2, 0, -2},
                    {3.5, -1, -5},
                    {2.5, 1.5, -5},
                    {-1, 0.5, -5}
            };

    std::vector<Eigen::Vector3i> ind
            {
                    {0, 1, 2},
                    {3, 4, 5}
            };

    std::vector<Eigen::Vector3f> cols
            {
                    {217.0, 238.0, 185.0},
                    {217.0, 238.0, 185.0},
                    {217.0, 238.0, 185.0},
                    {185.0, 217.0, 238.0},
                    {185.0, 217.0, 238.0},
                    {185.0, 217.0, 238.0}
            };

    auto pos_id = r.load_positions(pos);
    auto ind_id = r.load_indices(ind);
    auto col_id = r.load_colors(cols);

    int key = 0;
    int frame_count = 0;

    if (command_line)
    {
        r.clear(rst::Buffers::Color | rst::Buffers::Depth);

        r.set_model(get_model_matrix(angle));
        r.set_view(get_view_matrix(eye_pos));
        r.set_projection(get_projection_matrix(45, 1, 0.1, 50));

        r.draw(pos_id, ind_id, col_id, rst::Primitive::Triangle);
        cv::Mat image(700, 700, CV_32FC3, r.frame_buffer().data());
        image.convertTo(image, CV_8UC3, 1.0f);
        cv::cvtColor(image, image, cv::COLOR_RGB2BGR);

        cv::imwrite(filename, image);

        return 0;
    }

    while(key != 27)
    {
        r.clear(rst::Buffers::Color | rst::Buffers::Depth);

        r.set_model(get_model_matrix(angle));
        r.set_view(get_view_matrix(eye_pos));
        r.set_projection(get_projection_matrix(45, 1, 0.1, 50));

        r.draw(pos_id, ind_id, col_id, rst::Primitive::Triangle);

        cv::Mat image(700, 700, CV_32FC3, r.frame_buffer().data());
        image.convertTo(image, CV_8UC3, 1.0f);
        cv::cvtColor(image, image, cv::COLOR_RGB2BGR);
        cv::imshow("image", image);
        key = cv::waitKey(10);

        std::cout << "frame count: " << frame_count++ << '\n';
    }

    return 0;
}
// clang-format on