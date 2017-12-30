// Geogebra: https://ggbm.at/GT8w9nCV
#include <bits/stdc++.h>
#define cout if (1) cout

using namespace std;
typedef long long int ll;
typedef pair<ll,ll> pii;
#define pb push_back

typedef double cood;
const cood eps = 1e-8;

#include "gtest/gtest.h"
#include "../../code/geometry/basic.cpp"

bool isClose (cood a, cood b) {
	if (abs(a - b) <= eps) return true;
	return min(a*(1-eps),a*(1+eps)) <= b + 1e-15 && b <= max(a*(1-eps),a*(1+eps)) + 1e-15;
}

bool operator == (vec a, vec b)
{ return isClose(a.x,b.x) && isClose(a.y,b.y); }
bool operator != (vec a, vec b)
{ return !(a==b); }
bool operator == (cir a, cir b)
{ return a.c == b.c && isClose(a.r,b.r); }

vec A[] = {vec(1,4), vec(3,2), vec(7,6), vec(4,6)},
	B = vec(4,14),
	E[] = {vec(72,92), vec(78,90), vec(84,88), vec(54,98)};

TEST(geometry_basic_cir, cir) {
	cir a0(A[0],A[1],A[2]);

	EXPECT_EQ(a0, cir(vec(4,5), sqrt(10))) << "Creates correct circle";

	vec v[] = {A[0],A[1],A[2]};
	sort(v, v+3);
	do {
		EXPECT_EQ(cir(v[0],v[1],v[2]), a0) << "Doesn't depend on order";
	} while (next_permutation(v,v+3));

	vec D(4,6);
	cir a1(vec(3.1,4.1), sqrt(4.42));
	EXPECT_EQ(cir(A[0],A[1],A[3]), a1) << "Creates a different circle";
}

TEST(geometry_basic_cir, arc_area) {
	cir b(B,4);
	vec ray(4,0);

	vec Ba[] = {b.c + ray*.5, b.c + ray, b.c + ray*1.5};
	ray = ray.rot90();	
	vec Bb[] = {b.c + ray*.5, b.c + ray, b.c + ray*1.5};
	ray = ray.rotate(3*pi/4);
	vec Bc[] = {b.c + ray*.5, b.c + ray, b.c + ray*1.5};

	EXPECT_DOUBLE_EQ(b.arc_area(Ba[1],Bb[1]),4*pi) << "Correct";
	EXPECT_DOUBLE_EQ(b.arc_area(Ba[1],Bc[1]),-6*pi) << "Oriented";

	for (vec * u : {Ba,Bb,Bc}) {
		for (vec * v : {Ba,Bb,Bc}) {
			if (u == v) continue;
			double res = b.arc_area(u[0],v[0]);
			for (int i = 0; i < 3; i++)
				for (int j = 0; j < 3; j++)
					EXPECT_DOUBLE_EQ(res,b.arc_area(u[i],v[j])) << "Doesn't depend on ray length";
		}
	}

	cir b_big(b.c,b.r*2);
	EXPECT_DOUBLE_EQ(b_big.arc_area(Ba[0],Bb[1])/b.arc_area(Ba[0],Bb[1]), 4) << "Quadratic on ray";
}

TEST(geometry_basic_cir, arc_len) {
	cir b(B,4);
	vec ray(4,0);

	vec Ba[] = {b.c + ray*.5, b.c + ray, b.c + ray*1.5};
	ray = ray.rot90();	
	vec Bb[] = {b.c + ray*.5, b.c + ray, b.c + ray*1.5};
	ray = ray.rotate(3*pi/4);
	vec Bc[] = {b.c + ray*.5, b.c + ray, b.c + ray*1.5};

	EXPECT_DOUBLE_EQ(b.arc_len(Ba[1],Bb[1]),2*pi) << "Correct";
	EXPECT_DOUBLE_EQ(b.arc_len(Ba[1],Bc[1]),-3*pi) << "Oriented";

	for (vec * u : {Ba,Bb,Bc}) {
		for (vec * v : {Ba,Bb,Bc}) {
			if (u == v) continue;
			double res = b.arc_len(u[0],v[0]);
			for (int i = 0; i < 3; i++)
				for (int j = 0; j < 3; j++)
					EXPECT_DOUBLE_EQ(res,b.arc_len(u[i],v[j])) << "Doesn't depend on ray length";
		}
	}
	cir b_big(b.c,b.r*2);
	EXPECT_DOUBLE_EQ(b_big.arc_len(Ba[0],Bb[1])/b.arc_len(Ba[0],Bb[1]), 2) << "Linear on ray";
}

::testing::AssertionResult is_on_circle (vec v, cir c) { 
	if (abs(v.nr(c.c) - c.r) <= eps)
		return ::testing::AssertionSuccess();
	return ::testing::AssertionFailure() << v << " is not on " << c;
}

bool is_on_lin (vec v, lin l) { return abs(v.x * l.a + v.y * l.b - l.c) <= eps; }

::testing::AssertionResult check_cir_border_inter (cir a, cir b, bool once) {
	pair<vec,vec> res = a.border_inter(b);
	::testing::AssertionResult ret = ::testing::AssertionSuccess();
	if (once) {
		if (res.first != res.second) 
			return ret = ::testing::AssertionFailure() << "two different intersections: " << res.first << " and " << res.second;
	} else {
		if (res.first == res.second)
			return ret = ::testing::AssertionFailure() << "only one intersection: " << res.first;
	}
	
	if (ret) ret = is_on_circle(res.first, a);
	if (ret) ret = is_on_circle(res.first, b);
	if (ret) ret = is_on_circle(res.second, a);
	if (ret) ret = is_on_circle(res.second, b);

	return ret;
}

TEST(geometry_basic_cir, border_inter) {
	double e_d = E[0].nr(E[1]);
	cir e[] = {
		cir(E[0],1.5*e_d), // green
		cir(E[1],.5*e_d), cir(E[1],e_d), cir(E[1],1.8*e_d), // orange
		cir(E[2],e_d), cir(E[2],2*e_d), cir(E[2],2.3*e_d), cir(E[2],3*e_d), // purple
		cir(E[3],2*e_d), cir(E[3],1.5*e_d) // blue
	};

	for (int i = 1; i <= 9; i++) {
		EXPECT_TRUE(check_cir_border_inter(e[0], e[i], (i == 1 || i == 9))) << " where i = " << i;
		EXPECT_TRUE(check_cir_border_inter(e[i], e[0], (i == 1 || i == 9))) << " where i = " << i;
	}

	EXPECT_TRUE(check_cir_border_inter(e[3],e[4],false)) << "Inters after center";
	EXPECT_TRUE(check_cir_border_inter(e[3],e[6],false)) << "Inters after center";
	EXPECT_TRUE(check_cir_border_inter(e[4],e[3],false)) << "Inters after center";
	EXPECT_TRUE(check_cir_border_inter(e[6],e[3],false)) << "Inters after center";
	EXPECT_TRUE(check_cir_border_inter(e[8],e[7],true)) << "Borders touch";

	EXPECT_ANY_THROW(check_cir_border_inter(e[1],e[9],true)) << "No intersection";
	EXPECT_ANY_THROW(check_cir_border_inter(e[9],e[1],true)) << "No intersection";
	EXPECT_ANY_THROW(check_cir_border_inter(e[4],e[6],true)) << "Fully contained";
	EXPECT_ANY_THROW(check_cir_border_inter(e[6],e[4],true)) << "Fully contained";
	EXPECT_ANY_THROW(check_cir_border_inter(e[2],e[7],true)) << "Fully contained different centers";
	EXPECT_ANY_THROW(check_cir_border_inter(e[7],e[2],true)) << "Fully contained different centers";
}
