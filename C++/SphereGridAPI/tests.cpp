#include "gtest/gtest.h"
#include "Grid.h"
#include "CoordinatesMath.h"
#include <cassert>

using std::cout;

struct A {
    virtual ~A() {}
    virtual void foo() const = 0;
};
struct B : A { void foo() const override {} };
struct C : A { void foo() const override {} };
struct D : C { void foodie() const {} };

bool pred(const A& a){
    a.foo();
    return true;
}

bool predC(const C& c){
    c.foo();
    return true;
}

bool predCFlase(const C& c){
    c.foo();
    return false;
}

TEST(GISEx4, DocTest) {

    Coordinates coord{Longitude{20}, Latitude{30}};
    Grid<A, 1917> grid;
    const auto cell_ptr = grid.getCellAt(coord);
    auto viewD = cell_ptr->getEntitiesView<D>(); // O(1), always, even for size == N
    EXPECT_EQ(viewD.size(), 0ul); // assume passed successfully

    // adding a D object to the Grid, such that it should be added to the cell at coord  
    D dees[4];
    const auto cell_ptr2 = grid.add(coord, dees[0]);
    EXPECT_EQ(viewD.size(), 1ul); // the new object shall be in the view
    EXPECT_EQ(cell_ptr2, cell_ptr);

    // adding 3 D objects to the Grid, such that it should be added to the cell at coord  
    for(int i=1; i<4; ++i) {
        EXPECT_EQ(grid.add(coord, dees[i]), cell_ptr);
    }
    
    EXPECT_EQ(viewD.size(), 4ul); // the new objects shall be in the view

    int i=0;
    for(D* pd: viewD) { // correct type
        pd->foodie();
        EXPECT_EQ(pd, &dees[i]);
        ++i;
    }
}

TEST(GISEx4,AddAndGetTest){

    Coordinates coord{Longitude{20}, Latitude{30}};
    Grid<A, 2000> grid;
    const auto cell_ptr = grid.getCellAt(coord);
    D dees[4];

    for(int i=0; i<4; ++i) {
        EXPECT_EQ(grid.add(coord, dees[i]), cell_ptr);
    }

    EXPECT_EQ(cell_ptr->numEntities(),(size_t)4);
    EXPECT_EQ(cell_ptr->numEntities<D>(),(size_t)4);
    EXPECT_EQ(cell_ptr->numEntities<B>(),(size_t)0);

    B bees[4];

    for(int i=0; i<4; ++i) {
        EXPECT_EQ(grid.add(coord, bees[i]), cell_ptr);
    }

    EXPECT_EQ(cell_ptr->numEntities(),(size_t)8);
    EXPECT_EQ(cell_ptr->numEntities<D>(),(size_t)4);
    EXPECT_EQ(cell_ptr->numEntities<B>(),(size_t)4);
}


TEST(GISEx4,IteratorTest){

    Grid<A, 200> grid;

    size_t sum1=0;
    size_t sum2=0;
    size_t sum3=0;


    for(const auto& p : grid) {
        p.numEntities();
        sum1++;
    }
    EXPECT_EQ(sum1,grid.numCells());

    for(auto it=grid.begin();it!=grid.end();++it){
        sum2++;
    }
    EXPECT_EQ(sum2,grid.numCells());

    for(auto it=grid.begin();it!=grid.end();it++){
        sum3++;
    }
    EXPECT_EQ(sum3,grid.numCells());
}

TEST(GISEx4,NumTests){

    Grid<A, 2> grid;

    EXPECT_EQ(grid.numRows(),(size_t)2);

    Coordinates coord{Longitude{45}, Latitude{80}};

    EXPECT_EQ(grid.numCols(coord),(size_t)2);

    Coordinates coord2{Longitude{27.89}, Latitude{-80}};

    EXPECT_EQ(grid.numCols(coord2),(size_t)2);
}

TEST(GISEx4,EdgesTests){

    Grid<A, 10> grid;

    Coordinates coord1{Longitude{180}, Latitude{80}};
    Coordinates coord2{Longitude{-180}, Latitude{80}};

    const auto cell_ptr1 = grid.getCellAt(coord1);
    const auto cell_ptr2 = grid.getCellAt(coord2);

    EXPECT_EQ(cell_ptr1,cell_ptr2);

    Coordinates coord3{Longitude{46}, Latitude{-90}};
    Coordinates coord4{Longitude{46}, Latitude{-72}};

    const auto cell_ptr3 = grid.getCellAt(coord3);
    const auto cell_ptr4 = grid.getCellAt(coord4);

    EXPECT_EQ(cell_ptr3,cell_ptr4);

    Coordinates coord5{Longitude{46}, Latitude{-71}};

    const auto cell_ptr5 = grid.getCellAt(coord5);

    EXPECT_FALSE(cell_ptr4==cell_ptr5);

    Coordinates coord6{Longitude{-90}, Latitude{-90}};
    Coordinates coord7{Longitude{-89}, Latitude{-90}};

    const auto cell_ptr6 = grid.getCellAt(coord6);
    const auto cell_ptr7 = grid.getCellAt(coord7);

    EXPECT_EQ(cell_ptr6,cell_ptr7);
}

TEST(GISEx4,CellGetEntitiesTest){

    Coordinates coord{Longitude{45}, Latitude{80}};
    Grid<A, 2000> grid;
    const auto cell_ptr = grid.getCellAt(coord);
    B bees[2000];
    C cees[100];

    for(int i=0; i<2000; ++i) {
        grid.add(coord,bees[i]);
    }
    for(int i=0; i<100; ++i) {
        grid.add(coord,cees[i]);
    }


    vector<A*> entities=cell_ptr->getEntities(pred);

    EXPECT_EQ(entities.size(),(size_t)2100);

    vector<C*> entitiesC=cell_ptr->getEntities<C>(pred);

    EXPECT_EQ(entitiesC.size(),(size_t)100);

    vector<C*> entitiesC2=cell_ptr->getEntities<C>(predC);

    EXPECT_EQ(entitiesC2.size(),(size_t)100);

    vector<C*> entitiesC3=cell_ptr->getEntities<C>(predCFlase);

    EXPECT_EQ(entitiesC3.size(),(size_t)0);

    vector<C*> entitiesC4=cell_ptr->getEntities<C>(predC,50);

    EXPECT_EQ(entitiesC4.size(),(size_t)50);

    vector<C*> entitiesC5=cell_ptr->getEntities<C>(predC,200);

    EXPECT_EQ(entitiesC5.size(),(size_t)100);
}

TEST(GISEx4,CellIteratorTest){

    Coordinates coord{Longitude{45}, Latitude{80}};
    Grid<A, 2000> grid;
    const auto cell_ptr = grid.getCellAt(coord);
    B bees[2000];
    C cees[100];

    for(int i=0; i<2000; ++i) {
        grid.add(coord,bees[i]);
    }
    for(int i=0; i<100; ++i) {
        grid.add(coord,cees[i]);
    }

    size_t sum=0;

    for(const auto& entity:*cell_ptr){
        entity->foo();
        sum++;
    }

    EXPECT_EQ(sum, cell_ptr->numEntities());

    EXPECT_EQ((size_t)2000, cell_ptr->numEntities<B>());
    EXPECT_EQ((size_t)100, cell_ptr->numEntities<C>());
}



