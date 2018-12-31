#include "data_process.h"


vector<float> read_file_doubles(const string &filename) {
	FILE *f = fopen(filename.c_str(), "rt");
	if (!f) { fprintf(stderr, "could not read %s\n", filename.c_str()); exit(1); }
	vector<float> ans;
	while (1) {
		double value = 0.0;
		if (fscanf(f, "%lf", &value) != 1) {
			break;
		}
		ans.push_back(float(value));
		if (feof(f)) { break; }
	}
	fclose(f);
	return ans;
}

vector<vector<float>> read_file_doubles_mipmap(const string &filename, int width, int height, int bit, int mipmap)
{
	vector<vector<float>> ans;
	vector<float> tmpans = read_file_doubles(filename);
	ans.push_back(tmpans);
	for (int i = 0; i < mipmap - 1; i++)
	{
		vector<float> tmpans_mipmap;
		int tx = width / (1 << (i + 1));
		int ty = height / (1 << (i + 1));
		for (int j = 0; j < ty; j++)
		for (int k = 0; k < tx; k++)
		{
			int block_x = k * (1 << (i + 1));
			int block_y = j * (1 << (i + 1));
			double ans[2] = { 0.0,0.0 };
			for (int jj = 0; jj < (1 << (i + 1)); jj++)
			for (int kk = 0; kk < (1 << (i + 1)); kk++)
			for(int tt = 0; tt < bit; tt++)
			{
				ans[tt] += tmpans[bit * ((block_y + jj) * width + (block_x + kk)) + tt];
			}
			for (int tt = 0; tt < bit; tt++)
			{
				ans[tt] = ans[tt] / (1 << (i + 1)) / (1 << (i + 1));
				tmpans_mipmap.push_back(ans[tt]);
			}
		}
		ans.push_back(tmpans_mipmap);
	}
	return ans;
}