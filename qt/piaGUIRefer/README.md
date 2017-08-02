
/* Patch Image Analysist */

Versions: V2.0
Update: 1. Feature Extraction Area add scroll bar.
	2. Feature Extraction Area add all selection icon.
	3. solved the bad_alloc problem.


Q:how to install?
Linux system:
	1.Input opencv 3.2 library and include file
	$ cd <piaGUI> path
	$ cp opencv/lib64/* /usr/lib64/
	$ cp -r opencv/include/* /usr/include/
	
	2.start the piaGUI execution file.
	$ cd <piaGUI> path
	$ ./piaGUI

	3.Open the piaGUI, select csv file (testData/test.csv) on step 1, select Calibration method on step 2, and Feature Extraction methods on step 3. Then click start.

/* Calibration Method */

1. ECC Image Alignment:
	1-1. alignECC_Translation.
	1-2. alignECC_Euclidean.
	1-3. alignECC_Affine.
	1-4. alignECC_Homography.

2.Frequency Tranform
	2-1. alignDCT.
	2-2. alignDFT.

3.histogram
	3-1. histogramEqualization.

4.thresholding
	4-1. threshOtsu.
	4-2. threshBinary.
	4-3. threshBinaryINV.
	4-4. threshTrunc.
	4-5. threshToZero.
	4-6. threshToZeroINV.

/* Feature Extraction */
1. difference
	1-1. diffGlobalMean
	1-2. diffPixelsCount
	1-3. getPSNR

2. local feature
	2-1. localmeanSSE, sum of squared error
	2-2. localmeanSAE, sum of absolute error

3. block analysis
	3-1. maxNumBlackPixelsInBlock, use patch image.
	3-2. maxNumBlackPixelsInBlock, use src image.
	3-3. maxNumDiffPixelsInBlock, compare patch and ref image.

