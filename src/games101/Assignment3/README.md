⚠️⚠️⚠️作业三常见问题集合：
(1) bump mapping 部分的 h(u,v)=texture_color(u,v).norm, 其中 u,v 是 tex_coords, w,h 是 texture 的宽度与高度
(2) rasterizer.cpp 中 v = t.toVector4()
(3) get_projection_matrix 中的 eye_fov 应该被转化为弧度制
(4) bump 与 displacement 中修改后的 normal 仍需要 normalize
(5) 可能用到的 eigen 方法：norm(), normalized(), cwiseProduct()
(6) 实现 h(u+1/w,v) 的时候要写成 h(u+1.0/w,v)
(7) 正规的凹凸纹理应该是只有一维参量的灰度图，而本课程为了框架使用的简便性而使用了一张 RGB 图作为凹凸纹理的贴图，因此需要指定一种规则将彩色投影到灰度，而我只是「恰好」选择了 norm 而已。为了确保你们的结果与我一致，我才要求你们都使用 norm 作为计算方法。
(8) bump mapping & displacement mapping 的计算的推导日后将会在光线追踪部分详细介绍，目前请按照注释实现。

## Fragment shader completed 
- [x] normal fragment shader
- [x] phong fragment shader
- [x] texture fragment shader
- [x] bump fragment shader
- [x] displacement fragment shader
