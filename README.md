# RISC-V-CPU-Simulator
This is a C++ based RISC-V CPU simulation

1st stage implementation: basic structure **current stage**  
1.32 registers  
2.RAM  
3.CPU cores  

2nd stage implementation: single cycle  
1.fetch+decode+execute+memory+write back

3rd stage implementation: 5 stage pipeline  
1.IF-IF/ID-ID-ID/EX-EX/MEM-MEM/WB-WB 
2.pipline register  

4rd stage implementation: Hazard Process  
1.forwarding Unit  
2.Stall Unit  

5st stage implementation: Cache simulation
1.direct mapped 
2.set associative  
3.cache statistics

6st stage implementation: Branch Prediction  
1.2-bit saturating counter  

7st stage: benchmark:  
1.CPI  

