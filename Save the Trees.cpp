/*
 *  Problem: https://www.codechef.com/problems/KOL1509
 *  Display 2 * min area needed to enclose a set of points
 *  
 *  CodeChef Difficult Rating: Easy
 *  
 *  Approach:
 *  Min area enclosing a set of points = area of the convex hull enclosing the set.
 *  Find the convex hull using Graham's scan algorithm and compute the area
 *
 *  Running Time = O(nlogn).
*/

#include <iostream>
#include <cmath>
#include <vector>
#include <stack>
using namespace std;

struct point
{
    unsigned long long x, y;
};

void print_vec(vector<struct point> pt)
{
    for (vector<struct point>::iterator it = pt.begin(); it != pt.end(); ++it) {
        struct point temp = *it;
        cout << temp.x << " " << temp.y << endl;
    }
}

long long dist(point p1, point p2)
{
    return ((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
}

int orientation(point p0, point p1, point p2)
{
    long long val = (p1.y - p0.y) * (p2.x - p1.x) - (p2.y - p1.y) * (p1.x - p0.x);

    if (val == 0) {
        return 0;
        /* Collinear */
    } else if (val > 0) {
        return 1;
    } else {
        return 2;
    }
}

int compare(point ref, point pt, point pt1)
{
    int o = orientation(ref, pt, pt1);

    if (o == 0) {
        if  (dist(ref, pt) <= dist(ref, pt1)) {
            return -1;
        } else {
            return 1;
        }
    }

    if (o == 2) {
        return -1;
    } else {
        return 1;
    }
}

void merge(vector<struct point> &pt, unsigned long beg, unsigned long mid, unsigned long end)
{
    unsigned long i = beg, j = mid + 1, k = 0;
    vector<struct point> temp;

    while (i <= mid && j <= end) {
        int cmp = compare(pt[0], pt[i], pt[j]);

        if (cmp == -1) {
            temp.push_back(pt[i++]);
        } else if (cmp == 1) {
            temp.push_back(pt[j++]);
        }
    }

    while (i <= mid) {
        temp.push_back(pt[i++]);
    }

    while (j <= end) {
        temp.push_back(pt[j++]);
    }
    i = beg;

    for (j = 0; j < temp.size(); j++) {
        pt[i] = temp[j];
        i++;
    }
}

void merge_sort(vector<struct point> &pt, unsigned long begin, unsigned long end)
{
    if (begin >= end) {
        return;
    }

    unsigned long mid = (begin + end) / 2;
    merge_sort(pt, begin, mid);
    merge_sort(pt, mid+1, end);
    merge(pt, begin, mid, end);
}

point nextToTop(stack<struct point> S)
{
    point top = S.top();
    S.pop();
    point nextTop = S.top();
    S.push(top);
    return nextTop;
}

long long graham_scan(vector<struct point> pt)
{
    unsigned long size = pt.size();
    vector<struct point> ch;

    unsigned long min = 0;
    for (unsigned long i = 1; i < size; i++) {
        if (pt[i].y < pt[min].y) {
            min = i;
        } else if (pt[i].y == pt[min].y && pt[i].x < pt[min].x) {
            min = i;
        }
    }

    struct point t = pt[0];
    pt[0] = pt[min];
    pt[min] = t;

    merge_sort(pt, 1, size - 1);
#if 0
    cout << "After Sort \n";
    print_vec(pt);
#endif
    
    unsigned long idx = 1;
    for (unsigned long i = 1; i < size; i++) {
        while (i < size - 1 && orientation(pt[0], pt[i], pt[i+1]) == 0) {
            i++;
        }
        pt[idx++] = pt[i];
    }

    if (idx < 3) {
        return 0;
    }

#if 0
    cout << "Removing dup\n";
    for (unsigned long i = 0; i < idx; i++) {
        cout << pt[i].x << " " << pt[i].y << endl;
    }
#endif
    
    stack<struct point> S;
    S.push(pt[0]);
    S.push(pt[1]);
    S.push(pt[2]);

    for (unsigned long i = 3; i < idx; i++) {
        while (orientation(nextToTop(S), S.top(), pt[i]) != 2) {
            S.pop();
        }

        S.push(pt[i]);
    }

    while (!S.empty()) {
        ch.push_back(S.top());
        S.pop();
    }

    struct point te = ch[0];
    ch.push_back(te);

#if 0
    cout << "Convex Hull = \n";
    print_vec(ch);
#endif

    long long area = 0;
    for (unsigned long i = 1; i < ch.size(); i++) {
            area += (ch[i - 1].x * ch[i].y - ch[i - 1].y * ch[i].x);
    }

    return fabs(area);
}

int main()
{
    unsigned long T, N;
    cin >> T;

    while (T--) {
        cin >> N;
        vector<struct point> pt;
        vector<unsigned long long> coord;

        for (unsigned long i = 0; i < N; i++) {
            unsigned long long p;
            cin >> p;

            coord.push_back(p);
        }

        unsigned long long min, max;
        min = coord[0];
        max = coord[1];

        struct point pts;
        pts.x = min;
        pts.y = max;
        pt.push_back(pts);

        if (min > max) {
            unsigned long long t = min;
            min = max;
            max = t;
        }

        for (vector<unsigned long long>::iterator it = coord.begin() + 2; it != coord.end(); ++it) {
            pts.x = min;
            pts.y = *it;
            pt.push_back(pts);

            pts.x = max;
            pts.y = *it;
            pt.push_back(pts);

            if (*it < min) {
                min = *it;
            } else if (*it > max) {
                max = *it;
            }
        }
#if 0
        print_vec(pt);
        cout << "Size = " << pt.size() << endl;
#endif
        cout << graham_scan(pt) << endl;

    }
    return 0;
}
