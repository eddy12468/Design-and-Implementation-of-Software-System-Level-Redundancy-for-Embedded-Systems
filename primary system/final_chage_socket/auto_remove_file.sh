#!/bin/bash
#不同的專案有不同的路徑
array[0]=''

#專案的主幹目錄是相同的
RELEASE="/dmtcp/"
for path in ${RELEASE};
do
#拼接檔案路徑
releasepath=${RELEASE}
cd $releasepath



#判斷是否存在該目錄
if [ $? -eq 0 ];
then
echo $releasepath
echo "Contains file:"
#輸出所有的內容
echo *
num=`ls -l | grep '-' | wc -l`;
fi

if [ $num -gt 3 ];
then
num=`expr $num - 3`
clean=`ls -tr | head -$num | xargs`
echo "will delete file:"
echo ${clean}
#-n1 每次處理1個檔案
ls -tr | head -$num | xargs -i -n1 rm -rf {}

fi
done