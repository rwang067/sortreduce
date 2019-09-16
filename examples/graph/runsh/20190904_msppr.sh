# !/bin/bash
    
# echo "2019.9.4 compare with GraFSoft by msppr in Crawl in SSD 64GB R730" >> grafsoft.statistics 

# echo "MSPPR from echo, 2000*10 walks for each soource, numsources from 10^0 to 10^7"  >> grafsoft.statistics 
# ## Graphlet
# echo "app = MSPPR, dataset = Crawl from echo" >> grafsoft.statistics 
# for(( numsources = 1; numsources <= 10000000; numsources*=10))
# do
#     echo "numsources = " $numsources ", fixed 2000*10 walks for each soource from echo" >> grafsoft.statistics 
#     for(( times = 0; times < 5; times++))
#     do
#         echo "times = " $times " from echo"
#         free -m
#         sync; echo 1 > /proc/sys/vm/drop_caches
#         free -m
#         ./obj/randomwalks/ppr /home/wang/raid0_defghij/Crawl/GraFSoft/ /home/wang/raid0_defghij/Crawl/GraFSoft/data/ridx.dat /home/wang/raid0_defghij/Crawl/GraFSoft/data/matrix.dat 0 $numsources 24
#     done
# done

echo "2019.9.4 compare with GraFSoft by msppr in Friendster in SSD 64GB R730" >> grafsoft.statistics 

echo "MSPPR from echo, 2000*10 walks for each soource, numsources from 10^0 to 10^7"  >> grafsoft.statistics 
## Graphlet
echo "app = MSPPR, dataset = Friendster from echo" >> grafsoft.statistics 
for(( numsources = 1; numsources <= 10000000; numsources*=10))
do
    echo "numsources = " $numsources ", fixed 2000*10 walks for each soource from echo" >> grafsoft.statistics 
    for(( times = 0; times < 5; times++))
    do
        echo "times = " $times " from echo"
        free -m
        sync; echo 1 > /proc/sys/vm/drop_caches
        free -m
        ./obj/randomwalks/ppr /home/wang/raid0_defghij/Friendster/GraFSoft/ /home/wang/raid0_defghij/Friendster/GraFSoft/data/ridx.dat /home/wang/raid0_defghij/Friendster/GraFSoft/data/matrix.dat 0 $numsources 24
    done
done