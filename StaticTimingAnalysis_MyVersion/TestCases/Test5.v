input A;
output f;
wire _1_;
wire _2_;
wire _3_;
wire _4_;
wire _5_;
wire _6_;
wire _7_;
wire _16_;
DFFPOSX1 _8_ (
	.clk(clk),
	.D(_1_),
	.Q(_2_)
);
DFFPOSX1 _9_ (
	.clk(clk),
	.D(_3_),
	.Q(_4_)
);
DFFPOSX1 _10_ (
	.clk(clk),
	.D(_5_),
	.Q(_6_)
);
AND2X1 _12_ (
	.A(_2_),
	.B(_2_),
	.Y(_3_)
);
AND2X1 _13_ (
	.A(_4_),
	.B(_16_),
	.Y(_5_)
);
INVX2 _14_ (
	.A(_6_),
	.Y(_7_)
);
INVX2 _15_ (
	.A(_7_),
	.Y(f)
);
OR2X1 _11_ (
	.A(A),
	.B(_4_),
	.Y(_1_)
);
OR2X1 _17_ (
	.A(_6_),
	.B(_7_),
	.Y(_16_)
);