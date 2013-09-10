clear all;

load output/testFeature.dat
[m, n] = size(testFeature);

load dataset/label_test_uscd1.dat;
label = reshape(label_test_uscd1', m, 1);
normalIndex = find(label);
anormalIndex = find(label == 1);
an = length(anormalIndex);
nn = m - an;

normalFeature = abs(testFeature(normalIndex));
anormalFeature = abs(testFeature(anormalIndex));

av1 = sum(normalFeature)./nn
max1 = max(normalFeature)
hist(normalFeature(:, 1))
pause(1)

av2 = sum(anormalFeature)./an
max2 = max(anormalFeature)
hist(anormalFeature(:, 1))