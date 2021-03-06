// First argument to macro is a boolean indicating whether or not a function is
// essential or not. If a non-essential function is not exported by the DLL,
// then initialization fails.

#ifndef OVRFUNCV
#define OVRFUNCV(need, fn, params, cparams) OVRFUNC(need, void, fn, params, cparams)
#endif

OVRFUNC(1,  ovrBool,          ovr_Initialize,                 (), ())
OVRFUNC(1, ovrBool,          ovr_InitializeRenderingShim,    (), ())
OVRFUNCV(1,                   ovr_Shutdown,                   (), ())
OVRFUNC(1,  const char*,      ovr_GetVersionString,           (), ())
OVRFUNC(1,  int,              ovrHmd_Detect,                  (), ())
OVRFUNC(1,  ovrHmd,           ovrHmd_Create,                  (int index), (index))
OVRFUNCV(1,                   ovrHmd_Destroy,                 (ovrHmd hmd), (hmd))
OVRFUNC(0,  ovrHmd,           ovrHmd_CreateDebug,             (ovrHmdType type), (type))
OVRFUNC(1,  const char*,      ovrHmd_GetLastError,            (ovrHmd hmd), (hmd))
OVRFUNC(1,  ovrBool,          ovrHmd_AttachToWindow,          (ovrHmd hmd, void* window, const ovrRecti* destMirrorRect, const ovrRecti* sourceRenderTargetRect), (hmd, window, destMirrorRect, sourceRenderTargetRect))
OVRFUNC(1,  unsigned int,     ovrHmd_GetEnabledCaps,          (ovrHmd hmd), (hmd))
OVRFUNCV(1,                   ovrHmd_SetEnabledCaps,          (ovrHmd hmd, unsigned int hmdCaps), (hmd, hmdCaps))
OVRFUNC(1,  ovrBool,          ovrHmd_ConfigureTracking,       (ovrHmd hmd, unsigned int supportedTrackingCaps, unsigned int requiredTrackingCaps), (hmd, supportedTrackingCaps, requiredTrackingCaps))
OVRFUNCV(1,                   ovrHmd_RecenterPose,            (ovrHmd hmd), (hmd))
OVRFUNC(1,  ovrTrackingState, ovrHmd_GetTrackingState,        (ovrHmd hmd, double absTime), (hmd, absTime))
OVRFUNC(1,  ovrSizei,         ovrHmd_GetFovTextureSize,       (ovrHmd hmd, ovrEyeType eye, ovrFovPort fov, float pixelsPerDisplayPixel), (hmd, eye, fov, pixelsPerDisplayPixel))
OVRFUNC(1,  ovrBool,          ovrHmd_ConfigureRendering,      (ovrHmd hmd, const ovrRenderAPIConfig* apiConfig, unsigned int distortionCaps, const ovrFovPort eyeFovIn[2], ovrEyeRenderDesc eyeRenderDescOut[2] ), (hmd, apiConfig, distortionCaps, eyeFovIn, eyeRenderDescOut))
OVRFUNC(1,  ovrFrameTiming,   ovrHmd_BeginFrame,              (ovrHmd hmd, unsigned int frameIndex), (hmd, frameIndex))
OVRFUNCV(1,                   ovrHmd_EndFrame,                (ovrHmd hmd, const ovrPosef renderPose[2], const ovrTexture eyeTexture[2]), (hmd, renderPose, eyeTexture))
OVRFUNCV(1,                   ovrHmd_GetEyePoses,             (ovrHmd hmd, unsigned int frameIndex, ovrVector3f hmdToEyeViewOffset[2], ovrPosef outEyePoses[2], ovrTrackingState* outHmdTrackingState), (hmd, frameIndex, hmdToEyeViewOffset, outEyePoses, outHmdTrackingState))
OVRFUNC(1,  ovrPosef,         ovrHmd_GetHmdPosePerEye,        (ovrHmd hmd, ovrEyeType eye), (hmd, eye))
OVRFUNC(1,  ovrEyeRenderDesc, ovrHmd_GetRenderDesc,           (ovrHmd hmd, ovrEyeType eyeType, ovrFovPort fov), (hmd, eyeType, fov))
OVRFUNC(0,  ovrBool,          ovrHmd_CreateDistortionMesh,    (ovrHmd hmd, ovrEyeType eyeType, ovrFovPort fov, unsigned int distortionCaps, ovrDistortionMesh *meshData), (hmd, eyeType, fov, distortionCaps, meshData))
OVRFUNCV(0,                   ovrHmd_DestroyDistortionMesh,   (ovrDistortionMesh* meshData), (meshData))
OVRFUNCV(0,                   ovrHmd_GetRenderScaleAndOffset, (ovrFovPort fov, ovrSizei textureSize, ovrRecti renderViewport, ovrVector2f uvScaleOffsetOut[2]), (fov, textureSize, renderViewport, uvScaleOffsetOut))
OVRFUNC(1,  ovrFrameTiming,   ovrHmd_GetFrameTiming,          (ovrHmd hmd, unsigned int frameIndex), (hmd, frameIndex))
OVRFUNC(0,  ovrFrameTiming,   ovrHmd_BeginFrameTiming,        (ovrHmd hmd, unsigned int frameIndex), (hmd, frameIndex))
OVRFUNCV(0,                   ovrHmd_EndFrameTiming,          (ovrHmd hmd), (hmd))
OVRFUNCV(0,                   ovrHmd_ResetFrameTiming,        (ovrHmd hmd, unsigned int frameIndex), (hmd, frameIndex))
OVRFUNCV(0,                   ovrHmd_GetEyeTimewarpMatrices,  (ovrHmd hmd, ovrEyeType eye, ovrPosef renderPose, ovrMatrix4f twmOut[2]), (hmd, eye, renderPose, twmOut))
OVRFUNC(1,  ovrMatrix4f,      ovrMatrix4f_Projection,         (ovrFovPort fov, float znear, float zfar, ovrBool rightHanded ), (fov, znear, zfar, rightHanded))
OVRFUNC(1,  ovrMatrix4f,      ovrMatrix4f_OrthoSubProjection, (ovrMatrix4f projection, ovrVector2f orthoScale, float orthoDistance, float eyeViewAdjustX), (projection, orthoScale, orthoDistance, eyeViewAdjustX))
OVRFUNC(1,  double,           ovr_GetTimeInSeconds,           (), ())
OVRFUNC(1,  double,           ovr_WaitTillTime,               (double absTime), (absTime))
OVRFUNC(0,  ovrBool,          ovrHmd_ProcessLatencyTest,      (ovrHmd hmd, unsigned char rgbColorOut[3]), (hmd, rgbColorOut))
OVRFUNC(0,  const char*,      ovrHmd_GetLatencyTestResult,    (ovrHmd hmd), (hmd))
OVRFUNCV(0,                   ovrHmd_GetHSWDisplayState,      (ovrHmd hmd, ovrHSWDisplayState *hasWarningState), (hmd, hasWarningState))
OVRFUNC(0,  ovrBool,          ovrHmd_DismissHSWDisplay,       (ovrHmd hmd), (hmd))
OVRFUNC(0,  ovrBool,          ovrHmd_GetBool,                 (ovrHmd hmd, const char* propertyName, ovrBool defaultVal), (hmd, propertyName, defaultVal))
OVRFUNC(0,  ovrBool,          ovrHmd_SetBool,                 (ovrHmd hmd, const char* propertyName, ovrBool value), (hmd, propertyName, value))
OVRFUNC(0,  int,              ovrHmd_GetInt,                  (ovrHmd hmd, const char* propertyName, int defaultVal), (hmd, propertyName, defaultVal))
OVRFUNC(0,  ovrBool,          ovrHmd_SetInt,                  (ovrHmd hmd, const char* propertyName, int value), (hmd, propertyName, value))
OVRFUNC(0,  float,            ovrHmd_GetFloat,                (ovrHmd hmd, const char* propertyName, float defaultVal), (hmd, propertyName, defaultVal))
OVRFUNC(0,  ovrBool,          ovrHmd_SetFloat,                (ovrHmd hmd, const char* propertyName, float value), (hmd, propertyName, value))
OVRFUNC(0,  unsigned int,     ovrHmd_GetFloatArray,           (ovrHmd hmd, const char* propertyName, float values[], unsigned int arraySize), (hmd, propertyName, values, arraySize))
OVRFUNC(0,  ovrBool,          ovrHmd_SetFloatArray,           (ovrHmd hmd, const char* propertyName, float values[], unsigned int arraySize), (hmd, propertyName, values, arraySize))
OVRFUNC(0,  const char*,      ovrHmd_GetString,               (ovrHmd hmd, const char* propertyName, const char* defaultVal), (hmd, propertyName, defaultVal))
OVRFUNC(0,  ovrBool,          ovrHmd_SetString,               (ovrHmd hmddesc, const char* propertyName, const char* value), (hmddesc, propertyName, value))
OVRFUNC(0,  ovrBool,          ovrHmd_GetLatencyTest2DrawColor, (ovrHmd hmddesc, unsigned char rgbColorOut[3]), (hmddesc, rgbColorOut))

// These functions are undocumented and not present in the header, but are exported by OVR_CAPI.cpp
OVRFUNCV(0,                   ovrHmd_RegisterPostDistortionCallback, (ovrHmd hmddesc, void* callback), (hmddesc, callback))
OVRFUNCV(0,                   ovrHmd_AddDistortionTimeMeasurement, (ovrHmd hmddesc, double distortionTimeSeconds), (hmddesc, distortionTimeSeconds))
OVRFUNCV(0,                   ovrHmd_GetLatencyTestDrawColor, (ovrHmd hmddesc, unsigned char rgbColorOut[3]), (hmddesc, rgbColorOut))

#undef OVRFUNC
#undef OVRFUNCV
