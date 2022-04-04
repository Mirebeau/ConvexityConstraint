//
//  PrincipalAgent_3_Test.h
//  CGalTest
//
//  Created by Jean-Marie Mirebeau on 13/02/2015.
//  Copyright (c) 2015 Jean-Marie Mirebeau. All rights reserved.
//

#ifndef CGalTest_PrincipalAgent_3_Test_h
#define CGalTest_PrincipalAgent_3_Test_h

#include "Headers/Geometry_3.h"
#include "Headers/ConvexityConstraint_3.h"
#include "Headers/PrincipalAgent_3.h"
#include "Headers/ElementaryShapes_3.h"
#include <fstream>

namespace PrincipalAgent_3_Test {
    using namespace Geometry_3;
	typedef std::vector<CGT::Full_point> PointSet;

    
	/**
	 Three dimensional monopolist problem.
	 Minimise
	 int_Omega u + (1/2) |grad u- x|^2
	 subject to
	 u non-negative, convex.
	 
	 Input : a point set, as generated by MakeShape, and an identifier for the output.
	 Output : written to files.

	 !! TODO !! Add constraints for the boundary facets.
	 !! TODO !! Add a few more elementary shapes.
	 
	 */
    void Monopolist(const PointSet & pts, std::string filename){
        
        //Principal agent problem in 3D.
        std::ofstream os;
        
        NS::VectorType xx(pts.size());
        for(int i=0; i<pts.size(); ++i)
            xx[i]=CGT::Parabola( pts[i].first.point() );
        
        // Setup constraints and objective
        
        PositivityConstraint pos;
        ConvexityConstraint cvx(pts);
        std::vector<NS::Functionnal*> constraints = {&cvx,&pos};
        
        ScalarType val = cvx.Value(xx);
        PrincipalAgent pa(cvx.rt);

        /*{
            std::ostream & os = std::cout
            ExportVarArrow(val)
            ExportArrayArrow(cvx.values)
            ExportVarArrow(pa)
//            ExportVarArrow(cvx)
            << "\n";
        }*/
        
//        return;
        

        // Setup Newton
        
        NS::NewtonConstrained newton;
        newton.maxIter=50;
        
        newton.multiplier = 1./pts.size();
        //newton.multiplierBound = eps*newton.multiplier;
        newton.multiplierBound = 4e-7; //1.5e-05;
        
        
        newton.Solve(pa,xx,constraints);
        
        cvx.Compute(31);
		
		os.open(filename);
        os << "{"
        ExportVarArrow(pa)
        ExportVarArrow(newton)
        ExportVarArrow(cvx)
        << "}";
        
    }
    
    
    void Tetra0(){
        
        using CGT::Weighted_point;
        using CGT::Point;
        using CGT::InfoType;
        // Test basis function gradients on a simplex.
        int counter=0;
        std::vector<CGT::Full_point> pts =
        {
            {Weighted_point(Point(1,0,0),0),InfoType(counter++,1)},
            {Weighted_point(Point(0,1,0),0),InfoType(counter++,1)},
            {Weighted_point(Point(0,0,1),0),InfoType(counter++,1)},
            {Weighted_point(Point(0,0,0),0),InfoType(counter++,1)}
        };
        
        CGT::RT rt(pts.begin(), pts.end());
        std::array<CGT::Vector,4 > grads;
        std::array<int,4> indices;
        ScalarType volume;
        
        CGT::Cell_handle fh = rt.finite_cells_begin();
        for(int i=0; i<4; ++i) indices[i]=fh->vertex(i)->info().index;
        
        const ScalarType degeneracy =
        CGT::Degeneracy(fh, volume, grads);
        
        std::ostream & os = std::cout
        ExportArrayArrow(grads)
        ExportVarArrow(volume)
        ExportVarArrow(degeneracy)
        ExportArrayArrow(indices)
        << "\n";
    }
    
    void Octa0(){
        using CGT::Weighted_point;
        using CGT::Point;
        using CGT::InfoType;
        // Test basis function gradients on a simplex.
        int counter=0;
        std::vector<CGT::Full_point> pts =
        {
            {Weighted_point(Point(0,0,0),0),InfoType(counter++,0)},
            {Weighted_point(Point(1,0,0),0),InfoType(counter++,1)},
            {Weighted_point(Point(0,1,0),0),InfoType(counter++,1)},
            {Weighted_point(Point(0,0,1),0),InfoType(counter++,1)},
            {Weighted_point(Point(-1,0,0),0),InfoType(counter++,1)},
            {Weighted_point(Point(0,-1,0),0),InfoType(counter++,1)},
            {Weighted_point(Point(0,0,-1),0),InfoType(counter++,1)}
        };
        
        CGT::RT rt(pts.begin(), pts.end());

        
        NS::VectorType xx(pts.size());
        for(int i=0; i<pts.size(); ++i)
            xx[i]=CGT::Parabola( pts[i].first.point() );
        
        ConvexityConstraint cvx(pts);
        
        ScalarType val = cvx.Value(xx);
        cvx.Compute(-1);
        {
            std::ostream & os = std::cout
            ExportVarArrow(val)
            ExportArrayArrow(cvx.values)
            ExportVarArrow(cvx)
            ExportVarArrow(rt)
            << "\n";
        }
        
        return;
    }

}


int main(int argc, const char * argv[]){
	
	if(MainHelp(argc, argv)) return EXIT_SUCCESS;
	
	using namespace PrincipalAgent_3_Test;
	Monopolist(MakeShape(20,ShapeType::Cube),"PAO_3D_Cube.txt");
//	PrincipalAgent_3_Test::Monopolist(20,1e-4);
		
		
	Octa0();
	Tetra0();
		

}

#endif
