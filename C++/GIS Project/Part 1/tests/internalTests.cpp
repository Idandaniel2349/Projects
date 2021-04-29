#include "gtest/gtest.h"
#include "../GISNamedTypes.h"
#include "../CoordinatesMath.h"

TEST(Double, SimplePrecision) {
	Double<4> d1(1.12349487);
	Double<4> d2(1.12349487);
	EXPECT_EQ(d1, d2);
}

TEST(Double, Comparison) {
    Double<5> d = 3.1234567;
	//supported:
    //std::cout << std::setprecision(10) << d << std::endl;
    //intentionally unsupported:
    //std::cout << (d == 3.12346) << std::endl;
    EXPECT_TRUE(d == Double<5>{3.12346});
    EXPECT_TRUE(!(d == Double<6>{3.1234567}));
    EXPECT_TRUE(d == Double<5>{3.1234567});
    EXPECT_TRUE(d == Double<6>{3.12346});
    EXPECT_TRUE(d == Double<6>{3.1234604});
    EXPECT_TRUE(d == Double<5>{3.1234567});
    EXPECT_TRUE(Double<6>{3.1234604} == d);
    EXPECT_TRUE(Double<6>{3.1234604} >= d);
    EXPECT_TRUE(Double<6>{3.1234604} <= d);
    EXPECT_TRUE(Double<6>{3.1234605} > d);
    EXPECT_TRUE(Double<6>{3.123459} < d);
    EXPECT_TRUE(!(Double<6>{3.1234604} > d));
}

TEST(DoubleNamedType, Functionality) {
    Meters m1 { 1.133456789 };
    Meters m2 { 1.123456789 };
    Meters m3 = m1 + m2;
    EXPECT_EQ(m3, m2 + m1);

    double d1 { 1.133456789 };
    double d2 { 1.123456789 };
    double d3 = d2 + d1;
    EXPECT_EQ(d3, d1 + d2);

    // assert(m3 == d3); // doesn't compile - as we actually wish!
    EXPECT_EQ(m3, Meters { d3 });

    Meters msum { 0 };
    double sum;
    for (double d = 0.0000001; d <= 1.125335; d += 0.00001) {
        msum += Meters {d};
        sum += d;
    }
    EXPECT_EQ(msum, Meters { sum });

    msum = Meters { 0 };
    sum = 0;
    for (double d = 0.0000001; d <= 1.1254; d += 0.00001) {
        msum += Meters {d};
        sum += d;
    }
    EXPECT_EQ(msum, Meters { sum });

    Meters d = Meters { 0.124 } - Meters { 0.116 };
    EXPECT_EQ(d, Meters { 0.008 });
    EXPECT_EQ(d, Meters { 0.012 });
    EXPECT_EQ(d->roundToPrecision(), 0.01);
    double ddd = static_cast<double>(d);
    EXPECT_TRUE(ddd > 0.007999 && ddd < 0.008001);
    EXPECT_EQ(Meters {1.234567}, Meters {1.23});
    EXPECT_EQ(Meters {1.235}, Meters {1.24});
    EXPECT_TRUE(Meters {1.235} > Meters {1.23});

    // we want to allow the below, i.e. treat Double as double
    EXPECT_EQ(Double<0> {1} * Meters {1}, Meters {1});
    EXPECT_EQ(Double<2> {1} * Meters {1}, Meters {1});
    EXPECT_EQ(Meters {1} * Double<3> {1}, Meters {1});
    EXPECT_EQ(Meters {1} * Double<2> {1}, Meters {1});
}

template<class T, class U>
  concept SupportsMultiplication =
    requires(const std::remove_reference_t<T>& t,
             const std::remove_reference_t<U>& u) {
        t * u;
    };

template<class T, class U>
  concept SupportsDivision =
    requires(const std::remove_reference_t<T>& t,
             const std::remove_reference_t<U>& u) {
        t / u;
    };

template<class T, class U>
  concept SupportsPlus =
    requires(const std::remove_reference_t<T>& t,
             const std::remove_reference_t<U>& u) {
        t + u;
    };

template<class T, class U>
  concept SupportsMinus =
    requires(const std::remove_reference_t<T>& t,
             const std::remove_reference_t<U>& u) {
        t - u;
    };

template<class T, class U>
  concept SupportsPow =
    requires(const std::remove_reference_t<T>& t,
             const std::remove_reference_t<U>& u) {
        std::pow(t, u);
    };

TEST(DoubleNamedTypes, FunctionalityAndCompilation) {
    // the below is blocked as it should be:
    //assert (Meters {1.235} == Minutes {1.235});

    // Test Functional
    EXPECT_FALSE((std::equality_comparable_with<Meters, Minutes>));
    EXPECT_TRUE((std::equality_comparable_with<Minutes, Minutes>));

    EXPECT_TRUE((SupportsMultiplication<Meters, double>));
    EXPECT_TRUE((SupportsMultiplication<Meters, Double<0>>));
    EXPECT_FALSE((SupportsMultiplication<Meters, Meters>));
    EXPECT_FALSE((SupportsMultiplication<Meters, Minutes>));
    EXPECT_FALSE((SupportsMultiplication<NamedTypeDouble<0>, Meters>));
    EXPECT_FALSE((SupportsMultiplication<Meters, NamedTypeDouble<2>>));
    EXPECT_FALSE((SupportsPlus<Meters, Minutes>));
    EXPECT_TRUE((SupportsPlus<Meters, Meters>));
    EXPECT_FALSE((SupportsMinus<Meters, Minutes>));
    EXPECT_TRUE((SupportsMinus<Meters, Meters>));
    EXPECT_FALSE((SupportsDivision<Meters, Minutes>));
    EXPECT_FALSE((SupportsDivision<Double<2>, Meters>));
    EXPECT_TRUE((SupportsDivision<Meters, Meters>));
    EXPECT_TRUE((SupportsDivision<Meters, double>));

    // the below are blocked as they should be:
    // assert (Meters {1.235} == Minutes {1.235});
    // assert (Meters {1} * Meters {1} == 1);
    // assert (Meters {1} + Minutes {1} == 2);
    // assert (Meters {1} - Minutes {1} == 0);
    // assert (Meters {1} * Minutes {1} == 1);
    // assert (Meters {1} / Minutes {1} == 1);
    // assert (Double<2> {1} / Meters {1} == 1);
    // assert (NamedTypeDouble<0> {1} * Meters {1} == Meters {1} );
    // assert (Meters {1} * NamedTypeDouble<2> {1} == Meters {1} );

    // EXPECT_TRUE((std::equality_comparable_with<std::pow(Meters {2}, Meters {2}), 8>));

    // implicit cast to double is removed
    // the below doesn't compile:
    // double result = std::pow(Meters {2}, Meters {2});
    // but this does:
    EXPECT_EQ( std::pow(static_cast<double>(Meters {2}), static_cast<double>(Meters {2})), 4 );
}

TEST(Coordinates, Comparison) {
	Coordinates coords = {Longitude{34}, Latitude{34}};
    Coordinates coords2 = {Longitude{34.0000004}, Latitude{34}};
    EXPECT_TRUE(coords == coords2);
    Coordinates coords3 = {Longitude{34.00000050000001}, Latitude{34}};
    EXPECT_NE(coords, coords3);
    auto [longitude, latitude] = coords;
    EXPECT_TRUE(longitude == Longitude{34.000000});
    EXPECT_TRUE(Latitude{34.000003} != latitude);
}

TEST(CoordinatesMath, CalculateDistance) {
	Coordinates c1(Longitude(1), Latitude(3));
	Coordinates c2(Longitude(-1), Latitude(4));
	EXPECT_EQ(CoordinatesMath::calculateDistance(c1, c2), Meters{248265.89});
	//First location (default: 1600 Pennsylvania Ave NW, Washington, DC)
	Coordinates coor1 = {Longitude(-77.037852), Latitude(38.898556)};
	Coordinates white_house = coor1;
	//Second location (default: 1922 F St NW, Washington, DC)
	Coordinates coor2 = {Longitude(-77.043934), Latitude(38.897147)};
	EXPECT_EQ(Meters{549.16}, CoordinatesMath::calculateDistance(coor1, coor2));
	EXPECT_EQ(Meters{0.0}, CoordinatesMath::calculateDistance(coor1, white_house));
}

TEST(CoordinatesMath, CalculateDistance2) {
	Coordinates athens = {Longitude{23.725750}, Latitude{37.971536}}; // Athens , Acropolis
	Coordinates brussels = {Longitude{4.3826169}, Latitude{50.8119483}}; // Brussels , ULB
	EXPECT_EQ(Meters{2085989.93}, CoordinatesMath::calculateDistance(athens, brussels));
}

TEST(CoordinatesMath, MidpointVertical) {
    const double lng = -100;
	Coordinates almost_north_pole{Longitude{lng}, Latitude{89}}; 
	Coordinates almost_south_pole{Longitude{lng}, Latitude{-89}}; 
    auto midpoint = CoordinatesMath::midpoint(almost_north_pole, almost_south_pole);
    auto equator_point = Coordinates{Longitude{lng}, Latitude{0}};
	EXPECT_EQ(midpoint, equator_point);
}

TEST(CoordinatesMath, MidpointHorizontal0) {
    const double lat = 0;
	Coordinates point1{Longitude{42}, Latitude{lat}}; 
	Coordinates point2{Longitude{84}, Latitude{lat}}; 
    auto midpoint = CoordinatesMath::midpoint(point1, point2);
    auto expected_midpoint = Coordinates{Longitude{63}, Latitude{lat}};
	EXPECT_EQ(midpoint, expected_midpoint);
}

TEST(CoordinatesMath, MidpointHorizontal1) {
    const double lat = -10;
	Coordinates point1{Longitude{42}, Latitude{lat}}; 
	Coordinates point2{Longitude{84}, Latitude{lat}}; 
    auto midpoint = CoordinatesMath::midpoint(point1, point2);
    auto expected_midpoint = Coordinates{Longitude{63}, Latitude{-10.695563}};
	EXPECT_EQ(midpoint, expected_midpoint);
}

TEST(CoordinatesMath, MidpointHorizontal2) {
    const double lat = 35;
	Coordinates point1{Longitude{135}, Latitude{lat}}; 
	Coordinates point2{Longitude{45}, Latitude{lat}}; 
    auto midpoint = CoordinatesMath::midpoint(point1, point2);
    auto expected_midpoint = Coordinates{Longitude{90}, Latitude{44.719114}};
	EXPECT_EQ(midpoint, expected_midpoint);
}

TEST(CoordinatesMath, MidpointHorizontal3) {
    const double lat = 0;
	Coordinates point1{Longitude{20}, Latitude{lat}}; 
	Coordinates point2{Longitude{-160}, Latitude{lat}}; 
    auto midpoint = CoordinatesMath::midpoint(point1, point2);
    auto expected_midpoint = Coordinates{Longitude{-70}, Latitude{0}};
	EXPECT_EQ(midpoint, expected_midpoint);
}

TEST(CoordinatesMath, MidpointDiagonal) {
	Coordinates point1{Longitude{120}, Latitude{45}}; 
	Coordinates point2{Longitude{40}, Latitude{-45}}; 
    auto midpoint = CoordinatesMath::midpoint(point1, point2);
    auto expected_midpoint = Coordinates{Longitude{80}, Latitude{0}};
	EXPECT_EQ(midpoint, expected_midpoint);
}

TEST(CoordinatesMath, InitialBearingNorth) {
	Coordinates point1{Longitude{40}, Latitude{-65}}; 
	Coordinates point2{Longitude{40}, Latitude{5}}; 
    auto bearing = CoordinatesMath::initialBearing(point1, point2);
    auto expected_expected = 0;
	EXPECT_EQ(bearing, expected_expected);
}

TEST(CoordinatesMath, InitialBearingSouth) {
	Coordinates point1{Longitude{40}, Latitude{45}}; 
	Coordinates point2{Longitude{40}, Latitude{-45}}; 
    auto bearing = CoordinatesMath::initialBearing(point1, point2);
    auto expected_expected = 180;
	EXPECT_EQ(bearing, expected_expected);
}

TEST(CoordinatesMath, InitialBearingEast) {
    const double lat = 0;
	Coordinates point1{Longitude{40}, Latitude{lat}}; 
	Coordinates point2{Longitude{80}, Latitude{lat}}; 
    auto bearing = CoordinatesMath::initialBearing(point1, point2);
    auto expected_expected = 90;
	EXPECT_EQ(bearing, expected_expected);
}

TEST(CoordinatesMath, InitialBearingEast2) {
    const double lat = 0;
	Coordinates point1{Longitude{2}, Latitude{lat}}; 
	Coordinates point2{Longitude{-179}, Latitude{lat}}; 
    auto bearing = CoordinatesMath::initialBearing(point1, point2);
    auto expected_expected = 90;
	EXPECT_EQ(bearing, expected_expected);
}

TEST(CoordinatesMath, InitialBearingWest) {
    const double lat = 0;
	Coordinates point1{Longitude{0}, Latitude{lat}}; 
	Coordinates point2{Longitude{-179}, Latitude{lat}}; 
    auto bearing = CoordinatesMath::initialBearing(point1, point2);
    auto expected_expected = 270;
	EXPECT_EQ(bearing, expected_expected);
}

TEST(CoordinatesMath, InitialBearingDiagonal) {
    const std::size_t precision = 10;
	Coordinates point1{Longitude{32.553212}, Latitude{23.445111}}; 
	Coordinates point2{Longitude{-10.342237}, Latitude{67.123999}}; 
    Double<precision> bearing = CoordinatesMath::initialBearing(point1, point2);
    Double<precision> expected_bearing = 340.125719401638;
	EXPECT_EQ(bearing, expected_bearing);
}

TEST(CoordinatesMath, Wrap360) {
    EXPECT_EQ(CoordinatesMath::wrap360(0), 0);
    EXPECT_EQ(CoordinatesMath::wrap360(-1), 359);
}

TEST(CoordinatesMath, ClosestPointOnSegmentFirst) {
    Coordinates point{Longitude{40}, Latitude{10}}; 
    Coordinates seg_1{Longitude{50}, Latitude{0}}; 
    Coordinates seg_2{Longitude{120}, Latitude{0}}; 
	Coordinates closest = CoordinatesMath::closestPointOnSegment(point, seg_1, seg_2);
    Coordinates expected_closest{Longitude{50}, Latitude{0}};
    EXPECT_EQ(closest, expected_closest);
}

TEST(CoordinatesMath, ClosestPointOnSegmentAlong) {
    Coordinates point{Longitude{40}, Latitude{10}}; 
    Coordinates seg_1{Longitude{80}, Latitude{0}}; 
    Coordinates seg_2{Longitude{-60}, Latitude{0}}; 
	auto closest = CoordinatesMath::closestPointOnSegmentAndDistance(point, seg_1, seg_2);
    Coordinates expected_closest{Longitude{40}, Latitude{0}};
    EXPECT_EQ(closest.first, expected_closest);
    EXPECT_EQ(closest.first, CoordinatesMath::closestPointOnSegment(point, seg_2, seg_1));
    Meters dist1 = CoordinatesMath::distanceFromSegment(point, seg_2, seg_1);
    Meters dist2 = CoordinatesMath::calculateDistance(expected_closest, point);
    EXPECT_EQ(dist1, dist2);
}

TEST(CoordinatesMath, ClosestPointOnSegmentAlongMinus) {
    Coordinates point{Longitude{40}, Latitude{-10}}; 
    Coordinates seg_1{Longitude{80}, Latitude{0}}; 
    Coordinates seg_2{Longitude{-60}, Latitude{0}}; 
	auto closest = CoordinatesMath::closestPointOnSegmentAndDistance(point, seg_1, seg_2);
    Coordinates expected_closest{Longitude{40}, Latitude{0}};
    EXPECT_EQ(closest.first, expected_closest);
    EXPECT_EQ(closest.first, CoordinatesMath::closestPointOnSegment(point, seg_2, seg_1));
    Meters dist1 = CoordinatesMath::distanceFromSegment(point, seg_2, seg_1);
    Meters dist2 = CoordinatesMath::calculateDistance(expected_closest, point);
    EXPECT_EQ(dist1, dist2);
}

TEST(CoordinatesMath, ClosestPointOnSegmentSecond) {
    Coordinates point{Longitude{40}, Latitude{10}}; 
    Coordinates seg_1{Longitude{-60}, Latitude{0}}; 
    Coordinates seg_2{Longitude{0}, Latitude{0}}; 
	Coordinates closest = CoordinatesMath::closestPointOnSegment(point, seg_1, seg_2);
    Coordinates expected_closest{Longitude{0}, Latitude{0}};
    EXPECT_EQ(closest, expected_closest);
}

TEST(CoordinatesMath, ClosestPointOnSegmentSameFirst) {
    Coordinates point{Longitude{40}, Latitude{10}}; 
    Coordinates seg_1{Longitude{40}, Latitude{10}}; 
    Coordinates seg_2{Longitude{39.999998}, Latitude{10.000001}}; 
    auto closest = CoordinatesMath::closestPointOnSegmentAndDistance(point, seg_1, seg_2);
    Coordinates expected_closest{Longitude{40}, Latitude{10}};
    EXPECT_EQ(closest.first, expected_closest);
    EXPECT_EQ(closest.first, CoordinatesMath::closestPointOnSegment(point, seg_2, seg_1));
}

TEST(CoordinatesMath, ClosestPointOnSegmentSameFirst2) {
    Coordinates point{Longitude{-40}, Latitude{-10}}; 
    Coordinates seg_1{Longitude{-40}, Latitude{-10}}; 
    Coordinates seg_2{Longitude{-39.999998}, Latitude{-10.000001}}; 
	Coordinates closest = CoordinatesMath::closestPointOnSegment(point, seg_1, seg_2);
    Coordinates expected_closest{Longitude{-40}, Latitude{-10}};
    EXPECT_EQ(closest, expected_closest);
}

TEST(CoordinatesMath, ClosestPointOnSegmentSameSecond) {
    Coordinates point{Longitude{40}, Latitude{10}}; 
    Coordinates seg_1{Longitude{33}, Latitude{11}}; 
    Coordinates seg_2{Longitude{40}, Latitude{10}}; 
	Coordinates closest = CoordinatesMath::closestPointOnSegment(point, seg_1, seg_2);
    Coordinates expected_closest{Longitude{40}, Latitude{10}};
    EXPECT_EQ(closest, expected_closest);
}

TEST(CoordinatesMath, ClosestPointOnSegmentSameSecond2) {
    Coordinates point{Longitude{-40}, Latitude{-10}}; 
    Coordinates seg_1{Longitude{-33}, Latitude{-11}}; 
    Coordinates seg_2{Longitude{-40}, Latitude{-10}}; 
	Coordinates closest = CoordinatesMath::closestPointOnSegment(point, seg_1, seg_2);
    Coordinates expected_closest{Longitude{-40}, Latitude{-10}};
    EXPECT_EQ(closest, expected_closest);
}

TEST(CoordinatesMath, ClosestPointOnSegmentAndDistanceSecond) {
    Coordinates point{Longitude{40}, Latitude{10}}; 
    Coordinates seg_1{Longitude{-60}, Latitude{0}}; 
    Coordinates seg_2{Longitude{0}, Latitude{0}}; 
	auto closest = CoordinatesMath::closestPointOnSegmentAndDistance(point, seg_1, seg_2);
    Coordinates expected_closest{Longitude{0}, Latitude{0}};
    EXPECT_EQ(closest.first, expected_closest);
    EXPECT_EQ(closest.second, CoordinatesMath::calculateDistance(point, seg_2));    
    EXPECT_EQ(closest.second, CoordinatesMath::distanceFromSegment(point, seg_1, seg_2));
    EXPECT_EQ(closest.second, CoordinatesMath::distanceFromSegment(point, seg_2, seg_1));
}

TEST(CoordinatesMath, ClosestPointOnCircleInsideC) {
    Coordinates circle_center{Longitude{-20}, Latitude{30}}; 
    Meters circle_radius{2345.13};
    Coordinates c{Longitude{-20.001}, Latitude{30.002}};
	Coordinates closest = CoordinatesMath::closestPointOnCircle(c, circle_center, circle_radius);
    EXPECT_EQ(closest, c);
}

TEST(CoordinatesMath, ClosestPointOnCircleOutside) {
    Coordinates circle_center{Longitude{-20}, Latitude{30}}; 
    Meters circle_radius{22.23};
    Coordinates c{Longitude{-30.001}, Latitude{44.002}};
	Coordinates closest = CoordinatesMath::closestPointOnCircle(c, circle_center, circle_radius);
    auto dist_1 = CoordinatesMath::calculateDistance(circle_center, closest);
    auto dist_2 = CoordinatesMath::calculateDistance(c, closest);
    auto dist_3 = CoordinatesMath::calculateDistance(c, circle_center);
    EXPECT_EQ(CoordinatesMath::calculateDistance(circle_center, closest), Meters{22.23});
    EXPECT_EQ(dist_1 + dist_2, dist_3);

}

TEST(CoordinatesMath, ClosestPointOnCircleOutside2) {
    Coordinates circle_center{Longitude{20}, Latitude{0}}; 
    Meters circle_radius{22.01};
    Coordinates c{Longitude{-160}, Latitude{0}};
	Coordinates closest = CoordinatesMath::closestPointOnCircle(c, circle_center, circle_radius);
    auto dist_1 = CoordinatesMath::calculateDistance(circle_center, closest);
    auto dist_2 = CoordinatesMath::calculateDistance(c, closest);
    auto dist_3 = CoordinatesMath::calculateDistance(c, circle_center);
    EXPECT_EQ(CoordinatesMath::calculateDistance(circle_center, closest), Meters{22.01});
    EXPECT_EQ(dist_1 + dist_2, dist_3);
}

TEST(CoordinatesMath, ClosestPointOnCircleOutside3) {
    Coordinates circle_center{Longitude{100}, Latitude{0}};
    Coordinates antipodal_point{Longitude{-80}, Latitude{0}}; 
    Meters circle_radius{2.01};
	Coordinates closest = CoordinatesMath::closestPointOnCircle(antipodal_point, circle_center, circle_radius);
    auto dist_1 = CoordinatesMath::calculateDistance(circle_center, closest);
    auto dist_2 = CoordinatesMath::calculateDistance(antipodal_point, closest);
    auto dist_3 = CoordinatesMath::calculateDistance(antipodal_point, circle_center);
    EXPECT_EQ(CoordinatesMath::calculateDistance(circle_center, closest), circle_radius);
    EXPECT_EQ(dist_1 + dist_2, dist_3);
    EXPECT_EQ(Meters{2 * CoordinatesMath::half_earth_hemisphere}, dist_3); // antipodal_point

}

TEST(CoordinatesMath, Intersection) {
    Coordinates north{Longitude{30}, Latitude{80}};
    Coordinates south{Longitude{30}, Latitude{-80}};
    Coordinates west{Longitude{80}, Latitude{0}};
    Coordinates east{Longitude{-20}, Latitude{0}};
    auto intersection_point = CoordinatesMath::intersection(north, south, west, east);
    Coordinates expected_intersection_point{Longitude{30}, Latitude{0}};
    EXPECT_EQ(*intersection_point, expected_intersection_point);
}

TEST(CoordinatesMath, IntersectionWithMeridian) {
    Coordinates west{Longitude{80}, Latitude{0}};
    Coordinates east{Longitude{-20}, Latitude{0}};
    Latitude intersection_lat = CoordinatesMath::intersectionOnMeridian(west, east, Longitude{60});
    EXPECT_EQ(intersection_lat, Latitude{0});
    intersection_lat = CoordinatesMath::intersectionOnMeridian(east, west, Longitude{60});
    EXPECT_EQ(intersection_lat, Latitude{0});
}

TEST(CoordinatesMath, ClosestPointOnSegmentSameFirst3) {
    //From: https://stackoverflow.com/questions/32771458/distance-from-lat-lng-point-to-minor-arc-segment/35587935#35587935
    Coordinates p3{Longitude{-62.5}, Latitude{-10.5}}; 
    Coordinates p1{Longitude{-55.5}, Latitude{-10.1}}; 
    Coordinates p2{Longitude{-45.1}, Latitude{-15.2}}; 
    auto closest = CoordinatesMath::closestPointOnSegmentAndDistance(p3, p1, p2);    
    EXPECT_EQ(closest.first, p1);
    EXPECT_EQ(closest.second, Meters{767094.77});
}

TEST(CoordinatesMath, ClosestPointOnSegmentSecond2) {
    //From: https://stackoverflow.com/questions/32771458/distance-from-lat-lng-point-to-minor-arc-segment/35587935#35587935
    Coordinates p3{Longitude{42.0}, Latitude{25.0}}; 
    Coordinates p1{Longitude{35.61}, Latitude{21.72}}; 
    Coordinates p2{Longitude{40.70}, Latitude{23.65}}; 
    auto closest = CoordinatesMath::closestPointOnSegmentAndDistance(p3, p1, p2);    
    EXPECT_EQ(closest.first, p2);
    EXPECT_EQ(closest.second, Meters{199706.84});
}

TEST(CoordinatesMath, ClosestPointOnSegmentAlong2) {
    //From: https://stackoverflow.com/questions/32771458/distance-from-lat-lng-point-to-minor-arc-segment/35587935#35587935
    Coordinates p3{Longitude{69.0}, Latitude{51.0}}; 
    Coordinates p1{Longitude{60.5}, Latitude{40.5}}; 
    Coordinates p2{Longitude{80.5}, Latitude{50.5}}; 
    auto closest = CoordinatesMath::closestPointOnSegmentAndDistance(p3, p1, p2);  
    Coordinates expected_closest{Longitude{72.812256}, Latitude{47.475867}};
    EXPECT_EQ(closest.first, expected_closest);
    EXPECT_EQ(closest.second, Meters{479609.3});
    EXPECT_EQ(closest.second, CoordinatesMath::calculateDistance(closest.first, p3));   
}

TEST(CoordinatesMath, ClosestPointOnSegmentAlong4) {
    //From: https://moodle.tau.ac.il/mod/forum/discuss.php?d=42927
    Coordinates coord{Longitude{40.731437}, Latitude{-73.996967}}; 
    Coordinates c1{Longitude{40.732254}, Latitude{-73.996394}}; 
    Coordinates c2{Longitude{40.733566}, Latitude{-73.999446}}; 
    auto closest = CoordinatesMath::closestPointOnSegmentAndDistance(coord, c1, c2);    
    Coordinates expected_closest{Longitude{40.732486}, Latitude{-73.996933}}; 
    EXPECT_EQ(closest.first, expected_closest);
    EXPECT_EQ(closest.second, CoordinatesMath::calculateDistance(closest.first, coord));
}

TEST(CoordinatesMath, IntermediatePointMidpoint) { 
    Coordinates c1{Longitude{40.732254}, Latitude{-73.996394}}; 
    Coordinates c2{Longitude{40.733566}, Latitude{-73.999446}}; 
    EXPECT_EQ(CoordinatesMath::intermediatePoint(c1, c2, 0.5), CoordinatesMath::midpoint(c1, c2));
}

TEST(CoordinatesMath, ClosestPointOnSegmentRelativeBearing) {
    //From: https://stackoverflow.com/questions/32771458/distance-from-lat-lng-point-to-minor-arc-segment/35587935#35587935
    Coordinates p3{Longitude{-0.5}, Latitude{80.0}};
    Coordinates p1{Longitude{0}, Latitude{10}};  
    Coordinates p2{Longitude{0.5}, Latitude{80.0}}; 
    auto closest = CoordinatesMath::closestPointOnSegmentAndDistance(p3, p1, p2);    
	Coordinates expected_coordinates{Longitude{0.499995}, Latitude{79.999904}};
   	EXPECT_EQ(closest.first, expected_coordinates);
    auto dist = CoordinatesMath::calculateDistance(closest.first, p3);
    EXPECT_EQ(dist, closest.second);
}

TEST(CoordinatesMath, PoleSingularity) {
    Coordinates s_pole2{Longitude{100.2222}, Latitude{-90}}; 
    Coordinates s_pole1{Longitude{120.12}, Latitude{-90}}; 
    EXPECT_EQ(CoordinatesMath::calculateDistance(s_pole1, s_pole2), Meters{0});
    Coordinates n_pole1{Longitude{0.312}, Latitude{90}}; 
    Coordinates n_pole2{Longitude{-20.3321}, Latitude{90}}; 
    EXPECT_EQ(CoordinatesMath::calculateDistance(n_pole1, n_pole2), Meters{0});
}

TEST(CoordinatesMath, HemisphereDistance) {
    Coordinates s_pole1{Longitude{100.2222}, Latitude{-90}}; 
    Coordinates s_pole2{Longitude{120.12}, Latitude{-90}}; 
    Coordinates n_pole1{Longitude{0.312}, Latitude{90}}; 
    Coordinates n_pole2{Longitude{-20.3321}, Latitude{90}}; 
    Coordinates equator1{Longitude{100}, Latitude{0}}; 
    Coordinates equator2{Longitude{-80}, Latitude{0}}; 
    EXPECT_EQ(CoordinatesMath::calculateDistance(s_pole1, n_pole2), Meters{2 * CoordinatesMath::half_earth_hemisphere});
    EXPECT_EQ(CoordinatesMath::calculateDistance(n_pole1, s_pole2), Meters{2 * CoordinatesMath::half_earth_hemisphere});
    EXPECT_EQ(CoordinatesMath::calculateDistance(equator1, equator2), Meters{2 * CoordinatesMath::half_earth_hemisphere});
}

TEST(CoordinatesMath, HemisphereDistance2) {
    const double lng=48.853701;
    const double lat=2.292292;
    Coordinates point{Longitude{lng}, Latitude{lat}};
    Coordinates antipodal_point{Longitude{lng-180}, Latitude{-lat}}; 
    EXPECT_EQ(CoordinatesMath::calculateDistance(point, antipodal_point), Meters{2 * CoordinatesMath::half_earth_hemisphere});
}

TEST(CoordinatesMath, HalfHemisphereDistance) {
    Coordinates s_pole1{Longitude{100.3321}, Latitude{-90}}; 
    Coordinates n_pole1{Longitude{30.3321}, Latitude{90}}; 
    Coordinates equator{Longitude{-20.3321}, Latitude{0}}; 
    EXPECT_EQ(CoordinatesMath::calculateDistance(s_pole1, equator), Meters{CoordinatesMath::half_earth_hemisphere});
    EXPECT_EQ(CoordinatesMath::calculateDistance(n_pole1, equator), Meters{CoordinatesMath::half_earth_hemisphere});
}

TEST(CoordinatesMath, coordinatesByBearingAndDistance) {
    Coordinates s_pole{Longitude{100.3321}, Latitude{-90}}; 
    auto expected_n_pole = CoordinatesMath::coordinatesByBearingAndDistance(s_pole, 0, Meters{ 2 * CoordinatesMath::half_earth_hemisphere});
    EXPECT_EQ(expected_n_pole.latitude(), CoordinatesMath::n_pole_lat);
    Coordinates n_pole{Longitude{-10.3321}, Latitude{90}}; 
    auto expected_s_pole = CoordinatesMath::coordinatesByBearingAndDistance(n_pole, 180, Meters{ 2 * CoordinatesMath::half_earth_hemisphere});
    EXPECT_EQ(expected_s_pole.latitude(), CoordinatesMath::s_pole_lat);
    auto expected_equator1 = CoordinatesMath::coordinatesByBearingAndDistance(n_pole, 180, Meters{ CoordinatesMath::half_earth_hemisphere});
    EXPECT_EQ(expected_equator1.latitude(), CoordinatesMath::equator_lat);
    auto expected_equator2 = CoordinatesMath::coordinatesByBearingAndDistance(s_pole, 0, Meters{ CoordinatesMath::half_earth_hemisphere});
    EXPECT_EQ(expected_equator2.latitude(), CoordinatesMath::equator_lat);
    const double lng=48.853701;
    const double lat=2.292292;
    Coordinates point{Longitude{lng}, Latitude{lat}};
    auto other = CoordinatesMath::coordinatesByBearingAndDistance(point, 0, Meters{ 2 * CoordinatesMath::half_earth_hemisphere});
    EXPECT_EQ(other.latitude(), Latitude{-lat});
    EXPECT_EQ(point, CoordinatesMath::coordinatesByBearingAndDistance(other, 0, Meters{ 2 * CoordinatesMath::half_earth_hemisphere}));
}
