load -ASCII X_train.mat;

modeValue = 0;
maxStep = 100;
param.lambda = 10;

%param = struct{lambda, 0.1, lambda2, L, pos, false, mode, modeValue, numThreads,  -1, cholesky, false, ols, false, %max_length_path};

D = mexTrainDL(X_train, param);
save -ASCII D.mat D;

