// This program implement the ECM method for factorization using short Weierstrass elliptic curves
// Projective coordinates: ZY^2 = X^3 + aXZ^2 + bZ^3
// n : factored number 

AddProj:= function(xP, yP, zP, xQ, yQ, zQ, a, n)
    // return R = P + Q
    // Add: u = Y2-Y1, uu = u2, v = X2-X1, vv = v2, vvv = v*vv, R = vv*X1, A = uu-vvv-2*R, X3 = v*A, Y3 = u*(R-A)-vvv*Y1, Z3 = vvv
    // Dbl: XX = X1^2, ZZ = Z1^2, w = a*ZZ+3*XX, s = 2*Y1*Z1, ss = s2, sss = s*ss, R = Y1*s, RR = R2, B = (X1+R)2-XX-RR, h = w2-2*B, X3 = h*s, Y3 = w*(B-h)-2*RR, Z3 = sss

    local l, xR, yR, zR;
    local xx, zz, w, s, ss, sss, R, RR, BB, h;
    local u, uu, v, vv, vvv, AA;

    if (xP eq xQ) and (yP eq yQ) and (zP eq zQ) then
       xx := xP^2; zz := zP^2; w := 3*xx + a*zz; s := 2*yP*zP; ss := s^2; 
       sss := s*ss; R := yP*s; RR := R^2; BB := (xP + R)^2 - xx - RR; h := w^2 - 2*BB;
       xR := h*s mod n; yR := (w*(BB-h)-2*RR) mod n; zR := sss mod n;

    else // P ne Q
	u := yQ - yP; uu := u^2; v := xQ - xP; vv := v^2; vvv := v*vv; 
	R := vv*xP; AA := uu - vvv - 2*R; 
	xR := v*AA mod n; yR := (u*(R - AA)-vvv*yP) mod n; zR := vvv mod n;
    end if;
    return xR, yR, zR;
end function;

DblProj := function(xP, yP, zP, a, n)
	// return R = 2P
	return AddProj(xP, yP, zP, xP, yP, zP, a, n);
end function;

scalarProj := function(xP, yP, zP, k, a, n)
	// return [k]P
	local i, l, xR, yR, zR, ki;
	xR := xP; yR := yP; zR := zP;
	i := Floor(Log(2,k))-1;
	while i ge 0 do
	  xR, yR, zR := DblProj(xR, yR, zR, a, n);
	  ki:=Intseq(k,2);

    	  if ki[i+1] eq 1 then
	       xR, yR, zR := AddProj(xR, yR, zR, xP, yP, zP, a, n);
    	  end if;
	  i:=i-1;

	end while;

	return xR, yR, zR;
end function;

ecmProj := function(xP, yP, zP, k, a, n)
	// return prime factor of n
	local xR, yR, zR;
	xR := xP; yR := yP; zR := zP;
	for i := 2 to k do
		xR, yR, zR := scalarProj(xR, yR, zR, i, a, n);
		if (Gcd(zR, n) ne 1) and (Gcd(zR, n) ne n) then
			return Gcd(zR, n), i;
		end if;	
	end for;
	return 0, 0;
end function;

// Test 
n := 6887;
xP := 1512;
yP := 3166;
zP := 1;
a := 14;
b := (yP^2 - xP^3 - a*xP) mod n;
ecmProj(xP, yP, zP, 8, a, n);
//E := EllipticCurve([a, b]);

