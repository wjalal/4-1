#!/bin/bash
gzip -d gene_tpm_2017-06-05_v8_artery_coronary.gct.gz 
mv gene_tpm_2017-06-05_v8_artery_coronary.gct gene_tpm_2017-06-05_v8_artery_coronary.tsv


gcc transpose.c -o transpose
./transpose -t gene_tpm_2017-06-05_v8_artery_coronary.tsv > gtexv8_coronary_artery_trans.tsv
./transpose -h
./transpose -i 57000x250  -t gene_tpm_2017-06-05_v8_artery_coronary.tsv > gtexv8_coronary_artery_trans.tsv
paste
paste -d $'\034' <(seq $(wc -l < "$file") | sort -R) "$file" | awk -F $'\034' -v file="$file" '{
    f = file ($1 <= 20 ? ".20" : ".80")
    print $2 > f
}'
gawk
gawk '
    BEGIN {srand()}
    {f = FILENAME (rand() <= 0.8 ? ".80" : ".20"); print > f}
' gtexv8_coronary_artery_trans.tsv
wc -l gtexv8_coronary_artery_trans.tsv
wc -l gene_tpm_2017-06-05_v8_artery_coronary.tsv
tain -n 10 gtexv8_coronary_artery_trans.tsv
tail -n 10 gtexv8_coronary_artery_trans.tsv
tail -n 9 gtexv8_coronary_artery_trans.tsv
head --lines=-9 gtexv8_coronary_artery_trans.tsv.bak > gtexv8_coronary_artery_trans.tsv
wc -l gene_tpm_2017-06-05_v8_artery_coronary.tsv
wc -l gtexv8_coronary_artery_trans.tsv
head -n 1 gtexv8_coronary_artery_trans.tsv
tail -n 240 gtexv8_coronary_artery_trans.tsv > gtexv8_coronary_artery_trans.tsv
head --lines=-9 gtexv8_coronary_artery_trans.tsv.bak > gtexv8_coronary_artery_trans.tsv
tail -n 240 gtexv8_coronary_artery_trans.tsv > gtexv8_coronary_artery_trans_headless.tsv
wc -l gtexv8_coronary_artery_trans.tsv
wc -l gtexv8_coronary_artery_trans_headless.tsv
gawk 'BEGIN {srand()} {f = FILENAME (rand() <= 0.8 ? ".80" : ".20"); print > f}' gtexv8_coronary_artery_trans_headless.tsv
cat (head -n 1 gtexv8_coronary_artery_trans.tsv)
head -n 1 gtexv8_coronary_artery_trans.tsv
head -n 1 gtexv8_coronary_artery_trans.tsv > gtexv8_coronary_artery_trans_head.txt
cat gtexv8_coronary_artery_trans_head.txt gtexv8_coronary_artery_trans_headless.20.tsv > gtexv8_coronary_artery_TEST.tsv
cat gtexv8_coronary_artery_trans_head.txt gtexv8_coronary_artery_trans_headless.80.tsv > gtexv8_coronary_artery_TRAIN.tsv
sed -i 's/GTEX-*****-****-**-*****/GTEX-*****/g' gtexv8_coronary_artery_TEST.tsv
sed -i 's/GTEX-[^ ]/GTEX-*bogm/g' gtexv8_coronary_artery_TEST.tsv
echo "GTEX-131XG-0826-SM-5LZVS
GTEX-12WSE-1126-SM-7DUEH
GTEX-12696-0526-SM-5EQ3Z" | sed 's/\(GTEX-[[:alnum:]]\+\).*$/\1/'
sed -i 's/\(GTEX-[[:alnum:]]\+\).*$/\1/' gtexv8_coronary_artery_TEST.tsv
echo "GTEX-131XG-0826-SM-5LZVS 5 6 9 3 
GTEX-12WSE-1126-SM-7DUEH 6 9 4 3
GTEX-12696-0526-SM-5EQ3Z 0 5  3 2" | sed 's/\(GTEX-[[:alnum:]]\+\).*$/\1/'
echo "GTEX-131XG-0826-SM-5LZVS 5 6 9 3 
GTEX-12WSE-1126-SM-7DUEH 6 9 4 3
GTEX-12696-0526-SM-5EQ3Z 0 5  3 2" | sed 's/\(GTEX-[[:alnum:]]\+\).*/\1/'
echo "GTEX-131XG-0826-SM-5LZVS 5 6 9 3 
GTEX-12WSE-1126-SM-7DUEH 6 9 4 3
GTEX-12696-0526-SM-5EQ3Z 0 5  3 2" | sed 's/\(GTEX-[[:alnum:]]\+\)*/\1/'
echo "GTEX-131XG-0826-SM-5LZVS 5 6 9 3 
GTEX-12WSE-1126-SM-7DUEH 6 9 4 3
GTEX-12696-0526-SM-5EQ3Z 0 5  3 2" | sed 's/\(GTEX-[-A-B0-9]\+\).*/\1/'
echo "GTEX-131XG-0826-SM-5LZVS 5 6 9 3 
GTEX-12WSE-1126-SM-7DUEH 6 9 4 3
GTEX-12696-0526-SM-5EQ3Z 0 5  3 2" | sed 's/\(GTEX-[-A-Z0-9]\+\).*/\1/'
echo "GTEX-131XG-0826-SM-5LZVS 5 6 9 3 
GTEX-12WSE-1126-SM-7DUEH 6 9 4 3
GTEX-12696-0526-SM-5EQ3Z 0 5  3 2" | sed 's/\(GTEX-[-A-Z0-9]\+\).*$/\1/'
echo "GTEX-131XG-0826-SM-5LZVS 5 6 9 3 
GTEX-12WSE-1126-SM-7DUEH 6 9 4 3
GTEX-12696-0526-SM-5EQ3Z 0 5  3 2" | sed 's/\(GTEX-[[:alnum:]]\+\).*/\1/'
echo "GTEX-131XG-0826-SM-5LZVS 5 6 9 3 
GTEX-12WSE-1126-SM-7DUEH 6 9 4 3
GTEX-12696-0526-SM-5EQ3Z 0 5  3 2" | sed 's/\(GTEX-[[:alnum:]]\+\).*/\1/g'
echo "GTEX-131XG-0826-SM-5LZVS 5 6 9 3 
GTEX-12WSE-1126-SM-7DUEH 6 9 4 3
GTEX-12696-0526-SM-5EQ3Z 0 5  3 2" | sed 's/\(GTEX-[[:alnum:]]-\+\).*/\1/g'
echo "GTEX-131XG-0826-SM-5LZVS 5 6 9 3 
GTEX-12WSE-1126-SM-7DUEH 6 9 4 3
GTEX-12696-0526-SM-5EQ3Z 0 5  3 2" | sed 's/\(GTEX-[[:alnum:]]\+\)-.*/\1/g'
echo "GTEX-131XG-0826-SM-5LZVS 5 6 9 3 
GTEX-12WSE-1126-SM-7DUEH 6 9 4 3
GTEX-12696-0526-SM-5EQ3Z 0 5  3 2" | sed 's/\(GTEX-[[:alnum:]]\+\)-[-A-Z0-9]\+.*/\1/g'
echo "GTEX-131XG-0826-SM-5LZVS 5 6 9 3 
GTEX-12WSE-1126-SM-7DUEH 6 9 4 3
GTEX-12696-0526-SM-5EQ3Z 0 5  3 2" | sed 's/\(GTEX-[[:alnum:]]\+\)-[-A-Z0-9]\+.*/\1/g'
echo "GTEX-131XG-0826-SM-5LZVS 5 6 9 3 
GTEX-12WSE-1126-SM-7DUEH 6 9 4 3
GTEX-12696-0526-SM-5EQ3Z 0 5  3 2" | sed 's/\(GTEX-[[:alnum:]]\+\)-[-A-Z0-9]\+\(.*)/\1\3/g'
echo "GTEX-131XG-0826-SM-5LZVS 5 6 9 3 
GTEX-12WSE-1126-SM-7DUEH 6 9 4 3
GTEX-12696-0526-SM-5EQ3Z 0 5  3 2" | sed 's/\(GTEX-[[:alnum:]]\+)-[-A-Z0-9]\+\(.*)/\1\3/g'
echo "GTEX-131XG-0826-SM-5LZVS 5 6 9 3 
GTEX-12WSE-1126-SM-7DUEH 6 9 4 3
GTEX-12696-0526-SM-5EQ3Z 0 5  3 2" | sed 's/\(GTEX-[[:alnum:]]\+\)-[-A-Z0-9]\+\(.*\)/\1\3/g'
echo "GTEX-131XG-0826-SM-5LZVS 5 6 9 3 
GTEX-12WSE-1126-SM-7DUEH 6 9 4 3
GTEX-12696-0526-SM-5EQ3Z 0 5  3 2" | sed 's/\(GTEX-[[:alnum:]]\+\)-[-A-Z0-9]\+\(.*\)/\1\3/'
echo "GTEX-131XG-0826-SM-5LZVS 5 6 9 3 
GTEX-12WSE-1126-SM-7DUEH 6 9 4 3
GTEX-12696-0526-SM-5EQ3Z 0 5  3 2" | sed 's/\(GTEX-[[:alnum:]]\+\)-[-A-Z0-9]\+\(.*\)/\1d\3/g'
echo "GTEX-131XG-0826-SM-5LZVS 5 6 9 3 
GTEX-12WSE-1126-SM-7DUEH 6 9 4 3
GTEX-12696-0526-SM-5EQ3Z 0 5  3 2" | sed 's/\(GTEX-[[:alnum:]]\+\)-[[:alnum:]]\+-[[:alnum:]]\+-[[:alnum:]]\+\(.*\)/\1\3/'
echo "GTEX-131XG-0826-SM-5LZVS 5 6 9 3 
GTEX-12WSE-1126-SM-7DUEH 6 9 4 3
GTEX-12696-0526-SM-5EQ3Z 0 5  3 2" | sed 's/\(GTEX-[[:alnum:]]\+\)-[[:alnum:]]\+-[[:alnum:]]\+-[[:alnum:]]\+\(.*\)/\1\2/'
echo "GTEX-131XG-0826-SM-5LZVS 5 6 9 3 
GTEX-12WSE-1126-SM-7DUEH 6 9 4 3
GTEX-12696-0526-SM-5EQ3Z 0 5  3 2" | sed 's/\([[:alnum:]]\+-[[:alnum:]]\+\)-[[:alnum:]]\+-[[:alnum:]]\+-[[:alnum:]]\+\(.*\)/\1\2/'
sed -i 's/\([[:alnum:]]\+-[[:alnum:]]\+\)-[[:alnum:]]\+-[[:alnum:]]\+-[[:alnum:]]\+\(.*\)/\1\2/' gtexv8
sed -i 's/\([[:alnum:]]\+-[[:alnum:]]\+\)-[[:alnum:]]\+-[[:alnum:]]\+-[[:alnum:]]\+\(.*\)/\1\2/' gtexv8_coronary_artery_TEST.tsv
sed -i 's/\([[:alnum:]]\+-[[:alnum:]]\+\)-[[:alnum:]]\+-[[:alnum:]]\+-[[:alnum:]]\+\(.*\)/\1\2/' gtexv8_coronary_artery_TRAIN.tsv
sed 's/\([0-9]\)0-[0-9]9/\14.5' GTEx_Analysis_v8_Annotations_SubjectPhenotypesDS-rangemid.txt 
sed 's/\([0-9]\)0-[0-9]9/\14.5/' GTEx_Analysis_v8_Annotations_SubjectPhenotypesDS-rangemid.txt 
sed -i  's/\([0-9]\)0-[0-9]9/\14.5/' GTEx_Analysis_v8_Annotations_SubjectPhenotypesDS-rangemid.txt 
htop
sed 's/\t2\t/\t\t/' GTEx_Analysis_v8_Annotations_SubjectPhenotypesDS-rangemid.txt 
sed 's/\t2\t/\t1\t/' GTEx_Analysis_v8_Annotations_SubjectPhenotypesDS-rangemid.txt 
sed 's/\t2\t/\t1\t/
s/\t1\t/\t0\t/' GTEx_Analysis_v8_Annotations_SubjectPhenotypesDS-rangemid.txt 
sed 's/\t1\t/\t0\t/;s/\t2\t/\t1\t/' GTEx_Analysis_v8_Annotations_SubjectPhenotypesDS-rangemid.txt 
sed -i 's/\t1\t/\t0\t/;s/\t2\t/\t1\t/' GTEx_Analysis_v8_Annotations_SubjectPhenotypesDS-rangemid.txt 
htop
warp-cli disconnect
