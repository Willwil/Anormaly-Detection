clear all;

fprintf('loading train feature...\n');
load ../output/trainFeature.dat;
%pca component
coeff = princomp(zscore(trainFeature));
clear trainFeature;


fprintf('loading test feature...\n');
load ../output/testFeature.dat;
pcadata = testFeature * coeff;
clear trainFeature;
diff = testFeature - pcadata;
cost = sum(diff.^2, 2);
cost = sqrt(cost./size(diff, 2));


%compare
fprintf('comparing result...\n');
load ../dataset/label_test_uscd1.dat
[m, n] = size(label_test_uscd1);
n = m*n;
label = reshape(label_test_uscd1', 1, n);
label = label';
clear label_test_uscd1;


%%%%%%%%%%%%%%
selectn = 600;

%selected normal
normalItem = find(label == 0);
maxn = size(normalItem, 1);
normalRandom = randi(maxn, 1, selectn);
normalSelected = normalItem(normalRandom);
%selected anormal
anormalItem = find(label == 1);
maxn = size(anormalItem, 1);
anormalRandom = randi(maxn, 1, selectn);
anormalSelected = anormalItem(anormalRandom);
%visualize
reduceF = abs(pcadata(:, 1:25));
minValue = min(min(reduceF));
maxValue = max(max(reduceF));
normalData = reduceF(normalSelected, :);
anormalData = reduceF(anormalSelected, :);
normalData = floor((normalData - minValue)./(maxValue-minValue) .* 400);
anormalData = floor((anormalData - minValue)./(maxValue - minValue) .* 400);
%span = 0:255;
%normalData = repmat(span, 50, 2);
%normalData = normalData';

%image
figure(1);
subplot(1, 2, 1);
image(normalData);
subplot(1, 2, 2);
image(anormalData);

threshod = 0:0.1:10;
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
    
    precision = tpl/(tpl + fpl);
    accuracy = (tpl + tnl)/n;
    recall = tpl/(tpl + fnl);
    f1_score = 2*precision*recall/(precision + recall);
    tpr(i) = tpl/(tpl+fnl);
    fpr(i) = fpl/(fpl+tnl);
    if i>1
        auc = auc + 0.5*(tpr(i-1) + tpr(i))*(fpr(i-1) - fpr(i));
    end
    if(abs(fpl - fnl) < 100)
        fprintf('tp: %d, fp: %d\n', tpl, fpl);
        fprintf('tn: %d, fn: %d\n', tnl, fnl);
        fprintf('precison: %f\nrecall: %f\nf1: %f\n\n', precision, recall, f1_score);
    end
    
    
end

%plot(fpr, tpr, '-ro');
%hold on;
%t1 = 0:1;
%plot(t1, t1, '-.g');
%hold off;
%fprintf('area under curve: %f\n', auc);

