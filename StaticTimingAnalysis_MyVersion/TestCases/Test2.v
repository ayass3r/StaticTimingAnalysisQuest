input C;
input A;
input B;
wire _1_;
wire _2_;
wire _3_;
output f;
INVX1 _4_ (
	.A(C),
	.Y(_1_)
);
AND2X1 _5_ (
	.A(A),
	.B(_1_),
	.Y(_2_)
);
AND2X2 _6_ (
	.A(C),
	.B(B),
	.Y(_3_)
);
OR2X1 _7_ (
	.A(_2_),
	.B(_3_),
	.Y(f)
);