//---------------------------------------------------------------------------
/*

MIT LICENSE Copyright (c) 2021 Pawel Kuklik

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the SOFTWARE.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE. */
//---------------------------------------------------------------------------
//-------------------------------------------------------------------------------

#ifndef MINDOUBLE
#define MINDOUBLE -1E208
#endif

#ifndef MAXDOUBLE
#define MAXDOUBLE +1E208
#endif

//--------------------------------------------------------------------
// ON and OFF for general use.
//--------------------------------------------------------------------

#ifndef ON
#define ON 1
#endif

#ifndef OFF
#define OFF 0
#endif

//-------------------------------------------------------------------------------

#ifndef OK_RESULT
#define OK_RESULT 1
#endif

#ifndef FILE_NOT_FOUND
#define FILE_NOT_FOUND -1
#endif

//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
// These are OpenGL 3D panel popup menu item tags.
// ATTENTION: items are selected from popup menu and order must be in accordance
// with below numbers!
//-------------------------------------------------------------------------------

#ifndef MAP_POPUP_ITEM_SELECT_MODE
#define MAP_POPUP_ITEM_SELECT_MODE 0
#endif

#ifndef MAP_POPUP_ITEM_ROTATE
#define MAP_POPUP_ITEM_ROTATE 1
#endif

#ifndef MAP_POPUP_ITEM_ZOOM
#define MAP_POPUP_ITEM_ZOOM 2
#endif

#ifndef MAP_POPUP_ITEM_PAN
#define MAP_POPUP_ITEM_PAN 3
#endif


//---------------------------------------------------------------------------
// MESSAGES SYSTEM IN THE APP
//---------------------------------------------------------------------------

#ifndef CANT_CREATE_FILE
#define CANT_CREATE_FILE "Unable to create the file"
#endif

#ifndef SAVING_SUCCESFULL
#define SAVING_SUCCESFULL "File saving completed"
#endif

#ifndef LOADING_SUCCESFULL
#define LOADING_SUCCESFULL "File loading completed"
#endif

#ifndef WRONG_FILE_SIGNATURE
#define WRONG_FILE_SIGNATURE "Invalid file type or file corrupted"
#endif

#ifndef NO_VERSION_TO_LOAD_FOUND
#define NO_VERSION_TO_LOAD_FOUND "File versioning error. Possibly file was saved by newer version of the app"
#endif

//---------------------------------------------------------------------------

#ifndef NORMAL_TYPE
#define NORMAL_TYPE -1
#endif

//---------------------------------------------------------------------------

#ifndef SPHERE_DISPLAY_PARAMETER_SLICES
#define SPHERE_DISPLAY_PARAMETER_SLICES 4
#endif

#ifndef SPHERE_DISPLAY_PARAMETER_STACKS
#define SPHERE_DISPLAY_PARAMETER_STACKS 4
#endif

//---------------------------------------------------------------------------
// Which type of the object is displayed. Used to implement OpenGl picking.
//---------------------------------------------------------------------------

#ifndef DISPLAYED_OBJECT_TYPE_SURFACE_TRIANGLE
#define DISPLAYED_OBJECT_TYPE_SURFACE_TRIANGLE 0
#endif

#ifndef DISPLAYED_OBJECT_TYPE_DATA_POINT
#define DISPLAYED_OBJECT_TYPE_DATA_POINT 1
#endif

#ifndef DISPLAYED_OBJECT_TYPE_MARKER
#define DISPLAYED_OBJECT_TYPE_MARKER 2
#endif

#ifndef DISPLAYED_OBJECT_ELECTRODE
#define DISPLAYED_OBJECT_ELECTRODE 3
#endif

//---------------------------------------------------------------------------

#ifndef VALUE_BASED_ON_DATA_POINTS
#define VALUE_BASED_ON_DATA_POINTS 0
#endif

#ifndef VALUE_BASED_ON_GEO_NODES
#define VALUE_BASED_ON_GEO_NODES 1
#endif

//---------------------------------------------------------------------------

#ifndef SEGMENTATION_VALUE_NAME
#define SEGMENTATION_VALUE_NAME "Segmented areas"
#endif

//---------------------------------------------------------------------------

#ifndef MB_DOWN
#define MB_DOWN 1
#endif

#ifndef MB_UP
#define MB_UP 2
#endif

//--------------------------------------------------------------------
// Algorithms used for activation detection
//--------------------------------------------------------------------

#ifndef MAX_VALUE_LAT_DETECTION_ALG
#define MAX_VALUE_LAT_DETECTION_ALG 0
#endif

#ifndef MIN_VALUE_LAT_DETECTION_ALG
#define MIN_VALUE_LAT_DETECTION_ALG 1
#endif

#ifndef MAX_DOWN_SLOPE_LAT_DETECTION_ALG
#define MAX_DOWN_SLOPE_LAT_DETECTION_ALG 2
#endif

#ifndef MAX_UP_SLOPE_LAT_DETECTION_ALG
#define MAX_UP_SLOPE_LAT_DETECTION_ALG 3
#endif

#ifndef MAX_UPDOWN_SLOPE_LAT_DETECTION_ALG
#define MAX_UPDOWN_SLOPE_LAT_DETECTION_ALG 4
#endif

#ifndef MAX_DEVIATION_LAT_DETECTION_ALG
#define MAX_DEVIATION_LAT_DETECTION_ALG 5
#endif

#ifndef CROSSING_ZERO_LINE_DETECTION_ALG
#define CROSSING_ZERO_LINE_DETECTION_ALG 6
#endif

#ifndef ACCORDING_TO_LOCAL_PEAK_ALG
#define ACCORDING_TO_LOCAL_PEAK_ALG 7
#endif

#ifndef ONSET_OF_DEFLECTION_ALG
#define ONSET_OF_DEFLECTION_ALG 8
#endif

#ifndef OFFSET_OF_DEFLECTION_ALG
#define OFFSET_OF_DEFLECTION_ALG 9
#endif

//--------------------------------------------------------------------

//--------------------------------------------------------------------
// Value used in peak to peak amplitude assessment by LAT detection algorithms:
// Width of the window in which p2p amplitude is calclated.
// Attention: value in ms
//--------------------------------------------------------------------

#ifndef ACTIVATION_HALF_WIDTH_FOR_PEAK_TO_PEAK_EXTRACTION_MS
#define ACTIVATION_HALF_WIDTH_FOR_PEAK_TO_PEAK_EXTRACTION_MS 40
#endif

//---------------------------------------------------------------------

#ifndef INTERPOLATION_TYPE_FLAG_NO_INTERPOLATION
#define INTERPOLATION_TYPE_FLAG_NO_INTERPOLATION 0
#endif

#ifndef INTERPOLATION_TYPE_FLAG_NORMAL_INTERPOLATION
#define INTERPOLATION_TYPE_FLAG_NORMAL_INTERPOLATION 1
#endif

//---------------------------------------------------------------------

#ifndef ownGLUT_BITMAP_TIMES_ROMAN_10
#define ownGLUT_BITMAP_TIMES_ROMAN_10 4
#endif

#ifndef ownGLUT_BITMAP_HELVETICA_12
#define ownGLUT_BITMAP_HELVETICA_12 7
#endif

#ifndef ownGLUT_BITMAP_HELVETICA_18
#define ownGLUT_BITMAP_HELVETICA_18 8
#endif

#ifndef ownGLUT_BITMAP_TIMES_ROMAN_24
#define ownGLUT_BITMAP_TIMES_ROMAN_24 5
#endif

//---------------------------------------------------------------------

#ifndef MAPPING_SYSTEM_ORIGIN_UNDEFINED
#define MAPPING_SYSTEM_ORIGIN_UNDEFINED 0
#endif

#ifndef MAPPING_SYSTEM_ORIGIN_CARTO
#define MAPPING_SYSTEM_ORIGIN_CARTO 1
#endif

#ifndef MAPPING_SYSTEM_ORIGIN_NAVX
#define MAPPING_SYSTEM_ORIGIN_NAVX 2
#endif

#ifndef MAPPING_SYSTEM_ORIGIN_RHYTHMIA
#define MAPPING_SYSTEM_ORIGIN_RHYTHMIA 3
#endif

//---------------------------------------------------------------------

#ifndef DP_POINT_TYPE_NORMAL
#define DP_POINT_TYPE_NORMAL 0
#endif

#ifndef CARTO_POINT_TYPE_LOCATION_ONLY
#define CARTO_POINT_TYPE_LOCATION_ONLY 1
#endif

#ifndef CARTO_POINT_TYPE_SCAR
#define CARTO_POINT_TYPE_SCAR 2
#endif

#ifndef CARTO_POINT_TYPE_FLOATING
#define CARTO_POINT_TYPE_FLOATING 3
#endif

#ifndef CARTO_POINT_TYPE_TE
#define CARTO_POINT_TYPE_TE 4
#endif

//---------------------------------------------------------------------

#ifndef CARTO_POINT_LABELID_WITHOUT_TAG
#define CARTO_POINT_LABELID_WITHOUT_TAG -1
#endif

#ifndef CARTO_POINT_LABELID_NONE
#define CARTO_POINT_LABELID_NONE 4
#endif

#ifndef CARTO_POINT_LABELID_HIS
#define CARTO_POINT_LABELID_HIS 5
#endif

#ifndef CARTO_POINT_LABELID_PACING_SITE
#define CARTO_POINT_LABELID_PACING_SITE 6
#endif

#ifndef CARTO_POINT_LABELID_DOUBLE_POTENTIAL
#define CARTO_POINT_LABELID_DOUBLE_POTENTIAL 7
#endif

#ifndef CARTO_POINT_LABELID_FRAGMENTED_SIGNAL
#define CARTO_POINT_LABELID_FRAGMENTED_SIGNAL 8
#endif

#ifndef CARTO_POINT_LABELID_ABLATION
#define CARTO_POINT_LABELID_ABLATION 9
#endif

#ifndef CARTO_POINT_LABELID_SCAR
#define CARTO_POINT_LABELID_SCAR 10
#endif

#ifndef CARTO_POINT_LABELID_LOC_ONLY
#define CARTO_POINT_LABELID_LOC_ONLY 11
#endif

#ifndef CARTO_POINT_LABELID_TE
#define CARTO_POINT_LABELID_TE 12
#endif

//---------------------------------------------------------------------

#ifndef EGM_DURATION_ALGORITHM_SD
#define EGM_DURATION_ALGORITHM_SD 1
#endif

#ifndef EGM_DURATION_ALGORITHM_INTEGRAL
#define EGM_DURATION_ALGORITHM_INTEGRAL 2
#endif

#ifndef EGM_DURATION_ALGORITHM_INTEGRAL_AND_SD
#define EGM_DURATION_ALGORITHM_INTEGRAL_AND_SD 3
#endif

//---------------------------------------------------------------------

#ifndef NOT_DELETED_DATA_POINTS
#define NOT_DELETED_DATA_POINTS 1
#endif

#ifndef DELETED_DATA_POINTS
#define DELETED_DATA_POINTS 2
#endif

//---------------------------------------------------------------------

#ifndef LV_ANTERIOR_SEGMENT_NAME
#define LV_ANTERIOR_SEGMENT_NAME "LV_Anterior"
#endif

#ifndef LV_SEPTAL_SEGMENT_NAME
#define LV_SEPTAL_SEGMENT_NAME "LV_Septal"
#endif

#ifndef LV_LATERAL_SEGMENT_NAME
#define LV_LATERAL_SEGMENT_NAME "LV_Lateral"
#endif

