// Input: Given a positive integer k, a square-free integer D, and a desired security level l, 
// Output : a prime number r such that k | (r - 1) and (-D/r) = 1. 

findR := function(D, l, k) 
	local r;
	r := 2^l;
	repeat 
		r := NextPrime(r);
	until IsDivisibleBy(r - 1, k) and (LegendreSymbol(-D, r) eq 1);
	return r;
end function;


// Input: Given a prime number r, a square-free integer D 
// Output : a Square root of -D modulo r. 
findSquareRootofD := function(D, r) 
	local ret, d;
	F := GF(r);
	d := F!-D;
	ret := SquareRoot(d);
	return ret;
end function;


// Input: Given a prime number r, an integer k 
// Output : a kth root of unity modulo r. 
findZeta := function(r, k)
	local zeta;
	F := GF(r); 
	zeta := RootOfUnity(k, F);
	return zeta;
end function;

findZeta1 := function(r, k)
	local Zeta;
	zeta := 2^((r-1) div k);
	zeta := zeta mod r;
	return zeta;
end function;

// This function return the number of bit of a big integer x.

numbit := function(x)
  return Round(Log(2,x)+0.5);
end function;

generation := function()
	// Here we generate a random k;
	k := Random(2, 6);

	// One can choose any square free D.
	// Here we generate a random D < 100000;
	repeat
		D := Random(1, 100000);
	until IsSquarefree(D);
	
	return D, k;
end function;

// Cocks-Pinch method
cp := function(D, k) // cocks_pinch method
	local d, r, zeta, t, y, p, sr, t0, y0, j, l;
	//F := "output.txt";
	// Determine the security level corresponding the embedding degree k
	if k le 4 then
		l := 80;
		j := 1024 div k;
	else if k le 6 then 
		l := 112;
		j := 2048 div k;
	else if k le 10 then 
		l := 128;
		j := 3072 div k;
	else if k le 13 then 
		l := 192;
		j := 7680 div k;
	else l := 256; 
		j := 15360 div k;
	end if; end if; end if; end if;

	d := -D; r := 2^(2*l);
	repeat
		r := NextPrime(r);
	until IsDivisibleBy(r - 1, k) and (LegendreSymbol(-D, r) eq 1);

	printf "The subgroup order r = %o\n", r;
	//	R; r; numbit(r); Factorization((r - 1) div k);
	F := GF(r);
	sr := SquareRoot(F!-D);	sr := Integers()!sr;	
	printf "Square root of -D mod r = %o\n", sr;	
	zeta := RootOfUnity(k, F); // w := Random(r); zeta := w^((r - 1) div k); zeta := zeta mod r;
	zeta := Integers()!zeta;
	//zeta;
	
	t := zeta + 1 mod r;
	printf "zeta = %o\n", zeta;
	y := (t - 2)*Modinv(sr, r);// *sr div -D; 
	y := y mod r;
	printf "y = %o \n", y;

	j := (j + 1 - numbit(D) - 2*numbit(r)) div 2; 
	if j le 0 then 
		j := 1;
	else 
		j := 2^(j + 1);
	end if;
	repeat
		for b in [y, -y] do
			p := t^2 + D*(b + j*r)^2; //p := Integers()!p;
			if IsDivisibleBy(p, 4) and IsPrime(p div 4) then
				break;
			end if;
		end for;
		j := j + 1;//		j;
	until IsDivisibleBy(p, 4) and IsPrime(p div 4);
	p := p div 4; //j;
	printf "p := %o;\n", p;
	printf "r := %o;\n", r;
	printf "t := %o;\n", t;
	F := "output.txt";
	//fprintf F, "\nGenerate a pairing-friendly elliptic curve using  Cocks-Pinch method \n";
	fprintf F, "\n================================================================================";
	fprintf F, "\nWe are generating a curve at %o security level. Given: \nthe embedding degree k = %o, \nthe discriminant D = %o \n", l, k, D;
	fprintf F, "\nThe order of the subgroup of points: \nr = %o. \nThe group order size: log(r) = %o bits. \n", r, numbit(r);
	fprintf F, "\nThe characteristic of the ground finite field : \np = %o. \nThe finite field size: log(p) = %o bits.\n", p, numbit(p);
	fprintf F, "\nThe Frobenius trace :\nt = %o \n", t;
	delete F;
	return 0;
end function;

// Cocks-Pinch method
cp_modi := function(k, sp) // cocks_pinch method
	local i, d, r, zeta, zeta_k, t, y, p, sr, t0, y0, j, l;
	//F := "curve_para.txt";
	l := sp div 4;
	//printf "Start searching ... \n";
	if l le 80 then
		j := 1024 div k;
	else if l le 112 then 
		j := 2048 div k;
	else if l le 128 then 
		j := 3072 div k;
	else if l le 192 then 
		j := 7680 div k;
	else //l := 256; 
		j := 15360 div k;
	end if; end if; end if; end if;
	r := 2^(2*l - 2) + Random(1, 2^(2*l - 5)) + Random(1, 2^(2*l - 9));
	//for i := 1 to 50 do
	repeat
       	       	D := Random(1, 10000);
	until IsSquarefree(D);	
	d := -D;
	//D; 
	repeat
		r := NextPrime(r);
	until IsDivisibleBy(r - 1, k) and (LegendreSymbol(-D, r) eq 1);

	//	R; r; numbit(r); Factorization((r - 1) div k);
	Fp := GF(r);
	sr := SquareRoot(Fp!-D);	sr := Integers()!sr;		
	zeta_k := RootOfUnity(k, Fp); // w := Random(r); zeta := w^((r - 1) div k); zeta := zeta mod r;
	zeta_k := Integers()!zeta_k;
	zeta := zeta_k;
	
	// t := zeta + 1 mod r;
	// y := (t - 2)*Modinv(sr, r); 
	// y := y mod r; 

	j := (j + 1 - numbit(D) - 2*numbit(r)) div 2; 
	if j le 0 then 
		j := 1;
	else 
		j := 2^(j + 1);
	end if;
	repeat
		for i := 1 to k - 1 do
			t := zeta + 1 mod r;
			y := (t - 2)*Modinv(sr, r); 
			y := y mod r;
			for b in [y, -y] do
				p := t^2 + D*(b + j*r)^2; //p := Integers()!p;
				if IsDivisibleBy(p, 4) and IsPrime(p div 4) then
					break;
				end if;
			end for;
			zeta := zeta*zeta_k; zeta := zeta mod r;
		end for;
		j := j + 1;//		j;
	until IsDivisibleBy(p, 4) and IsPrime(p div 4);
	p := p div 4; 
	n := p + 1 - t;
	//fprintf F, "%o\n", D;
	//fprintf F, "%o\n", p;
	//fprintf F, "%o\n", n;
	//fprintf F, "%o\n", t;
	//fprintf F, "%o\n", r;
	
	//end for; 
	//delete F;
	return r, p, n, t, D;
	
end function;

//load "input.m";

//cp(D0, k0);

