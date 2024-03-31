#include <iostream>
#include <vector>
#include <math.h>
#include <utility>
#include <algorithm>

using namespace std;

struct Point{
    public:
 
        double x, y;
        Point(){}

        Point(double x, double y){
            this->x = x;
            this->y = y;
        }
}

Point createPoint(){
    double x, y;
    cin >> x >> y;

    return Point(x, y);
}
typedef vector<Point> vPt;
class Triangle{
    private:
        vPt verts;

    public:   
      
        Triangle(){
            setVerts();
        }

        Triangle(Point p1, Point p2, Point p3){
            verts.push_back(p1);
            verts.push_back(p2);
            verts.push_back(p3);
        }

        void setVerts(){

            verts.clear();

            verts.push_back(createPoint());
            verts.push_back(createPoint());
            verts.push_back(createPoint());
        }


        vPt getVerts(){
            return verts;
        }

        double area(){
            return 0.5 * abs((verts[1].x - verts[0].x) * (verts[2].y - verts[0].y) - (verts[2].x - verts[0].x) * (verts[1].y - verts[0].y));
        }
};

pair<bool, Point> segmentIntersection(Point a, Point b, Point c, Point d){
    bool result = false;
    Point intersectionPoint;

    double num1 = (d.x - c.x)*(c.y-a.y) - (d.y-c.y)*(c.x-a.x);
    double num2 = (b.x - a.x)*(c.y-a.y) - (b.y - a.y)*(c.x-a.x);

    double denum = (d.x - c.x)*(b.y-a.y) - (d.y - c.y)*(b.x-a.x);

    if(denum != 0){
        double alpha = num1 / denum;
        double beta = num2 / denum;

        if(alpha <= 1 && alpha > 0 && beta <= 1 && beta > 0){
            result = true;
            intersectionPoint.x = a.x + alpha * (b.x - a.x);
            intersectionPoint.y = a.y + alpha * (b.y - a.y);
        }
    }

    return make_pair(result, intersectionPoint);    
}

bool pointIsInside(Point p, Triangle t){
    bool res = false;
    vPt verts = t.getVerts();
    for(int i = 0; i <= 2; i++){
        Point l1 = verts[i];
        Point l2;

        if(i == 2){
            l2 = verts[0];
        } else {
            l2 = verts[i+1];
        }
        if (l1.y == l2.y) continue;

        bool conditionY = (p.y < l1.y) != (p.y < l2.y);
        bool conditionX = p.x < (l1.x + ((p.y - l1.y)/(l2.y - l1.y))*(l2.x - l1.x));
        if(conditionX && conditionY){
            res = !res;
        }
    }

    return res;
}

vPt findIntersectionPoints(Triangle& trg1, Triangle& trg2){
  
    auto uniqueInsert = [&intersectionPolygon](Point p){
        bool flag = false;
        for (Point i : intersectionPolygon){
            if((i.x == p.x) && (i.y == p.y)){
                check = true;
                break;
            }
        }
        if (!flag) intersectionPolygon.push_back(p);
    };

    vPt intersectionPolygon;
    vPt verts1 = trg1.getVerts();
    vPt verts2 = trg2.getVerts();
    for (Point i : verts1){
        if (pointIsInside(i, trg2)){
            uniqueInsert(i);
        }
    }

    for (Point i : verts2){
        if (pointIsInside(i, trg1)){
            uniqueInsert(i);
        }
    }

    for (int i = 0; i <= 2; i++){
        Point a1 = verts1[i];
        Point a2;
        if(i == 2){
            a2 = verts1[0];
        } else {
            a2 = verts1[i+1];
        }
        for (int j = 0; j <= 2; j++){
            Point b1 = verts2[j];
            Point b2;
            if(j == 2){
                b2 = verts2[0];
            } else {
                b2 = verts2[i+1];
            }
            pair<bool, Point> res = segmentIntersection(a1,a2,b1,b2);
            if (res.first == true){
                uniqueInsert(res.second);
            }
        }
    }
    return intersectionPolygon;
}

double polygonArea(vPt polygon) {
    int size = polygon.size();
    if (size <= 2) return 0;
    if (size == 3) return Triangle(polygon[0], polygon[1], polygon[2]).area();
    Point center(0, 0);
    for (Point i : polygon){
        center.x += i.x;
        center.y += i.y;
    }
    center.x /= size;
    center.y /= size;
    sort(polygon.begin(), polygon.end(), [&center](Point& a, Point& b){
        return atan2(a.x - center.x, a.y - center.y) < atan2(b.x - center.x, b.y - center.y);
    });

    double area = 0;
    while (polygon.size() > 3){
        area += Triangle(polygon[0], polygon[1], polygon[2]).area();
        polygon.erase(polygon.begin());
    }
    area += Triangle(polygon[0], polygon[1], polygon[2]).area();
    
    return area;
}

int main(){
    Triangle trg1;
    Triangle trg2;
    cout << polygonArea(findIntersectionPoints(trg1, trg2));
    
}