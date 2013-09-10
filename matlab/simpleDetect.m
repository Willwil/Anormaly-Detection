clear all;

load output/normalisedTest.dat;
[m, n] = size(normalisedTest);
p = 20.0;

state = zeros(m, 1);

for i = 1:n
    [N C] = imhist(normalisedTest(:, i));
    %bar(C, N);
    re = (N <= 28.3465 * p);
    sum(re);
    %disp(sum(N))
    C = C(re);
    l = length(C);
    for j = 1:l
        throwout = (normalisedTest(:, i) > C(j)-1/254 & ...
        normalisedTest(:, i) <= C(j) + 1/254);
        state = state | throwout;
        sum(state);
    end
    %disp(state(1:30))
    %N = N(re);
    %length(N)
    %length(anormalset);
end

% state = zeros(m, n);
% l = length(anormalset)
% for i = 1:l
%     re = (normalisedTest > anormalset(i)-0.5 & ...
%     normalisedTest <= anormalset(i) + 0.5);
%     state = state | re;
% end

%disp('rate: ')
%result = any(state, 2);
result = state;
posN = sum(result);
%disp(state(1:10, 1:20))


%disp('comparing result')
load dataset/label_test_uscd1.dat;
label = reshape(label_test_uscd1', m, 1);
normalIndex = find(label);
anormalIndex = find(label == 1);
an = length(anormalIndex);
nn = m - an;
tp = result & label;
fp = result & (~label);
tn = (~result) & (~label);
fn = (~result) & label;
tpl = sum(sum(tp))
fpl = sum(sum(fp))
tnl = sum(sum(tn))
fnl = sum(sum(fn))
precision = tpl/(tpl + fpl)
accuracy = (tpl + tnl)/m
recall = tpl/(tpl + fnl)
f1_score = 2*precision*recall/(precision + recall)







