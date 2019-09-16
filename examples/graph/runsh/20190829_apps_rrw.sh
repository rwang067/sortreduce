# !/bin/bash
    
# echo "2019.8.25 compare with GraFSoft by apps in SSD 64GB R730" >> grafsoft.statistics 

# ## FS
# ##########################

# ## PersonalizedPageRank
# echo "app = PersonalizedPageRank, dataset = Friendster from echo" >> grafsoft.statistics 
# echo "R = 2000, L = 10, choose prob=0.2 from echo" >> grafsoft.statistics
# for(( times = 0; times < 5; times++))
# do
#     echo "times = " $times " from echo"
#     free -m
#     sync; echo 1 > /proc/sys/vm/drop_caches
#     free -m
#     ./obj/randomwalks/ppr /home/wang/raid0_defghij/Friendster/GraFSoft/ /home/wang/raid0_defghij/Friendster/GraFSoft/data/ridx.dat /home/wang/raid0_defghij/Friendster/GraFSoft/data/matrix.dat 12 1 24
# done

# ## SimRank
# echo "app = SimRank, dataset = Friendster from echo" >> grafsoft.statistics 
# echo "R = 2000, L = 11, choose prob=0.2 from echo" >> grafsoft.statistics 
# for(( times = 0; times < 5; times++))
# do
#     echo "times = " $times " from echo"
#     free -m
#     sync; echo 1 > /proc/sys/vm/drop_caches
#     free -m
#     ./obj/randomwalks/sr /home/wang/raid0_defghij/Friendster/GraFSoft/ /home/wang/raid0_defghij/Friendster/GraFSoft/data/ridx.dat /home/wang/raid0_defghij/Friendster/GraFSoft/data/matrix.dat 12 13 24
# done

# ## Graphlet
# echo "app = Graphlet, dataset = Friendster from echo" >> grafsoft.statistics 
# echo "R = 100000, L = 4, choose prob=0.2 from echo" >> grafsoft.statistics 
# for(( times = 0; times < 5; times++))
# do
#     echo "times = " $times " from echo"
#     free -m
#     sync; echo 1 > /proc/sys/vm/drop_caches
#     free -m
#     ./obj/randomwalks/gl /home/wang/raid0_defghij/Friendster/GraFSoft/ /home/wang/raid0_defghij/Friendster/GraFSoft/data/ridx.dat /home/wang/raid0_defghij/Friendster/GraFSoft/data/matrix.dat 100000 4 24
# done

# ## Friendster   
# echo "app = RandomWalkDomination, dataset = Friendster from echo" >> grafsoft.statistics 
# echo "R = 1×N, L = 6, choose prob=0.2 from echo" >> grafsoft.statistics 
# for(( times = 0; times < 5; times++))
# do
#     echo "times = " $times " from echo"
#     free -m
#     sync; echo 1 > /proc/sys/vm/drop_caches
#     free -m
#     ./obj/randomwalks/rwd /home/wang/raid0_defghij/Friendster/GraFSoft/ /home/wang/raid0_defghij/Friendster/GraFSoft/data/ridx.dat /home/wang/raid0_defghij/Friendster/GraFSoft/data/matrix.dat 1 6 24
# done

## Kron31
##########################

# ## PersonalizedPageRank
# echo "app = PersonalizedPageRank, dataset = Kron31 from echo" >> grafsoft.statistics 
# echo "R = 2000, L = 10, choose prob=0.2 from echo" >> grafsoft.statistics 
# for(( times = 0; times < 5; times++))
# do
#     echo "times = " $times " from echo"
#     free -m
#     sync; echo 1 > /proc/sys/vm/drop_caches
#     free -m
#     ./obj/randomwalks/ppr /home/wang/raid0_defghij/Kron31/GraFSoft/ /home/wang/raid0_defghij/Kron31/GraFSoft/data/ridx.dat /home/wang/raid0_defghij/Kron31/GraFSoft/data/matrix.dat 0 1 24
# done

# ## SimRank
# echo "app = SimRank, dataset = Kron31 from echo" >> grafsoft.statistics 
# echo "R = 2000, L = 11, choose prob=0.2 from echo" >> grafsoft.statistics 
# for(( times = 0; times < 5; times++))
# do
#     echo "times = " $times " from echo"
#     free -m
#     sync; echo 1 > /proc/sys/vm/drop_caches
#     free -m
#     ./obj/randomwalks/sr /home/wang/raid0_defghij/Kron31/GraFSoft/ /home/wang/raid0_defghij/Kron31/GraFSoft/data/ridx.dat /home/wang/raid0_defghij/Kron31/GraFSoft/data/matrix.dat 0 6 24
# done

# ## Graphlet
# echo "app = Graphlet, dataset = Kron31 from echo" >> grafsoft.statistics 
# echo "R = 100000, L = 4, choose prob=0.2 from echo" >> grafsoft.statistics 
# for(( times = 0; times < 5; times++))
# do
#     echo "times = " $times " from echo"
#     free -m
#     sync; echo 1 > /proc/sys/vm/drop_caches
#     free -m
#     ./obj/randomwalks/gl /home/wang/raid0_defghij/Kron31/GraFSoft/ /home/wang/raid0_defghij/Kron31/GraFSoft/data/ridx.dat /home/wang/raid0_defghij/Kron31/GraFSoft/data/matrix.dat 100000 4 24
# done

# ## RWD   
# echo "app = RandomWalkDomination, dataset = Kron31 from echo" >> grafsoft.statistics 
# echo "R = 1×N, L = 6, choose prob=0.2 from echo" >> grafsoft.statistics 
# for(( times = 0; times < 5; times++))
# do
#     echo "times = " $times " from echo"
#     free -m
#     sync; echo 1 > /proc/sys/vm/drop_caches
#     free -m
#     ./obj/randomwalks/rwd /home/wang/raid0_defghij/Kron31/GraFSoft/ /home/wang/raid0_defghij/Kron31/GraFSoft/data/ridx.dat /home/wang/raid0_defghij/Kron31/GraFSoft/data/matrix.dat 1 6 24
# done

# ## Crawl
# ##########################

# ## PersonalizedPageRank
# echo "app = PersonalizedPageRank, dataset = Crawl from echo" >> grafsoft.statistics 
# echo "R = 2000, L = 10, choose prob=0.2 from echo" >> grafsoft.statistics 
# for(( times = 0; times < 5; times++))
# do
#     echo "times = " $times " from echo"
#     free -m
#     sync; echo 1 > /proc/sys/vm/drop_caches
#     free -m
#     ./obj/randomwalks/ppr /home/wang/raid0_defghij/Crawl/GraFSoft/ /home/wang/raid0_defghij/Crawl/GraFSoft/data/ridx.dat /home/wang/raid0_defghij/Crawl/GraFSoft/data/matrix.dat 0 1 24
# done

# ## SimRank
# echo "app = SimRank, dataset = Crawl from echo" >> grafsoft.statistics 
# echo "R = 2000, L = 11, choose prob=0.2 from echo" >> grafsoft.statistics 
# for(( times = 0; times < 5; times++))
# do
#     echo "times = " $times " from echo"
#     free -m
#     sync; echo 1 > /proc/sys/vm/drop_caches
#     free -m
#     ./obj/randomwalks/sr /home/wang/raid0_defghij/Crawl/GraFSoft/ /home/wang/raid0_defghij/Crawl/GraFSoft/data/ridx.dat /home/wang/raid0_defghij/Crawl/GraFSoft/data/matrix.dat 0 6 24
# done

# ## Graphlet
# echo "app = Graphlet, dataset = Crawl from echo" >> grafsoft.statistics 
# echo "R = 100000, L = 4, choose prob=0.2 from echo" >> grafsoft.statistics 
# for(( times = 0; times < 5; times++))
# do
#     echo "times = " $times " from echo"
#     free -m
#     sync; echo 1 > /proc/sys/vm/drop_caches
#     free -m
#     ./obj/randomwalks/gl /home/wang/raid0_defghij/Crawl/GraFSoft/ /home/wang/raid0_defghij/Crawl/GraFSoft/data/ridx.dat /home/wang/raid0_defghij/Crawl/GraFSoft/data/matrix.dat 100000 4 24
# done

# ## RWD   
# echo "app = RandomWalkDomination, dataset = Crawl from echo" >> grafsoft.statistics 
# echo "R = 1×N, L = 6, choose prob=0.2 from echo" >> grafsoft.statistics 
# for(( times = 0; times < 5; times++))
# do
#     echo "times = " $times " from echo"
#     free -m
#     sync; echo 1 > /proc/sys/vm/drop_caches
#     free -m
#     ./obj/randomwalks/rwd /home/wang/raid0_defghij/Crawl/GraFSoft/ /home/wang/raid0_defghij/Crawl/GraFSoft/data/ridx.dat /home/wang/raid0_defghij/Crawl/GraFSoft/data/matrix.dat 1 6 24
# done


# echo "Raw Random walks from echo, L = 10, vary R from 10^3 to 10^10"  >> grafsoft.statistics 
# ## Graphlet
# echo "app = Graphlet, dataset = Crawl from echo" >> grafsoft.statistics 
# for(( R = 100000000; R <= 10000000000; R*=10))
# do
#     echo "R = " $R ", fixed L = 10, from echo" >> grafsoft.statistics 
#     for(( times = 0; times < 2; times++))
#     do
#         echo "times = " $times " from echo"
#         free -m
#         sync; echo 1 > /proc/sys/vm/drop_caches
#         free -m
#         ./obj/randomwalks/gl /home/wang/raid0_defghij/Crawl/GraFSoft/ /home/wang/raid0_defghij/Crawl/GraFSoft/data/ridx.dat /home/wang/raid0_defghij/Crawl/GraFSoft/data/matrix.dat $R 10 24
#     done
# done


# echo "Raw Random walks from echo, R = 100000, vary L from 2^2 to 2^12"  >> grafsoft.statistics 
# ## Graphlet
# echo "app = Graphlet, dataset = Crawl from echo" >> grafsoft.statistics 
# for(( L = 4; L <= 4096; L*=2))
# do
#     echo "L = " $L ", fixed R = 10^5, from echo" >> grafsoft.statistics 
#     for(( times = 0; times < 2; times++))
#     do
#         echo "times = " $times " from echo"
#         free -m
#         sync; echo 1 > /proc/sys/vm/drop_caches
#         free -m
#         ./obj/randomwalks/gl /home/wang/raid0_defghij/Crawl/GraFSoft/ /home/wang/raid0_defghij/Crawl/GraFSoft/data/ridx.dat /home/wang/raid0_defghij/Crawl/GraFSoft/data/matrix.dat 100000 $L 24
#     done
# done

echo "2019.8.29 compare with GraFSoft by rrw on FS in SSD 64GB R730" >> grafsoft.statistics 

echo "Raw Random walks from echo, L = 10, vary R from 10^3 to 10^10"  >> grafsoft.statistics 
## Graphlet
echo "app = Graphlet, dataset = Friendster from echo" >> grafsoft.statistics 
for(( R = 1000; R <= 10000000000; R*=10))
do
    echo "R = " $R ", fixed L = 10, from echo" >> grafsoft.statistics 
    for(( times = 0; times < 5; times++))
    do
        echo "times = " $times " from echo"
        free -m
        sync; echo 1 > /proc/sys/vm/drop_caches
        free -m
        ./obj/randomwalks/gl /home/wang/raid0_defghij/Friendster/GraFSoft/ /home/wang/raid0_defghij/Friendster/GraFSoft/data/ridx.dat /home/wang/raid0_defghij/Friendster/GraFSoft/data/matrix.dat $R 10 24
    done
done


echo "Raw Random walks from echo, R = 100000, vary L from 2^2 to 2^12"  >> grafsoft.statistics 
## Graphlet
echo "app = Graphlet, dataset = Friendster from echo" >> grafsoft.statistics 
for(( L = 4; L <= 4096; L*=2))
do
    echo "L = " $L ", fixed R = 10^5, from echo" >> grafsoft.statistics 
    for(( times = 0; times < 5; times++))
    do
        echo "times = " $times " from echo"
        free -m
        sync; echo 1 > /proc/sys/vm/drop_caches
        free -m
        ./obj/randomwalks/gl /home/wang/raid0_defghij/Friendster/GraFSoft/ /home/wang/raid0_defghij/Friendster/GraFSoft/data/ridx.dat /home/wang/raid0_defghij/Friendster/GraFSoft/data/matrix.dat 100000 $L 24
    done
done