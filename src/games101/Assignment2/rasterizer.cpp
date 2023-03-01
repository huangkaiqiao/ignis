// clang-format off
//
// Created by goksu on 4/6/19.
//

#include <algorithm>
#include <vector>
#include "rasterizer.hpp"
#include <opencv2/opencv.hpp>
#include <math.h>


rst::pos_buf_id rst::rasterizer::load_positions(const std::vector<Eigen::Vector3f> &positions)
{
    auto id = get_next_id();
    pos_buf.emplace(id, positions);

    return {id};
}

rst::ind_buf_id rst::rasterizer::load_indices(const std::vector<Eigen::Vector3i> &indices)
{
    auto id = get_next_id();
    ind_buf.emplace(id, indices);

    return {id};
}

rst::col_buf_id rst::rasterizer::load_colors(const std::vector<Eigen::Vector3f> &cols)
{
    auto id = get_next_id();
    col_buf.emplace(id, cols);

    return {id};
}

auto to_vec4(const Eigen::Vector3f& v3, float w = 1.0f)
{
    return Vector4f(v3.x(), v3.y(), v3.z(), w);
}


/*static bool insideTriangle(int x, int y, const Vector3f* _v)
{   
    // TODO : Implement this function to check if the point (x, y) is inside the triangle represented by _v[0], _v[1], _v[2]
    Vector3f v_ab, v_ap, v_bc, v_bp, v_ca, v_cp;
    float cp1z, cp2z, cp3z;
    v_ab << _v[1].x() - _v[0].x(), _v[1].y() - _v[0].y(), 0;
    v_ap << x - _v[0].x(), y - _v[0].y(), 0;
    cp1z = v_ab.x() * v_ap.y() - v_ap.x() * v_ab.y();
    v_bc << _v[2].x() - _v[1].x(), _v[2].y() - _v[1].y(), 0;
    v_bp << x - _v[1].x(), y - _v[1].y(), 0;
    cp2z = v_bc.x() * v_bp.y() - v_bp.x() * v_bc.y();
    v_ca << _v[0].x() - _v[2].x(), _v[0].y() - _v[2].y(), 0;
    v_cp << x - _v[2].x(), y - _v[2].y(), 0;
    cp3z = v_ca.x() * v_cp.y() - v_cp.x() * v_ca.y();
    return (cp1z > 0 && cp2z >0 && cp3z >0) || (cp1z < 0 && cp2z <0 && cp3z <0);
    // return ;
}*/
//判断测试点是否在三角形内
static bool insideTriangle(int x, int y, const Vector3f* _v)
{
	// TODO : Implement this function to check if the point (x, y) is inside the triangle represented by _v[0], _v[1], _v[2]

	//测试点的坐标为(x, y)
	//三角形三点的坐标分别为_v[0], _v[1], _v[2]

	//叉乘公式为(x1, y1)X(x2, y2) = x1*y2 - y1*x2

	//（1）准备三角形各边的的向量
	Eigen::Vector2f side1;
	side1 << _v[1].x() - _v[0].x(), _v[1].y() - _v[0].y();
	Eigen::Vector2f side2;
	side2 << _v[2].x() - _v[1].x(), _v[2].y() - _v[1].y();
	Eigen::Vector2f side3;
	side3 << _v[0].x() - _v[2].x(), _v[0].y() - _v[2].y();

	//（2）准备测量点和三角形各点连线的向量
	Eigen::Vector2f v1;
	v1 << x - _v[0].x(), y - _v[0].y();
	Eigen::Vector2f v2;
	v2 << x - _v[1].x(), y - _v[1].y();
	Eigen::Vector2f v3;
	v3 << x - _v[2].x(), y - _v[2].y();

	//（3）三角形各边的的向量叉乘测量点和三角形各点连线的向量
	float z1 = side1.x() * v1.y() - side1.y() * v1.x();
	float z2 = side2.x() * v2.y() - side2.y() * v2.x();
	float z3 = side3.x() * v3.y() - side3.y() * v3.x();

	//（4）判断叉乘结果是否有相同的符号
	if ((z1 > 0 && z2 > 0 && z3 > 0) || (z1 < 0 && z2 < 0 && z3 < 0))
	{
		return true;
	}
	else
	{
		return false;
	}
}


static std::tuple<float, float, float> computeBarycentric2D(float x, float y, const Vector3f* v)
{
    float c1 = (x*(v[1].y() - v[2].y()) + (v[2].x() - v[1].x())*y + v[1].x()*v[2].y() - v[2].x()*v[1].y()) / (v[0].x()*(v[1].y() - v[2].y()) + (v[2].x() - v[1].x())*v[0].y() + v[1].x()*v[2].y() - v[2].x()*v[1].y());
    float c2 = (x*(v[2].y() - v[0].y()) + (v[0].x() - v[2].x())*y + v[2].x()*v[0].y() - v[0].x()*v[2].y()) / (v[1].x()*(v[2].y() - v[0].y()) + (v[0].x() - v[2].x())*v[1].y() + v[2].x()*v[0].y() - v[0].x()*v[2].y());
    float c3 = (x*(v[0].y() - v[1].y()) + (v[1].x() - v[0].x())*y + v[0].x()*v[1].y() - v[1].x()*v[0].y()) / (v[2].x()*(v[0].y() - v[1].y()) + (v[1].x() - v[0].x())*v[2].y() + v[0].x()*v[1].y() - v[1].x()*v[0].y());
    // printf("x=%f, y=%f, v[0]=(%f,%f), v[1]=(%f, %f), v[2]=(%f, %f), c1=%f, c2=%f, c3=%f\t", x, y, v[0], v[1], v[2], c1, c2, c3);
    return {c1,c2,c3};
}

void rst::rasterizer::draw(pos_buf_id pos_buffer, ind_buf_id ind_buffer, col_buf_id col_buffer, Primitive type)
{
    auto& buf = pos_buf[pos_buffer.pos_id];
    auto& ind = ind_buf[ind_buffer.ind_id];
    auto& col = col_buf[col_buffer.col_id];

    float f1 = (50 - 0.1) / 2.0;
    float f2 = (50 + 0.1) / 2.0;

    Eigen::Matrix4f mvp = projection * view * model;
    for (auto& i : ind)
    {
        Triangle t;
        Eigen::Vector4f v[] = {
                mvp * to_vec4(buf[i[0]], 1.0f),
                mvp * to_vec4(buf[i[1]], 1.0f),
                mvp * to_vec4(buf[i[2]], 1.0f)
        };
        //Homogeneous division
        for (auto& vec : v) {
            vec /= vec.w();
        }
        //Viewport transformation
        for (auto & vert : v)
        {
            vert.x() = 0.5*width*(vert.x()+1.0);
            vert.y() = 0.5*height*(vert.y()+1.0);
            vert.z() = vert.z() * f1 + f2;
        }

        for (int i = 0; i < 3; ++i)
        {
            t.setVertex(i, v[i].head<3>());
            // t.setVertex(i, v[i].head<3>());
            // t.setVertex(i, v[i].head<3>());
        }

        auto col_x = col[i[0]];
        auto col_y = col[i[1]];
        auto col_z = col[i[2]];

        t.setColor(0, col_x[0], col_x[1], col_x[2]);
        t.setColor(1, col_y[0], col_y[1], col_y[2]);
        t.setColor(2, col_z[0], col_z[1], col_z[2]);

        rasterize_triangle(t);
    }
}

//Screen space rasterization
void rst::rasterizer::rasterize_triangle(const Triangle& t) {

    auto v = t.toVector4();
    
    // TODO : Find out the bounding box of current triangle.
    // iterate through the pixel and find if the current pixel is inside the triangle
    float top = t.v[0].y(), left = t.v[0].x(), bottom = t.v[0].y(), right = t.v[0].x();
    for(Vector3f p: t.v) {
        top = std::max(p.y(), top);
        left = std::min(p.x(), left);
        bottom = std::min(p.y(), bottom);
        right = std::max(p.x(), right);
    }
    std::vector<Eigen::Vector2d> bb{{left, bottom}, {right, top}};  // bounding_box

    // TODO : set the current pixel (use the set_pixel function) to the color of the triangle (use getColor function) if it should be painted.
    for(int i=bb[0][1]; i<bb[1][1]; i++) {
        for(int j=bb[0][0]; j<bb[1][0]; j++) {
            if(insideTriangle(j, i, t.v)) {
                auto[alpha, beta, gamma] = computeBarycentric2D(j, i, t.v);
                // float alpha = 242, beta = 123, gamma = 31;
                float w_reciprocal = 1.0/(alpha / v[0].w() + beta / v[1].w() + gamma / v[2].w());
                float z_interpolated = alpha * v[0].z() / v[0].w() + beta * v[1].z() / v[1].w() + gamma * v[2].z() / v[2].w();
                z_interpolated *= w_reciprocal;
                float idv = z_interpolated;
                
                int idx = get_index(j, i);
                if(depth_buf[idx] == std::numeric_limits<float>::infinity() || idx < depth_buf[idx]) {
                    // printf("start paint!%f %f %f\n", depth_buf[idx], idv, t.v[0][2]);
                    depth_buf[idx] = idv;
                    Vector3f point = {static_cast<float>(j), static_cast<float>(i), 1};
                    set_pixel(point, t.getColor());
                }
            }
        }
    }
}

void rst::rasterizer::set_model(const Eigen::Matrix4f& m)
{
    model = m;
}

void rst::rasterizer::set_view(const Eigen::Matrix4f& v)
{
    view = v;
}

void rst::rasterizer::set_projection(const Eigen::Matrix4f& p)
{
    projection = p;
}

void rst::rasterizer::clear(rst::Buffers buff)
{
    if ((buff & rst::Buffers::Color) == rst::Buffers::Color)
    {
        std::fill(frame_buf.begin(), frame_buf.end(), Eigen::Vector3f{0, 0, 0});
    }
    if ((buff & rst::Buffers::Depth) == rst::Buffers::Depth)
    {
        std::fill(depth_buf.begin(), depth_buf.end(), std::numeric_limits<float>::infinity());
    }
}

rst::rasterizer::rasterizer(int w, int h) : width(w), height(h)
{
    frame_buf.resize(w * h);
    depth_buf.resize(w * h);
}

int rst::rasterizer::get_index(int x, int y)
{
    return (height-1-y)*width + x;
}

void rst::rasterizer::set_pixel(const Eigen::Vector3f& point, const Eigen::Vector3f& color)
{
    //old index: auto ind = point.y() + point.x() * width;
    auto ind = (height-1-point.y())*width + point.x();
    frame_buf[ind] = color;

}

// clang-format on