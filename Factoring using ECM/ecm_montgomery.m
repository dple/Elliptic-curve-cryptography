// This program implement the ECM method for factorization using short Weierstrass elliptic curves
// Projective coordinates: ZY^2 = X^3 + aXZ^2 + bZ^3
// n : factored number 

AddMont:= function(x1, xP, zP, xQ, zQ, n)
    // return R = P + Q
    // Add: A = X2+Z2, B = X2-Z2, C = X3+Z3, D = X3-Z3, DA = D*A, CB = C*B, X5 = (DA+CB)^2, Z5 = X1*(DA-CB)^2

    local xR, zR;
    local AA, BB, C, D, DA, CB;

    if (xP ne xQ) or (zP ne zQ) then
       AA := xP + zP; BB := xP - zP; C := xQ + zQ; D := xQ - zQ;
       DA := D*AA; CB := C*BB; xR := (DA + CB)^2 mod n; zR := x1*(DA - CB)^2 mod n;
    end if;
    return xR, zR;
end function;

DblMont := function(xP, zP, a24, n)
	// return: R = 2P, 
	// Assume: 4*a24=a+2.
	// Double: A = X1+Z1, AA = A^2, B = X1-Z1, BB = B^2, C = AA-BB, X3 = AA*BB, Z3 = C*(BB+a24*C)
	local AA, A2, BB, B2, C, xR, zR;
	AA := xP + xP; A2 := AA^2; BB := xP- zP; B2 := BB^2; C := A2-B2; xR := A2*B2 mod n; zR := C*(B2 + a24*C) mod n;
	return xR, zR;
end function;

scalarMont := function(x1, xP, zP, k, a24, n)
	// return [k]P
	local i, l, xR0, zR0, xR1, zR1, ki;
	xR0 := xP; zR0 := zP;
	xR1, zR1 := DblMont(xP, zP, a24, n);
	i := Floor(Log(2,k))-2;
	while i ge 0 do
	  ki:=Intseq(k,2);	
    	  if ki[i+1] eq 0 then
	      xR1, zR1 := AddMont(x1, xR0, zR0, xR1, zR1, n);
    	      xR0, zR0 := DblMont(xR0, zR0, a24, n);
	  else 
	      xR0, zR0 := AddMont(x1, xR0, zR0, xR1, zR1, n);
    	      xR1, zR1 := DblMont(xR1, zR1, a24, n);		
	  end if;
	  i:=i-1;

	end while;

	return xR0, zR0;

end function;

ecmMont := function(x1, xP, zP, k, a24, n)
	// return prime factor of n
	local xR, zR;
	xR := xP; zR := zP;
	for i := 2 to k do
		xR, zR := scalarMont(x1, xR, zR, i, a24, n);
		if (Gcd(zR, n) ne 1) and (Gcd(zR, n) ne n) then
			return Gcd(zR, n), i;
		end if;		
	end for;
	return 0, 0;
end function;

// Test 
// E : y^2 = x^3 + x - 2; xP = 2; yP = 166;
// Convert to Montgomery curve By^2 = x^3 + Ax^2 + x. Equation x^3 + ax + b = 0 must have at least one root, alpha, and 3*alpha^2 + a is quadratic residue. 
// alpha = 1 (root of x^3 + x - 2 = 0), (3*alpha^2 + a) = 4 (IsSquare = 2^2); s = sqrt((3*alpha^2 + a)^-1) = 533; B = s = 533, A = 3*alpha*s = 1599;
n := 6887;
F<x> := PolynomialRing(Rationals());
px := x^3 + x - 2;
alpha := Roots(px);
R := ResidueClassRing(n);
a := 1;
x := R!Modinv((3*alpha^2 + a), n);
sqrts := AllSquaresRoots(x); // 533, 3443, 3444, 6354
s := 533; B := s; AA := 3*alpha*s;
xP := 2;
yP := 166;

xP := s*(xP - alpha); yP := s*yP;
x1 := xP;
zP := 1;

l := Modinv(4, n);
a24 := (AA + 2)*l mod n;

ecmMont(x1, xP, zP, 8, a24, n);
//E := EllipticCurve([a, b]);

