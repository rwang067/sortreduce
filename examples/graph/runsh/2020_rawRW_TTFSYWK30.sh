# !/bin/bash

echo "2020.5.18 compare with Graphene/GraFSoft by raw RW in Twitter in Node-2 (SSD-raid0 64GB R730)" >> grafsoft.statistics 
echo "Raw RW from echo, R*10 walks, R vary from 10^3 to 10^9"  >> grafsoft.statistics 
## Raw Random walk
echo "app = Raw RW, dataset = Twitter from echo" >> grafsoft.statistics 
for(( R = 1000; R <= 10000000; R *= 10 ))
do
    echo "R = " $R ", R*10 walks from echo" >> grafsoft.statistics 
    for(( times = 0; times < 5; times++))
    do
        echo "times = " $times " from echo"
        free -m
        sync; echo 1 > /proc/sys/vm/drop_caches
        free -m
        rm /home/kvgroup/ruiwang/data/raid0_defghij_ssd/Twitter/twitter_rv.net_GraFSoft/vertex/*
        ./obj/randomwalks/rawrandomwalks /home/kvgroup/ruiwang/data/raid0_defghij_ssd/Twitter/twitter_rv.net_GraFSoft/vertex /home/kvgroup/ruiwang/data/raid0_defghij_ssd/Twitter/twitter_rv.net_GraFSoft/data/ridx.dat /home/kvgroup/ruiwang/data/raid0_defghij_ssd/Twitter/twitter_rv.net_GraFSoft/data/matrix.dat $R 10
    done
done

echo "2020.5.18 compare with Graphene/GraFSoft by raw RW in Kron30 in Node-2 (SSD-raid0 64GB R730)" >> grafsoft.statistics 
echo "Raw RW from echo, R*10 walks, R vary from 10^3 to 10^9"  >> grafsoft.statistics 
## Raw Random walk
echo "app = Raw RW, dataset = Kron30 from echo" >> grafsoft.statistics 
for(( R = 1000; R <= 100000000; R *= 10 ))
do
    echo "R = " $R ", R*10 walks from echo" >> grafsoft.statistics 
    for(( times = 0; times < 5; times++))
    do
        echo "times = " $times " from echo"
        free -m
        sync; echo 1 > /proc/sys/vm/drop_caches
        free -m
        rm /home/kvgroup/ruiwang/data/raid0_defghij_ssd/Kron30/kron30_32-sorted.txt_GraFSoft/vertex/*
        ./obj/randomwalks/rawrandomwalks /home/kvgroup/ruiwang/data/raid0_defghij_ssd/Kron30/kron30_32-sorted.txt_GraFSoft/vertex /home/kvgroup/ruiwang/data/raid0_defghij_ssd/Kron30/kron30_32-sorted.txt_GraFSoft/data/ridx.dat /home/kvgroup/ruiwang/data/raid0_defghij_ssd/Kron30/kron30_32-sorted.txt_GraFSoft/data/matrix.dat $R 10
    done
done

# echo "2020.5.17 compare with Graphene/GraFSoft by raw RW in Yahoo in Node-2 (SSD-raid0 64GB R730)" >> grafsoft.statistics 
# echo "Raw RW from echo, R*10 walks, R vary from 10^3 to 10^9"  >> grafsoft.statistics 
# ## Raw Random walk
# echo "app = Raw RW, dataset = Yahoo from echo" >> grafsoft.statistics 
# for(( R = 1000; R <= 1000000000; R *= 10 ))
# do
#     echo "R = " $R ", R*10 walks from echo" >> grafsoft.statistics 
#     for(( times = 0; times < 5; times++))
#     do
#         echo "times = " $times " from echo"
#         free -m
#         sync; echo 1 > /proc/sys/vm/drop_caches
#         free -m
#         rm /home/kvgroup/ruiwang/data/raid0_defghij_ssd/Yahoo/GraFSoft/vertex/*
#         ./obj/randomwalks/rawrandomwalks /home/kvgroup/ruiwang/data/raid0_defghij_ssd/Yahoo/GraFSoft/vertex /home/kvgroup/ruiwang/data/raid0_defghij_ssd/Yahoo/GraFSoft/data/ridx.dat /home/kvgroup/ruiwang/data/raid0_defghij_ssd/Yahoo/GraFSoft/data/matrix.dat $R 10
#     done
# done


# echo "2020.5.16 compare with Graphene/GraFSoft by raw RW in Friendster in Node-2 (SSD-raid0 64GB R730)" >> grafsoft.statistics 
# echo "Raw RW from echo, R*10 walks, R vary from 10^3 to 10^9"  >> grafsoft.statistics 
# ## Raw Random walk
# echo "app = Raw RW, dataset = Friendster from echo" >> grafsoft.statistics 
# for(( R = 1000; R <= 1000000000; R *= 10 ))
# do
#     echo "R = " $R ", R*10 walks from echo" >> grafsoft.statistics 
#     for(( times = 0; times < 5; times++))
#     do
#         echo "times = " $times " from echo"
#         free -m
#         sync; echo 1 > /proc/sys/vm/drop_caches
#         free -m
#         rm /home/kvgroup/ruiwang/data/raid0_defghij_ssd/Friendster/GraFSoft/vertex/*
#         ./obj/randomwalks/rawrandomwalks /home/kvgroup/ruiwang/data/raid0_defghij_ssd/Friendster/GraFSoft/vertex /home/kvgroup/ruiwang/data/raid0_defghij_ssd/Friendster/GraFSoft/data/ridx.dat /home/kvgroup/ruiwang/data/raid0_defghij_ssd/Friendster/GraFSoft/data/matrix.dat $R 10
#     done
# done

# ./obj/randomwalks/rawrandomwalks /home/kvgroup/ruiwang/data/raid0_defghij_ssd/Twitter/twitter_rv.net_GraFSoft/vertex /home/kvgroup/ruiwang/data/raid0_defghij_ssd/Twitter/twitter_rv.net_GraFSoft/data/ridx.dat /home/kvgroup/ruiwang/data/raid0_defghij_ssd/Twitter/twitter_rv.net_GraFSoft/data/matrix.dat 1000 10

    
# echo "2019.9.16 compare with GraFSoft by single-source RW in Crawl in SSD 64GB R730" >> grafsoft.statistics 
# echo "single-source RW from echo, R*10 walks, R vary from 10^0 to 10^7"  >> grafsoft.statistics 
# ## Graphlet
# echo "app = single-source RW, dataset = Crawl from echo" >> grafsoft.statistics 
# for(( R = 1000; R <= 1000000000; R*=100))
# do
#     echo "R = " $R ", R*10 walks from echo" >> grafsoft.statistics 
#     for(( times = 0; times < 5; times++))
#     do
#         echo "times = " $times " from echo"
#         free -m
#         sync; echo 1 > /proc/sys/vm/drop_caches
#         free -m
#         rm /home/wang/raid0_defghij/Crawl/GraFSoft/vertex/*
#         ./obj/randomwalks/ppr /home/wang/raid0_defghij/Crawl/GraFSoft/vertex /home/wang/raid0_defghij/Crawl/GraFSoft/data/ridx.dat /home/wang/raid0_defghij/Crawl/GraFSoft/data/matrix.dat 0 1 $R 10 24
#     done
# done


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