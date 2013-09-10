#!usr/sh


pieces=5
vocab=30
# bash makebag.sh
# ./bagword output/trainFeature.dat output/traincorpus.dat ${vocab} 200 ${pieces}
# ./bagword output/testFeature.dat output/testcorpus.dat ${vocab} 1 ${pieces}

# #to one file
# echo "" >> ./output/traincorpus.dat.total
# for (( i=1; $i <= pieces; i++ ))
# do
# 	cat ./output/traincorpus.dat.${i} >> ./output/traincorpus.dat.total
# done



lda_dir='/home/administrator/Desktop/lda-c-dist/'
alpha=0.1
k=30

# ${lda_dir}lda est ${alpha} $k ${lda_dir}settings.txt ./output/traincorpus.dat random ./output >> /dev/null
# if [ $? -ne 0 ]
# then
# 	echo "excute est failed"
# 	exit 1
# fi

# ${lda_dir}lda inf ${lda_dir}inf-settings.txt ./output/final ./output/testcorpus.dat ./output/result >> /dev/null
# if [ $? -ne 0 ]
# then
# 	echo "excute inf failed"
# 	exit 1
# fi

head -n 7200 ./output/result-lda-lhood.dat > ./output/result-lda-lhood.dat.1
head -n 14400 ./output/result-lda-lhood.dat | tail -n 7200 > ./output/result-lda-lhood.dat.2
head -n 21600 ./output/result-lda-lhood.dat | tail -n 7200 > ./output/result-lda-lhood.dat.3
tail -n 14400 ./output/result-lda-lhood.dat | head -n 7200 > ./output/result-lda-lhood.dat.4
tail -n 7200 ./output/result-lda-lhood.dat > ./output/result-lda-lhood.dat.5
for (( i=1; $i <= pieces; i++ ))
do
	./likelyhood ./output/result-lda-lhood.dat.${i} output/result.dat.${i} -1 ./dataset/label_test_uscd1.dat
done

# for (( i=1; $i <= pieces; i++ ))
# do
# 	echo "i: $i"
# 	${lda_dir}lda inf ${lda_dir}inf-settings.txt ./output/final ./output/testcorpus.dat.${i} ./output/result.${i} >> /dev/null
# 	if [ $? -ne 0 ]
# 	then
# 		echo "excute inf failed"
# 		exit 1;
# 	fi
# 	./likelyhood ./output/result.${i}-lda-lhood.dat output/result.dat.${i} -9 ./dataset/label_test_uscd1.dat
# done


# for (( i=1; $i <= pieces; i++ ))
# do
# 	echo "i: $i"
# 	#${lda_dir}lda est ${alpha} $k ${lda_dir}settings.txt ./output/traincorpus.dat.${i} random ./output >> /dev/null
# 	#if [ $? -ne 0 ]
# 	#then
# 		#echo "excute est failed"
# 		#exit 1;
# 	#fi

# 	${lda_dir}lda inf ${lda_dir}inf-settings.txt ./output/final ./output/testcorpus.dat.${i} ./output/result.${i} >> /dev/null
# 	if [ $? -ne 0 ]
# 	then
# 		echo "excute inf failed"
# 		exit 1;
# 	fi

# 	./likelyhood ./output/result.${i}-lda-lhood.dat output/result.dat.${i} -4 ./dataset/label_test_uscd1.dat
# done

