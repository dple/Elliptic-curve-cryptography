IsBijection := function(aold, bold, a, b, ax, bx, c)
	local i, tmp, ai, bi, r;
	r := false;
	Z<x> := PolynomialRing(Integers());
	for i := 1 to c do
		ai := a div aold[i]; bi := (b - bold[i]) div aold[i];
		tmp := Evaluate(ax[i], ai*x + bi);

		if tmp eq bx then
			return true;
		else
			r := false;
		end if;
	end for;
	return r;
end function;

MIN_co := 1;
MAX_co := 20;
// For \phi(k) = 2
Faccyc2 := function(k, ma, mb)
	local max, rx, n, i, j, tx, aold, bold, nold, count;	
	aold := [];
	bold := [];
	nold := [];
	count := 2;
	Z<x> := PolynomialRing(Integers());
	rx := CyclotomicPolynomial(k);
	aold[1] := 1; bold[1] := 1; nold[1] := x^2;
	for i := 1 to ma do
		for a in [-i, i] do
			for j:= 0 to mb do 
				for b in [j, -j] do
					tx := a*x + b;
					f := Evaluate(rx, tx - 1);
					if IsIrreducible(f) then
						qx := f + tx - 1;

						if IsBijection(aold, bold, a, b, nold, f, count - 1) eq false then
							if IsIrreducible(qx) then
								F := "output.txt";
								fprintf F, "MNT curves : nx:= %o \n", f;
								fprintf F, "The order of finite field qx := %o \n", qx;						
								fprintf F, "The Frobenius trace tx := %o \n", tx;
								delete F;
							else 
								F := "output.txt";
								fprintf F, "Supersingular curves : qx:= %o \n", Factorization(qx);
								fprintf F, "The order of the largest subgroup of points (f = )rx := %o \n", f;
								fprintf F, "The Frobenius trace tx := %o; \n", a*x + b;
								delete F;
							end if;
							aold[count] := a; bold[count] := b; 
							nold[count] := f; count := count + 1;
						end if;

					else 
						L := Factorization(f); 

						for nx in L do
							if IsBijection(aold, bold, a, b, nold, nx[1], count - 1) eq false then
								if Degree(nx[1]) eq 2 then
									H := "rx.m";
									fprintf H, "rx := %o;\n", f;
									fprintf H, "tx := %o;\n", a*x + b;
									fprintf H, "Getqx(%o, %o, %o, %o);\n", MIN_co, MAX_co, nx[1], tx;
									aold[count] := a; bold[count] := b; 
									nold[count] := nx[1]; count := count + 1;
									delete H;
								end if;
								//else 
								//	if nx[1]^2 eq f then
								//		printf "Supersingular curves : qx:= %o \n",Factorization(qx);
								//		printf "(f = )rx := %o; \n", f;
								//		printf "tx := %o; \n", a*x + b;
								//	end if;
								//end if;
							end if;									

						end for;
					end if;

				end for; // for b

			end for; // for j
			
		end for; // for a

	end for; // for i
	return 0;
end function;

Getqx := function(min, max, rx, tx)
	local qx, nx;
	Z<x> := PolynomialRing(Integers());
	F := "output.txt";
	fprintf F, "\n=========================================================================================";
	fprintf F, "\nThe polynomial representing the order of the largest subground of points is: %o\n", rx;
	fprintf F, "The polynomial representing the Frobenius trace is: %o\n", tx;
	fprintf F, "For these parameters, we get the following families of curves:";
	delete F;
	for i := min to max do
		nx := i*rx;
		qx := nx + tx - 1;
		if IsIrreducible(qx) then 
			F := "output.txt";
			fprintf F, "The polynomial representing the order of the ground finite field is: %o\n", qx;
			fprintf F, "The cofactor of curves is: %o\n", i;
			fprintf F, "----------------------------------------------------------------------------------\n";
			//qx; i;
			delete F;
		end if;
		//else
		//	L := Factorization(qx);
		//	for nx in L do
		//		if Degree(nx[1]) eq 1 and nx[2] eq 2 then
		//			L;
		//		end if;
		//	end for;
		//end if;
		
	end for; 
	return 0;
end function;

load "input.m";

Faccyc2(k, MAX_a, MAX_b);
Z<x> := PolynomialRing(Integers());

load "rx.m";
