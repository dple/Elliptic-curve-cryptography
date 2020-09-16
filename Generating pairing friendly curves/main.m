System("./get_para");
load "parameters.txt";
load "bandw.m"; 
load "cp.m";

// This function returns the number of bits of x
numbit := function(x)
  return Round(Log(2,x)+0.5);
end function;

// Get a family of curves. Given k
Getfamily := function(curve, k) 
	local rx, px, nx, tx, D, F;

	Z<x>:=PolynomialRing(Integers());
	px := 0; tx := 0; nx := 0; rx := 0;
	k;
	if curve eq 1 then
		D := 0; //c := 1;
		if k eq 3 then 		
			tx := 6*x - 1;
			px := 12*x^2 - 1;
			rx := px + 1 - tx;
		end if;
		if k eq 4 then 		
			tx := x + 1;
			x := x^2 + x + 1;
			rx := px + 1 - tx;
		end if;
		if k eq 6 then
			tx := 2*x - 1;
			px := 4*x^2 + 1;
			rx := px + 1 - x;
		end if;
	else 
		if k eq 4 then 	
			tx := -108*x^3 - 216*x^2 - 144*x - 32;
			px := 3888*x^6 + 15552*x^5 + 26136*x^4 + 23652*x^3 + 12180*x^2 + 3396*x + 403;
			D := 3;
			rx := 324*x^4 + 972*x^3 + 1098*x^2 + 558*x + 109;

		else if k eq 6 then 	
			tx := -64*x^2 + 16*x + 2;
			px := 4096*x^5 - 2048*x^4 + 192*x^3 - 48*x^2 + 17*x + 1;
			D := 1;
			rx := 4096*x^4 - 2048*x^3 + 192*x^2 + 16*x + 1;

		else if k eq 12 then 	
			tx := 6*x^2 + 1;
			px := 36*x^4 + 36*x^3 + 24*x^2 + 6*x + 1;
			D := 3;
			rx := px + 1 - tx;
			//printf "Here !\n";

		else if k eq 18 then 	
			tx := 444528*x^4 + 592704*x^3 + 296352*x^2 + 65952*x + 5521;
			px := 461078666496*x^8 + 1284433428096*x^7 + 1564374047040*x^6 + 1088278335648*x^5 + 473078255328*x^4 + 131624074008*x^3 + 22896702948*x^2 + 2277529014*x + 99213811;
			D := 3;
			rx := 16003008*x^6 + 32006016*x^5 + 26671680*x^4 + 11862072*x^3 + 2971512*x^2 + 397800*x + 22249;

		else 
			//load "bandw.m"; 
			D, tx, px, rx := ebandw(k); 	
		end if; end if; end if; end if;
	end if;
	
	nx := px + 1 - tx;
	//F := Open("curve_family", "w"); 
	//fprintf F, "D0 := %o;\n", D;
	//fprintf F, "nx0 := %o;\n", nx;
	//fprintf F, "px0 := %o;\n", px;
	//fprintf F, "rx0 := %o;\n", rx;
	//fprintf F, "tx0 := %o;\n", tx;
	//fprintf F, "k0 := %o;\n", k;
	//delete F;
	return D, nx, px, rx, tx;
end function;

// Main function 
Getvalues := function(D, px, tx, rx, k, sp)
	local r, p, n, t, z, F;
	z := Degree(px); 
	z := sp div z; 
	z := 2^(z - 2) + Random(1, 2^(z - 5)) + Random(1, 2^(z - 7));
	repeat
		z := z + 1; 
		p := Evaluate(px, z); 
		r := Evaluate(rx, z); //numbit(r);
		t := Evaluate(tx, z); 
	until IsProbablePrime(p) and IsProbablePrime(r);

	n := p + 1 - t;
	F := Open("curve_para.txt", "w");
	fprintf F, "%o\n", k;
	fprintf F, "%o\n", D;
	fprintf F, "%o\n", IntegerToString(p);
	fprintf F, "%o\n", IntegerToString(n);
	fprintf F, "%o\n", IntegerToString(r);
	delete F;
	return r, p, n, t; //p, r, T; numbit(r); ham(T); toNAF(T);ham(z);
end function;

mainpro := function(curve, k, sp)	
	local nx, px, rx, tx, D, t, r, p, n, z, Fp, E, s, F, oF, FF;
	Z<x>:=PolynomialRing(Rationals());
	p := 0; n := 0; r := 0; t := 0;
	//F := Open("curve_para.txt", "w");
	//FF := Open("curve_MNT.txt", "w");
	oF := Open("curve_output.txt", "w");

	printf "You choose curve = %o (1. MNT, 2. Cocks-Pinch, 3. Brezing-Weng)\n", curve;
	printf "The embedding degree k = %o\n", k;
	printf "The minimum bit of p = %o\n", sp;

	if (curve eq 1) then // MNT curves
		FF := Open("curve_MNT.txt", "w");
		fprintf FF, "%o\n", k;	
		fprintf FF, "%o\n", sp;
		delete FF;
		System("./mnt_modi"); 
		F := Open("curve_para.txt", "r");
		s := Gets(F);		
		if IsEof(s) then
			printf "Cannot find any MNT curve in this range. Please try for other parameters ! \n";
			return 0;
		end if;
		D := StringToInteger(Gets(F)); p := StringToInteger(Gets(F)); n := StringToInteger(Gets(F)); r := StringToInteger(Gets(F));
		t := p + 1 - n;
		delete F;
	end if;
	
	if (curve eq 3) then // BW curves
		F := Open("curve_para.txt", "w");
		D, nx, px, rx, tx := Getfamily(curve, k);	//nx; px, rx, tx;
		r, p, n, t := Getvalues(D, px, tx, rx, k, sp);
	end if;	

	if (curve eq 2) then // Cocks-Pinch curves
		F := Open("curve_para.txt", "w");
		//for i := 1 to 10 do
			//repeat  D := Random(1, 1000); until IsSquarefree(D);
			//cp(D, k);
		//end for;
		r, p, n, t, D := cp_modi(k, sp);			
		//end if;  
		fprintf F, "%o\n", k;
		fprintf F, "%o\n", D;
		fprintf F, "%o\n", IntegerToString(p);
		fprintf F, "%o\n", IntegerToString(n);
		fprintf F, "%o\n", IntegerToString(r);
		delete F;
	end if;
	Fp := GF(p);
	// Save in file
	fprintf oF, "This program generates a pairing-friendly elliptic curve, which haves: \n";
	fprintf oF, "\n- the embedding degree k = %o\n", k;
	fprintf oF, "- the order of the finite field p = %o \n", p;
	fprintf oF, "- the bits number of p = %o \n", numbit(p);
	fprintf oF, "- the order of the largest subgroup of points r:= %o \n", r;	
	fprintf oF, "- the bits number of r = %o \n", numbit(r);
	fprintf oF, "- the Frobenius trace t = %o \n", t;
	//fprintf oF, "\n%o\n", Fp;
	fprintf oF, "\n The elliptic curve E defined over the finite filed %o, is of form: y^2 = x^3 + A*x + B, where: \n", Fp;
	fprintf oF, "\n the order of elliptic curve E = %o\n", n; 
	//fprintf oF, "The point P = %o of prime order %o\n\n", P, r;
	delete oF;
	System("./cm_modi");


	// Show in screen 
	printf "This program generates a pairing-friendly elliptic curve, which haves: \n";
	printf "\n- the embedding degree k = %o\n", k;
	printf "- the order of the finite field p = %o \n", p;
	printf "- the bits number of p = %o \n", numbit(p);
	printf "- the order of the largest subgroup of points r:= %o \n", r;	
	printf "- the bits number of r = %o \n", numbit(r);
	printf "- the Frobenius trace t = %o \n", t;
	//fprintf oF, "\n%o\n", Fp;
	printf "\nThe elliptic curve E defined over the finite filed %o, is of form: y^2 = x^3 + A*x + B, where: \n", Fp;
	printf "\nthe order of elliptic curve E = %o\n", n; 
	//fprintf oF, "The point P = %o of prime order %o\n\n", P, r;
	F := Open("tmp.txt", "r");
	//s := Gets(F);
	A := StringToInteger(Gets(F));
	B := StringToInteger(Gets(F));
	delete F;
	printf "A = %o \n", A;
	printf "B = %o \n", B;

	//Fp := GF(p);
	E := EllipticCurve([Fp|A,B]);
	fl := Floor(n/r);
	g := fl*Random(E);
	printf "The generator of the largest subgroup of points of the elliptic curve E is \n G = %o \n", g;

	oF := Open("curve_output.txt", "a");	
	fprintf oF, "\n The generator of the largest subgroup of points of the elliptic curve E is \nG = %o \n", g;
	delete oF;

	return 0;

end function;


mainpro(c, k, sp);

