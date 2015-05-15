double Interpolate(float x, float y, float index1[5], float index2[5], float f[5][5])
{
	int x1, x2, y1, y2; float Intermediate;
	bool Error1 = true, Error2 = true;
	for (int i = 0; i<5; i++)
		if (x >= index1[i] && x <= index1[i+1]) {x1 = i; x2 = i+1; Error1 = false; break;}
	for (int i = 0; i<5; i++)
		if (y >= index2[i] && y <= index2[i+1]) {y1 = i; y2 = i+1; Error2 = false; break;}
	if (Error1 || Error2) return 0;
	Intermediate =  (f[x1][y1] * (index1[x2]-x) * (index2[y2] - y)) + (f[x2][y1] * (x - index1[x1]) * (index2[y2] - y)) + (f[x1][y2] * (index1[x2] -x) * (y-index2[y1])) + (f[x2][y2] * (x-index1[x1]) * (y-index2[y1]));
	return Intermediate/((index1[x2]-index1[x1])*(index2[y2]-index2[y1]));
}