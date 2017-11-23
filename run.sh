#!/bin/sh

make clean
jobid=$(qsub jobfile | cut -d '.' -f 1)
jobid=$(qsub -W depend=afterok:${jobid} jobfile2 | cut -d '.' -f 1)
jobid=$(qsub -W depend=afterok:${jobid} jobfile4 | cut -d '.' -f 1)
jobid=$(qsub -W depend=afterok:${jobid} jobfile8 | cut -d '.' -f 1)
qsub -W depend=afterok:${jobid} jobresult
