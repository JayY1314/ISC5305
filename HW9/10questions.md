### 1. Memory Layout

Flattening from `vector<vector<double>>` to `vector<double>` improves performance because:
- **Contiguous Memory**: All elements are stored in a single contiguous block, enabling efficient hardware prefetching
- **TLB Efficiency**: Reduces TLB pressure by accessing fewer memory pages
- **Cache Performance**: Better spatial locality allows entire cache lines to be utilized effectively
- **No Pointer Indirection**: Eliminates the overhead of following pointers to access matrix elements

### 2. Cache Associativity

For very small N (e.g., N=16), the Column-Major penalty would be less severe because:
- The entire matrix (16×16 = 256 doubles = 2KB) fits in L1 cache
- Cache misses are minimal regardless of access pattern
- The performance difference becomes significant only when the matrix exceeds cache capacity

### 3. False Sharing

Using atomic updates in the inner loop causes "cache line ping-ponging":
- Multiple threads writing to different elements in the same cache line (64 bytes)
- Each write invalidates the cache line in other cores
- Forces constant cache line transfers between cores, destroying performance
- Solution: Accumulate locally, then write once (as implemented)

### 4. OpenMP Scheduling

`schedule(guided)` outperforms `schedule(dynamic)` when:
- Work chunk sizes vary significantly
- Guided reduces scheduling overhead by starting with large chunks and decreasing size
- Dynamic has constant overhead per chunk request, which becomes expensive for many small chunks

### 5. Thread Overhead

Creating/destroying threads in the measurement loop is bad because:
- Thread creation has significant overhead (memory allocation, OS calls)
- This overhead contaminates timing measurements
- **Design Pattern**: Use a thread pool pattern - create threads once, reuse them for multiple tasks

### 6. Hardware Oversubscription

When T > P_phys, the OS must:
- **Time-slice threads** across physical cores using a scheduler
- **Context switch** between threads, saving/restoring register state
- **Cache pollution** as different threads' data compete for cache space
- This incurs latency from context switch overhead and reduced cache efficiency

### 7. MPI vs. Shared Memory

MPI has no simple auto-balance equivalent because:
- **No shared memory**: Processes cannot share an atomic counter
- **Communication cost**: Dynamic work distribution requires explicit message passing
- **Design philosophy**: MPI favors explicit, predictable communication patterns
- Load balancing must be implemented manually (cyclic, master-worker, etc.)

### 8. Collective Operations

`MPI_Scatterv` is preferred over a loop of `MPI_Send` because:
- **Algorithmic complexity**: Collective operations can use optimized algorithms (e.g., tree-based) with O(log P) complexity vs. O(P) for linear sends
- **Network topology awareness**: MPI implementations optimize for network topology
- **Reduced message overhead**: Fewer message headers and better bandwidth utilization
- **Synchronization**: Built-in synchronization reduces deadlock risk

### 9. Deadlocks

This condition is called a **deadlock**. It occurs when:
- Process A waits for data from B while holding data B needs
- Both processes block indefinitely
- **Solution**: Non-blocking communication (`MPI_Isend`/`MPI_Irecv`) allows overlapping communication and computation, preventing circular dependencies

### 10. Amdahl's Law

Given: Speedup = 10x on 16 cores, Serial fraction = 5%

Amdahl's Law: Speedup_max = 1 / (S + (1-S)/P)

Where S = serial fraction = 0.05, P = number of cores

With infinite cores: Speedup_max = 1 / 0.05 = **20x maximum speedup**

The 5% serial portion limits maximum speedup to 20x regardless of parallelism.

---


