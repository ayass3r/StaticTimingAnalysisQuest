double Interpolate(float x, float y, float index1[6], float index2[6], float f[6][6])
{
	float x1, x2, y1, y2, Intermediate;
	bool Error1 = true, Error2 = true;
	for (int i = 0; i<5; i++)
		if (x >= x[i] && x <= x[i+1]) {x1 = x[i]; x2 = x[i+1]; Error1 = false; break;}
	for (int i = 0; i<5; i++)
		if (y >= y[i] && y <= y[i+1]) {y1 = y[i]; y2 = y[i+1]; Error2 = false; break;}
	if (Error1 || Error2) return 0;
	Intermediate =  (f[x1][y1] * (x2 -x) * (y2 - y)) + (f[x2][y1] * (x - x1) * (y2 -y1)) + (f[x1][y2] * (x2 -x) * (y-y1)) + (f[x2][y2] * (x-x1) * (y-y1));
	return Intermediate/((x2-x1)*(y2-y1));
}