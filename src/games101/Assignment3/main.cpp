#include <cmath>
#include <iostream>
#include <opencv2/opencv.hpp>

#include "global.hpp"
#include "rasterizer.hpp"
#include "Triangle.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "OBJ_Loader.h"

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

Eigen::Matrix4f get_model_matrix(float angle)
{
    Eigen::Matrix4f rotation;
    angle = angle * MY_PI / 180.f;
    rotation << cos(angle), 0, sin(angle), 0,
                0, 1, 0, 0,
                -sin(angle), 0, cos(angle), 0,
                0, 0, 0, 1;

    Eigen::Matrix4f scale;
    scale << 2.5, 0, 0, 0,
              0, 2.5, 0, 0,
              0, 0, 2.5, 0,
              0, 0, 0, 1;

    Eigen::Matrix4f translate;
    translate << 1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1;

    return translate * rotation * scale;
}

Eigen::Matrix4f get_projection_matrix(float eye_fov, float aspect_ratio, float zNear, float zFar)
{
    // Use the same projection matrix from the previous assignments
    Eigen::Matrix4f projection;
    float h = tan(eye_fov/2)*abs(zNear);
    float w = aspect_ratio*h;
    Eigen::Matrix4f persp;
    // persp << -2*zNear/w, 0, 0, 0,
    //          0, -2*zNear/h, 0, 0,
    //          0, 0, (zFar+zNear)/(zFar-zNear), 2*zFar*zNear/(zFar-zNear), 
    //          0, 0, 1, 0;
    persp << 2*zNear/w, 0, 0, 0,
             0, 2*zNear/h, 0, 0,
             0, 0, (zFar+zNear)/(zNear-zFar), 2*zFar*zNear/(zNear-zFar), 
             0, 0, -1, 0;
    projection = persp * projection;
    return projection;
}

Eigen::Vector3f vertex_shader(const vertex_shader_payload& payload)
{
    return payload.position;
}

/**
 * 此处用法线向量对图片进行染色 
 */
Eigen::Vector3f normal_fragment_shader(const fragment_shader_payload& payload)
{
    Eigen::Vector3f return_color = (payload.normal.head<3>().normalized() + Eigen::Vector3f(1.0f, 1.0f, 1.0f)) / 2.f;
    // printf("%f %f\t", return_color.x(), payload.color.x());
    Eigen::Vector3f result;
    result << return_color.x() * 255, return_color.y() * 255, return_color.z() * 255;
    return result;
}

static Eigen::Vector3f reflect(const Eigen::Vector3f& vec, const Eigen::Vector3f& axis)
{
    auto costheta = vec.dot(axis);
    return (2 * costheta * axis - vec).normalized();
}

struct light
{
    Eigen::Vector3f position;
    Eigen::Vector3f intensity;
};

Eigen::Vector3f texture_fragment_shader(const fragment_shader_payload& payload)
{
    Eigen::Vector3f return_color = {0, 0, 0};
    if (payload.texture)
    {
        // TODO: Get the texture value at the texture coordinates of the current fragment
	return_color = payload.texture->getColor(payload.tex_coords[0], payload.tex_coords[1]);
    }
    Eigen::Vector3f texture_color;
    texture_color << return_color.x(), return_color.y(), return_color.z();

    Eigen::Vector3f ka = Eigen::Vector3f(0.005, 0.005, 0.005);
    Eigen::Vector3f kd = texture_color / 255.f;
    Eigen::Vector3f ks = Eigen::Vector3f(0.7937, 0.7937, 0.7937);

    auto l1 = light{{20, 20, 20}, {500, 500, 500}};
    auto l2 = light{{-20, 20, 0}, {500, 500, 500}};

    std::vector<light> lights = {l1, l2};
    Eigen::Vector3f amb_light_intensity{10, 10, 10};
    Eigen::Vector3f eye_pos{0, 0, 10};

    float p = 150;

    Eigen::Vector3f color = texture_color;
    Eigen::Vector3f point = payload.view_pos;
    Eigen::Vector3f normal = payload.normal;

    Eigen::Vector3f result_color = {0, 0, 0};

    for (auto& light : lights)
    {
        // For each light source in the code, calculate what the *ambient*, *diffuse*, and *specular* 
        // components are. Then, accumulate that result on the *result_color* object.
	Vector3f light_dir = light.position - point;  // 光照方向
        Vector3f view_dir = (eye_pos - point).normalized(); // 视线方向
        float r2 = pow(light_dir.norm(), 2.f);  // 点与光源距离
        light_dir.normalize();  // 光照向量归一化 Tips: 如果在未归一的情况下计算半程向量，会影响到高光
        Vector3f half_vec = (light_dir + view_dir).normalized();  // 半程向量

        auto intensity = light.intensity / r2;  // 光照强度(与距离的平方成反比)
        Vector3f ambient = ka.cwiseProduct(amb_light_intensity);   // 环境光
        Vector3f diffuse = kd.cwiseProduct(intensity)* std::max(0.f, normal.dot(light_dir));  // 漫反射
        Vector3f specular = ks.cwiseProduct(intensity) * pow(std::max(0.f, normal.dot(half_vec)), p);  // 高光
        result_color += ambient + diffuse + specular;  // 光照累积


    }

    return result_color * 255.f;
}

Eigen::Vector3f phong_fragment_shader(const fragment_shader_payload& payload)
{
    Eigen::Vector3f ka = Eigen::Vector3f(0.005, 0.005, 0.005);
    Eigen::Vector3f kd = payload.color;
    Eigen::Vector3f ks = Eigen::Vector3f(0.7937, 0.7937, 0.7937);

    auto l1 = light{{20, 20, 20}, {500, 500, 500}};
    auto l2 = light{{-20, 20, 0}, {500, 500, 500}};

    std::vector<light> lights = {l1, l2};
    Eigen::Vector3f amb_light_intensity{10, 10, 10};
    Eigen::Vector3f eye_pos{0, 0, 10};

    float p = 150;

    Eigen::Vector3f color = payload.color;
    Eigen::Vector3f point = payload.view_pos;
    Eigen::Vector3f normal = payload.normal;

    Eigen::Vector3f result_color = {0, 0, 0};
    for (auto& light : lights)
    {
        // For each light source in the code, calculate what the *ambient*, *diffuse*, and *specular* 
        // components are. Then, accumulate that result on the *result_color* object.
        Vector3f light_dir = light.position - point;  // 光照方向
        Vector3f view_dir = (eye_pos - point).normalized(); // 视线方向
        float r2 = pow(light_dir.norm(), 2.f);  // 点与光源距离
        light_dir.normalize();  // 光照向量归一化 Tips: 如果在未归一的情况下计算半程向量，会影响到高光
        Vector3f half_vec = (light_dir + view_dir).normalized();  // 半程向量

        auto intensity = light.intensity / r2;  // 光照强度(与距离的平方成反比)
        Vector3f ambient = ka.cwiseProduct(amb_light_intensity);   // 环境光
        Vector3f diffuse = kd.cwiseProduct(intensity)* std::max(0.f, normal.dot(light_dir));  // 漫反射
        Vector3f specular = ks.cwiseProduct(intensity) * pow(std::max(0.f, normal.dot(half_vec)), p);  // 高光
        result_color += ambient + diffuse + specular;  // 光照累积

    }

    return result_color * 255.f;
}



Eigen::Vector3f displacement_fragment_shader(const fragment_shader_payload& payload)
{
    
    Eigen::Vector3f ka = Eigen::Vector3f(0.005, 0.005, 0.005);
    Eigen::Vector3f kd = payload.color;
    Eigen::Vector3f ks = Eigen::Vector3f(0.7937, 0.7937, 0.7937);

    auto l1 = light{{20, 20, 20}, {500, 500, 500}};
    auto l2 = light{{-20, 20, 0}, {500, 500, 500}};

    std::vector<light> lights = {l1, l2};
    Eigen::Vector3f amb_light_intensity{10, 10, 10};
    Eigen::Vector3f eye_pos{0, 0, 10};

    float p = 150;

    Eigen::Vector3f color = payload.color; 
    Eigen::Vector3f point = payload.view_pos;
    Eigen::Vector3f normal = payload.normal;

    float kh = 0.2, kn = 0.1;
    
    // Implement displacement mapping here
    Vector3f n = normal;
    float x = n.x(), y = n.y(), z = n.z();
    Vector3f t = {x*y/sqrt(x*x+z*z),sqrt(x*x+z*z),z*y/sqrt(x*x+z*z)};
    Vector3f b = n.cross(t);
    Matrix3f TBN;
    TBN << 
	    t.x(), b.x(), n.x(),
	    t.y(), b.y(), n.y(),
	    t.z(), b.z(), n.z();
    Texture* const hmap = payload.texture;
    float u = payload.tex_coords.x(), v = payload.tex_coords.y();
    int w = hmap->width;
    int h = hmap->height;
    float dU = kh * kn * (hmap->getColor(u+1.f/w,v).norm() - hmap->getColor(u,v).norm());
    float dV = kh * kn * (hmap->getColor(u,v+1.f/h).norm() - hmap->getColor(u,v).norm());
    Vector3f ln = {-dU, -dV, 1.f};
    n = (TBN * ln).normalized();
    point += kn * n * hmap->getColor(u,v).norm();

    Eigen::Vector3f result_color = {0, 0, 0};
    for (auto& light : lights)
    {
        // For each light source in the code, calculate what the *ambient*, *diffuse*, and *specular* 
        // components are. Then, accumulate that result on the *result_color* object.
        Vector3f light_dir = light.position - point;  // 光照方向
        Vector3f view_dir = (eye_pos - point).normalized(); // 视线方向
        float r2 = pow(light_dir.norm(), 2.f);  // 点与光源距离
        light_dir.normalize();  // 光照向量归一化 Tips: 如果在未归一的情况下计算半程向量，会影响到高光
        Vector3f half_vec = (light_dir + view_dir).normalized();  // 半程向量

        auto intensity = light.intensity / r2;  // 光照强度(与距离的平方成反比)
        Vector3f ambient = ka.cwiseProduct(amb_light_intensity);   // 环境光
        Vector3f diffuse = kd.cwiseProduct(intensity)* std::max(0.f, normal.dot(light_dir));  // 漫反射
        Vector3f specular = ks.cwiseProduct(intensity) * pow(std::max(0.f, normal.dot(half_vec)), p);  // 高光
        result_color += ambient + diffuse + specular;  // 光照累积

    }
    
    return result_color * 255.f;
}


Eigen::Vector3f bump_fragment_shader(const fragment_shader_payload& payload)
{
    
    Eigen::Vector3f ka = Eigen::Vector3f(0.005, 0.005, 0.005);
    Eigen::Vector3f kd = payload.color;
    Eigen::Vector3f ks = Eigen::Vector3f(0.7937, 0.7937, 0.7937);

    auto l1 = light{{20, 20, 20}, {500, 500, 500}};
    auto l2 = light{{-20, 20, 0}, {500, 500, 500}};

    std::vector<light> lights = {l1, l2};
    Eigen::Vector3f amb_light_intensity{10, 10, 10};
    Eigen::Vector3f eye_pos{0, 0, 10};

    float p = 150;

    Eigen::Vector3f color = payload.color; 
    Eigen::Vector3f point = payload.view_pos;
    Eigen::Vector3f normal = payload.normal;


    float kh = 0.2, kn = 0.1;

    // Implement bump mapping here
    Vector3f n = normal;
    auto x = n.x(), y = n.y(), z = n.z();
    Vector3f t = {x*y/sqrt(x*x+z*z),sqrt(x*x+z*z),z*y/sqrt(x*x+z*z)};
    Vector3f b = n.cross(t);
    Matrix3f TBN; 
    TBN << 
	    t.x(), b.x(), n.x(),
	    t.y(), b.y(), n.y(),
	    t.z(), b.z(), n.z();
    //auto u = point.x(), v = point.y();
    auto u = payload.tex_coords.x(), v = payload.tex_coords.y();
    int w = payload.texture->width;
    int h = payload.texture->height;
    auto hmap = payload.texture;
    //printf("%f %f\t", u, v);
    float dU = kh * kn * (hmap->getColor(u+1.f/w,v).norm()-hmap->getColor(u,v).norm());
    float dV = kh * kn * (hmap->getColor(u,v+1.f/h).norm()-hmap->getColor(u,v).norm());
    Vector3f ln = {-dU, -dV, 1.f};
    //point += (kn * normal * payload.texture->getColor(u, v).norm());
    normal = (TBN * ln).normalized();

    Eigen::Vector3f result_color = {0.f, 0.f, 0.f};
    //result_color = normal;
    result_color = normal.normalized();

    return result_color * 255.f;
}

int main(int argc, const char** argv)
{
    std::vector<Triangle*> TriangleList;

    float angle = 140.0;
    bool command_line = false;

    std::string filename = "output.png";
    objl::Loader Loader;
    std::string obj_path = "../models/spot/";

    // Load .obj File
    bool loadout = Loader.LoadFile("../models/spot/spot_triangulated_good.obj");
    for(auto mesh:Loader.LoadedMeshes)
    {
        for(int i=0;i<mesh.Vertices.size();i+=3)
        {
            Triangle* t = new Triangle();
            for(int j=0;j<3;j++)
            {
                t->setVertex(j,Vector4f(mesh.Vertices[i+j].Position.X,mesh.Vertices[i+j].Position.Y,mesh.Vertices[i+j].Position.Z,1.0));
                t->setNormal(j,Vector3f(mesh.Vertices[i+j].Normal.X,mesh.Vertices[i+j].Normal.Y,mesh.Vertices[i+j].Normal.Z));
                t->setTexCoord(j,Vector2f(mesh.Vertices[i+j].TextureCoordinate.X, mesh.Vertices[i+j].TextureCoordinate.Y));
            }
            TriangleList.push_back(t);
        }
    }

    rst::rasterizer r(700, 700);

    auto texture_path = "hmap.jpg";
    r.set_texture(Texture(obj_path + texture_path));

    std::function<Eigen::Vector3f(fragment_shader_payload)> active_shader = phong_fragment_shader;

    if (argc >= 2)
    {
        command_line = true;
        filename = std::string(argv[1]);

        if (argc == 3 && std::string(argv[2]) == "texture")
        {
            std::cout << "Rasterizing using the texture shader\n";
            active_shader = texture_fragment_shader;
            texture_path = "spot_texture.png";
            r.set_texture(Texture(obj_path + texture_path));
        }
        else if (argc == 3 && std::string(argv[2]) == "normal")
        {
            std::cout << "Rasterizing using the normal shader\n";
            active_shader = normal_fragment_shader;
        }
        else if (argc == 3 && std::string(argv[2]) == "phong")
        {
            std::cout << "Rasterizing using the phong shader\n";
            active_shader = phong_fragment_shader;
        }
        else if (argc == 3 && std::string(argv[2]) == "bump")
        {
            std::cout << "Rasterizing using the bump shader\n";
            active_shader = bump_fragment_shader;
        }
        else if (argc == 3 && std::string(argv[2]) == "displacement")
        {
            std::cout << "Rasterizing using the displacement shader\n";
            active_shader = displacement_fragment_shader;
        }
    }

    Eigen::Vector3f eye_pos = {0,0,10};

    r.set_vertex_shader(vertex_shader);
    r.set_fragment_shader(active_shader);

    int key = 0;
    int frame_count = 0;

    if (command_line)
    {
        r.clear(rst::Buffers::Color | rst::Buffers::Depth);
        r.set_model(get_model_matrix(angle));
        r.set_view(get_view_matrix(eye_pos));
        r.set_projection(get_projection_matrix(45.0, 1, 0.1, 50));

        r.draw(TriangleList);
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
        r.set_projection(get_projection_matrix(45.0, 1, 0.1, 50));

        //r.draw(pos_id, ind_id, col_id, rst::Primitive::Triangle);
        r.draw(TriangleList);
        cv::Mat image(700, 700, CV_32FC3, r.frame_buffer().data());
	image.convertTo(image, CV_8UC3, 1.0f);
	cv::cvtColor(image, image, cv::COLOR_RGB2BGR);

	cv::imshow("image", image);
	cv::imwrite(filename, image);
        key = cv::waitKey(10);

        if (key == 'a' )
        {
            angle -= 0.1;
        }
        else if (key == 'd')
        {
            angle += 0.1;
        }

    }
    return 0;
}
