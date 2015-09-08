#!/bin/bash
# Constant configuration
GNU_RADIO_PIPELINE='/home/ntlhui/workspace/radio_collar_tracker/cw_detect/cw_detect_deploy.py'
RAW_DATA_COMPILER='/home/ntlhui/workspace/radio_collar_tracker/raw_gps_analysis/raw_gps_analysis.py'
DISPLAY_DATA='/home/ntlhui/workspace/radio_collar_tracker/collarDisplay/display_data.py'
META_FILE_READER='/home/ntlhui/workspace/radio_collar_tracker/meta_file_reader/read_meta_file.py'
FREQUENCY_CALCULATOR='/home/ntlhui/workspace/radio_collar_tracker/ppm_adjust/get_beat_frequency.py'
# User supplied configuration
data_dir='/home/ntlhui/workspace/radio_collar_tracker_test/RUN_001075/'
run=1075

# Generated variables
num_raw_files=`ls ${data_dir} | grep 'RAW_DATA_' | wc -l`
# TODO fix this to be modular
num_collars=1
raw_file=`printf "%s/RUN_%06d.raw" ${data_dir} ${run}`
collar_file_prefix=`printf "%s/RUN_%06d_" ${data_dir} ${run}`
meta_file=`printf "%s/META_%06d" ${data_dir} ${run}`
sdr_center_freq=`${META_FILE_READER} -i ${meta_file} -t center_freq`
# TODO fix this to be modular!
sdr_ppm=-64

# Concatenate raw files together
for i in ${num_raw_files}
do
	cat `printf "%s/RAW_DATA_%06d_%06d" ${data_dir} ${run} ${i}` >> ${raw_file}
done
echo ${raw_file}

# Run through GNU Radio pipeline for each collar
for i in ${num_collars}
do
	# Get collar beat frequency
	frequency=168059000
	beat_freq=`${FREQUENCY_CALCULATOR} -f ${sdr_center_freq} -c ${frequency} -e ${sdr_ppm}`
	echo ${beat_freq}
	# Execute pipeline
	${GNU_RADIO_PIPELINE} -f ${beat_freq} -i ${raw_file} -o `printf "%s%06d.raw" ${collar_file_prefix} ${i}`
	${RAW_DATA_COMPILER} -i ${data_dir} -o ${data_dir} -r ${run} -c ${i}
done

# For each collar, make map
for i in ${num_collars}
do
	data_file=`printf '%s/RUN_%06d_COL_%06d.csv' ${data_dir} ${run} ${i}`
	${DISPLAY_DATA} -i ${data_file} -o ${data_dir} -r ${run} -c ${i}
done
