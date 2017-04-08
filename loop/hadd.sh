# use: hadd.sh output/test/

cd $1
hadd -f BBall.root BB-4p-*.root
hadd -f BBBall.root BBB-4p-*.root
cd -
