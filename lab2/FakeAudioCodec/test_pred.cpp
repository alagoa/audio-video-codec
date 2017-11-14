int main(int argc, char const *argv[])
{
	short* values= {5,10,15,20,25,30,35,40}
	Predictor predictor;
	for(auto &e : values) {
		predictor.order1_predict(e);
	}
}