#pragma once
#include <stdbool.h>
#include "math.h"

#define NT_PI        3.14159265358979323846264338327950288   // pi             
#define NT_PI_2      1.57079632679489661923132169163975144   // pi/2    

typedef struct{
    double x;
    double y;
}NT_VEC2;

NT_VEC2 NT_VEC2_New(double x,double y);

typedef struct{
    double x;
    double y;
    double w;
    double h;
}NT_RECT;

NT_RECT NT_RECT_New(double x,double y,double w,double h);
bool NT_RECT_IsCollidingWithPoint(NT_RECT aabb, NT_VEC2 point);
bool NT_RECT_IsCollidingWithRect(NT_RECT rect1,NT_RECT rect2);
bool NT_RECT_IsCollidingWithLine(NT_RECT rect, NT_VEC2 p1, NT_VEC2 p2);
bool NT_RECT_Equal(NT_RECT first,NT_RECT second);
float NT_Clamp(float value, float min, float max);

#ifdef NEWTON_IMP
#undef NEWTON_IMP
NT_VEC2 NT_VEC2_New(double x,double y){
    return (NT_VEC2){x,y};
}


NT_RECT NT_RECT_New(double x,double y,double w,double h){
    return (NT_RECT){x,y,w,h};
}
bool NT_RECT_IsCollidingWithPoint(NT_RECT aabb, NT_VEC2 point) {
    return (
            (aabb.x <= point.x && point.x <= aabb.x +aabb.w) &&
            (aabb.y <= point.y && point.y <= aabb.y +aabb.h)
    );
}
bool NT_RECT_IsCollidingWithRect(NT_RECT rect1,NT_RECT rect2){
    double minX1 = rect1.x;
    double maxX1 = rect1.x + rect1.w;
    double minY1 = rect1.y;
    double maxY1 = rect1.y + rect1.h;

    double minX2 = rect2.x;
    double maxX2 = rect2.x + rect2.w;
    double minY2 = rect2.y;
    double maxY2 = rect2.y + rect2.h;

    // Check for no overlap along the x-axis
    if (maxX1 < minX2 || minX1 > maxX2) {
        return false;
    }

    // Check for no overlap along the y-axis
    if (maxY1 < minY2 || minY1 > maxY2) {
        return false;
    }

    // If no separation along any axis, then the AABBs overlap
    return true;
}
int orientation(NT_VEC2 p, NT_VEC2 q, NT_VEC2 r) {
    double val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
    if (val == 0) return 0; // collinear
    return (val > 0) ? 1 : 2; // clock or counterclock wise
}

// Function to check if point q lies on line segment pr
int onSegment(NT_VEC2 p, NT_VEC2 q, NT_VEC2 r) {
    if (q.x <= fmax(p.x, r.x) && q.x >= fmin(p.x, r.x) &&
        q.y <= fmax(p.y, r.y) && q.y >= fmin(p.y, r.y))
        return 1;
    return 0;
}

// Function to check if two line segments p1q1 and p2q2 intersect
int doIntersect(NT_VEC2 p1, NT_VEC2 q1, NT_VEC2 p2, NT_VEC2 q2) {
    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);

    // General case
    if (o1 != o2 && o3 != o4)
        return 1;

    // Special cases
    // p1, q1 and p2 are collinear and p2 lies on segment p1q1
    if (o1 == 0 && onSegment(p1, p2, q1)) return 1;

    // p1, q1 and q2 are collinear and q2 lies on segment p1q1
    if (o2 == 0 && onSegment(p1, q2, q1)) return 1;

    // p2, q2 and p1 are collinear and p1 lies on segment p2q2
    if (o3 == 0 && onSegment(p2, p1, q2)) return 1;

    // p2, q2 and q1 are collinear and q1 lies on segment p2q2
    if (o4 == 0 && onSegment(p2, q1, q2)) return 1;

    return 0;
}

// Function to check if a line touches or intersects a rectangle
bool NT_RECT_IsCollidingWithLine(NT_RECT rect, NT_VEC2 p1, NT_VEC2 p2) {
    NT_VEC2 rectPoints[4] = {
        {rect.x, rect.y},
        {rect.x + rect.w, rect.y},
        {rect.x + rect.w, rect.y + rect.h},
        {rect.x, rect.y + rect.h}
    };

    for (int i = 0; i < 4; i++) {
        if (doIntersect(p1, p2, rectPoints[i], rectPoints[(i + 1) % 4])) {
            return true;
        }
    }
    return false;
}

bool NT_RECT_Equal(NT_RECT first,NT_RECT second){
    return (first.x == second.x && first.y == second.y && first.w == second.w && first.h == second.h);
}

float NT_Clamp(float value, float min, float max){
    if (value < min) {
        return min;
    }
    if (value > max) {
        return max;
    }
    return value;
}
#endif