input A;
wire _1_;
wire _2_;
wire _3_;
wire _4_;
wire _5_;
wire _6_;
wire _7_;
wire _8_;
wire _9_;
wire _10_;
wire _11_;
output f;
DFFPOSX1 _12_ (
	.clk(clk),
	.D(_1_),
	.Q(_2_)
);
DFFPOSX1 _13_ (
	.clk(clk),
	.D(_8_),
	.Q(_9_)
);
DFFPOSX1 _14_ (
	.clk(clk),
	.D(_6_),
	.Q(_7_)
);
INVX2 _15_ (
	.A(A),
	.Y(_1_)
);
OR2X1 _16_ (
	.A(_2_),
	.B(_1_),
	.Y(_3_)
);
NAND2X1 _17_ (
	.A(_2_),
	.B(_3_),
	.Y(_4_)
);
NOR2X1 _18_ (
	.A(_4_),
	.B(_3_),
	.Y(_5_)
);
AND2x2 _19_ (
	.A(_5_),
	.B(_10_),
	.Y(_6_)
);
INVX2 _20_ (
	.A(_9_),
	.Y(_10_)
);
INVX2 _21_ (
	.A(_7_),
	.Y(f)
);
INVX2 _22_ (
	.A(_7_),
	.Y(_11_)
);
AND2X1 _23_ (
	.A(_11_),
	.B(_1_),
	.Y(_8_)
);
