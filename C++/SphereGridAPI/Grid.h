//idandaniel1 205999014

#pragma once

#include "GISNamedTypes.h"
#include <vector>
#include "ranges"
#include "unordered_map"
#include "string"
#include "memory"
#include "CoordinatesMath.h"
#include "exception"
#include <iterator> // For std::forward_iterator_tag
#include <cstddef>  // For std::ptrdiff_t

using std::vector;
using std::unordered_map;
using std::string;
using std::unique_ptr;
using std::make_unique;
using std::exception;
using std::move;



// Two concepts required by the below functions:

template<class Me, class Other> concept derived_or_same =
    std::same_as<Me, Other> || std::derived_from<Me, Other>;

template<class Me, class Other> concept concrete_derived_or_same =
	!std::is_abstract_v<Me> && derived_or_same<Me, Other>;

// class Grid
template<typename Entity, std::size_t num_rows> requires (num_rows > 0)
class Grid {


    // private inner class Cell
    class Cell {
        friend class Grid;
        Coordinates location;
        vector<Entity*> cellEntites;
        mutable unordered_map<string,vector<Entity*>> entitesByTypes;

        //used with limit and without
        template<typename ActualT, typename PredicateFunc> requires concrete_derived_or_same<ActualT, Entity>
        std::vector<ActualT*> generalGetEntities(PredicateFunc&& func, std::size_t limit, bool isLimit) const {
            vector<ActualT*> reEntities;
            if(entitesByTypes.contains(typeid(ActualT).name())){//if there are any entities with the type of ActualT
                for(const auto& entity : entitesByTypes.at(typeid(ActualT).name())){//go over all entities with said type
                    if(isLimit && reEntities.size()==limit){//if we care about the limit and reached it: break
                        break;
                    }
                    if(func(*((ActualT*)entity))){//add to reEntities if PredicateFunc return true on it
                        reEntities.push_back((ActualT*)entity);
                    }
                }
            }
            return reEntities;
        }

        template<typename ActualT> requires concrete_derived_or_same<ActualT, Entity>
        void addToCell(ActualT& e){
            string entityType = typeid(ActualT).name();
            cellEntites.push_back(&e);
            if(!entitesByTypes.contains(entityType)){
                entitesByTypes.insert({entityType,vector<Entity*>()});
            }
            (entitesByTypes[entityType]).push_back(&e);
            return;
        }

    public:
        // Cell’s Constructors and Assignment:
        // There are no constructors that are required by the API. You should decide whether to implement, block or rely on the default for copy and move constructors, as well as for the assignment operators.

        Cell(Coordinates _location):location(_location){}

        //use the default  for copy, move constructors and assignment operators.

        // Cell’s Destructor:
        // You should decide whether there is a need to implement a user defined destructor.
        //no need for a destructor

        // (Cell::A1) Getting Entities from a Cell:
        // returns all entities in that cell, which return true for the PredicateFunc.
        template<typename PredicateFunc>
        std::vector<Entity*> getEntities(PredicateFunc&& func) const {
            vector<Entity*> reEntities;
            for(const auto &entity: cellEntites){
                if(func(*entity)){
                    reEntities.push_back(entity);
                }
            }
            return reEntities;
        }

        // (Cell::B1) 
        // Getting specific type of Entities from a Cell:
        // returns all entities of type ActualT which return true for the PredicateFunc.
        template<typename ActualT, typename PredicateFunc> requires concrete_derived_or_same<ActualT, Entity>
        std::vector<ActualT*> getEntities(PredicateFunc&& func) const {
            return generalGetEntities<ActualT,PredicateFunc>(func,0, false);
        }

        // (Cell::B2) 
        // same as (Cell::B1) above but with a limit on the number of returned entities (up to limit entities).
        template<typename ActualT, typename PredicateFunc> requires concrete_derived_or_same<ActualT, Entity>
        std::vector<ActualT*> getEntities(PredicateFunc&& func, std::size_t limit) const {
            return generalGetEntities<ActualT,PredicateFunc>(func,limit, true);
        }

        // (Cell::B3) 
        // returns a range of all entities of type ActualT.
        // Complexity of this function is required to be O(1)
        // This function returns a view that is updated “behind the scene” automatically in case additional objects of type ActualT are added to this Cell via the Grid. Order of entities in the view shall be the same as the order of their insertion to the grid.
        // Iterating over the returned view generates pointers with the correct type, i.e. ActualT*.
        template<typename ActualT> requires concrete_derived_or_same<ActualT, Entity>
        std::ranges::sized_range auto getEntitiesView() const {
            // helper class
            class view_to_vector_of_pointers {
                std::vector<Entity*>& vec;
            public:
                struct pointers_view_iterator: public std::vector<Entity*>::iterator {
                    pointers_view_iterator() {}
                    pointers_view_iterator(typename vector<Entity*>::iterator itr): vector<Entity*>::iterator(itr) {}
                    ActualT* operator*() { return static_cast<ActualT*>(this->std::vector<Entity*>::iterator::operator*()); }
                    const ActualT* operator*() const { return static_cast<const ActualT*>(this->std::vector<Entity*>::iterator::operator*()); }
                    pointers_view_iterator& operator++() {
                        this->std::vector<Entity*>::iterator::operator++();
                        return *this;
                    }
                    pointers_view_iterator operator++(int) {
                        auto old = *this;
                        ++(*this);
                        return old;
                    }
                };
                view_to_vector_of_pointers(std::vector<Entity*>& vec): vec(vec) {}
                auto begin() const { return pointers_view_iterator { vec.begin() }; }
                auto end() const { return pointers_view_iterator { vec.end() }; }
                std::size_t size() const { return vec.size(); }
            };
            return view_to_vector_of_pointers{ entitesByTypes[typeid(ActualT).name()] };
        }
   
        // Additional auxiliary functions:

        // (Cell::C1) 
        std::size_t numEntities() const noexcept {
            return cellEntites.size();
        }

        // (Cell::C2) 
        // NOTE: complexity required to be O(1)
        template<typename ActualT> requires concrete_derived_or_same<ActualT, Entity>
        std::size_t numEntities() const noexcept {
            if(entitesByTypes.contains(typeid(ActualT).name())){
                vector<Entity*> entities = entitesByTypes.at(typeid(ActualT).name());
                return entities.size();
            }
            return 0;
        }

        // Iterators begin and end:
        // The Cell would have begin and end iterators for iterating over all pointers to Entity. Retrieved pointers are non-const, i.e. the user can call non-const methods on the retrieved entities. There is no defined order for the iteration. Iteration itself shall not create any copies.
        auto begin() const noexcept {
            return cellEntites.begin();
        }

        auto end() const noexcept {
            return cellEntites.end();
        }
    };

    //private inner Row Class
    class Row{
        vector<unique_ptr<Cell>> rowCells;
        size_t cellWidth;
    public:
        Row(size_t rowNumber,double cellDegreeHeight){
            //calculate number of cells at row
            Latitude floorLatitude=(Latitude)(rowNumber*cellDegreeHeight - 90);
            Latitude ceilLatitude=(Latitude)(floorLatitude->roundToPrecision()+cellDegreeHeight);
            Meters floorPerimeter=CoordinatesMath::perimeterOnLatitude(floorLatitude);
            Meters ceilPerimeter=CoordinatesMath::perimeterOnLatitude(ceilLatitude);
            Meters avg = (floorPerimeter+ceilPerimeter)/2;
            Meters CellMeterHeight=CoordinatesMath::calculateDistance(Coordinates(Longitude(0),ceilLatitude),Coordinates(Longitude(0),floorLatitude));
            size_t numOfCells =  ceil(avg/CellMeterHeight);

            cellWidth=360/numOfCells;

            //make the cells
            for(size_t i=0;i<numOfCells;i++){
                Longitude longitude=(Longitude)(i*cellWidth - 180);
                rowCells.push_back(make_unique<Cell>(Cell(Coordinates(longitude,(Latitude)floorLatitude))));
            }
        }


        size_t getNumOfCells(){return rowCells.size();};

        size_t getCellWidth(){return cellWidth;};

        Cell * getCell(size_t colNum){return rowCells.at(colNum).get();};

        auto begin() const noexcept {
            return rowCells.begin();
        }

        auto end() const noexcept {
            return rowCells.end();
        }
    };

    //Grid arguments
    vector<unique_ptr<Row>> grid;
    double cellDegreeHeight;


    //uses for add and getCellAt(here we do not return const so we can use for add)
    Cell* getCell(Coordinates c) const {
        Latitude latitude = c.latitude();
        Longitude longitude = c.longitude();
        if(latitude->roundToPrecision()<-90 || latitude->roundToPrecision()>90
           || longitude->roundToPrecision()<-180 || longitude->roundToPrecision()>180){
            throw std::out_of_range("Latitude should be between -90 and 90 and Longitude between -180 and 180");
        }

        size_t rowNum = floor((latitude->roundToPrecision()+90)/cellDegreeHeight);
        //on coordinates between two cells we need the bottom one
        if(std::fmod(latitude->roundToPrecision(),cellDegreeHeight)==0 && latitude->roundToPrecision()!=-90){
            rowNum--;
        }

        Row * row=grid.at(rowNum).get();

        size_t colNum=floor((longitude->roundToPrecision()+180)/row->getCellWidth());
        if(longitude->roundToPrecision()==180){//180 is like -180(we are going to the right)
            colNum=0;
        }

        return row->getCell(colNum);
    }

public:
    // Grid’s Constructors and Assignment:
    // There are no constructors that are required by the API. You should decide whether to implement, block or rely on the default for copy and move constructors, as well as for the assignment operators.

    Grid():cellDegreeHeight((double)180/num_rows){

        for(size_t i=0; i<num_rows;i++){
            grid.push_back(make_unique<Row>(Row(i,cellDegreeHeight)));
        }
    }

    Grid(const Grid& grid)=delete;
    Grid(const Grid&& grid)=delete;
    Grid& operator=(Grid& other)=delete;
    Grid& operator=(Grid&& other)=delete;


    // Grid’s Destructor:
    // You should decide whether there is a need to implement a user defined destructor.
    //no need for a destructor

    //public iterator class
    class iterator{
        friend class Grid;
        typename vector<unique_ptr<Row>>::const_iterator row_end_itr;
        typename vector<unique_ptr<Row>>::const_iterator row_itr;
        typename vector<unique_ptr<Cell>>::const_iterator cell_itr;

        //private ctors
        iterator(const Grid& g)
                :row_end_itr(g.grid.end()),row_itr(g.grid.begin()),cell_itr(((*row_itr).get())->begin()){};

        iterator(const Grid& g,int)
                :row_end_itr(g.grid.end()),row_itr(g.grid.end()),cell_itr({}){};


    public:
        using iterator_category = std::forward_iterator_tag;
        using difference_type   = std::ptrdiff_t;
        using value_type = unique_ptr<Cell>;
        using reference = unique_ptr<Cell>&;
        using pointer = unique_ptr<Cell>*;

        bool operator!=(iterator other) const {
            return row_itr != other.row_itr || cell_itr != other.cell_itr;
        }

        iterator& operator++() {
            ++cell_itr;
            if(cell_itr == ((*row_itr).get())->end()) {
                ++row_itr;
                if(row_itr!=row_end_itr){
                    cell_itr=((*row_itr).get())->begin();
                }
                else{
                    cell_itr={};
                }
            }
            return *this;
        }

        iterator operator++(int) { iterator tmp = *this; ++(*this); return tmp; }

        const Cell& operator*() {
            return *((*cell_itr).get());
        }
    };

    // Adding Entities to the Grid:

    // (Grid::A1) 
    template<typename ActualT> requires derived_or_same<ActualT, Entity>
    const Cell* add(Coordinates c, ActualT& e) {

        Cell * addCell=getCell(c);

        addCell->addToCell(e);

        return addCell;
    }

    // Getting Cells from the Grid:

    // (Grid::B1) 
    const Cell* getCellAt(Coordinates c) const {
        return getCell(c);
    }

    // (Grid::B2) (BONUS: you may choose not to implemnt this function)
    std::vector<const Cell*> getCellsAt(Coordinates center, Meters radius) const {
        //not implementing
        (void) center; 
        (void) radius; 
        return std::vector<const Cell*>{};
    }

    // additional auxiliary functions:

    // (Grid::C1) 
    std::size_t numRows() const noexcept {
        return num_rows;
    } 
    
    // (Grid::C2) 
    std::size_t numCols(Coordinates c) const noexcept {
        Latitude latitude=c.latitude();
        if(latitude->roundToPrecision()<-90 || latitude->roundToPrecision()>90){//we only care about latitude here
            //can't throw exception so print error and return -1
            std::cout<<"Latitude should be between -90 and 90"<<std::endl;
            return -1;
        }

        size_t rowNum = floor((latitude->roundToPrecision()+90)/cellDegreeHeight);

        Row * row=grid.at(rowNum).get();

        size_t numOfCells=row->getNumOfCells();

        return numOfCells;
    }

    // (Grid::C3) 
    std::size_t numCells() const noexcept {
        size_t sum=0;
        for(const auto& row:grid){
            sum+=row->getNumOfCells();
        }
        return sum;
    }

    // iterators begin and end:
    // The Grid would have a const version of begin and end iterators for iterating over all Cells. There is no defined order for the iteration. The iteration itself shall not create any copies.    
    
    // Set this to true if you are implementing a sparse grid.
    static constexpr bool is_sparse = false;  
 
    // Following would iterate over:
    // 1. Only non empty Cells, if is_sparse==true 
    // 2. All Cells, if is_sparse==false

    auto begin() const noexcept {
        return iterator{*this};
    }

    auto end() const noexcept {
        return iterator{*this,-1};
    }
};
