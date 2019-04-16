#ifndef BENCHMARK_H
#define BENCHMARK_H

// For benchmarking rdtsc and tsc deadline has to be present.
// This function checks those features.
void cpuid_checks();


// Reports are in the format:
//
//  NSAMPLE   MEAN    MIN     MAX     STDDEV    STDDEV/MEAN

#define CHECKPOINT_MAXNS 1

// Type 1 checkpoints measures the time interval between consecutive checkpoints.
//  Argument `n` is an identifier assiciated with each type of checkpoint.
//
// Used for timers.
void type1_checkpoint(int ns);

// Type 2 checkpoints measures the time interval between begin and end checkpoints.
//
// Used for forks etc.
void type2_checkpoint_begin(int ns);
void type2_checkpoint_end(int ns);


extern i64 nc_quota;
void quota_accounting_begin(void);
void quota_accounting_end(void);


// Note touch should in principle only accept non-negative values.
#define TOUCH_MAX_N 8
#define TOUCH_BUF_SZ 8192
void touch(int n, u64 v);
void report_touches();  // by default, resets touch nums
void report_touch(int n);
#endif // BENCHMARK_H
