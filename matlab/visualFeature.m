clear all;

fprintf('loading train feature...\n');




fprintf('loading test labels...\n');
load ../dataset/label_test_uscd1.dat;
[m, n] = size(label_test_uscd1);
n = m*n;
label = reshape(label_test_uscd1', 1, n);

selectn = 600;

%selected normal
normalItem = find(label == 0);
maxn = size(normalItem, 2);
normalRandom = randi(maxn, 1, selectn);
normalSelected = normalItem(normalRandom);

%selected anormal
anormalItem = find(label == 1);
maxn = size(anormalItem, 2);
anormalRandom = randi(maxn, 1, selectn);
anormalSelected = anormalItem(anormalRandom);

%test
fprintf('loading test feature...\n');
load ../output/testFeature.dat;
minValue = min(min(abs(testFeature)));
maxValue = max(max(abs(testFeature)));


normalData = testFeature(normalSelected, :);
anormalData = testFeature(anormalSelected, :);
% normalData = floor((normalData - minValue)./(maxValue-minValue));
% anormalData = floor((anormalData - minValue)./(maxValue - minValue));

%image
% figure(1);
% subplot(1, 2, 1);
% image(normalData);
% subplot(1, 2, 2);
% image(anormalData);

x = 1:selectn;
normalMax = max(normalData, [], 2);
anormalMax = max(anormalData, [], 2);

% subplot(1, 2, 1);
% [hn1, center1] = hist(normalMax, 50);
% plot(center1, hn1);
% subplot(1, 2, 2);
% [hn2, center2] = hist(anormalMax, 50);
% plot(center2, hn2);


plot(x, normalMax, '*g');
hold on;
anormalMax = max(anormalData');
plot(x, anormalMax, 'or');
hold off;

% 
% threshold = 10;
% dan = size(find(normalMax > threshold));
% dnn = size(find(anormalMax > threshold));
% disp(dan);
% disp(dnn);