# COMP2020-P5-Cache Simulator
## Team Members
- Le Hoang Vinh
- Bui Duc Khanh An
- Le Nu Uyen Phuong
- Tong Duy Hai

## Write-up question and graph

### Graph #1: Miss Rate vs Cache Size
<p align="center">
  <img src="https://user-images.githubusercontent.com/84661482/151161395-18ff7e0e-5a57-4583-b1db-99ab5e8b46b3.png">
</p>

### Questions 1-8:
1. What is the smallest capacity that brings the miss rate to less than 10%? 

   **Answer:** 2^12B (9.05% miss).

2. What is the smallest capacity that brings the miss rate to less than 5%?

   **Answer:** 2^14B (1.62% miss).
   
3. Today's processors generally have 32KB to 128KB first-level data caches. By what ratio does increasing the cache size from 16KB to 32KB reduce the miss rate? (2.0 would be halving the miss rate; 1.0 would be no change in miss rate; less than 1.0 would be an increase in misses).

   **Answer:** Ratio = 1.62/1.34 ~ 1.21.
   
4. By what ratio does increasing the cache size from 32KB to 64KB reduce the miss rate?

   **Answer:** Ratio = 1.34/1.17 ~ 1.15.
   
5. When deciding on an ideal cache size, engineers typically look for the "knee" of the curve. (Links to an external site.) When considering various cache sizes, we want the point at which increasing to that size yields a great benefit, but increasing beyond that size yields far less benefit. What would you say is the ideal cache size for a direct mapped cache? 

   **Answer:** 2^14B. From the graph, knee of the curve could be observed at x = 14 (Log of cache size 14). 
 
6. What is the smallest capacity that brings the miss rate of the 2-way set associative cache to less than 10%?

   **Answer:** 2^12B (8.53% miss).
   
7. What is the smallest capacity that brings the miss rate of the 2-way set associative cache to less than 5%?

   **Answer:** 2^13B (2.66% miss).
   
8. How large must the direct-mapped cache be before it equals or exceeds the performance of the 1 KB 2-way assoc?

   **Answer:** 2^11B. The miss rate of 1 KB 2-way set assoc is 11.18%. Hence, direct-mapped cache must at least have the capacity of 2^11B / 2KB  (12.86% miss) before it outperforms the performance of the 1 KB 2-way set associative cache.

### Graph #2: Bus Writes vs Cache Size for 2 types of Caches
<p align="center">
  <img src="https://user-images.githubusercontent.com/84661482/151166658-37b368eb-caa9-46c3-84cc-0aa6cf763aba.png">
</p>

### Questions 9-11:
9. At what cache size do the two write policies generate approximately the same amount of writes to the bus?

   **Answer:** 1KB. It could be observed from the graph, at x = 10 (cache size 1 KB), the two write policies generate almost the same amount of writes to the bus.
 
10. Why does the difference between the two schemes diverge at small cache sizes?

    **Answer:** 
    - Write-through cache: the traffic is the average number of bytes written by each store times the number of stores, therefore the traffics are consistent for every trace regardless of the cache size. 
    - Write-back cache: the traffic is the number of dirty evictions times the block size. 
    Since at most one eviction happens per miss and small cache sizes result in more miss rates, thus, small cache sizes result in large amount of write-back data. Therefore, the difference between the two write policies will diverge when cache size is small.
   
11. Why does the difference between the two schemes diverge at large cache sizes?

    **Answer:** Based on Q10 and graph #2, writethrough traffic is a constant for each trace, while larger cache sizes result in smaller writeback data. Hence, as cache size grow, the difference between write-back and write-through traffic will diverge.

### Graph #3: Miss Rate vs. Block Size
<p align="center">
  <img src="https://user-images.githubusercontent.com/84661482/151237307-ac96285e-f94a-45f3-982a-79db45c43241.png">
</p>

### Questions 12-14:
12. Explain the observed miss rate associated with a small block size.

    **Answer:** As the block size decreases, the number of cold misses increases. This is because we can't take use of the spatial locality, which potentially leads to more cold misses.
   
13. Explain the observed miss rate associated with a large block size.

    **Answer:** As the block size increases, the number of cold misses decreases. This is due to the fact that we have exploited the spatial locality, generating less cold misses. However, conflict misses are more frequent and increases as cold misses decreases.
    
14. What is the block size with the lowest miss rate?

    **Answer:** 512B (0.52% miss).
   
### Graph #4: Total Memory Traffic vs Block Size
<p align="center">
  <img src="https://user-images.githubusercontent.com/84661482/151290015-ff03abfd-25e6-4248-bf1b-04afa1525b99.png">
</p>

### Questions 15-17:
15. What is the block size with the lowest total write-back traffic (transferred in + write-back transferred out)

    **Answer:** 4B.
   
16. What are the two sources of additional traffic as the block size grows? Explain why each grows.

    **Answer:** Two sources of additional traffic as the block size grows:
    - Data transfers to fill cache blocks (the number of misses times the block size). 
    - Write traffics (for the write-back cache system, the traffic is the number of dirty evictions times the block size). 
    These two sources are directly propotional to block size, hence traffice would increase as block size grows.
   
17. Given that current processors typically use, say, 64B blocks, which metric (miss rate or traffic) are today's caches designed to minimize?

    **Answer:** From the data and graph, we could classify 64B block to be "small" block size. Given small block size and the graphs, we could observe that:
    - Graph #1: Miss rate are high given small block size.
    - Graph #4: Traffic are small given small block size.
    Therefore it is better for today's caches to be designed to focus on improving the high miss rate (reduce miss) given small block size like 64B and already low traffic.
