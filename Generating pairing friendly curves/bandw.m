// Input: polynomial fx
// Output: factors of this polynomial
procedure outputfactors(fx)
	local i, L;
	L := Factorization(fx);
	for i in L do 
		i;
	end for;
end procedure;

// Input: two integers m, n. 
// Output: whether n is quadratic residue mod m
IsSquareMod := function(n, m)
	if m le 1 then
		printf "The second parameter must be greater than 1 \n";
		return -1;
	else 
		return JacobiSymbol(n,m);
	end if;
end function;

// Input: a square-fee number D, an integer k, and a polynomial fx
// Output: representation of sqrt(-D) modulo rx
sqrtD := function(D, k, rx)
	local ret, n, e, d, m, c;
	Q<x>:=PolynomialRing(Integers());
	//K := CyclotomicField(k);
	//rx := DefiningPolynomial(K);
	//I := ideal< Q| rx>;
	//RI<t> :=quo< Q| I>;

	ret := 1; m := 1; c := 0;
	if ((D gt 1) and (k mod D) ne 0) or (IsSquarefree(D) eq false) or (D le 0) then
		//printf "Wrong parameters. D must divide k!\n";
		return 0;
	end if;

	if (D mod 2) eq 0 and (k mod 8) ne 0 then
		//printf "Failure! Wrong parameters! \n";
		//printf "If 2|D, then 8|k! \n";
		return 0;
	end if;

	if (D mod 4) ne 3 and (k mod 4) ne 0 then
		//printf "Failure! Wrong parameters! \n";
		//printf "If D != 3 mod 4, then 4|k! \n";
		return 0;
	end if;

	if D eq 1 then 
		if (k mod 4) eq 0 then 
			return x^(k div 4); 
		else 
			return 0; 
		end if;
	end if;
	L := Factorization(D);
	m := false;
	for d in L do 
		if (d[1] eq 2) then
			ret := ret*x^(k div 4)*x^(k div 8)*(1 + x^(k div 4)); 
			m := true;
		else 
			e := k div d[1];
			n := (d[1] - 1) div 2;	

			for i := 1 to n do
				ret *:= (x^(i*e) - x^(k - (i*e)));
			end for;
				
			if (d[1] mod 4) eq 1 then
				//ret := ret*x^(k div 4);
				c +:= 1;
			end if;
			m := false;
		end if;
		c +:= 1;
	end for;

	if m eq true then
		ret := ret div x^(k div 4);
	end if;
	if (c mod 2) eq 0 then
		ret *:= x^(k div 4);
	end if;

	ret := ret mod rx; 
	return ret;

end function;

// Check whether the rational polynomial f represents integers.
isIntegerQ := function(f)
	local i, z;
	
	for i := 0 to 100 do
		z := Evaluate(f, i);
		if z in Integers() then return 1; end if;
	end for;
	return 0;
end function;
// Check whether the polynomial f/m represents integers.
isInteger := function(f, m)
	local i, z, c;
	c := 0;
	m := Integers()!m;
	for i := 0 to m - 1 do
		z := Evaluate(f, i);
		if z mod m eq 0 then c +:= 1; end if;
		//if z in Integers() then c +:= 1; end if;
	end for;
	return c;
end function;

// Check whether a polynomial is near irreducible !
IsNearIrreducible := function(f)
	local c, ret;
	c := Content (f);
	f := f div c;
	ret := IsIrreducible(f);
	return ret, f;
end function;

// Convert f(X)/Q into f(X)/Z
Convert := function(r, p, t, w)
	local a, b, g, c;
	Z<x> := PolynomialRing(Integers()); 
	
	for a := 1 to 100 do
		for b := 0 to 100 do
			g := Evaluate(p, a*x + b); 
			c := Content(g);
    			if c eq w then 
				p := g div c; 
				r := Evaluate(r, a*x + b);
				c := Content(r);
				r := r div c;
				t := Evaluate(t, a*x + b);
    				return r, p, t;
			end if;
		end for;
	end for;
	return 0, 0, 0;
end function;


// This function checks whether a polynomial f/m represent prime numbers 
// Input: an integer m, and a polynomial f
// Output: true or false
isPrime := function(f, m)
	local a, c;
	c := Content(f); f := f div c; 

	if (m mod c) ne 0 then  
		return 0;
	end if;
	m := m div c;

	if LeadingCoefficient(f) le 0 or Degree(f) eq 0 or IsIrreducible(f) eq false or isInteger(f,m) eq 0 then
		return 0;
	end if;

	return 1;	
end function;

// Brezing and Weng method
bandw := function(D, k)
	local d, l, bx, alpha, yx, rx, qx, tx, zeta, zeta_k, best_rho_n, best_rho_d, best_r, best_t, rho_n, rho_d, best_d;
	d := D;
	if (D mod 4) ne 3 then
		d *:= 4;
	end if;
	
	l := Lcm(d, k);
	Q<x>:=PolynomialRing(Rationals()); 
	K := CyclotomicField(l);
	rx := DefiningPolynomial(K); 
	alpha := l div k;
	zeta_k := x^alpha; 
	bx := sqrtD(D, l, rx);
	best_rho_n := 2; best_rho_d := 1; best_d := 0; best_r := 0; best_p := 0; best_t := 0;
	zeta := zeta_k;
	for i := 1 to k - 1 do 
		tx := zeta + 1;
		yx := (tx - 2); yx := yx mod rx; yx := yx*bx/D; yx := yx mod rx; 
		for ax in [yx, -yx] do	
			qx := (tx^2 + D*yx^2)/4;

			if LeadingCoefficient(qx) gt 0 and Degree(qx) gt 0 and IsIrreducible(qx) and isIntegerQ(qx) eq 1 then
				rho_n := Degree(qx); rho_d := Degree(rx);

				if  rho_n*best_rho_d lt best_rho_n*rho_d then 
					best_rho_n := rho_n; best_rho_d := rho_d; best_d := D; 
					best_p := qx; best_r := rx; best_t := tx; 
				end if;
			end if;
			
		end for; 
		zeta := zeta*zeta_k mod rx;
	end for; 	

	return best_d, best_t, best_p, best_r;
end function;


// Brezing and Weng method
vbandw := function(k)
	local d, l, bx, alpha, yx, rx, qx, tx, zeta, zeta_k, best_rho_n, best_rho_d, best_r, best_t, rho_n, rho_d, best_d;
	Q<x>:=PolynomialRing(Rationals());
	best_rho_n := 2; best_rho_d := 1; best_d := 0; best_r := 0; best_p := 0; best_t := 0;

	for D := 1 to 10 do
		d := D;
		if (D mod 4) ne 3 then
			d *:= 4;
		end if;
	
		l := Lcm(d, k);
		K := CyclotomicField(l);
		rx := DefiningPolynomial(K); 
		alpha := l div k;
		zeta_k := x^alpha; 
		bx := sqrtD(D, l, rx);
		zeta := zeta_k;
		for i := 1 to k - 1 do 
			tx := zeta + 1;
			yx := (tx - 2); yx := yx mod rx; yx := yx*bx/D; yx := yx mod rx; 
			for ax in [yx, -yx] do	
				qx := (tx^2 + D*yx^2)/4;

				if LeadingCoefficient(qx) gt 0 and Degree(qx) gt 0 and IsIrreducible(qx) and isInteger(qx,100) gt 0 then
					rho_n := Degree(qx); rho_d := Degree(rx);

					if  rho_n*best_rho_d lt best_rho_n*rho_d then 
						best_rho_n := rho_n; best_rho_d := rho_d; best_d := D; 
						best_p := qx; best_r := rx; best_t := tx; 
					end if;
				end if;	
			end for; 
			zeta := zeta*zeta_k mod rx;
		end for; 	
	end for; 

	return best_d, best_t, best_p, best_r;
end function;
// Extended Brezing and Weng method. This function try to find a family of pairing friendly which offers the best rho-value
// Input: an embedding degree. If K0 = 0, we try to find the best families of curves with embedding degree not greater than MAXK defined as below.
// Outout: families of curves with the best rho-value.
BM := 20;
BD := 30;
MAXK := 50;
ebandw := function(K0)
	local i, j, k, d, W, max_K, min_K, mode, phi, n, zeta_k, zeta, srD, a, b, c, p, best_rho_n, best_rho_d, rho_n, rho_d, ww, g, best_n, best_d, kz; //phi := [];
	local best_p, best_w, best_r, best_t;

	if (K0 eq 0) then mode := 0; else mode := 1; end if;
	if (K0 ne 0) and (K0 lt 3 or K0 gt MAXK) then return 0; end if;
	// Generate cyclotomic poly
	Z<x>:=PolynomialRing(Integers());
	//for i := 1 to BM*MAXK do  		phi[i] := CyclotomicPolynomial(i); 	end for;
	//F := Open("output_ebanw.txt", "w");
	
	if mode eq 0 then
		//fprintf F, "Find best BW families of pairing-friendly elliptic curves \n";
		//fprintf F, "To find all individual curves, use eband(k), where 3 <= k <= %o \n", MAXK;
		//delete F;
		min_K := 3;
		max_K := MAXK;
	else min_K := K0; max_K := K0;
	end if;
	
	for k := min_K to max_K do 
		best_rho_n := 2; best_rho_d := 1; best_d := 0; best_n := 0; best_r := 0; best_p := 0; best_t := 0;
		// try r(x) as phi_{nk}(x)
		for i := 1 to BM do 
			r := CyclotomicPolynomial(i*k); r := Z!r; 
			// Set k-th root of unity
			zeta_k := x^i; zeta_k := zeta_k mod r; //zeta_k;
			// Try for small discriminants ...
			for d := 1 to BD do
				if IsSquarefree(d) eq false then continue; end if;
				W := 4*d; srD := sqrtD(d, i*k, r); //printf "K = %o. Square root of -%o mod %o is %o \n", i*k, d, r, srD;
				if srD eq 0 then continue; end if;
					// try for all the other k-th roots of unity
					zeta := zeta_k; 
					for j := 1 to k - 1 do 
						if Gcd(j, k) ne 1 then zeta *:= zeta_k; zeta := zeta mod r; continue; end if;
						a := zeta + 1; b := (a - 2); b := b mod r; b := b*srD; b := b mod r; p := (d*a^2 + b^2); ww := W; rho_n := Degree(p); rho_d := Degree(r);
						//if d mod 15 eq 0 then printf "d = %o, a = %o, r = %o, p = %o \n", d, a, r, p; end if;
						g := Gcd(rho_n, rho_d); rho_n := rho_n div g; rho_d := rho_d div g;
						//c := Content(p); p := p div c; 
						//if (W mod c) eq 0 then  ww := W div c;
						if (mode eq 1) or (rho_n*best_rho_d lt best_rho_n*rho_d) then 
						//p := isPrime(p, ww);	
							if isPrime(p, ww) eq 1 then
								//if mode eq 1 then
									// F := "output_ebandw.txt";
									//fprintf F, "\n For the embedding degree k = %o, the discriminant D = %o, we get \n", k, d;
									//fprintf F, "t(x) = %o, q(x) = (%o) / %o, r(x) = %o \n", a, p, ww, r;
									//fprintf F, "rho-value = %o/%o\n", rho_n, rho_d;
								//end if;
										
								if  rho_n*best_rho_d lt best_rho_n*rho_d then 
									best_rho_n := rho_n; best_rho_d := rho_d; best_d := d; best_n := i; kz := i*k;
									best_p := p; best_r := r; best_t := a; best_w := ww;
								end if;
							end if;
						end if; //end if;
						zeta *:= zeta_k; zeta := zeta mod r; 
					end for; 
				//end if; 
			end for;
		end for;
		// F := "output_ebandw.txt";
       		//fprintf F, "\nGenerate families of pairing-friendly elliptic curves with the best rho-value using  Brezing-Weng method \n";
	        //fprintf F, "\n================================================================================";
        	//fprintf F, "\nFor the embedding degree k = %o we get the following family of curves with the best rho-value = %o / %o, and the discriminant D = %o. \n", k, best_rho_n, best_rho_d, best_d;
	        //fprintf F, "\nThe polynomial representing the square root of (-D) : \nsqrt(-D) = %o. \n", bx;
	        //fprintf F, "\nThe polynomial representing the order of the subgroup of points: \nr(x) = %o. \n", best_r;
        	//fprintf F, "\nThe polynomial representing the ground finite field : \nq(x) = %o.\n", best_p;
	        //fprintf F, "\nThe polynomial representing the Frobenius trace :\nt(x) = %o \n", best_t;

		//fprintf F, "For k = %o, with D = %o, best rho = %o / %o\n", k, best_d, best_rho_n, best_rho_d;
		//fprintf F, "zeta_{%o}. Trace = %o, p = (%o) / %o, r = %o \n", kz, best_t, best_p, best_w, best_r;
	end for;
	best_r, best_p, best_t := Convert(best_r, best_p, best_t, best_w);
	//delete F;
	return best_d, best_t, best_p, best_r;
end function;

