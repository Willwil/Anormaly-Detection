load -ASCII D.mat;
load -ASCII X_test.mat;

modeValue = 0;
maxStep = 100;
param.lambda = 0.1;

%param = struct{lambda, 0.1, lambda2, L, pos, false, mode, modeValue, numThreads,  -1, cholesky, false, ols, false, %max_length_path};

A = mexLasso(X_test, D, param);
save -ASCII A.mat A;
