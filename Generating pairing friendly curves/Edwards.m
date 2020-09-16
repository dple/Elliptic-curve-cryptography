// condition (1 - a)/3 is square.
// given p characteristic of finite field
// a, b coefficient of the Weierstrass curve

transform := function(p, a, b)
	Z := Integers();
	R<x> := PolynomialRing(Z);
	F := GF(p);
	s := SquareRoot(F!(1 - a)/3);
	if (2*s^3 - s) eq b then
		alpha := s;
	else
		alpha := F!-s;
	end if;
	//E := EllipticCurve([F|a,b]);
	//IsSquare(F!(3*alpha - 2));SquareRoot(F!(3*alpha - 2));
	//IsSquare(F!(3*alpha + 2));
	a2 := alpha*3;
	s1 := SquareRoot(F!(a2 + 2));
	d := F!(1 - 4/s1);
	a2 := Z!a2; d := Z!d;
	g := x^3 + a2*x^2 + x; // Montgomery form;	
	return g;
end function;



