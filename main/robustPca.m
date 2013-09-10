clear all;
start_spams;

%train
fprintf('loading train feature...\n');
load ../output/trainFeature.dat;
fprintf('reducing train feature...\n');
[wcoef, score] = princomp(trainFeature);
clear trainFeature;
param.K = 1000;
param.lambda = 0.15;
param.iter =1000;
buildcost = zeros(1, 30);

for k=1:30
    fprintf('training the sparse dictionary...\n');
    param.K = 100*k;
    [D, model] = mexTrainDL(score', param);
    buildcost(k) = sum(sum((score' - D*model).^2));
    save -mat ../output/sparse.dat D wcoef;
    clear D wcoef;
    
end
clear score;
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
pause(60);

%test
fprintf('loading test feature...\n');
load ../output/testFeature.dat;
load -mat ../output/sparse.dat;
testscore = testFeature * wcoef;
clear testFeature;
fprintf('get sparse coeffice for tester...\n');
param.approx=0;
A = mexLasso(testscore', D, param);
save -mat alpha.dat A;

%result
fprintf('process the result... \n');
costMatrix = testscore' - D*A;
cost = sum((costMatrix.^2), 1);
cost = sqrt(cost);

%compare
fprintf('comparing result...\n');
load dataset/label_test_uscd1.dat
[m, n] = size(label_test_uscd1);
n = m*n;
label = reshape(label_test_uscd1', 1, n);
clear label_test_uscd1;


threshod = 0:0.01:1;
len = length(threshod);
tpr = zeros(1, len);
fpr = zeros(1, len);
auc = 0;

for i=1:len
    result = cost > threshod(i);
    tp = result & label;
    fp = result & (~label);
    tn = (~result) & (~label);
    fn = (~result) & label;
    tpl = sum(sum(tp));
    fpl = sum(sum(fp));
    tnl = sum(sum(tn));
    fnl = sum(sum(fn));
    fprintf('tp: %d, fp: %d\n', tpl, fpl);
    fprintf('tn: %d, fn: %d\n', tnl, fnl);
    precision = tpl/(tpl + fpl);
    accuracy = (tpl + tnl)/n;
    recall = tpl/(tpl + fnl);
    f1_score = 2*precision*recall/(precision + recall);
    tpr(i) = tpl/(tpl+fnl);
    fpr(i) = fpl/(fpl+tnl);
    if i>1
        auc = auc + 0.5*(tpr(i-1) + tpr(i))*(fpr(i-1) - fpr(i));
    end
    fprintf('precison: %f\nrecall: %f\nf1: %f\n\n', precision, recall, f1_score);
    
end

plot(fpr, tpr, '-ro');
hold on;
t1 = 0:1;
plot(t1, t1, '-.g');
hold off;
fprintf('area under curve: %f\n', auc);