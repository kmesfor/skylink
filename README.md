TODO: update this page

Resources:
* Switched from CORGIS database to BTS database (https://www.transtats.bts.gov/DL_SelectFields.aspx?gnoyr_VQ=FGJ&QO_fu146_anzr=) because we need information on flight expected vs actual time, distance, cancelled, and specific flight paths
* Also using a lookup table to convert airport codes into location information

Raw data information (internal documentation):
* ARR_DELAY: Difference in minutes between scheduled and actual arrival time. Early arrivals show negative numbers.
* ARR_DELAY_NEW: Difference in minutes between scheduled and actual arrival time. Early arrivals set to 0.	
* CANCELLED: Cancelled Flight Indicator (1=Yes, 0=No)
* CRS_ELAPSED_TIME: CRS Elapsed Time of Flight, in Minutes (scheduled time amount)
* ACTUAL_ELAPSED_TIME: Elapsed Time of Flight, in Minutes
* AIR_TIME: Flight Time, in Minutes	
* DISTANCE: Distance between airports (miles)