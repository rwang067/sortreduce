# !/bin/bash
    
echo "2019.9.16 compare with GraFSoft by single-source RW in Crawl in SSD 64GB R730" >> grafsoft.statistics 
echo "single-source RW from echo, R*10 walks, R vary from 10^0 to 10^7"  >> grafsoft.statistics 
## Graphlet
echo "app = single-source RW, dataset = Crawl from echo" >> grafsoft.statistics 
for(( R = 1000; R <= 1000000000; R*=100))
do
    echo "R = " $R ", R*10 walks from echo" >> grafsoft.statistics 
    for(( times = 0; times < 5; times++))
    do
        echo "times = " $times " from echo"
        free -m
        sync; echo 1 > /proc/sys/vm/drop_caches
        free -m
        rm /home/wang/raid0_defghij/Crawl/GraFSoft/vertex/*
        ./obj/randomwalks/ppr /home/wang/raid0_defghij/Crawl/GraFSoft/vertex /home/wang/raid0_defghij/Crawl/GraFSoft/data/ridx.dat /home/wang/raid0_defghij/Crawl/GraFSoft/data/matrix.dat 0 1 $R 10 24
    done
done


# echo "2019.9.16 compare with GraFSoft by single-source RW in Friendster in SSD 64GB R730" >> grafsoft.statistics 
# echo "single-source RW from echo, R*10 walks, R vary from 10^0 to 10^7"  >> grafsoft.statistics 
# ## Graphlet
# echo "app = single-source RW, dataset = Friendster from echo" >> grafsoft.statistics 
# for(( R = 1000; R <= 1000000000; R*=10))
# do
#     echo "R = " $R ", R*10 walks from echo" >> grafsoft.statistics 
#     for(( times = 0; times < 5; times++))
#     do
#         echo "times = " $times " from echo"
#         free -m
#         sync; echo 1 > /proc/sys/vm/drop_caches
#         free -m
#         rm /home/wang/raid0_defghij/Friendster/GraFSoft/vertex/*
#         ./obj/randomwalks/ppr /home/wang/raid0_defghij/Friendster/GraFSoft/vertex /home/wang/raid0_defghij/Friendster/GraFSoft/data/ridx.dat /home/wang/raid0_defghij/Friendster/GraFSoft/data/matrix.dat 12 1 $R 10 24
#     done
# done