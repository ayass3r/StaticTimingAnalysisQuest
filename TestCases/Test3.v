input A;
wire _1_;
wire _2_;
wire _3_;
wire _4_;
wire _5_;
output f;
AND2X1 _6_ (
	.A(_2_),
	.B(_1_),
	.Y(_3_)
);
DFFPOSX1 _7_ (
	.CLK(clk),
	.D(_3_),
	.Q(_4_)
);
INVX2 _8_ (
	.A(_4_),
	.Y(_5_)
);
DFFPOSX1 _9_ (
	.CLK(clk),
	.D(_5_),
	.Q(_2_)
);
INVX3 _10_ (
	.A(_2_),
	.Y(f)
);
INVX2 _11_ (
	.A(A),
	.Y(_1_)
);