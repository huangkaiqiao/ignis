#pragma once

#include "Object.hpp"

#include <cstring>

bool rayTriangleIntersect(const Vector3f& v0, const Vector3f& v1, const Vector3f& v2, const Vector3f& orig,
                          const Vector3f& dir, float& tnear, float& u, float& v)
{
    // TODO: Implement this function that tests whether the triangle
    // that's specified bt v0, v1 and v2 intersects with the ray (whose
    // origin is *orig* and direction is *dir*)
    // Also don't forget to update tnear, u and v.

    // Moller-Trumbore
    // Check if the ray is parallel to the triangle
    // Vector3f N = normalize(crossProduct(v0, v1));
    // float prod = dotProduct(dir, N);
    // if(prod == 0) {
    //     return false;
    // }

    // Check if the ray-plane intersection lies outside the triangle
    /*Vector3f O_v0 = orig - v0;
    Vector3f v1_0 = v1 - v0;
    Vector3f v2_1 = v2 - v1;
    Vector3f matrix[3] = {
        {-dir.x, v1_0.x, v2_1.x},
        {-dir.y, v1_0.y, v2_1.y},
        {-dir.z, v1_0.z, v2_1.z},
    };
    Vector3f inverse[3];
    for(int i=0; i<3; i++){
        int j = (i+1)%3;
        int k = (i+2)%3;
        float x = matrix[j].y * matrix[k].z - matrix[j].z * matrix[k].y;
        float y = matrix[j].z * matrix[k].x - matrix[j].x * matrix[k].z;
        float z = matrix[j].x * matrix[k].y - matrix[j].y * matrix[k].x;
        inverse[i] = Vector3f(x, y, z);
    }
    float det = dotProduct(inverse[0], -dir);
    for(int i=0; i<3; i++){
        inverse[i] = inverse[i]/det;
    }
    tnear = dotProduct(inverse[0], O_v0);
    u     = dotProduct(inverse[1], O_v0);
    v     = dotProduct(inverse[2], O_v0);
    Vector3f P = (1-u-v)*v0 + u*v1 + v*v2;
    if(dotProduct(P-v0, v1_0)>0 && dotProduct(P-v1, v2_1)>0 && dotProduct(P-v2, v0-v2)>0){
        return true;
    }
    return false;*/


    const float EPSILON = 0.0000001;
    Vector3f vertex0 = v0;
    Vector3f vertex1 = v1;  
    Vector3f vertex2 = v2;
    Vector3f edge1, edge2, h, s, q;
    Vector3f rayVector = dir;
    Vector3f rayOrigin = orig;
    // float a,f,u,v;
    float a,f;
    edge1 = vertex1 - vertex0;
    edge2 = vertex2 - vertex0;
    h = crossProduct(rayVector, edge2);
    a = dotProduct(edge1, h);
    if (a > -EPSILON && a < EPSILON)
        return false;    // This ray is parallel to this triangle.
    f = 1.0/a;
    s = rayOrigin - vertex0;
    u = f * dotProduct(s, h);
    if (u < 0.0 || u > 1.0)
        return false;
    q = crossProduct(s, edge1);
    v = f * dotProduct(rayVector, q);
    if (v < 0.0 || u + v > 1.0)
        return false;
    // At this stage we can compute t to find out where the intersection point is on the line.
    tnear = f * dotProduct(edge2, q);
    if (tnear > EPSILON) // ray intersection
    {
        // outIntersectionPoint = rayOrigin + rayVector * t;
        return true;
    }
    else // This means that there is a line intersection but not a ray intersection.
        return false;
}

class MeshTriangle : public Object
{
public:
    MeshTriangle(const Vector3f* verts, const uint32_t* vertsIndex, const uint32_t& numTris, const Vector2f* st)
    {
        uint32_t maxIndex = 0;
        for (uint32_t i = 0; i < numTris * 3; ++i)
            if (vertsIndex[i] > maxIndex)
                maxIndex = vertsIndex[i];
        maxIndex += 1;
        vertices = std::unique_ptr<Vector3f[]>(new Vector3f[maxIndex]);
        memcpy(vertices.get(), verts, sizeof(Vector3f) * maxIndex);
        vertexIndex = std::unique_ptr<uint32_t[]>(new uint32_t[numTris * 3]);
        memcpy(vertexIndex.get(), vertsIndex, sizeof(uint32_t) * numTris * 3);
        numTriangles = numTris;
        stCoordinates = std::unique_ptr<Vector2f[]>(new Vector2f[maxIndex]);
        memcpy(stCoordinates.get(), st, sizeof(Vector2f) * maxIndex);
    }

    bool intersect(const Vector3f& orig, const Vector3f& dir, float& tnear, uint32_t& index,
                   Vector2f& uv) const override
    {
        bool intersect = false;
        for (uint32_t k = 0; k < numTriangles; ++k)
        {
            const Vector3f& v0 = vertices[vertexIndex[k * 3]];
            const Vector3f& v1 = vertices[vertexIndex[k * 3 + 1]];
            const Vector3f& v2 = vertices[vertexIndex[k * 3 + 2]];
            float t, u, v;
            if (rayTriangleIntersect(v0, v1, v2, orig, dir, t, u, v) && t < tnear)
            {
                tnear = t;
                uv.x = u;
                uv.y = v;
                index = k;
                intersect |= true;
            }
        }

        return intersect;
    }

    void getSurfaceProperties(const Vector3f&, const Vector3f&, const uint32_t& index, const Vector2f& uv, Vector3f& N,
                              Vector2f& st) const override
    {
        const Vector3f& v0 = vertices[vertexIndex[index * 3]];
        const Vector3f& v1 = vertices[vertexIndex[index * 3 + 1]];
        const Vector3f& v2 = vertices[vertexIndex[index * 3 + 2]];
        Vector3f e0 = normalize(v1 - v0);
        Vector3f e1 = normalize(v2 - v1);
        N = normalize(crossProduct(e0, e1));
        const Vector2f& st0 = stCoordinates[vertexIndex[index * 3]];
        const Vector2f& st1 = stCoordinates[vertexIndex[index * 3 + 1]];
        const Vector2f& st2 = stCoordinates[vertexIndex[index * 3 + 2]];
        st = st0 * (1 - uv.x - uv.y) + st1 * uv.x + st2 * uv.y;
    }

    Vector3f evalDiffuseColor(const Vector2f& st) const override
    {
        float scale = 5;
        float pattern = (fmodf(st.x * scale, 1) > 0.5) ^ (fmodf(st.y * scale, 1) > 0.5);
        return lerp(Vector3f(0.815, 0.235, 0.031), Vector3f(0.937, 0.937, 0.231), pattern);
    }

    std::unique_ptr<Vector3f[]> vertices;
    uint32_t numTriangles;
    std::unique_ptr<uint32_t[]> vertexIndex;
    std::unique_ptr<Vector2f[]> stCoordinates;
};
