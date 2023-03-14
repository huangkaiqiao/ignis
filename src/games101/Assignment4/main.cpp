#include <chrono>
#include <cmath>
#include <iostream>
#include <opencv2/opencv.hpp>

std::vector<cv::Point2f> control_points;

void mouse_handler(int event, int x, int y, int flags, void *userdata) 
{
    if (event == cv::EVENT_LBUTTONDOWN && control_points.size() < 4) 
    {
        std::cout << "Left button of the mouse is clicked - position (" << x << ", "
        << y << ")" << '\n';
        control_points.emplace_back(x, y);
    }     
}

void naive_bezier(const std::vector<cv::Point2f> &points, cv::Mat &window) 
{
    auto &p_0 = points[0];
    auto &p_1 = points[1];
    auto &p_2 = points[2];
    auto &p_3 = points[3];

    for (double t = 0.0; t <= 1.0; t += 0.001) 
    {
        auto point = std::pow(1 - t, 3) * p_0 + 3 * t * std::pow(1 - t, 2) * p_1 +
                 3 * std::pow(t, 2) * (1 - t) * p_2 + std::pow(t, 3) * p_3;

        window.at<cv::Vec3b>(point.y, point.x)[2] = 255;
    }
}

int c(int n, int r)  // combination
{
    if(r*2 > n){
        r = n - r;
    }
    if(r == 0){
        return 1;
    }
    if(r == 1){
        return n;
    }
    return n*c(n-1, r-1)/r;
}

cv::Point2f recursive_bezier(const std::vector<cv::Point2f> &control_points, float t) 
{
    // Implement de Casteljau's algorithm
    //t*p1 + (1-t)p2
    //(1-t)^2*p1 + 2*t*(1-t)*p2 + t^2*p3
    //(1-t)^3*p1 + 3*(1-t)^2*t*p2 + 3*(1-t)*t^1*p3 + t^3*p4
    int len = control_points.size();
    cv::Point2f point = {0.f, 0.f};
    for(int i=0; i<len; i++){
        point += c(len-1, i)* pow(1-t, len-1-i) * pow(t, i)* control_points[i];
        // printf("c:%d, (1-t):%f, t:%f, cp:(%f, %f)\n", c(len-1, i), pow(1-t, len-1-i), pow(t, i), control_points[i].x, control_points[i].y);
        // printf("t:%.4f, x:%.4f, y:%.4f\n", t, point.x, point.y);
    }
    // printf("\npoint: %.f, %.f\n", point.x, point.y);
    return point;

}

void bezier(const std::vector<cv::Point2f> &control_points, cv::Mat &window) 
{
    // Iterate through all t = 0 to t = 1 with small steps, and call de Casteljau's 
    // recursive Bezier algorithm.
    // printf("combination:%d %d %d %d\n", c(3,0), c(3,1), c(3,2), c(3,3));
    int resolution = 1000;

    for(int t=0; t<resolution; t++){
        cv::Point2f point = recursive_bezier(control_points,  static_cast<float>(t)/resolution);
        bool antialiasing = true;
        if(!antialiasing){
            window.at<cv::Vec3b>(point.y, point.x)[0] = 255;
            continue;
        }
        float py = point.y - floor(point.y);
        float px = point.x - floor(point.x);
        float d1 = sqrt(pow((py - 0.5),2) + pow((px - 0.5),2));  // 实际像素距离当前像素中心点

        for(int i=-1; i<2; i++){
            for(int j=-1; j<2; j++){
                double color = window.at<cv::Vec3b>(point.y + i, point.x + j)[0];
                float dy = py - i - 0.5;
                float dx = px - j - 0.5;
                float d2 = sqrt(pow(dy, 2) + pow(dx, 2));
                //1.1284
                // window.at<cv::Vec3b>(point.y + i, point.x + j)[0] = std::min(255., color + pow(2*sqrt(pow(i+0.5-oy, 2)+ pow(j+0.5-ox, 2))/sqrt((0.5-oy)*(0.5-oy)+(0.5-ox)*(0.5-ox)), 2) * 255);
                window.at<cv::Vec3b>(point.y + i, point.x + j)[0] = std::min(255., color + 0.5642/d2*255);
                // printf("%lf\t", color);
        // window.at<cv::Vec3b>(point.y + i, point.x + j)[0] = 255;
            }
        }
    }
    
    
    
}

int main() 
{
    cv::Mat window = cv::Mat(700, 700, CV_8UC3, cv::Scalar(0));
    cv::cvtColor(window, window, cv::COLOR_BGR2RGB);
    cv::namedWindow("Bezier Curve", cv::WINDOW_AUTOSIZE);

    cv::setMouseCallback("Bezier Curve", mouse_handler, nullptr);

    int key = -1;
    while (key != 27) 
    {
        for (auto &point : control_points) 
        {
            cv::circle(window, point, 3, {255, 255, 255}, 3);
        }

        if (control_points.size() == 4) 
        {
            // naive_bezier(control_points, window);
              bezier(control_points, window);

            cv::imshow("Bezier Curve", window);
            cv::imwrite("my_bezier_curve.png", window);
            key = cv::waitKey(0);

            return 0;
        }

        cv::imshow("Bezier Curve", window);
        key = cv::waitKey(20);
    }

return 0;
}
