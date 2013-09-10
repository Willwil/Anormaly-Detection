clear all;

load output/trainFeature.dat;
load output/testFeature.dat;


[n1, m1] = size(trainFeature);
[n, m2] = size(testFeature);
result = zeros(1, n);

[wcoef, score] = princomp(trainFeature);
testscore = testFeature * wcoef;

k = 8;
seg = 2;
groups = size(trainFeature, 2)/seg;
groups = 10;
v = 1:k;
wc=zeros(k);
threshod = 500;

for j=1:groups
    disp('iters ')
    ends = j*seg;
    selectCols = ends-seg+1:ends;
    c = corr(score(:, selectCols(:)), score(:, selectCols(:)));
    disp(c);
    pause(5);
    OPTIONS = statset('MaxIter', 200, 'Display', 'off');
    obj = gmdistribution.fit(score(:, selectCols(:)), k, 'Options', OPTIONS);
    [idx, nlogl, post, logpdf] = cluster(obj, testscore(:, selectCols(:)));
    %disp(nlogl);
%     disp(min(logpdf));
%     disp(max(logpdf));
%     disp(mean(logpdf));
%     disp(std(logpdf));
    good = (logpdf >= mean(logpdf) - 0.2*std(logpdf));
    result(good(:)) = result(good(:))+1;
    
    
%     for i=1:k
%         wc(i)=length(find(idx == i));
%     end;
%     %disp(wc);
%     %plot(v, wc);
%     good = find(wc > threshod);
%     goodL = length(good);  
%     for i=1:goodL
%         goodItem = find(idx==good(i));
%         result(goodItem(:)) = result(goodItem(:))+1;
%     end
end

clear trainFeature;
clear testFeature;
results = result;
result = results <= groups * 0.9;

load dataset/label_test_uscd1.dat
label = reshape(label_test_uscd1', 1, n);
clear label_test_uscd1;

tp = result & label;
fp = result & (~label);
tn = (~result) & (~label);
fn = (~result) & label;
tpl = sum(sum(tp))
fpl = sum(sum(fp))
tnl = sum(sum(tn))
fnl = sum(sum(fn))
precision = tpl/(tpl + fpl)
accuracy = (tpl + tnl)/n
recall = tpl/(tpl + fnl)
f1_score = 2*precision*recall/(precision + recall)




