input A;
wire _1_;
wire _2_;
wire _3_;
wire _4_;
wire _5_;
wire _6_;
output f;
BUFX2 _7_ (
	.A(A),
	.Y(_1_)
);
AND2X2 _8_ (
	.A(_1_),
	.B(_2_),
	.Y(_3_)
);
DFFPOSX1 _9_ (
	.clk(clk),
	.D(_6_),
	.Q(_2_)
);
OR2X1 _10_ (
	.A(_1_),
	.B(_4_),
	.Y(_6_)
);
DFFPOSX1 _11_ (
	.clk(clk),
	.D(_3_),
	.Q(_4_)
);
AND2X1 _12_ (
	.A(_1_),
	.B(_4_),
	.Y(_5_)
);
BUFX2 _13_ (
	.A(_5_),
	.Y(f)
);