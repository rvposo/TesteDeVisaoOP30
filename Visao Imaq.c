#include <windows.h> 
#include "nivision.h"
#include <userint.h>

#include "Vision Scripts OP30 Cluster GEM.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////  FUNÇÕES DE IMAQ  ////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
// Function Name: IVA_CLRExtractBlue
//
// Description  : Extracts the blue color plane from a color image.
//
// Parameters   : image  -  Input image
//
// Return Value : success
//
////////////////////////////////////////////////////////////////////////////////
int IVA_CLRExtractBlue(Image* image)
{
    int success = 1;
    Image* plane;


    //-------------------------------------------------------------------//
    //                         Extract Color Plane                       //
    //-------------------------------------------------------------------//

    // Creates an 8 bit image that contains the extracted plane.
    VisionErrChk(plane = imaqCreateImage(IMAQ_IMAGE_U8, 2));

    // Extracts the blue color plane
    VisionErrChk(imaqExtractColorPlanes(image, IMAQ_RGB, NULL, NULL, plane));

    // Copies the color plane in the main image.
    VisionErrChk(imaqDuplicate(image, plane));

Error:
    imaqDispose(plane);

    return success;
}

////////////////////////////////////////////////////////////////////////////////
//
// Function Name: IVA_SimpleEdge
//
// Description  : Finds prominent edges along an array of pixel coordinates.
//
// Parameters   : image           -  Input image
//                roi             -  Region of Interest
//                Type            -  Determines how the function evaluates the
//                                   threshold and hysteresis values.
//                thresholdLevel  -  The pixel value at which an edge occurs.
//                pHysteresys     -  A value that helps determine edges in noisy
//                                   images.
//                pProcess        -  Determines which edges the function looks for.
//                psubpixel       -  Finds edges with subpixel accuracy by
//                                   interpolating between points to find the crossing
//                                   of the given threshold.
//                ivaData         -  Internal Data structure
//
// Return Value : success
//
////////////////////////////////////////////////////////////////////////////////
/*int IVA_SimpleEdge(Image* image, ROI* roi, int pType,
                                   int pThreshold, int pHysteresys, int pProcess,
                                   int pSubpixel, IVA_Data* ivaData)
{
    int success = 1;
    ROIProfile* roiProfile = NULL;
    SimpleEdgeOptions simpleEdgeOptions;
    PointFloat* edges = NULL;
    Point edges_pt[NUM_MAX_EDGES];
    int i;


    //-------------------------------------------------------------------//
    //                    Edge Detector - Simple Edge                    //
    //-------------------------------------------------------------------//

    // Calculates the profile of the pixels along the edge of each contour in the region of interest.
    VisionErrChk(roiProfile = imaqROIProfile(image, roi));

    simpleEdgeOptions.type = pType;
    simpleEdgeOptions.threshold = pThreshold;
    simpleEdgeOptions.hysteresis = pHysteresys;
    simpleEdgeOptions.process = pProcess;
    simpleEdgeOptions.subpixel = pSubpixel;

    // Finds prominent edges along the array of pixel coordinates.
    VisionErrChk(edges = imaqSimpleEdge(image, roiProfile->pixels, roiProfile->report.dataCount, &simpleEdgeOptions, &quant_edges));
	if(quant_edges>NUM_MAX_EDGES)
		quant_edges=NUM_MAX_EDGES;
    // Log the results in the points array for future caliper operations.
    for (i = 0 ; i < quant_edges  ; i++)
    {
        VisionErrChk(IVA_AddPoint(ivaData, edges[i]));
        pontos_edge[i]=edges[i];
		edges_pt[i]=imaqMakePoint(edges[i].x,edges[i].y);
    }
	if(mostrar_vision && quant_edges> 0)
	{
		imaqOverlayPoints (image, edges_pt, quant_edges, &IMAQ_RGB_YELLOW, 1, IMAQ_POINT_AS_CROSS, NULL,NULL );
	}
		

Error:
    // Disposes temporary structures.
    imaqDispose(roiProfile);
    imaqDispose(edges);

    return success;
}
*/
////////////////////////////////////////////////////////////////////////////////
//
// Function Name: IVA_SimpleEdge
//
// Description  : Finds prominent edges along an array of pixel coordinates.
//
// Parameters   : image           -  Input image
//                roi             -  Region of Interest
//                Type            -  Determines how the function evaluates the
//                                   threshold and hysteresis values.
//                thresholdLevel  -  The pixel value at which an edge occurs.
//                pHysteresys     -  A value that helps determine edges in noisy
//                                   images.
//                pProcess        -  Determines which edges the function looks for.
//                psubpixel       -  Finds edges with subpixel accuracy by
//                                   interpolating between points to find the crossing
//                                   of the given threshold.
//                ivaData         -  Internal Data structure
//                stepIndex       -  Step index (index at which to store the results
//                                   in the resuts array)
//
// Return Value : success
//
////////////////////////////////////////////////////////////////////////////////
int IVA_SimpleEdge(Image* image,
                                   ROI* roi,
                                   int pType,
                                   int pThreshold,
                                   int pHysteresys,
                                   int pProcess,
                                   int pSubpixel,
                                   IVA_Data* ivaData,
                                   int stepIndex)
{
    int success = 1;
    Image* tmpImage;
    ImageType imageType;
    ROIProfile* roiProfile = NULL;
    SimpleEdgeOptions simpleEdgeOptions;
    PointFloat* edges = NULL;
    int numEdges;
    int i;
    int numObjectResults;
    IVA_Result* edgeResults;
    unsigned int visionInfo;
    TransformReport* realWorldPosition = NULL;

    //-------------------------------------------------------------------//
    //                    Edge Detector - Simple Edge                    //
    //-------------------------------------------------------------------//

    // Creates a temporary image that contains the luminance plane if the input image is a color image.
    VisionErrChk(tmpImage = imaqCreateImage(IMAQ_IMAGE_U8, 7));
    VisionErrChk(imaqGetImageType (image, &imageType));
    if (imageType == IMAQ_IMAGE_RGB || imageType == IMAQ_IMAGE_HSL)
        VisionErrChk(imaqExtractColorPlanes(image, IMAQ_HSL, NULL, NULL, tmpImage))
    else
        VisionErrChk(imaqDuplicate(tmpImage, image));

    // Calculates the profile of the pixels along the edge of each contour in the region of interest.
    VisionErrChk(roiProfile = imaqROIProfile(tmpImage, roi));

    simpleEdgeOptions.type = pType;
    simpleEdgeOptions.threshold = pThreshold;
    simpleEdgeOptions.hysteresis = pHysteresys;
    simpleEdgeOptions.process = pProcess;
    simpleEdgeOptions.subpixel = pSubpixel;

    // Finds prominent edges along the array of pixel coordinates.
    VisionErrChk(edges = imaqSimpleEdge(tmpImage, roiProfile->pixels, roiProfile->report.dataCount, &simpleEdgeOptions, &numEdges));

    // ////////////////////////////////////////
    // Store the results in the data structure.
    // ////////////////////////////////////////
    
    // First, delete all the results of this step (from a previous iteration)
    IVA_DisposeStepResults(ivaData, stepIndex);

    // Check if the image is calibrated.
    VisionErrChk(imaqGetVisionInfoTypes(image, &visionInfo));

    // If the image is calibrated, we also need to log the calibrated position (x and y) -> 4 results instead of 2
    numObjectResults = (visionInfo & IMAQ_VISIONINFO_CALIBRATION ? 4 : 2);
        
    ivaData->stepResults[stepIndex].numResults = numEdges * numObjectResults + 1;
    ivaData->stepResults[stepIndex].results = malloc (sizeof(IVA_Result) * ivaData->stepResults[stepIndex].numResults);
    edgeResults = ivaData->stepResults[stepIndex].results;

    if (visionInfo & IMAQ_VISIONINFO_CALIBRATION)
        realWorldPosition = imaqTransformPixelToRealWorld(image, edges, numEdges);

    if (edgeResults)
    {
        #if defined (IVA_STORE_RESULT_NAMES)
            sprintf(edgeResults->resultName, "# of Edges");
        #endif
        edgeResults->type = IVA_NUMERIC;
        edgeResults->resultVal.numVal = numEdges;
        edgeResults++;
        
        for (i = 0 ; i < numEdges ; i++)
        {
            #if defined (IVA_STORE_RESULT_NAMES)
                sprintf(edgeResults->resultName, "Edge %d.X Position (Pix.)", i + 1);
            #endif
            edgeResults->type = IVA_NUMERIC;
            edgeResults->resultVal.numVal = edges[i].x;
            edgeResults++;
            
            #if defined (IVA_STORE_RESULT_NAMES)
                sprintf(edgeResults->resultName, "Edge %d.Y Position (Pix.)", i + 1);
            #endif
            edgeResults->type = IVA_NUMERIC;
            edgeResults->resultVal.numVal = edges[i].y;
            edgeResults++;
            
            if (visionInfo & IMAQ_VISIONINFO_CALIBRATION)
            {
                #if defined (IVA_STORE_RESULT_NAMES)
                    sprintf(edgeResults->resultName, "Edge %d.X Position (World)", i + 1);
                #endif
                edgeResults->type = IVA_NUMERIC;
                edgeResults->resultVal.numVal = realWorldPosition->points[i].x;
                edgeResults++;

                #if defined (IVA_STORE_RESULT_NAMES)
                    sprintf(edgeResults->resultName, "Edge %d.Y Position (World)", i + 1);
                #endif
                edgeResults->type = IVA_NUMERIC;
                edgeResults->resultVal.numVal = realWorldPosition->points[i].y;
                edgeResults++;
            }
        }
		quant_edges=numEdges;
    }

Error:
    // Disposes temporary structures.
    imaqDispose(tmpImage);
    imaqDispose(roiProfile);
    imaqDispose(edges);
    imaqDispose(realWorldPosition);

    return success;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Function Name: IVA_ColorPM
//
// Description  : Searches for areas in an image that match a given color pattern
//                image.
//
// Parameters   : image             -  Input image
//                templatePath      -  Template image path
//                mode              -  Specifies the features to use when looking
//                                     for the color pattern in the image.
//                subpixel          -  Subpixel accuracy
//                angleRangeMin     -  Range that specifies how much the pattern is
//                                     expected to be rotated in the image.
//                angleRangeMax     -  Range that specifies how much the pattern is
//                                     expected to be rotated in the image.
//                colorScoreWeight  -  Percent contribution of the color score to
//                                     the final color pattern matching score.
//                colorSensitivity  -  Sensitivity of the color information in the image.
//                searchStrategy    -  Specifies how the color features of the
//                                     image are used during the search phase.
//                matchesRequested  -  Number of valid matches expected.
//                score             -  Minimum score a match can have for the
//                                     function to consider the match valid.
//                searchRectTop     -  Search rectangle - top
//                searchRectLeft    -  Search rectangle - left
//                searchRectHeight  -  Search rectangle - height
//                searchRectWidth   -  Search rectangle - width
//                ivaData           -  Internal Data structure
//
// Return Value : success
//
////////////////////////////////////////////////////////////////////////////////
int IVA_ColorPM(Image* image, char* templatePath,
                       int mode,
                       int subpixel, float angleRangeMin[], float angleRangeMax[], double colorScoreWeight,
                       int colorSensitivity,
                       int searchStrategy,
                       int matchesRequested, float score,
                       int searchRectTop, int searchRectLeft, int searchRectHeight, int searchRectWidth,
                       IVA_Data2* ivaData)
{
    int success = 1;
    Image* imageTemplate;
    MatchColorPatternOptions cpmOptions;
    int i;
    RotationAngleRange angleRange[2];
    PatternMatch* cpmResults = NULL;
    int numMatchesFound;


    //-------------------------------------------------------------------//
    //                       Color Pattern Matching                      //
    //-------------------------------------------------------------------//

    // Creates and read the image template.
    VisionErrChk(imageTemplate = imaqCreateImage(IMAQ_IMAGE_RGB, 7));
    VisionErrChk(imaqReadVisionFile(imageTemplate, templatePath, NULL, NULL));

    // Fill in the Color Pattern Matching options.
    cpmOptions.matchMode = mode;
    cpmOptions.featureMode = IMAQ_COLOR_AND_SHAPE_FEATURES;
    cpmOptions.minContrast = 0;
    cpmOptions.subpixelAccuracy = subpixel;

    for (i = 0 ; i < 2 ; i++)
    {
        angleRange[i].lower = angleRangeMin[i];
        angleRange[i].upper = angleRangeMax[i];
    }

    cpmOptions.angleRanges = angleRange;
    cpmOptions.numRanges = 2;
    cpmOptions.colorWeight = colorScoreWeight;
    cpmOptions.sensitivity = colorSensitivity;
    cpmOptions.strategy = searchStrategy;
    cpmOptions.numMatchesRequested = matchesRequested;
    cpmOptions.minMatchScore = score;

    cpmResults = NULL;
    numMatchesFound = 0;

    // Searches for areas in the image that match a given color.
    VisionErrChk(cpmResults = imaqMatchColorPattern(image, imageTemplate, &cpmOptions, imaqMakeRect(searchRectTop, searchRectLeft, searchRectHeight, searchRectWidth), &numMatchesFound));
	
	template_encontrado = numMatchesFound;
	if( template_encontrado)
     	imaqOverlayPatternMatch( image, cpmResults, &IMAQ_RGB_BLUE, NULL );

    // Add the points to the results array for future caliper operations.
    for (i = 0 ; i < numMatchesFound ; i++)
        VisionErrChk(IVA_AddPoint(ivaData, cpmResults[i].position));

Error:
    // Disposes temporary image and structures.
    imaqDispose(imageTemplate);
    imaqDispose(cpmResults);

    return success;
}

// /////////////////////////////////////////////////////////////////////////////
//
// Function Name: IVA_NthOrder
//
// Description  : Filters an image using a non-linear filter. For each pixel,
//                the algorithm takes the neighborhood specified by the given
//                filter sizes and replaces the pixel with the nth smallest value
//                in the neighborhood.
//
// Parameters   : image       - Input image
//                filterSize  -  Filter Size
//
// Return Value : success
//
// /////////////////////////////////////////////////////////////////////////////
int IVA_NthOrder(Image* image, int filterSize)
{
    int success = 1;
    int borderSize;


    // Returns the border size of the given image.
    VisionErrChk(imaqGetBorderSize (image, &borderSize));

    // Increase the border size, if necessary.
    if (filterSize > borderSize)
        VisionErrChk(imaqSetBorderSize (image, filterSize));

    // Filters an image using a non-linear filter. For each pixel, the algorithm
    // takes the neighborhood specified by the given filter sizes and replaces
    // the pixel with the nth smallest value in the neighborhood.
    VisionErrChk(imaqNthOrderFilter(image, image, filterSize, filterSize, (int)floor(filterSize * filterSize / 2.0), NULL));

Error:
    return success;
}

// /////////////////////////////////////////////////////////////////////////////
//
// Function Name: IVA_NthOrder
//
// Description  : Filters an image using a non-linear filter. For each pixel,
//                the algorithm takes the neighborhood specified by the given
//                filter sizes and replaces the pixel with the nth smallest value
//                in the neighborhood.
//
// Parameters   : image       - Input image
//                filterSizeX  -  Filter Size X
//                filterSizeY  -  Filter Size Y
//
// Return Value : success
//
// /////////////////////////////////////////////////////////////////////////////
/*static int IVA_NthOrder(Image* image, int filterSizeX, int filterSizeY)
{
    int success = 1;
    int borderSize;
    int maxSize;

    // Returns the border size of the given image.
    VisionErrChk(imaqGetBorderSize (image, &borderSize));

    maxSize = filterSizeX > filterSizeY ? filterSizeX : filterSizeY;

    // Increase the border size, if necessary.
    if (maxSize > borderSize)
        VisionErrChk(imaqSetBorderSize (image, maxSize));

    // Filters an image using a non-linear filter. For each pixel, the algorithm
    // takes the neighborhood specified by the given filter sizes and replaces
    // the pixel with the nth smallest value in the neighborhood.
    VisionErrChk(imaqNthOrderFilter(image, image, filterSizeX, filterSizeY, (int)floor(filterSizeX * filterSizeY / 2.0), NULL));

Error:
    return success;
}
  */


////////////////////////////////////////////////////////////////////////////////
//
// Function Name: IVA_CLRExtractIntensity
//
// Description  : Extracts the Intensity plane from a color image.
//
// Parameters   : image  -  Input image
//
// Return Value : success
//
////////////////////////////////////////////////////////////////////////////////
int IVA_CLRExtractIntensity(Image* image)
{
    int success = 1;
    Image* plane;


    //-------------------------------------------------------------------//
    //                         Extract Color Plane                       //
    //-------------------------------------------------------------------//

    // Creates an 8 bit image that contains the extracted plane.
    VisionErrChk(plane = imaqCreateImage(IMAQ_IMAGE_U8, 7));

    // Extracts the intensity plane
    VisionErrChk(imaqExtractColorPlanes(image, IMAQ_HSI, NULL, NULL, plane));

    // Copies the color plane in the main image.
    VisionErrChk(imaqDuplicate(image, plane));

Error:
    imaqDispose(plane);

    return success;
}

////////////////////////////////////////////////////////////////////////////////
//
// Function Name: IVA_CLRExtractValue
//
// Description  : Extracts the value plane from a color image.
//
// Parameters   : image  -  Input image
//
// Return Value : success
//
////////////////////////////////////////////////////////////////////////////////
int IVA_CLRExtractValue(Image* image)
{
    int success = 1;
    Image* plane;


    //-------------------------------------------------------------------//
    //                         Extract Color Plane                       //
    //-------------------------------------------------------------------//

    // Creates an 8 bit image that contains the extracted plane.
    VisionErrChk(plane = imaqCreateImage(IMAQ_IMAGE_U8, 2));

    // Extracts the value plane
    VisionErrChk(imaqExtractColorPlanes(image, IMAQ_HSV, NULL, NULL, plane));

    // Copies the color plane in the main image.
    VisionErrChk(imaqDuplicate(image, plane));

Error:
    imaqDispose(plane);

    return success;
}

////////////////////////////////////////////////////////////////////////////////
//
// Function Name: IVA_CLRExtractLuminance
//
// Description  : Extracts the luminance plane from a color image.
//
// Parameters   : image  - Input image
//
// Return Value : success
//
////////////////////////////////////////////////////////////////////////////////
int IVA_CLRExtractLuminance(Image* image)
{
    int success = 1;
    Image* plane;


    //-------------------------------------------------------------------//
    //                         Extract Color Plane                       //
    //-------------------------------------------------------------------//

    // Creates an 8 bit image that contains the extracted plane.
    VisionErrChk(plane = imaqCreateImage(IMAQ_IMAGE_U8, 2));

    // Extracts the luminance plane
    VisionErrChk(imaqExtractColorPlanes(image, IMAQ_HSL, NULL, NULL, plane));

    // Copies the color plane in the main image.
    VisionErrChk(imaqDuplicate(image, plane));

Error:
    imaqDispose(plane);

    return success;
}

////////////////////////////////////////////////////////////////////////////////
//
// Function Name: IVA_GetPoint
//
// Description  : Retrieves a point from the PointsResults array
//
// Parameters   : ivaData     -  Internal data structure
//                stepIndex   -  Index of the step that produced the point
//                pointIndex  -  Index of the x value.
//
// Return Value : point
//
////////////////////////////////////////////////////////////////////////////////
PointFloat IVA_GetPoint(IVA_Data* ivaData, int stepIndex, int pointIndex)
{
    PointFloat point;
    
    point = imaqMakePointFloat (0.0, 0.0);
    
    if ((pointIndex + 1) < ivaData->stepResults[stepIndex].numResults)
        point = imaqMakePointFloat(ivaData->stepResults[stepIndex].results[pointIndex].resultVal.numVal,
                                   ivaData->stepResults[stepIndex].results[pointIndex + 1].resultVal.numVal);
    
    return point;
}
////////////////////////////////////////////////////////////////////////////////
//
// Function Name: IVA_CLRExtractGreen
//
// Description  : Extracts the green color plane from a color image.
//
// Parameters   : image  - Input image
//
// Return Value : success
//
////////////////////////////////////////////////////////////////////////////////
int IVA_CLRExtractGreen(Image* image)
{
    int success = 1;
    Image* plane;


    //-------------------------------------------------------------------//
    //                         Extract Color Plane                       //
    //-------------------------------------------------------------------//

    // Creates an 8 bit image that contains the extracted plane.
    VisionErrChk(plane = imaqCreateImage(IMAQ_IMAGE_U8, 2));

    // Extracts the green color plane
    VisionErrChk(imaqExtractColorPlanes(image, IMAQ_RGB, NULL, plane, NULL));

    // Copies the color plane in the main image.
    VisionErrChk(imaqDuplicate(image, plane));

Error:
    imaqDispose(plane);

    return success;
}


////////////////////////////////////////////////////////////////////////////////
//
// Function Name: IVA_CLRExtractRed
//
// Description  : Extracts the red color plane from a color image.
//
// Parameters   : image  -  Input image
//
// Return Value : success
//
////////////////////////////////////////////////////////////////////////////////
int IVA_CLRExtractRed(Image* image)
{
    int success = 1;
    Image* plane;


    //-------------------------------------------------------------------//
    //                         Extract Color Plane                       //
    //-------------------------------------------------------------------//

    // Creates an 8 bit image that contains the extracted plane.
    VisionErrChk(plane = imaqCreateImage(IMAQ_IMAGE_U8, 2));

    // Extracts the red color plane
    VisionErrChk(imaqExtractColorPlanes(image, IMAQ_RGB, plane, NULL, NULL));

    // Copies the color plane in the main image.
    VisionErrChk(imaqDuplicate(image, plane));

Error:
    imaqDispose(plane);

    return success;
}


////////////////////////////////////////////////////////////////////////////////
//
// Function Name: IVA_LineProfile_Gray
//
// Description  : Calculates the profile of the pixels along the boundary 
//                of an ROI descriptor.
//
// Parameters   : image  -  Input image
//                roi    -  Region of interest
//             
// Return Value : success
//
////////////////////////////////////////////////////////////////////////////////
int IVA_LineProfile_Gray(Image* image, ROI* roi)
{
    int success = 1;        
    ROIProfile* roiProfile = NULL;
	variacao_padrao = -999;

    //-------------------------------------------------------------------//
    //                       Line Profile Grayscale                      //
    //-------------------------------------------------------------------//

    // Calculates the profile of the pixels along the boundary of an ROI descriptor.
    roiProfile = imaqROIProfile (image, roi);  
	variacao_padrao = roiProfile->report.stdDev;
	
Error:
    imaqDispose(roiProfile);
                                                     
    return success;
}


////////////////////////////////////////////////////////////////////////////////
//
// Function Name: IVA_ReadText
//
// Description  : Reads text in the image.
//
// Parameters   : image     -  Input image
//                fileName  -  Character Set File Path
//                roi       -  Region of interest specifying the location of
//                             the text in the image.
//
// Return Value : success
//
////////////////////////////////////////////////////////////////////////////////
int IVA_ReadText(Image* image, char* fileName, ROI* roi)
{
    int success = 1;
    CharSet* characterSet = NULL;
    ReadTextOptions readOptions;
    OCRProcessingOptions processingOptions;
    OCRSpacingOptions spacingOptions;
    ReadTextReport2* ocrReport = NULL;


    //-------------------------------------------------------------------//
    //                             Read Text                             //
    //-------------------------------------------------------------------//

    // Creates a new character set
    VisionErrChk(characterSet = imaqCreateCharSet());

    // Reads the character set and session properties from the character set file.
    VisionErrChk(imaqReadOCRFile(fileName, characterSet, NULL, &readOptions, &processingOptions, &spacingOptions));

    // Reads text in the image.
    VisionErrChk(ocrReport = imaqReadText2(image, characterSet, roi, &readOptions, &processingOptions, &spacingOptions));
	strcpy(ocr_lido, "");
	if(ocrReport->numCharacterReports>0)
	{
		strcpy(ocr_lido, ocrReport->readString);
	}
Error:
    imaqDispose(characterSet);
    imaqDispose(ocrReport);

    return success;
}

////////////////////////////////////////////////////////////////////////////////
//
// Function Name: IVA_EdgeTool
//
// Description  : Finds edges along a path in an image.
//
// Parameters   : image               -  Input image
//                roi                 -  Region of Interest
//                pProcess            -  Determines which edges the function looks for.
//                pThreshold          -  Threshold value for the contrast of the edge.
//                pFilterWidth        -  Number of pixels that the function averages
//                                       to find the contrast at either side of the edge.
//                pSteepness          -  Span, in pixels, of the slope of the edge
//                                       projected along the path specified by the input points.
//                pSubpixelType       -  Method for interpolating.
//                pSubpixelDivisions  -  Number of samples the function obtains from a pixel.
//                ivaData             -  Internal Data structure
//
// Return Value : None (inline function)
//
////////////////////////////////////////////////////////////////////////////////
int IVA_EdgeTool(Image* image, ROI* roi, int pProcess,
                        unsigned int pThreshold, unsigned int pFilterWidth, unsigned int pSteepness,
                        unsigned int pSubpixelType,
                        unsigned int pSubpixelDivisions, IVA_Data2* ivaData)
{
    int success = 1;
    ROIProfile* roiProfile = NULL;
    EdgeOptions edgeOptions;
    EdgeReport* edgeReport = NULL;
    int i;


    //-------------------------------------------------------------------//
    //                     Edge Detector - Edge Tool                     //
    //-------------------------------------------------------------------//

    // Calculates the profile of the pixels along the edge of each contour in a region of interest.
    VisionErrChk(roiProfile = imaqROIProfile(image, roi));

    edgeOptions.threshold = pThreshold;
    edgeOptions.width = pFilterWidth;
    edgeOptions.steepness = pSteepness;
    edgeOptions.subpixelType = pSubpixelType;
    edgeOptions.subpixelDivisions = pSubpixelDivisions;

    // Finds edges along a path in the image.
    VisionErrChk(edgeReport = imaqEdgeTool(image, roiProfile->pixels, roiProfile->report.dataCount, &edgeOptions, &quant_edges));
    // Log the results in the points array for future caliper operations.
    switch (pProcess)
    {
        case 0:
        {
            VisionErrChk(IVA_AddPoint(ivaData, edgeReport[0].coordinate));
            break;
        }

        case 1:
        {
            VisionErrChk(IVA_AddPoint(ivaData, edgeReport[0].coordinate));
            VisionErrChk(IVA_AddPoint(ivaData, edgeReport[quant_edges - 1].coordinate));
            break;
        }

        case 2:
        {
        	if(quant_edges>10)
        		quant_edges=10;
            for (i = 0 ; i < quant_edges  ; i++)
            {
                VisionErrChk(IVA_AddPoint(ivaData, edgeReport[i].coordinate));
                pontos_edge[i]=edgeReport[i].coordinate;
            }
            break;
        }
    }

Error:
    // Disposes temporary structures.
    imaqDispose(roiProfile);
    imaqDispose(edgeReport);

    return success;
}


////////////////////////////////////////////////////////////////////////////////
// Function Name: IVA_MatchPattern
// Description  : Searches for areas in an image that match a given pattern
//                image.
// Parameters   : image             -  Input image
//                ivaData           -  Internal Data structure
//                templatePath      -  Template image path
//                matchMode         -  Specifies the method to use when looking
//                                     for the pattern in the image.
//                subpixel          -  Subpixel accuracy
//                angleRangeMin     -  Range that specifies how much the pattern is
//                                     expected to be rotated in the image.
//                angleRangeMax     -  Range that specifies how much the pattern is
//                                     expected to be rotated in the image.
//                matchesRequested  -  Number of valid matches expected.
//                score             -  Minimum score a match can have for the
//                                     function to consider the match valid.
//                searchRectTop     -  Search rectangle - top
//                searchRectLeft    -  Search rectangle - left
//                searchRectHeight  -  Search rectangle - height
//                searchRectWidth   -  Search rectangle - width
// Return Value : success
////////////////////////////////////////////////////////////////////////////////
int IVA_MatchPattern(Image* image, IVA_Data2* ivaData, char* templatePath,
                            int matchMode, int subpixel,
                            float angleRangeMin[], float angleRangeMax[], int matchesRequested, float score,
                            int searchRectTop, int searchRectLeft, int searchRectHeight, int searchRectWidth)
{
    int success = 1;
    Image* imageTemplate;
    MatchPatternOptions matchPatternOptions;
    int i;
    RotationAngleRange angleRange[2];
    PatternMatch* pmResults = NULL;
    int numMatchesFound;

	template_encontrado = 0;

    //-------------------------------------------------------------------//
    //                          Pattern Matching                         //
    //-------------------------------------------------------------------//

    // Creates and read the image template.
    VisionErrChk(imageTemplate = imaqCreateImage(IMAQ_IMAGE_U8, 2));
    VisionErrChk(imaqReadVisionFile(imageTemplate, templatePath, NULL, NULL));

    // Fill in the Pattern Matching options.
    matchPatternOptions.mode = matchMode;
    matchPatternOptions.minContrast = 0;
    matchPatternOptions.subpixelAccuracy = subpixel;

    for (i = 0 ; i < 2 ; i++)
    {
        angleRange[i].lower = angleRangeMin[i];
        angleRange[i].upper = angleRangeMax[i];
    }

    matchPatternOptions.angleRanges = angleRange;
    matchPatternOptions.numRanges = 2;
    matchPatternOptions.numMatchesRequested = matchesRequested;
    matchPatternOptions.matchFactor = 0;
    matchPatternOptions.minMatchScore = score;

    pmResults = NULL;
    numMatchesFound = 0;

    // Searches for areas in the image that match a given pattern.
    VisionErrChk(pmResults = imaqMatchPattern2(image, imageTemplate, &matchPatternOptions, NULL, imaqMakeRect(searchRectTop, searchRectLeft, searchRectHeight, searchRectWidth), &numMatchesFound));


    // Add the points to the results array for future caliper operations.
    for (i = 0 ; i < numMatchesFound ; i++)
        VisionErrChk(IVA_AddPoint(ivaData, pmResults[i].position));
	template_encontrado = numMatchesFound;
	if( template_encontrado)
     	imaqOverlayPatternMatch( image, pmResults, &IMAQ_RGB_BLUE, NULL );

Error:
    // Disposes temporary image and structures.
    imaqDispose(imageTemplate);
    imaqDispose(pmResults);

    return success;
}

////////////////////////////////////////////////////////////////////////////////
//
// Function Name: IVA_MatchPattern_2
//
// Description  : Searches for areas in an image that match a given pattern
//                image.
//
// Parameters   : image             -  Input image
//                ivaData           -  Internal Data structure
//                templatePath      -  Template image path
//                angleRangeMin     -  Range that specifies how much the pattern is
//                                     expected to be rotated in the image.
//                angleRangeMax     -  Range that specifies how much the pattern is
//                                     expected to be rotated in the image.
//                advOptionsItems   -  Advanced Options (Items to set)
//                advOptionsValues  -  Advanced Options (Values)
//                numAdvOption      -  Number of elements in the advOptions array
//                matchesRequested  -  Number of valid matches expected.
//                score             -  Minimum score a match can have for the
//                                     function to consider the match valid.
//                roi               -  search area
//                stepIndex         -  Step index (index at which to store the results in the resuts array)
//
// Return Value : success
//
////////////////////////////////////////////////////////////////////////////////
int IVA_MatchPattern_2(Image* image,
                                     IVA_Data* ivaData,
                                     char* templatePath,
									 int algorithm,
                                     float angleRangeMin[],
                                     float angleRangeMax[],
									 int advOptionsItems[],
									 float advOptionsValues[],
									 unsigned int numAdvOptions,
                                     int matchesRequested,
                                     float score,
                                     ROI* roi,
                                     int stepIndex)
{
    int success = 1;
    Image* imageTemplate = NULL;
    int i;
    RotationAngleRange angleRange[2];
	PMMatchAdvancedSetupDataOption* advancedOptions;
    PatternMatchReport* pmResults = NULL;
    PatternMatch* pmResults_2 = NULL;
    int numMatchesFound = 0;
    int numObjectResults;
    IVA_Result* patternMacthingResults;
    unsigned int visionInfo;


    //-------------------------------------------------------------------//
    //                          Pattern Matching                         //
    //-------------------------------------------------------------------//

    // Creates and read the image template.
    VisionErrChk(imageTemplate = imaqCreateImage(IMAQ_IMAGE_U8, 7));
    VisionErrChk(imaqReadVisionFile(imageTemplate, templatePath, NULL, NULL));

    // Set the angle range.
    for (i = 0 ; i < 2 ; i++)
    {
        angleRange[i].lower = angleRangeMin[i];
        angleRange[i].upper = angleRangeMax[i];
    }

	advancedOptions = (PMMatchAdvancedSetupDataOption*)malloc(numAdvOptions * sizeof(PMMatchAdvancedSetupDataOption));
    for (i = 0 ; i < numAdvOptions ; i++)
    {
        advancedOptions[i].matchSetupOption = advOptionsItems[i];
        advancedOptions[i].value = advOptionsValues[i];
    }

    // Searches for areas in the image that match a given pattern.
    VisionErrChk(pmResults = imaqMatchPattern4(image, imageTemplate, algorithm, matchesRequested, score, angleRange, 2, roi, advancedOptions, numAdvOptions, &numMatchesFound));

    // ////////////////////////////////////////
    // Store the results in the data structure.
    // ////////////////////////////////////////
    
    // First, delete all the results of this step (from a previous iteration)
    IVA_DisposeStepResults(ivaData, stepIndex);

    // Check if the image is calibrated.
    VisionErrChk(imaqGetVisionInfoTypes(image, &visionInfo));

    // If the image is calibrated, we also need to log the calibrated position (x and y) -> 6 results instead of 4
    numObjectResults = (visionInfo & IMAQ_VISIONINFO_CALIBRATION ? 7 : 4);
        
    ivaData->stepResults[stepIndex].numResults = numMatchesFound * numObjectResults + 1;
    ivaData->stepResults[stepIndex].results = malloc (sizeof(IVA_Result) * ivaData->stepResults[stepIndex].numResults);
    patternMacthingResults = ivaData->stepResults[stepIndex].results;
	template_encontrado = numMatchesFound;
	/*pmResults_2->corner[0].x = pmResults->corner[0].x;
	pmResults_2->corner[0].y = pmResults->corner[0].y;
	pmResults_2->corner[1].x = pmResults->corner[1].x;
	pmResults_2->corner[1].y = pmResults->corner[1].y;
	pmResults_2->corner[2].x = pmResults->corner[2].x;
	pmResults_2->corner[2].y = pmResults->corner[2].y;
	pmResults_2->corner[3].x = pmResults->corner[3].x;
	pmResults_2->corner[3].y = pmResults->corner[3].y;
	pmResults_2->position = pmResults->position;
	//pmResults_2->position[0].y = pmResults->position[0].y;
	pmResults_2->rotation = pmResults->rotation;
	pmResults_2->scale = pmResults->scale;
	pmResults_2->score = pmResults->score; */
	if( template_encontrado)
     	imaqOverlayPatternMatch_2( image, pmResults, &IMAQ_RGB_BLUE, NULL );
//int imaqOverlayFindPatternResults( Image* image, const ROI* roi, const PatternMatch* matches, int numMatches, const FindPatternOptions* options ) {
    
    if (patternMacthingResults)
    {
        #if defined (IVA_STORE_RESULT_NAMES)
            sprintf(patternMacthingResults->resultName, "# of objects");
        #endif
        patternMacthingResults->type = IVA_NUMERIC;
        patternMacthingResults->resultVal.numVal = numMatchesFound;
        patternMacthingResults++;
        
        for (i = 0 ; i < numMatchesFound ; i++)
        {
            #if defined (IVA_STORE_RESULT_NAMES)
                sprintf(patternMacthingResults->resultName, "Match %d.X Position (Pix.)", i + 1);
            #endif
            patternMacthingResults->type = IVA_NUMERIC;
            patternMacthingResults->resultVal.numVal = pmResults[i].position.x;
            patternMacthingResults++;
            
            #if defined (IVA_STORE_RESULT_NAMES)
                sprintf(patternMacthingResults->resultName, "Match %d.Y Position (Pix.)", i + 1);
            #endif
            patternMacthingResults->type = IVA_NUMERIC;
            patternMacthingResults->resultVal.numVal = pmResults[i].position.y;
            patternMacthingResults++;
            
            if (visionInfo & IMAQ_VISIONINFO_CALIBRATION)
            {
                #if defined (IVA_STORE_RESULT_NAMES)
                    sprintf(patternMacthingResults->resultName, "Match %d.X Position (World)", i + 1);
                #endif
                patternMacthingResults->type = IVA_NUMERIC;
                patternMacthingResults->resultVal.numVal = pmResults[i].calibratedPosition.x;
                patternMacthingResults++;

                #if defined (IVA_STORE_RESULT_NAMES)
                    sprintf(patternMacthingResults->resultName, "Match %d.Y Position (World)", i + 1);
                #endif
                patternMacthingResults->type = IVA_NUMERIC;
                patternMacthingResults->resultVal.numVal = pmResults[i].calibratedPosition.y;
                patternMacthingResults++;
            }

            #if defined (IVA_STORE_RESULT_NAMES)
                sprintf(patternMacthingResults->resultName, "Match %d.Angle (degrees)", i + 1);
            #endif
            patternMacthingResults->type = IVA_NUMERIC;
            patternMacthingResults->resultVal.numVal = pmResults[i].rotation;
            patternMacthingResults++;

			if (visionInfo & IMAQ_VISIONINFO_CALIBRATION)
            {
				#if defined (IVA_STORE_RESULT_NAMES)
					sprintf(patternMacthingResults->resultName, "Match %d.Calibrated Angle (degrees)", i + 1);
				#endif
				patternMacthingResults->type = IVA_NUMERIC;
				patternMacthingResults->resultVal.numVal = pmResults[i].calibratedRotation;
				patternMacthingResults++;
			}

			#if defined (IVA_STORE_RESULT_NAMES)
                sprintf(patternMacthingResults->resultName, "Match %d.Score", i + 1);
            #endif
            patternMacthingResults->type = IVA_NUMERIC;
            patternMacthingResults->resultVal.numVal = pmResults[i].score;
            patternMacthingResults++;
        }
    }

Error:
    // Disposes temporary image and structures.
	free(advancedOptions);
    imaqDispose(imageTemplate);
    imaqDispose(pmResults);

    return success;
}
////////////////////////////////////////////////////////////////////////////////
// Function Name: IVA_FindCircularEdge
//
// Description  : Locates a circular edge in an annular search area.
//
// Parameters   : image              -  Input image.
//                centerX            -  X coordinate location of the center of the annulus.
//                centerY            -  Y coordinate location of the center of the annulus.
//                pInnerRadius       -  Internal radius of the annulus.
//                pOuterRadius       -  External radius of the annulus.
//                pStartAngle        -  Start angle, in degrees, of the annulus.
//                pEndAngle          -  End angle, in degrees, of the annulus.
//                pThreshold         -  Threshold for the contrast of an edge.
//                pFilterWidth       -  Number of pixels that the function averages
//                                      to find the contrast at either side of the edge.
//                pSteepness         -  Span, in pixels, of the slope of the edge
//                                      projected along the path specified by the input points.
//                pSubsamplingRatio  -  Number of pixels that separates two
//                                      consecutive search lines.
//                ivaData            -  Internal Data structure
// Return Value : success
////////////////////////////////////////////////////////////////////////////////
int IVA_FindCircularEdge(Image* image, int centerX, int centerY, int pInnerRadius, int pOuterRadius,
                                double pStartAngle, double pEndAngle, int pThreshold, int pFilterWidth,
                                int pSteepness, double pSubsamplingRatio,
                                int pDirection, IVA_Data2* ivaData)
{
    int success = 1;
    Annulus searchArea;
    FindEdgeOptions edgeOptions;
    CircularEdgeReport* circularEdgeReport = NULL;
    int i;
    //-------------------------------------------------------------------//
    //                         Find Circular Edge                        //
    //-------------------------------------------------------------------//
    searchArea.center.x = centerX;
    searchArea.center.y = centerY;
    searchArea.innerRadius = pInnerRadius;
    searchArea.outerRadius = pOuterRadius;
    searchArea.startAngle = pStartAngle;
    searchArea.endAngle = pEndAngle;
    edgeOptions.threshold = pThreshold;
    edgeOptions.width = pFilterWidth;
    edgeOptions.steepness = pSteepness;
    edgeOptions.subsamplingRatio = pSubsamplingRatio;
    edgeOptions.showSearchArea = 1;
    edgeOptions.showSearchLines = 1;
    edgeOptions.showEdgesFound = 1;
    edgeOptions.showResult = 1;
    // Locates a circular edge in the annular search area.
    VisionErrChk(circularEdgeReport = imaqFindCircularEdge(image, searchArea, pDirection, &edgeOptions, &transform_circ));
    // Log the results in the points array for future caliper operations.
    for (i = 0 ; i < circularEdgeReport->numCoordinates ; i++)
        VisionErrChk(IVA_AddPoint(ivaData, circularEdgeReport->coordinates[i]));
Error:
    // Disposes the edge report
    imaqDisposeCircularEdgeReport(circularEdgeReport);
    return success;
}

////////////////////////////////////////////////////////////////////////////////
//
// Function Name: IVA_Quantify
//
// Description  : Calculates statistical parameters on an image.
//
// Parameters   : image  -  Input image
//                roi    -  Region of interest
//
// Return Value : success
//
////////////////////////////////////////////////////////////////////////////////
int IVA_Quantify(Image* image, ROI* roi)
{
    int success = 1;
    Image* imageMask;
    QuantifyReport* quantifyReport = NULL;
	valor_medio_quantity = -1;
	valor_max_quantity = -1;
	valor_min_quantity = -1;
	
    //-------------------------------------------------------------------//
    //                              Quantify                             //
    //-------------------------------------------------------------------//

    // Creates a temporary image mask.
    VisionErrChk(imageMask = imaqCreateImage(IMAQ_IMAGE_U8, 2));

    // Creates the mask based on the region of interest.
    VisionErrChk(imaqROIToMask(imageMask, roi, 255, image, NULL));

    // Calculates statistical parameters on the image.
    VisionErrChk(quantifyReport = imaqQuantify(image, imageMask));
	valor_medio_quantity 	=  quantifyReport->global.mean;
	valor_max_quantity 		=  quantifyReport->global.max;
	valor_min_quantity		=  quantifyReport->global.min;	
			
Error:
    imaqDispose(imageMask);
    imaqDispose(quantifyReport);

    return success;
}


////////////////////////////////////////////////////////////////////////////////
// Function Name: IVA_CentroidROI
// Description  : Computes the centroid of a ROI of an image.
// Parameters   : image    -  Input image
//                roi      -  Region of interest
//                ivaData  -  Internal Data structure
// Return Value : success
////////////////////////////////////////////////////////////////////////////////
int IVA_CentroidROI(Image* image, ROI* roi, IVA_Data2* ivaData)
{
    int success = 1;
    Image* imageMask;
	PointFloat centroid;
    //-------------------------------------------------------------------//
    //                              Centroid                             //
    //-------------------------------------------------------------------//
    // Creates the image mask.
    VisionErrChk(imageMask = imaqCreateImage(IMAQ_IMAGE_U8, 2));
    // Transforms the region of interest into a mask image.
    VisionErrChk(imaqROIToMask(imageMask, roi, 255, image, NULL));
    // Computes the centroid of the ROI in the image.
    VisionErrChk(imaqCentroid(image, &centroid, imageMask));
    // Adds the centroid to the array of points for future caliper operations.
    VisionErrChk(IVA_AddPoint(ivaData, centroid));
	centro_id.x=centroid.x;
	centro_id.y=centroid.y;
Error:
    imaqDispose(imageMask);

    return success;
}

////////////////////////////////////////////////////////////////////////////////
//
// Function Name: IVA_Histogram_GrayMask
//
// Description  : Calculates statistical parameters from the region
//                of interest of an image.
//
// Parameters   : image  -  Input image
//                roi    -  Region of interest
//
// Return Value : success
//
////////////////////////////////////////////////////////////////////////////////
int IVA_Histogram_GrayMask(Image* image, ROI* roi)
{
    int success = 1;
    Image* imageMask;
    HistogramReport* histogramReport = NULL;


    //-------------------------------------------------------------------//
    //                 Histogram Grayscale with Mask                     //
    //-------------------------------------------------------------------//

    // Creates a temporary image mask.
    VisionErrChk(imageMask = imaqCreateImage(IMAQ_IMAGE_U8, 7));

    // Creates the mask based on the region of interest.
    VisionErrChk(imaqROIToMask(imageMask, roi, 255, image, NULL));

    // Calculates statistical parameters on the image.
    VisionErrChk(histogramReport = imaqHistogram (image, 256, 0.0, 0.0, imageMask));
	valor_min_quantity 		= histogramReport->min;
	valor_medio_quantity 	= histogramReport->mean;
	valor_max_quantity 		= histogramReport->max;

Error:
    imaqDispose(imageMask);
    imaqDispose(histogramReport);
                                                     
    return success;
}

////////////////////////////////////////////////////////////////////////////////
//
// Function Name: IVA_InitData
//
// Description  : Initializes data for buffer management and caliper points.
//
// Parameters   : None
//
// Return Value : success
//
////////////////////////////////////////////////////////////////////////////////
IVA_Data2* IVA_InitData2(void)
{
    int success = 1;
    IVA_Data2* ivaData = NULL;
    int i;


    // Allocate the data structure.
    VisionErrChk(ivaData = (IVA_Data2*)malloc(sizeof (IVA_Data2)));

    // Initializes the image pointers to NULL.
    for (i = 0 ; i < MAX_BUFFERS ; i++)
        ivaData->buffers[i] = NULL;

    // Initializes the array of points to INIT_POINTS_ARRAY_ELEMENTS elements.
    ivaData->numPoints = INIT_POINTS_ARRAY_ELEMENTS;

    ivaData->pointsResults = (PointFloat*)malloc(ivaData->numPoints * sizeof(PointFloat));
    for (i = 0 ; i < INIT_POINTS_ARRAY_ELEMENTS ; i++)
    {
        ivaData->pointsResults[i].x = 0;
        ivaData->pointsResults[i].y = 0;
    }

    ivaData->pointIndex = -1;

Error:
    return ivaData;
}

////////////////////////////////////////////////////////////////////////////////
//
// Function Name: IVA_InitData
//
// Description  : Initializes data for buffer management and results.
//
// Parameters   : # of steps
//                # of coordinate systems
//
// Return Value : success
//
////////////////////////////////////////////////////////////////////////////////
IVA_Data* IVA_InitData(int numSteps, int numCoordSys)
{
    int success = 1;
    IVA_Data* ivaData = NULL;
    int i;


    // Allocate the data structure.
    VisionErrChk(ivaData = (IVA_Data*)malloc(sizeof (IVA_Data)));

    // Initializes the image pointers to NULL.
    for (i = 0 ; i < IVA_MAX_BUFFERS ; i++)
        ivaData->buffers[i] = NULL;

    // Initializes the steo results array to numSteps elements.
    ivaData->numSteps = numSteps;

    ivaData->stepResults = (IVA_StepResults*)malloc(ivaData->numSteps * sizeof(IVA_StepResults));
    for (i = 0 ; i < numSteps ; i++)
    {
        #if defined (IVA_STORE_RESULT_NAMES)
            sprintf(ivaData->stepResults[i].stepName, "");
        #endif
        ivaData->stepResults[i].numResults = 0;
        ivaData->stepResults[i].results = NULL;
    }

    // Create the coordinate systems
	ivaData->baseCoordinateSystems = NULL;
	ivaData->MeasurementSystems = NULL;
	if (numCoordSys)
	{
		ivaData->baseCoordinateSystems = (CoordinateSystem*)malloc(sizeof(CoordinateSystem) * numCoordSys);
		ivaData->MeasurementSystems = (CoordinateSystem*)malloc(sizeof(CoordinateSystem) * numCoordSys);
	}

    ivaData->numCoordSys = numCoordSys;

Error:
    return ivaData;
}
////////////////////////////////////////////////////////////////////////////////
//
// Function Name: IVA_DisposeData
//
// Description  : Releases the memory allocated in the IVA_Data structure
//
// Parameters   : ivaData  -  Internal data structure
//
// Return Value : success
//
////////////////////////////////////////////////////////////////////////////////
int IVA_DisposeData2(IVA_Data2* ivaData)
{
    int i;


    // Releases the memory allocated for the image buffers.
    for (i = 0 ; i < MAX_BUFFERS ; i++)
        imaqDispose(ivaData->buffers[i]);

    // Releases the memory allocated for the array of points.
    free(ivaData->pointsResults);
    ivaData->numPoints = 0;
    ivaData->pointIndex = -1;

    free(ivaData);

    return 1;
}

////////////////////////////////////////////////////////////////////////////////
//
// Function Name: IVA_DisposeData2
//
// Description  : Releases the memory allocated in the IVA_Data structure
//
// Parameters   : ivaData  -  Internal data structure
//
// Return Value : success
//
////////////////////////////////////////////////////////////////////////////////
/*int IVA_DisposeData2(IVA_Data2* ivaData)
{
    int i;


    // Releases the memory allocated for the image buffers.
    for (i = 0 ; i < MAX_BUFFERS ; i++)
        imaqDispose(ivaData->buffers[i]);

    // Releases the memory allocated for the array of points.
    free(ivaData->pointsResults);
    ivaData->numPoints = 0;
    ivaData->pointIndex = -1;

    free(ivaData);

    return 1;
}*/


////////////////////////////////////////////////////////////////////////////////
//
// Function Name: IVA_DisposeData
//
// Description  : Releases the memory allocated in the IVA_Data structure
//
// Parameters   : ivaData  -  Internal data structure
//
// Return Value : success
//
////////////////////////////////////////////////////////////////////////////////
int IVA_DisposeData(IVA_Data* ivaData)
{
    int i;


    // Releases the memory allocated for the image buffers.
    for (i = 0 ; i < IVA_MAX_BUFFERS ; i++)
        imaqDispose(ivaData->buffers[i]);

    // Releases the memory allocated for the array of measurements.
    for (i = 0 ; i < ivaData->numSteps ; i++)
        IVA_DisposeStepResults(ivaData, i);

    free(ivaData->stepResults);

    // Dispose of coordinate systems
    if (ivaData->numCoordSys)
    {
        free(ivaData->baseCoordinateSystems);
        free(ivaData->MeasurementSystems);
    }

    free(ivaData);

    return TRUE;
}


/*
////////////////////////////////////////////////////////////////////////////////
//
// Function Name: IVA_DisposeStepResults
//
// Description  : Dispose of the results of a specific step.
//
// Parameters   : ivaData    -  Internal data structure
//                stepIndex  -  step index
//
// Return Value : success
//
////////////////////////////////////////////////////////////////////////////////
int IVA_DisposeStepResults(IVA_Data* ivaData, int stepIndex)
{
    int i;

    
    for (i = 0 ; i < ivaData->stepResults[stepIndex].numResults ; i++)
    {
        if (ivaData->stepResults[stepIndex].results[i].type == IVA_STRING)
            free(ivaData->stepResults[stepIndex].results[i].resultVal.strVal);
    }

    free(ivaData->stepResults[stepIndex].results);

    return TRUE;
}

 */
////////////////////////////////////////////////////////////////////////////////
// Function Name: IVA_PushBuffer
// Description  : Stores an image in a buffer
// Parameters   : ivaData       -  Internal data structure
//                image         -  image
//                bufferNumber  -  Buffer index
// Return Value : success
////////////////////////////////////////////////////////////////////////////////
int IVA_PushBuffer(IVA_Data* ivaData, Image* image, int bufferNumber)
{
    int success = 1;
    ImageType imageType;
    // Release the previous image that was contained in the buffer
    VisionErrChk(imaqDispose(ivaData->buffers[bufferNumber]));
    // Creates an image buffer of the same type of the source image.
    VisionErrChk(imaqGetImageType(image, &imageType));
    VisionErrChk(ivaData->buffers[bufferNumber] = imaqCreateImage(imageType, 7));
    // Copies the image in the buffer.
    VisionErrChk(imaqDuplicate(ivaData->buffers[bufferNumber], image));
Error:
    return success;
}

////////////////////////////////////////////////////////////////////////////////
// Function Name: IVA_PopBuffer
// Description  : Retrieves an image from a buffer
// Parameters   : ivaData       -  Internal data structure
//                bufferNumber  -  Buffer index
// Return Value : success
////////////////////////////////////////////////////////////////////////////////
Image* IVA_GetBuffer(IVA_Data* ivaData, int bufferNumber)
{
    return ivaData->buffers[bufferNumber];
}



////////////////////////////////////////////////////////////////////////////////
// Function Name: IVA_AddPoint
// Description  : Adds a point to the PointsResults array for future Caliper
//                operations
// Parameters   : ivaData       -  Internal data structure
// Return Value : success
////////////////////////////////////////////////////////////////////////////////
int IVA_AddPoint(IVA_Data2* ivaData, PointFloat point)
{
    int success = 1;


    ivaData->pointIndex++;

    // If we need to add a point passed the size of the array, reallocate the array with twice the size
    if (ivaData->pointIndex >= ivaData->numPoints)
    {
        ivaData->numPoints = ivaData->numPoints * 2;
        VisionErrChk(ivaData->pointsResults = (PointFloat*)realloc(ivaData->pointsResults,
                                                                   ivaData->numPoints * sizeof(PointFloat)));
    }

    ivaData->pointsResults[ivaData->pointIndex].x = point.x;
    ivaData->pointsResults[ivaData->pointIndex].y = point.y;

Error:
    return success;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////  FUNÇÕES DE IMAQ  ////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////




////////////////////////////////////////////////////////////////////////////////
//
// Function Name: IVA_ParticleFilter
//
// Description  : Filters particles based on their morphological measurements.
//
// Parameters   : image          -  Input image
//                pParameter     -  Morphological measurement that the function
//                                  uses for filtering.
//                plower         -  Lower bound of the criteria range.
//                pUpper         -  Upper bound of the criteria range.
//                pCalibrated    -  Whether to take a calibrated measurement or not.
//                pExclude       -  TRUE indicates that a match occurs when the
//                                  value is outside the criteria range.
//                criteriaCount  -  number of particle filter criteria.
//                rejectMatches  -  Set this parameter to TRUE to transfer only
//                                  those particles that do not meet all the criteria.
//                                  Set this parameter to FALSE to transfer only those
//                                  particles that meet all the criteria to the destination.
//                connectivity   -  Set this parameter to 1 to use connectivity-8
//                                  to determine whether particles are touching.
//                                  Set this parameter to 0 to use connectivity-4
//                                  to determine whether particles are touching.
//
// Return Value : success
//
////////////////////////////////////////////////////////////////////////////////
int IVA_ParticleFilter(Image* image, int pParameter[], float plower[], float pUpper[],
                              int pCalibrated[], int pExclude[], int criteriaCount,
                              int rejectMatches,
                              int connectivity)
{
    int success = 1;
    ParticleFilterCriteria2* particleCriteria = NULL;
    int i;


    //-------------------------------------------------------------------//
    //                          Particle Filter                          //
    //-------------------------------------------------------------------//

    // Fill in the ParticleFilterCriteria2 structure.
    particleCriteria = (ParticleFilterCriteria2*)malloc(criteriaCount * sizeof(ParticleFilterCriteria2));

    for (i = 0 ; i < criteriaCount ; i++)
    {
        particleCriteria[i].parameter = pParameter[i];
        particleCriteria[i].lower = plower[i];
        particleCriteria[i].upper = pUpper[i];
        particleCriteria[i].calibrated = pCalibrated[i];
        particleCriteria[i].exclude = pExclude[i];
    }

    // Filters particles based on their morphological measurements.
    VisionErrChk(imaqParticleFilter2(image, image, particleCriteria, criteriaCount, rejectMatches, connectivity, NULL));

Error:
    free(particleCriteria);

    return success;
}




////////////////////////////////////////////////////////////////////////////////
//
// Function Name: IVA_ColorLocation
//
// Description  : Searches for areas in an image that match a given color pattern
//                image.
//
// Parameters   : image             -  Input image
//                templatePath      -  Template image path
//                colorSensitivity  -  Sensitivity of the color information in the image.
//                searchStrategy    -  Specifies how the color features of the
//                                     image are used during the search phase.
//                matchesRequested  -  Number of valid matches expected.
//                score             -  Minimum score a match can have for the
//                                     function to consider the match valid.
//                searchRectTop     -  Search rectangle - top
//                searchRectLeft    -  Search rectangle - left
//                searchRectHeight  -  Search rectangle - height
//                searchRectWidth   -  Search rectangle - width
//                ivaData           -  Internal Data structure
//
// Return Value : success
//
////////////////////////////////////////////////////////////////////////////////
/*int IVA_ColorLocation(Image* image, char* templatePath,
                             int colorSensitivity,
                             int searchStrategy,
                             int matchesRequested, float score,
                             int searchRectTop, int searchRectLeft, int searchRectHeight, int searchRectWidth,
                             IVA_Data2* ivaData)
{
    int success = 1;
    Image* imageTemplate;
    MatchColorPatternOptions cpmOptions;
    PatternMatch* cpmResults = NULL;
    int numMatchesFound;
    int i;


    //-------------------------------------------------------------------//
    //                           Color Location                          //
    //-------------------------------------------------------------------//

    // Creates and read the image template.
    VisionErrChk(imageTemplate = imaqCreateImage(IMAQ_IMAGE_RGB, 7));
    VisionErrChk(imaqReadVisionFile(imageTemplate, templatePath, NULL, NULL));

    // Fill in the Color Pattern Matching options.
    cpmOptions.matchMode = IMAQ_MATCH_SHIFT_INVARIANT;
    cpmOptions.featureMode = IMAQ_COLOR_FEATURES;
    cpmOptions.minContrast = 0;
    cpmOptions.subpixelAccuracy = 0;
    cpmOptions.angleRanges = NULL;
    cpmOptions.numRanges = 0;
    cpmOptions.colorWeight = 500;
    cpmOptions.sensitivity = colorSensitivity;
    cpmOptions.strategy = searchStrategy;
    cpmOptions.numMatchesRequested = matchesRequested;
    cpmOptions.minMatchScore = score;

    cpmResults = NULL;
    numMatchesFound = 0;

    // Searches for areas in the image that match a given color.
    VisionErrChk(cpmResults = imaqMatchColorPattern(image, imageTemplate, &cpmOptions, imaqMakeRect(searchRectTop, searchRectLeft, searchRectHeight, searchRectWidth), &numMatchesFound));
    
    
	cores_encontradas = numMatchesFound;
	
	
    // Add the points to the results array for future caliper operations.
    for (i = 0 ; i < numMatchesFound ; i++)
        VisionErrChk(IVA_AddPoint(ivaData, cpmResults[i].position));

Error:
    // Disposes temporary image and structures.
    imaqDispose(imageTemplate);
    imaqDispose(cpmResults);

    return success;
}
 */
////////////////////////////////////////////////////////////////////////////////
//
// Function Name: IVA_AddClosedContour
//
// Description  : Creates a new ROI contour based on the provided array of points
//
// Parameters   : roi            -  Region of interest
//                xCoordinates   -  x coordinates (coming from Vision Assistant)
//                yCoordinates   -  y coordinates (coming from Vision Assistant)
//                numPoints      -  Number of points
//
// Return Value : success
//
////////////////////////////////////////////////////////////////////////////////
int IVA_AddClosedContour(ROI* roi, int xCoordinates[], int yCoordinates[], int numPoints)
{
    int success = 1;
    Point* contourPoints = NULL;
    int i;


    VisionErrChk(contourPoints = (Point*)malloc(numPoints * sizeof(Point)));

    for (i = 0 ; i < numPoints ; i++)
    {
        contourPoints[i] = imaqMakePoint(xCoordinates[i], yCoordinates[i]);
    }

    // Creates a new ROI contour based on the provided array of points.
    VisionErrChk(imaqAddClosedContour(roi, contourPoints, numPoints));

Error:
    free(contourPoints);

    return success;
}


////////////////////////////////////////////////////////////////////////////////
//
// Function Name: IVA_MaskFromROI
//
// Description  : Copies the source image to the destination image in the
//                following manner: If a pixel in the mask has a value of 0,
//                the function sets the corresponding source pixel to 0.
//                Otherwise the function copies the corresponding source pixel
//                to the destination image.
//
// Parameters   : image          -  Input image
//                roi            -  Region of interest
//                invertMask     -  Inverts the mask
//                extractRegion  -  Extracts masked region
//
// Return Value : On success, this function returns a non-zero value.
//                On failure, this function returns 0.
//                To get extended error information, call imaqGetLastError().
//
////////////////////////////////////////////////////////////////////////////////
int IVA_MaskFromROI(Image* image, ROI* roi, int invertMask,
                                    int extractRegion)
{
    int success = 1;
    Image* imageMask;
    PixelValue pixValue;
    Rect roiBoundingBox;


    // Creates the image mask.
    VisionErrChk(imageMask = imaqCreateImage(IMAQ_IMAGE_U8, 2));

    // Transforms the region of interest into a mask image.
    VisionErrChk(imaqROIToMask(imageMask, roi, 255, image, NULL));

    if (invertMask)
    {
        pixValue.grayscale = 255;

        // Inverts the mask image.
        VisionErrChk(imaqXorConstant(imageMask, imageMask, pixValue));
    }

    // Masks the input image using the mask image we just created.
    VisionErrChk(imaqMask(image, image, imageMask));

    if (extractRegion)
    {
        // Gets the bounding box for the region of interest.
        VisionErrChk(imaqGetROIBoundingBox(roi, &roiBoundingBox));

        // Extracts the bounding box.
        VisionErrChk(imaqScale(image, image, 1, 1, IMAQ_SCALE_SMALLER, roiBoundingBox));
    }

Error:
    imaqDispose(imageMask);

    return success;
}


////////////////////////////////////////////////////////////////////////////////
//
// Function Name: IVA_CLRThreshold
//
// Description  : Thresholds a color image.
//
// Parameters   : image      -  Input image
//                min1       -  Minimum range for the first plane
//                max1       -  Maximum range for the first plane
//                min2       -  Minimum range for the second plane
//                max2       -  Maximum range for the second plane
//                min3       -  Minimum range for the third plane
//                max3       -  Maximum range for the third plane
//                colorMode  -  Color space in which to perform the threshold
//
// Return Value : success
//
////////////////////////////////////////////////////////////////////////////////
static int IVA_CLRThreshold(Image* image, int min1, int max1, int min2, int max2, int min3, int max3, int colorMode)
{
    int success = 1;
    Image* thresholdImage;
    Range plane1Range;
    Range plane2Range;
    Range plane3Range;


    //-------------------------------------------------------------------//
    //                          Color Threshold                          //
    //-------------------------------------------------------------------//

    // Creates an 8 bit image for the thresholded image.
    VisionErrChk(thresholdImage = imaqCreateImage(IMAQ_IMAGE_U8, 2));

    // Set the threshold range for the 3 planes.
    plane1Range.minValue = min1;
    plane1Range.maxValue = max1;
    plane2Range.minValue = min2;
    plane2Range.maxValue = max2;
    plane3Range.minValue = min3;
    plane3Range.maxValue = max3;

    // Thresholds the color image.
    VisionErrChk(imaqColorThreshold(thresholdImage, image, 1, colorMode, &plane1Range, &plane2Range, &plane3Range));

    // Copies the threshold image in the souce image.
    VisionErrChk(imaqDuplicate(image, thresholdImage));

Error:
    imaqDispose(thresholdImage);

    return success;
}




////////////////////////////////////////////////////////////////////////////////
//
// Function Name: IVA_Particle
//
// Description  : Computes the number of particles detected in a binary image and
//                a 2D array of requested measurements about the particle.
//
// Parameters   : image                      -  Input image
//                connectivity               -  Set this parameter to 1 to use
//                                              connectivity-8 to determine
//                                              whether particles are touching.
//                                              Set this parameter to 0 to use
//                                              connectivity-4 to determine
//                                              whether particles are touching.
//                pixelMeasurements          -  Array of measuremnets parameters
//                numPixelMeasurements       -  Number of elements in the array
//                calibratedMeasurements     -  Array of measuremnets parameters
//                numCalibratedMeasurements  -  Number of elements in the array
//                ivaData                    -  Internal Data structure
//
// Return Value : success
//
////////////////////////////////////////////////////////////////////////////////
int IVA_Particle(Image* image, int connectivity,
                                 int pPixelMeasurements[], int numPixelMeasurements,
                                 int pCalibratedMeasurements[], int numCalibratedMeasurements,
                                 IVA_Data2* ivaData)
{
    int success = 1;
    int numParticles;
    double* pixelMeasurements = NULL;
    double* calibratedMeasurements = NULL;
    double tempDouble;
    int i;
    int j;
    PointFloat particleCenter;


    //-------------------------------------------------------------------//
    //                         Particle Analysis                         //
    //-------------------------------------------------------------------//

    // Counts the number of particles in the image.
    VisionErrChk(imaqCountParticles(image, connectivity, &numParticles));

    // Allocate the arrays for the measurements.
    pixelMeasurements = (double*)malloc(numParticles * numPixelMeasurements * sizeof(double));
    calibratedMeasurements = (double*)malloc(numParticles * numCalibratedMeasurements * sizeof(double));

    for (i = 0 ; i < numParticles ; i++)
    {
        // Computes the requested pixel measurements about the particle.
        for (j = 0 ; j < numPixelMeasurements ; j++)
        {
            VisionErrChk(imaqMeasureParticle(image, i, FALSE, pPixelMeasurements[j], &pixelMeasurements[i*numPixelMeasurements + j]));
        }

        // Computes the requested calibrated measurements about the particle.
        for (j = 0 ; j < numCalibratedMeasurements ; j++)
        {
            VisionErrChk(imaqMeasureParticle(image, i, TRUE, pCalibratedMeasurements[j], &calibratedMeasurements[i*numCalibratedMeasurements + j]));
        }
        

        // Computes the position of each particle and adds it to the points array for future caliper operations.
        VisionErrChk(imaqMeasureParticle(image, i, FALSE, IMAQ_MT_CENTER_OF_MASS_X, &tempDouble));
        CoordCentrMass[i].x = particleCenter.x = tempDouble;
        
        VisionErrChk(imaqMeasureParticle(image, i, FALSE, IMAQ_MT_CENTER_OF_MASS_Y, &tempDouble));
        CoordCentrMass[i].y = particleCenter.y = tempDouble;

        // Add the point to the results array for future caliper operations.
        VisionErrChk(IVA_AddPoint(ivaData, particleCenter));
    }
	//numero_particulas = numParticles;//MeasurementType_enum
Error:
    free(pixelMeasurements);
    free(calibratedMeasurements);

    return success;
}

////////////////////////////////////////////////////////////////////////////////
//
// Function Name: IVA_DisposeStepResults
//
// Description  : Dispose of the results of a specific step.
//
// Parameters   : ivaData    -  Internal data structure
//                stepIndex  -  step index
//
// Return Value : success
//
////////////////////////////////////////////////////////////////////////////////
int IVA_DisposeStepResults(IVA_Data* ivaData, int stepIndex)
{
    int i;

    
    for (i = 0 ; i < ivaData->stepResults[stepIndex].numResults ; i++)
    {
        if (ivaData->stepResults[stepIndex].results[i].type == IVA_STRING)
            free(ivaData->stepResults[stepIndex].results[i].resultVal.strVal);
    }

    free(ivaData->stepResults[stepIndex].results);

    return TRUE;
}




////////////////////////////////////////////////////////////////////////////////
//
// Function Name: IVA_SetPixelValue
//
// Description  : Initialize a PixelValue structure to 0
//
// Parameters   : pixelValue         -  PixelValue Structure
//                imageType          -  image type
//                grayscaleConstant
//                redConstant
//                greenConstant
//                blueConstant
//
// Return Value : None
//
////////////////////////////////////////////////////////////////////////////////
void IVA_SetPixelValue(PixelValue*  pixelValue,
                              ImageType    imageType,
                              float        grayscaleConstant,
                              unsigned int redConstant,
                              unsigned int greenConstant,
                              unsigned int blueConstant)
{
    // Sets the pixel value.
    switch (imageType)
    {
        case IMAQ_IMAGE_U8:
        case IMAQ_IMAGE_I16:
        case IMAQ_IMAGE_SGL:
            pixelValue->grayscale = grayscaleConstant;
            break;
        case IMAQ_IMAGE_COMPLEX:
            pixelValue->complex.r = 0;
            pixelValue->complex.i = 0;
            break;
        case IMAQ_IMAGE_RGB:
            pixelValue->rgb.R = redConstant;
            pixelValue->rgb.G = greenConstant;
            pixelValue->rgb.B = blueConstant;
            pixelValue->rgb.alpha = 0;
            break;
        case IMAQ_IMAGE_HSL:
            pixelValue->hsl.L = 0;
            pixelValue->hsl.S = 0;
            pixelValue->hsl.H = 0;
            pixelValue->hsl.alpha = 0;
            break;
        default:
            break;
    }
}



////////////////////////////////////////////////////////////////////////////////
//
// Function Name: IVA_ColorLocation
//
// Description  : Searches for areas in an image that match a given color pattern
//                image.
//
// Parameters   : image             -  Input image
//                templatePath      -  Template image path
//                colorSensitivity  -  Sensitivity of the color information in the image.
//                searchStrategy    -  Specifies how the color features of the
//                                     image are used during the search phase.
//                matchesRequested  -  Number of valid matches expected.
//                score             -  Minimum score a match can have for the
//                                     function to consider the match valid.
//                searchRectTop     -  Search rectangle - top
//                searchRectLeft    -  Search rectangle - left
//                searchRectHeight  -  Search rectangle - height
//                searchRectWidth   -  Search rectangle - width
//                ivaData           -  Internal Data structure
//
// Return Value : success
//
////////////////////////////////////////////////////////////////////////////////
int IVA_ColorLocation(Image* image, char* templatePath,
                             int colorSensitivity,
                             int searchStrategy,
                             int matchesRequested, float score,
                             int searchRectTop, int searchRectLeft, int searchRectHeight, int searchRectWidth,
                             IVA_Data2* ivaData)
{
    int success = 1;
    Image* imageTemplate;
    MatchColorPatternOptions cpmOptions;
    PatternMatch* cpmResults = NULL;
    int numMatchesFound;
    int i;


    //-------------------------------------------------------------------//
    //                           Color Location                          //
    //-------------------------------------------------------------------//

    // Creates and read the image template.
    VisionErrChk(imageTemplate = imaqCreateImage(IMAQ_IMAGE_RGB, 7));
    VisionErrChk(imaqReadVisionFile(imageTemplate, templatePath, NULL, NULL));

    // Fill in the Color Pattern Matching options.
    cpmOptions.matchMode = IMAQ_MATCH_SHIFT_INVARIANT;
    cpmOptions.featureMode = IMAQ_COLOR_FEATURES;
    cpmOptions.minContrast = 0;
    cpmOptions.subpixelAccuracy = 0;
    cpmOptions.angleRanges = NULL;
    cpmOptions.numRanges = 0;
    cpmOptions.colorWeight = 500;
    cpmOptions.sensitivity = colorSensitivity;
    cpmOptions.strategy = searchStrategy;
    cpmOptions.numMatchesRequested = matchesRequested;
    cpmOptions.minMatchScore = score;

    cpmResults 			= NULL;
	template_encontrado	= 0;
    numMatchesFound 	= 0;

    // Searches for areas in the image that match a given color.
    VisionErrChk(cpmResults = imaqMatchColorPattern(image, imageTemplate, &cpmOptions, imaqMakeRect(searchRectTop, searchRectLeft, searchRectHeight, searchRectWidth), &numMatchesFound));
    
    
	template_encontrado = numMatchesFound;
	if( template_encontrado)
	{
     	imaqOverlayPatternMatch( image, cpmResults, &IMAQ_RGB_BLUE, NULL );
		//score_encontrado = cpmResults[0].score;
	}
	
    // Add the points to the results array for future caliper operations.
    for (i = 0 ; i < numMatchesFound ; i++)
	{
        //VisionErrChk(IVA_AddPoint(ivaData, cpmResults[i].position));
		//score_encontrado = cpmResults[0].score;
		
	}
Error:
    // Disposes temporary image and structures.
    imaqDispose(imageTemplate);
    imaqDispose(cpmResults);

    return success;
}

 ////////////////////////////////////////////////////////////////////////////////
//
// Function Name: IVA_MatchGeometricPattern
//
// Description  : Searches for areas in an image that match a given color pattern
//                image.
//
// Parameters   : image                -  Input image
//                ivaData              -  Internal Data structure
//                templatePath         -  Template image path
//                extraction           -  Extraction mode
//                curveThreshold       -  Specifies the minimum contrast at a
//                                        pixel for it to be considered as part
//                                        of a curve.
//                edgeFilterSize       -  Specifies the width of the edge filter
//                                        the function uses to identify curves in
//                                        the image.
//                curveMinLength       -  Specifies the smallest curve the
//                                        function will identify as a curve.
//                curveRowStepSize     -  Specifies the distance, in the x direction
//                                        between two pixels the function inspects
//                                        for curve seed points.
//                curveColumnStepSize  -  Specifies the distance, in the y direction,
//                                        between two pixels the function inspects
//                                        for curve seed points.
//                curveMaxEndPointGap  -  Specifies the maximum gap, in pixels,
//                                        between the endpoints of a curve that the
//                                        function identifies as a closed curve.
//                matchMode            -  Specifies the method to use when looking
//                                        for the pattern in the image.
//                subpixel             -  Subpixel accuracy
//                rangeMin             -  Match constraints range min array
//                                        (angle 1, angle 2, scale, occlusion)
//                rangeMax             -  Match constraints range max array
//                                        (angle 1, angle 2, scale, occlusion)
//                matchesRequested     -  Number of valid matches expected.
//                score                -  Minimum score a match can have for the
//                                        function to consider the match valid.
//                contrastReversal     -  Use this element to specify the contrast of the matches to search for in the image.
//                searchStrategy       -  Specifies the aggressiveness of the search strategy.
//                roi                  -  Search area
//                stepIndex            -  Step index (index at which to store the results in the resuts array)
//
// Return Value : success
//
////////////////////////////////////////////////////////////////////////////////
int IVA_MatchGeometricPattern2(Image* image,
                                               IVA_Data* ivaData,
                                               char* templatePath,
                                               int extraction,
                                               int curveThreshold,
                                               int edgeFilterSize,
                                               int curveMinLength,
                                               int curveRowStepSize,
                                               int curveColumnStepSize,
                                               int curveMaxEndPointGap,
                                               int matchMode,
                                               int subpixel,
                                               float rangeMin[],
                                               float rangeMax[],
                                               int matchesRequested,
                                               float score,
                                               int contrastReversal,
                                               int searchStrategy,
                                               ROI* roi,
                                               int stepIndex)
{
    int success = 1;
    Image* imageTemplate;
    CurveOptions curveOptions;
    MatchGeometricPatternOptions matchOptions;
    RangeFloat angleRange[2];
    int i;
    MatchGeometricPatternAdvancedOptions3 advancedMatchOptions;
    GeometricPatternMatch3* pmResults = NULL;
    int numMatchesFound;
    int numObjectResults;
    IVA_Result* patternMacthingResults;
    unsigned int visionInfo;


    //-------------------------------------------------------------------//
    //                        Geometric Matching                         //
    //-------------------------------------------------------------------//

    // Creates and read the image template.
    VisionErrChk(imageTemplate = imaqCreateImage(IMAQ_IMAGE_U8, 7));
    VisionErrChk(imaqReadVisionFile(imageTemplate, templatePath, NULL, NULL));

    // Fill in the Curve options
    curveOptions.extractionMode = extraction;
    curveOptions.threshold = curveThreshold;
    curveOptions.filterSize = edgeFilterSize;
    curveOptions.minLength = curveMinLength;
    curveOptions.rowStepSize = curveRowStepSize;
    curveOptions.columnStepSize = curveColumnStepSize;
    curveOptions.maxEndPointGap = curveMaxEndPointGap;
    curveOptions.onlyClosed = 0;
    curveOptions.subpixelAccuracy = 0;

    // Fill in the Pattern Matching options.
    matchOptions.mode = matchMode;
    matchOptions.subpixelAccuracy = subpixel;

    for (i = 0 ; i < 2 ; i++)
    {
        angleRange[i].minValue = rangeMin[i];
        angleRange[i].maxValue = rangeMax[i];
    }

    matchOptions.angleRanges = angleRange;
    matchOptions.numAngleRanges = 2;
    matchOptions.scaleRange.minValue = rangeMin[2];
    matchOptions.scaleRange.maxValue = rangeMax[2];
    matchOptions.occlusionRange.minValue = rangeMin[3];
    matchOptions.occlusionRange.maxValue = rangeMax[3];
    matchOptions.numMatchesRequested = matchesRequested;
    matchOptions.minMatchScore = score;

    // Set the advanced match options
    advancedMatchOptions.subpixelIterations = 20;
    advancedMatchOptions.subpixelTolerance = 0;
    advancedMatchOptions.initialMatchListLength = 200;
    advancedMatchOptions.targetTemplateCurveScore = 0;
    advancedMatchOptions.correlationScore = 1;
    advancedMatchOptions.minMatchSeparationDistance = 20;
    advancedMatchOptions.minMatchSeparationAngle = 10;
    advancedMatchOptions.minMatchSeparationScale = 10;
    advancedMatchOptions.maxMatchOverlap = 80;
    advancedMatchOptions.coarseResult = 0;
    advancedMatchOptions.enableCalibrationSupport = 1;
    advancedMatchOptions.enableContrastReversal = contrastReversal;
    advancedMatchOptions.matchStrategy = searchStrategy;
    advancedMatchOptions.refineMatchFactor = 2;
    advancedMatchOptions.subpixelMatchFactor = 2;

    pmResults = NULL;
    numMatchesFound = 0;

    // Searches for areas in the image that match a given color.
    VisionErrChk(pmResults = imaqMatchGeometricPattern3(image, imageTemplate, &curveOptions, &matchOptions, &advancedMatchOptions, roi, &numMatchesFound));
	template_encontrado = numMatchesFound;
	if( template_encontrado)
     	//imaqOverlayPatternMatch( image, pmResults, &IMAQ_RGB_BLUE, NULL );
            OverlayGeometricPatternMatch2 (image, &pmResults[0], &IMAQ_RGB_GREEN);

    // ////////////////////////////////////////
    // Store the results in the data structure.
    // ////////////////////////////////////////
    
    // First, delete all the results of this step (from a previous iteration)
    IVA_DisposeStepResults(ivaData, stepIndex);

    // Check if the image is calibrated.
    VisionErrChk(imaqGetVisionInfoTypes(image, &visionInfo));

    // If the image is calibrated, we also need to log the calibrated position (x and y) -> 10 results instead of 8
    numObjectResults = (visionInfo & IMAQ_VISIONINFO_CALIBRATION ? 10 : 8);

    ivaData->stepResults[stepIndex].numResults = numMatchesFound * numObjectResults + 1;
    ivaData->stepResults[stepIndex].results = malloc (sizeof(IVA_Result) * ivaData->stepResults[stepIndex].numResults);
    patternMacthingResults = ivaData->stepResults[stepIndex].results;
    
    if (patternMacthingResults)
    {
        #if defined (IVA_STORE_RESULT_NAMES)
            sprintf(patternMacthingResults->resultName, "# Matches");
        #endif
        patternMacthingResults->type = IVA_NUMERIC;
        patternMacthingResults->resultVal.numVal = numMatchesFound;
        patternMacthingResults++;
        
        for (i = 0 ; i < numMatchesFound ; i++)
        {
            #if defined (IVA_STORE_RESULT_NAMES)
                sprintf(patternMacthingResults->resultName, "Match %d.X Position (Pix.)", i + 1);
            #endif
            patternMacthingResults->type = IVA_NUMERIC;
            patternMacthingResults->resultVal.numVal = pmResults[i].position.x;
            patternMacthingResults++;
            
            #if defined (IVA_STORE_RESULT_NAMES)
                sprintf(patternMacthingResults->resultName, "Match %d.Y Position (Pix.)", i + 1);
            #endif
            patternMacthingResults->type = IVA_NUMERIC;
            patternMacthingResults->resultVal.numVal = pmResults[i].position.y;
            patternMacthingResults++;
            
            if (visionInfo & IMAQ_VISIONINFO_CALIBRATION)
            {
                #if defined (IVA_STORE_RESULT_NAMES)
                    sprintf(patternMacthingResults->resultName, "Match %d.X Position (World)", i + 1);
                #endif
                patternMacthingResults->type = IVA_NUMERIC;
                patternMacthingResults->resultVal.numVal = pmResults[i].calibratedPosition.x;
                patternMacthingResults++;

                #if defined (IVA_STORE_RESULT_NAMES)
                    sprintf(patternMacthingResults->resultName, "Match %d.Y Position (World)", i + 1);
                #endif
                patternMacthingResults->type = IVA_NUMERIC;
                patternMacthingResults->resultVal.numVal = pmResults[i].calibratedPosition.y;
                patternMacthingResults++;
            }

            #if defined (IVA_STORE_RESULT_NAMES)
                sprintf(patternMacthingResults->resultName, "Match %d.Angle (degrees)", i + 1);
            #endif
            patternMacthingResults->type = IVA_NUMERIC;
            patternMacthingResults->resultVal.numVal = pmResults[i].rotation;
            patternMacthingResults++;

            #if defined (IVA_STORE_RESULT_NAMES)
                sprintf(patternMacthingResults->resultName, "Match %d.Scale", i + 1);
            #endif
            patternMacthingResults->type = IVA_NUMERIC;
            patternMacthingResults->resultVal.numVal = pmResults[i].scale;
            patternMacthingResults++;
            
            #if defined (IVA_STORE_RESULT_NAMES)
                sprintf(patternMacthingResults->resultName, "Match %d.Score", i + 1);
            #endif
            patternMacthingResults->type = IVA_NUMERIC;
            patternMacthingResults->resultVal.numVal = pmResults[i].score;
            patternMacthingResults++;

            #if defined (IVA_STORE_RESULT_NAMES)
                sprintf(patternMacthingResults->resultName, "Match %d.Occlusion", i + 1);
            #endif
            patternMacthingResults->type = IVA_NUMERIC;
            patternMacthingResults->resultVal.numVal = pmResults[i].occlusion;
            patternMacthingResults++;
            
            #if defined (IVA_STORE_RESULT_NAMES)
                sprintf(patternMacthingResults->resultName, "Match %d.Template Target Curve Score", i + 1);
            #endif
            patternMacthingResults->type = IVA_NUMERIC;
            patternMacthingResults->resultVal.numVal = pmResults[i].templateMatchCurveScore;
            patternMacthingResults++;
            
            #if defined (IVA_STORE_RESULT_NAMES)
                sprintf(patternMacthingResults->resultName, "Match %d.Correlation Score", i + 1);
            #endif
            patternMacthingResults->type = IVA_NUMERIC;
            patternMacthingResults->resultVal.numVal = pmResults[i].correlationScore;
            patternMacthingResults++;
        }
    }
    
Error:
    // Disposes temporary image and structures.
    imaqDispose(imageTemplate);
    imaqDispose(pmResults);

    return success;
}

//**********************************************************************
//  OverlayGeometricPatternMatch2 is used to overlay the results of       
//  imaqMatchGeometricPattern2() on the search image.                     
//**********************************************************************
int OverlayGeometricPatternMatch2( Image* image, const GeometricPatternMatch3* match, const RGBValue* color ) {
    Point boundingRect[4];  
    int i;

    // Overlay the center of the pattern match.
    if( imaqOverlayOval( image, imaqMakeRect( L_OFFSET(match->position.y), L_OFFSET(match->position.x), OVERLAY_PATTERN_SIZE, OVERLAY_PATTERN_SIZE), color, IMAQ_DRAW_VALUE, NULL ) == FALSE )
        return FALSE;

    // Overlay a crosshair at the center of the pattern match.
    if( imaqOverlayLine( image, imaqMakePoint(ROUND_TO_INT(match->position.x - OVERLAY_PATTERN_SIZE),ROUND_TO_INT(match->position.y)),imaqMakePoint(ROUND_TO_INT(match->position.x+OVERLAY_PATTERN_SIZE),ROUND_TO_INT(match->position.y)),color,NULL)==FALSE)
        return FALSE;
    if( imaqOverlayLine( image, imaqMakePoint(ROUND_TO_INT(match->position.x),ROUND_TO_INT(match->position.y - OVERLAY_PATTERN_SIZE)),imaqMakePoint(ROUND_TO_INT(match->position.x),ROUND_TO_INT(match->position.y+OVERLAY_PATTERN_SIZE)),color,NULL)==FALSE)
        return FALSE;

    // Overlay the bounding rectangle of the pattern match.
    for ( i = 0; i < 4; i++ ) {
        boundingRect[i].x = ROUND_TO_INT(match->corner[i].x);
        boundingRect[i].y = ROUND_TO_INT(match->corner[i].y);
    }
    if ( imaqOverlayClosedContour(image, boundingRect, 4, color, IMAQ_DRAW_VALUE, NULL ) == FALSE )
        return FALSE;

    return TRUE;

}
////////////////////////////////////////////////////////////////////////////////
//
// Function Name: IVA_EdgeTool2
//
// Description  : Finds edges along a path in an image.
//
// Parameters   : image                  -  Input image
//                roi                    -  Region of Interest
//                pProcessType           -  Determines which edges the function looks for. 
//                pPolarity              -  Specifies the polarity of the edges to be found
//                pKernelSize            -  Specifies the size of the edge detection kernel
//                pWidth                 -  Specifies the number of pixels averaged perpendicular
//                                          to the search direction to compute the edge profile
//                                          strength at each point along the search ROI
//                pMinThreshold          -  Specifies the minimum edge strength (gradient magnitude)
//                                          required for a detected edge
//                pInterpolationType     -  Specifies the interpolation method used to locate the edge position
//                pColumnProcessingMode  -  Specifies the method used to find the straight edge
//                ivaData                -  Internal Data structure
//                stepIndex              -  Step index (index at which to store
//                                          the results in the resuts array)
//
// Return Value : None (inline function)
//
////////////////////////////////////////////////////////////////////////////////
int IVA_EdgeTool2(Image* image,
                                  ROI* roi,
                                  int pProcessType,
                                  int pPolarity,
                                  int pKernelSize,
                                  int pWidth,
                                  float pMinThreshold,
                                  int pInterpolationType,
                                  int pColumnProcessingMode,
                                  IVA_Data* ivaData,
                                  int stepIndex)
{
    int success = 1;
    EdgeOptions2 edgeOptions;
    EdgeReport2* edgeReport = NULL;
    int i;
    int numObjectResults;
    IVA_Result* edgeResults;
    unsigned int visionInfo;
	Point p;


    //-------------------------------------------------------------------//
    //                     Edge Detector - Edge Tool3                    //
    //-------------------------------------------------------------------//

    edgeOptions.polarity = pPolarity;
    edgeOptions.kernelSize = pKernelSize;
    edgeOptions.width = pWidth;
    edgeOptions.minThreshold = pMinThreshold;
    edgeOptions.interpolationType = pInterpolationType;
    edgeOptions.columnProcessingMode = pColumnProcessingMode;

    // Finds edges along a path in the image.
    VisionErrChk(edgeReport = imaqEdgeTool4(image, roi, pProcessType, &edgeOptions, FALSE));

    // ////////////////////////////////////////
    // Store the results in the data structure.
    // ////////////////////////////////////////
    
    // First, delete all the results of this step (from a previous iteration)
    IVA_DisposeStepResults(ivaData, stepIndex);

    // Check if the image is calibrated.
    VisionErrChk(imaqGetVisionInfoTypes(image, &visionInfo));

    // If the image is calibrated, we also need to log the calibrated position (x and y) -> 9 results instead of 6
    numObjectResults = (visionInfo & IMAQ_VISIONINFO_CALIBRATION ? 9 : 6);

    ivaData->stepResults[stepIndex].numResults = edgeReport->numEdges * numObjectResults + 1;
    ivaData->stepResults[stepIndex].results = malloc (sizeof(IVA_Result) * ivaData->stepResults[stepIndex].numResults);
    edgeResults = ivaData->stepResults[stepIndex].results;
    
    if (edgeResults)
    {
        #if defined (IVA_STORE_RESULT_NAMES)
            sprintf(edgeResults->resultName, "# of Edges");
        #endif
        edgeResults->type = IVA_NUMERIC;
        edgeResults->resultVal.numVal = edgeReport->numEdges;
        edgeResults++;
        
        for (i = 0 ; i < edgeReport->numEdges ; i++)
        {
            #if defined (IVA_STORE_RESULT_NAMES)
                sprintf(edgeResults->resultName, "Edge %d.X Position (Pix.)", i + 1);
            #endif
            edgeResults->type = IVA_NUMERIC;
            edgeResults->resultVal.numVal = edgeReport->edges[i].position.x;
            edgeResults++;
            
            #if defined (IVA_STORE_RESULT_NAMES)
                sprintf(edgeResults->resultName, "Edge %d.Y Position (Pix.)", i + 1);
            #endif
            edgeResults->type = IVA_NUMERIC;
            edgeResults->resultVal.numVal = edgeReport->edges[i].position.y;
            edgeResults++;
            
            if (visionInfo & IMAQ_VISIONINFO_CALIBRATION)
            {
                #if defined (IVA_STORE_RESULT_NAMES)
                    sprintf(edgeResults->resultName, "Edge %d.X Position (World)", i + 1);
                #endif
                edgeResults->type = IVA_NUMERIC;
                edgeResults->resultVal.numVal = edgeReport->edges[i].calibratedPosition.x;
                edgeResults++;

                #if defined (IVA_STORE_RESULT_NAMES)
                    sprintf(edgeResults->resultName, "Edge %d.Y Position (World)", i + 1);
                #endif
                edgeResults->type = IVA_NUMERIC;
                edgeResults->resultVal.numVal = edgeReport->edges[i].calibratedPosition.y;
                edgeResults++;
            }

            #if defined (IVA_STORE_RESULT_NAMES)
                sprintf(edgeResults->resultName, "Edge %d.Distance (Pix.)", i + 1);
            #endif
            edgeResults->type = IVA_NUMERIC;
            edgeResults->resultVal.numVal = edgeReport->edges[i].distance;
            edgeResults++;

            if (visionInfo & IMAQ_VISIONINFO_CALIBRATION)
            {
                #if defined (IVA_STORE_RESULT_NAMES)
                    sprintf(edgeResults->resultName, "Edge %d.Distance (World)", i + 1);
                #endif
                edgeResults->type = IVA_NUMERIC;
                edgeResults->resultVal.numVal = edgeReport->edges[i].calibratedDistance;
                edgeResults++;
            }
            
            #if defined (IVA_STORE_RESULT_NAMES)
                sprintf(edgeResults->resultName, "Edge %d.Strength", i + 1);
            #endif
            edgeResults->type = IVA_NUMERIC;
            edgeResults->resultVal.numVal = edgeReport->edges[i].magnitude;
            edgeResults++;

            #if defined (IVA_STORE_RESULT_NAMES)
                sprintf(edgeResults->resultName, "Edge %d.Noise", i + 1);
            #endif
            edgeResults->type = IVA_NUMERIC;
            edgeResults->resultVal.numVal = edgeReport->edges[i].noisePeak;
            edgeResults++;
            
            #if defined (IVA_STORE_RESULT_NAMES)
                sprintf(edgeResults->resultName, "Edge %d.Rising", i + 1);
            #endif
            edgeResults->type = IVA_NUMERIC;
            edgeResults->resultVal.numVal = edgeReport->edges[i].rising;
            edgeResults++;
        }
        quant_edges = edgeReport->numEdges;
		if(quant_edges>NUM_MAX_EDGES)
			quant_edges=NUM_MAX_EDGES;
	    // Log the results in the points array for future caliper operations.
		coord_edge_y = 0;
		coord_edge_x = 0;
	    for (i = 0 ; i < quant_edges  ; i++)
	    {
	        //pontos_edge[i]=edges[i];
	        pontos_edge[i].x=edgeReport->edges[i].position.x;
	        pontos_edge[i].y=edgeReport->edges[i].position.y;
			p = imaqMakePoint (pontos_edge[i].x, pontos_edge[i].y);
			imaqOverlayPoints(image, &p, 1, &IMAQ_RGB_BLUE, 1, IMAQ_POINT_AS_CROSS, NULL, NULL);
			coord_edge_y	= pontos_edge[i].y + coord_edge_y;
			coord_edge_x	= pontos_edge[i].x + coord_edge_x;
	    }
		coord_edge_y	= coord_edge_y/quant_edges;
		coord_edge_x	= coord_edge_x/quant_edges;
        
    }
    
Error:
    // Disposes temporary structures.
    imaqDispose(edgeReport);

    return success;
}
////////////////////////////////////////////////////////////////////////////////
//
// Function Name: IVA_ReadBarcode
//
// Description  : Reads a barcode from an image.
//
// Parameters   : image         -  Input image
//                minBarWidth   -  Minimum bar width of the barcode in the image in pixels
//                minStrength   -  Minimum edge strength required to allow barcode for reading
//                minNumBars    -  Minimum number of bars required in the barcode to be qualified for reading
//                skipLocation  -  If TRUE, specifies that the function should assume that the barcode code occupies most of the search region.
//                barcodeTypes  -  Types of barcode to read
//                numTypes      -  number of elements in the barcodeTypes array
//                numBarcodes   -  number of barcodes to read
//                roi           -  Region of interest specifying the location of
//                                 the barcode in the image.
//                validate  -  If type is IMAQ_I2_OF_5, IMAQ_CODE39, or
//                             IMAQ_CODABAR, set validate to TRUE to use the
//                             error correction information of the barcode to
//                             validate the data.
//
// Return Value : success
//
////////////////////////////////////////////////////////////////////////////////
int IVA_ReadBarcode(Image* image,
                                    unsigned int minBarWidth,
                                    unsigned int minStrength,
                                    unsigned int minNumBars,
                                    int skipLocation,
                                    int barcodeTypes[],
                                    unsigned int numTypes,
                                    unsigned int numBarcodes,
                                    ROI* roi,
                                    int validate,
                                    int gradeBarcode,
                                    unsigned int scanWidth)
{
	strcpy(barcode_lido, "");
    int success = 1;
    Barcode1DSearchOptions barcodeSearchOptions;
    BarcodeGradingOptions gradingOptions;
    BarcodeInfoReportandGrading* barcodeReport = NULL;
    RGBValue redColor = IMAQ_INIT_RGB_RED;


    barcodeSearchOptions.minBarcodeWidth = minBarWidth;
    barcodeSearchOptions.minEdgeStrength = minStrength;
    barcodeSearchOptions.minBars = minNumBars;
    barcodeSearchOptions.skipLocationSearch = skipLocation;
    gradingOptions.calcGradingReport = gradeBarcode;
    gradingOptions.scanProfileWidth = scanWidth;


    VisionErrChk(barcodeReport = imaqReadBarcode3(image, &barcodeSearchOptions, barcodeTypes, numTypes, numBarcodes, roi, validate, &gradingOptions));

    for (int i=0; i< barcodeReport->numBarcodes ; i++)
    {
        Point boundingBox[barcodeReport->info[i].numBox];
        for (int j=0; j < barcodeReport->info[i].numBox ; j++)
        {
            boundingBox[j] = imaqMakePoint(barcodeReport->info[i].boundingBox[j].x, barcodeReport->info[i].boundingBox[j].y);
        }
        
        VisionErrChk(imaqOverlayClosedContour(image, boundingBox, barcodeReport->info[i].numBox, &redColor, IMAQ_DRAW_VALUE, ""));
    }
	if(barcodeReport->info != NULL)
		if(barcodeReport->info->data != NULL)
			strcpy(barcode_lido, barcodeReport->info->data);
Error:
    imaqDispose(barcodeReport);

    return success;
}
////////////////////////////////////////////////////////////////////////////////
//
// Function Name: IVA_InitCaliperResults
//
// Description  : 
//
// Parameters   :  image                 - Input image
//                 ivaData               - Internal data structure
//                 stepIndex             - Step index (index at which to store the results in the resuts array)
//                 numResults            - Number of results the caliper steps will log
//                 numCalibratedResults  - Number of calibrated results the caliper steps will log
//                 caliperResults        - Pointer to caliper results
//
// Return Value : TRUE
//
////////////////////////////////////////////////////////////////////////////////
int IVA_InitCaliperResults(Image* image,
                           IVA_Data* ivaData,
                           int stepIndex,
                           int numResults,
                           int numCalibratedResults,
                           IVA_Result** caliperResults)
{
    int success = TRUE;
    int visionInfo;

    
    // First, delete all the results of this step (from a previous iteration)
    IVA_DisposeStepResults(ivaData, stepIndex);

    // Check if the image is calibrated.
    VisionErrChk(imaqGetVisionInfoTypes(image, &visionInfo));

    ivaData->stepResults[stepIndex].numResults = (visionInfo & IMAQ_VISIONINFO_CALIBRATION ?
                                                  numCalibratedResults :
                                                  numResults);

    VisionErrChk(ivaData->stepResults[stepIndex].results = malloc(sizeof(IVA_Result) * ivaData->stepResults[stepIndex].numResults));

    *caliperResults = ivaData->stepResults[stepIndex].results;

Error:
    return success;
}


// //////////////////////////////////////////////////////////////////////////////
//
// Function Name: IVA_GetDistance
//
// Description  : Computes the distance between two points.
//
// Parameters   : image           - Input image
//                ivaData         - Internal data structure
//                stepIndex1      - Index of the step that created point#1
//                resultIndex1    - Index of the x coordinate of point#1
//                stepIndex2      - Index of the step that created point#2
//                resultIndex2    - Index of the x coordinate of point#2
//                caliperResults  - Pointer to caliper results
//
// Return Value : success
//
// //////////////////////////////////////////////////////////////////////////////
int IVA_GetDistance(Image* image,
                    IVA_Data* ivaData,
                    int stepIndex1,
                    int resultIndex1,
                    int stepIndex2,
                    int resultIndex2,
                    IVA_Result** caliperResults)
{
    int success = TRUE;
    PointFloat point1;
    PointFloat point2;
    float distance;
    unsigned int visionInfo;
	
	medida_distancia = 0;
    TransformReport* realWorldPosition1 = NULL;
    TransformReport* realWorldPosition2 = NULL;


    //-------------------------------------------------------------------//
    //                         Caliper: Distance                         //
    //-------------------------------------------------------------------//

    point1 = IVA_GetPoint(ivaData, stepIndex1, resultIndex1);
    point2 = IVA_GetPoint(ivaData, stepIndex2, resultIndex2);
    
    // Computes the distance between two points.
    VisionErrChk(imaqGetDistance(point1, point2, &distance));
	
    // ////////////////////////////////////////
    // Store the results in the data structure.
    // ////////////////////////////////////////
    
    #if defined (IVA_STORE_RESULT_NAMES)
        sprintf((*caliperResults)->resultName, "Distance (Pix.)");
    #endif
    (*caliperResults)->type = IVA_NUMERIC;
    (*caliperResults)->resultVal.numVal = distance;
    (*caliperResults)++;
	medida_distancia = distance;
    // Check if the image is calibrated.
    VisionErrChk(imaqGetVisionInfoTypes(image, &visionInfo));
    if (visionInfo & IMAQ_VISIONINFO_CALIBRATION)
    {
        realWorldPosition1 = imaqTransformPixelToRealWorld(image, &point1, 1);
        realWorldPosition2 = imaqTransformPixelToRealWorld(image, &point2, 1);

        // Computes the distance between two real-world points.
        imaqGetDistance(realWorldPosition1->points[0], realWorldPosition2->points[0], &distance);

        #if defined (IVA_STORE_RESULT_NAMES)
            sprintf((*caliperResults)->resultName, "Distance (Calibrated)");
        #endif
        (*caliperResults)->type = IVA_NUMERIC;
        (*caliperResults)->resultVal.numVal = distance;
        (*caliperResults)++;
    }

Error:
    imaqDispose(realWorldPosition1);
    imaqDispose(realWorldPosition2);
    return success;
}
