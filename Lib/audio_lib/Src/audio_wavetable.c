/**
 * @file    audio_wavetable.c
 * @author  Sebastián Del Moral
 * @brief   Library to handle wavetable updates.
 * @version 0.1
 * @date    2021-10-30
 * 
 * @copyright Copyright (c) 2021
 * 
 */

/* Includes ------------------------------------------------------------------*/
#include "audio_wavetable.h"

/* Private includes ----------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

#define WAVE_TABLE_SIZE         ( 256U )

#define WAVE_DEFAULT_FREQ       ( 440.0F )
#define WAVE_DEFAULT_WAVE       ( AUDIO_WAVE_SAW )

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

const float saw_wave[WAVE_TABLE_SIZE] = {-0.992188F,-0.984375F,-0.976562F,-0.968750F,-0.960938F,-0.953125F,-0.945312F,-0.937500F,-0.929688F,-0.921875F,-0.914062F,-0.906250F,-0.898438F,-0.890625F,-0.882812F,-0.875000F,-0.867188F,-0.859375F,-0.851562F,-0.843750F,-0.835938F,-0.828125F,-0.820312F,-0.812500F,-0.804688F,-0.796875F,-0.789062F,-0.781250F,-0.773438F,-0.765625F,-0.757812F,-0.750000F,-0.742188F,-0.734375F,-0.726562F,-0.718750F,-0.710938F,-0.703125F,-0.695312F,-0.687500F,-0.679688F,-0.671875F,-0.664062F,-0.656250F,-0.648438F,-0.640625F,-0.632812F,-0.625000F,-0.617188F,-0.609375F,-0.601562F,-0.593750F,-0.585938F,-0.578125F,-0.570312F,-0.562500F,-0.554688F,-0.546875F,-0.539062F,-0.531250F,-0.523438F,-0.515625F,-0.507812F,-0.500000F,-0.492188F,-0.484375F,-0.476562F,-0.468750F,-0.460938F,-0.453125F,-0.445312F,-0.437500F,-0.429688F,-0.421875F,-0.414062F,-0.406250F,-0.398438F,-0.390625F,-0.382812F,-0.375000F,-0.367188F,-0.359375F,-0.351562F,-0.343750F,-0.335938F,-0.328125F,-0.320312F,-0.312500F,-0.304688F,-0.296875F,-0.289062F,-0.281250F,-0.273438F,-0.265625F,-0.257812F,-0.250000F,-0.242188F,-0.234375F,-0.226562F,-0.218750F,-0.210938F,-0.203125F,-0.195312F,-0.187500F,-0.179688F,-0.171875F,-0.164062F,-0.156250F,-0.148438F,-0.140625F,-0.132812F,-0.125000F,-0.117188F,-0.109375F,-0.101562F,-0.093750F,-0.085938F,-0.078125F,-0.070312F,-0.062500F,-0.054688F,-0.046875F,-0.039062F,-0.031250F,-0.023438F,-0.015625F,-0.007812F,0.000000F,0.007812F,0.015625F,0.023438F,0.031250F,0.039062F,0.046875F,0.054688F,0.062500F,0.070312F,0.078125F,0.085938F,0.093750F,0.101562F,0.109375F,0.117188F,0.125000F,0.132812F,0.140625F,0.148438F,0.156250F,0.164062F,0.171875F,0.179688F,0.187500F,0.195312F,0.203125F,0.210938F,0.218750F,0.226562F,0.234375F,0.242188F,0.250000F,0.257812F,0.265625F,0.273438F,0.281250F,0.289062F,0.296875F,0.304688F,0.312500F,0.320312F,0.328125F,0.335938F,0.343750F,0.351562F,0.359375F,0.367188F,0.375000F,0.382812F,0.390625F,0.398438F,0.406250F,0.414062F,0.421875F,0.429688F,0.437500F,0.445312F,0.453125F,0.460938F,0.468750F,0.476562F,0.484375F,0.492188F,0.500000F,0.507812F,0.515625F,0.523438F,0.531250F,0.539062F,0.546875F,0.554688F,0.562500F,0.570312F,0.578125F,0.585938F,0.593750F,0.601562F,0.609375F,0.617188F,0.625000F,0.632812F,0.640625F,0.648438F,0.656250F,0.664062F,0.671875F,0.679688F,0.687500F,0.695312F,0.703125F,0.710938F,0.718750F,0.726562F,0.734375F,0.742188F,0.750000F,0.757812F,0.765625F,0.773438F,0.781250F,0.789062F,0.796875F,0.804688F,0.812500F,0.820312F,0.828125F,0.835938F,0.843750F,0.851562F,0.859375F,0.867188F,0.875000F,0.882812F,0.890625F,0.898438F,0.906250F,0.914062F,0.921875F,0.929688F,0.937500F,0.945312F,0.953125F,0.960938F,0.968750F,0.976562F,0.984375F,0.992188F,1.000000F,};
const float rsaw_wave[WAVE_TABLE_SIZE] = {0.992188F,0.984375F,0.976562F,0.968750F,0.960938F,0.953125F,0.945312F,0.937500F,0.929688F,0.921875F,0.914062F,0.906250F,0.898438F,0.890625F,0.882812F,0.875000F,0.867188F,0.859375F,0.851562F,0.843750F,0.835938F,0.828125F,0.820312F,0.812500F,0.804688F,0.796875F,0.789062F,0.781250F,0.773438F,0.765625F,0.757812F,0.750000F,0.742188F,0.734375F,0.726562F,0.718750F,0.710938F,0.703125F,0.695312F,0.687500F,0.679688F,0.671875F,0.664062F,0.656250F,0.648438F,0.640625F,0.632812F,0.625000F,0.617188F,0.609375F,0.601562F,0.593750F,0.585938F,0.578125F,0.570312F,0.562500F,0.554688F,0.546875F,0.539062F,0.531250F,0.523438F,0.515625F,0.507812F,0.500000F,0.492188F,0.484375F,0.476562F,0.468750F,0.460938F,0.453125F,0.445312F,0.437500F,0.429688F,0.421875F,0.414062F,0.406250F,0.398438F,0.390625F,0.382812F,0.375000F,0.367188F,0.359375F,0.351562F,0.343750F,0.335938F,0.328125F,0.320312F,0.312500F,0.304688F,0.296875F,0.289062F,0.281250F,0.273438F,0.265625F,0.257812F,0.250000F,0.242188F,0.234375F,0.226562F,0.218750F,0.210938F,0.203125F,0.195312F,0.187500F,0.179688F,0.171875F,0.164062F,0.156250F,0.148438F,0.140625F,0.132812F,0.125000F,0.117188F,0.109375F,0.101562F,0.093750F,0.085938F,0.078125F,0.070312F,0.062500F,0.054688F,0.046875F,0.039062F,0.031250F,0.023438F,0.015625F,0.007812F,0.000000F,-0.007812F,-0.015625F,-0.023438F,-0.031250F,-0.039062F,-0.046875F,-0.054688F,-0.062500F,-0.070312F,-0.078125F,-0.085938F,-0.093750F,-0.101562F,-0.109375F,-0.117188F,-0.125000F,-0.132812F,-0.140625F,-0.148438F,-0.156250F,-0.164062F,-0.171875F,-0.179688F,-0.187500F,-0.195312F,-0.203125F,-0.210938F,-0.218750F,-0.226562F,-0.234375F,-0.242188F,-0.250000F,-0.257812F,-0.265625F,-0.273438F,-0.281250F,-0.289062F,-0.296875F,-0.304688F,-0.312500F,-0.320312F,-0.328125F,-0.335938F,-0.343750F,-0.351562F,-0.359375F,-0.367188F,-0.375000F,-0.382812F,-0.390625F,-0.398438F,-0.406250F,-0.414062F,-0.421875F,-0.429688F,-0.437500F,-0.445312F,-0.453125F,-0.460938F,-0.468750F,-0.476562F,-0.484375F,-0.492188F,-0.500000F,-0.507812F,-0.515625F,-0.523438F,-0.531250F,-0.539062F,-0.546875F,-0.554688F,-0.562500F,-0.570312F,-0.578125F,-0.585938F,-0.593750F,-0.601562F,-0.609375F,-0.617188F,-0.625000F,-0.632812F,-0.640625F,-0.648438F,-0.656250F,-0.664062F,-0.671875F,-0.679688F,-0.687500F,-0.695312F,-0.703125F,-0.710938F,-0.718750F,-0.726562F,-0.734375F,-0.742188F,-0.750000F,-0.757812F,-0.765625F,-0.773438F,-0.781250F,-0.789062F,-0.796875F,-0.804688F,-0.812500F,-0.820312F,-0.828125F,-0.835938F,-0.843750F,-0.851562F,-0.859375F,-0.867188F,-0.875000F,-0.882812F,-0.890625F,-0.898438F,-0.906250F,-0.914062F,-0.921875F,-0.929688F,-0.937500F,-0.945312F,-0.953125F,-0.960938F,-0.968750F,-0.976562F,-0.984375F,-0.992188F,-1.000000F};
const float square_wave[WAVE_TABLE_SIZE] = {1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F,-1.000000F};
const float sin_wave[WAVE_TABLE_SIZE] = {0.000000F,0.024541F,0.049068F,0.073565F,0.098017F,0.122411F,0.146730F,0.170962F,0.195090F,0.219101F,0.242980F,0.266713F,0.290285F,0.313682F,0.336890F,0.359895F,0.382683F,0.405241F,0.427555F,0.449611F,0.471397F,0.492898F,0.514103F,0.534998F,0.555570F,0.575808F,0.595699F,0.615232F,0.634393F,0.653173F,0.671559F,0.689541F,0.707107F,0.724247F,0.740951F,0.757209F,0.773010F,0.788346F,0.803208F,0.817585F,0.831470F,0.844854F,0.857729F,0.870087F,0.881921F,0.893224F,0.903989F,0.914210F,0.923880F,0.932993F,0.941544F,0.949528F,0.956940F,0.963776F,0.970031F,0.975702F,0.980785F,0.985278F,0.989177F,0.992480F,0.995185F,0.997290F,0.998795F,0.999699F,1.000000F,0.999699F,0.998795F,0.997290F,0.995185F,0.992480F,0.989177F,0.985278F,0.980785F,0.975702F,0.970031F,0.963776F,0.956940F,0.949528F,0.941544F,0.932993F,0.923880F,0.914210F,0.903989F,0.893224F,0.881921F,0.870087F,0.857729F,0.844854F,0.831470F,0.817585F,0.803208F,0.788346F,0.773010F,0.757209F,0.740951F,0.724247F,0.707107F,0.689541F,0.671559F,0.653173F,0.634393F,0.615232F,0.595699F,0.575808F,0.555570F,0.534998F,0.514103F,0.492898F,0.471397F,0.449611F,0.427555F,0.405241F,0.382683F,0.359895F,0.336890F,0.313682F,0.290285F,0.266713F,0.242980F,0.219101F,0.195090F,0.170962F,0.146730F,0.122411F,0.098017F,0.073565F,0.049068F,0.024541F,0.000000F,-0.024541F,-0.049068F,-0.073565F,-0.098017F,-0.122411F,-0.146730F,-0.170962F,-0.195090F,-0.219101F,-0.242980F,-0.266713F,-0.290285F,-0.313682F,-0.336890F,-0.359895F,-0.382683F,-0.405241F,-0.427555F,-0.449611F,-0.471397F,-0.492898F,-0.514103F,-0.534998F,-0.555570F,-0.575808F,-0.595699F,-0.615232F,-0.634393F,-0.653173F,-0.671559F,-0.689541F,-0.707107F,-0.724247F,-0.740951F,-0.757209F,-0.773010F,-0.788346F,-0.803208F,-0.817585F,-0.831470F,-0.844854F,-0.857729F,-0.870087F,-0.881921F,-0.893224F,-0.903989F,-0.914210F,-0.923880F,-0.932993F,-0.941544F,-0.949528F,-0.956940F,-0.963776F,-0.970031F,-0.975702F,-0.980785F,-0.985278F,-0.989177F,-0.992480F,-0.995185F,-0.997290F,-0.998795F,-0.999699F,-1.000000F,-0.999699F,-0.998795F,-0.997290F,-0.995185F,-0.992480F,-0.989177F,-0.985278F,-0.980785F,-0.975702F,-0.970031F,-0.963776F,-0.956940F,-0.949528F,-0.941544F,-0.932993F,-0.923880F,-0.914210F,-0.903989F,-0.893224F,-0.881921F,-0.870087F,-0.857729F,-0.844854F,-0.831470F,-0.817585F,-0.803208F,-0.788346F,-0.773010F,-0.757209F,-0.740951F,-0.724247F,-0.707107F,-0.689541F,-0.671559F,-0.653173F,-0.634393F,-0.615232F,-0.595699F,-0.575808F,-0.555570F,-0.534998F,-0.514103F,-0.492898F,-0.471397F,-0.449611F,-0.427555F,-0.405241F,-0.382683F,-0.359895F,-0.336890F,-0.313682F,-0.290285F,-0.266713F,-0.242980F,-0.219101F,-0.195090F,-0.170962F,-0.146730F,-0.122411F,-0.098017F,-0.073565F,-0.049068F,-0.024541F};
const float tri_wave[WAVE_TABLE_SIZE] = {-0.984375F,-0.968750F,-0.953125F,-0.937500F,-0.921875F,-0.906250F,-0.890625F,-0.875000F,-0.859375F,-0.843750F,-0.828125F,-0.812500F,-0.796875F,-0.781250F,-0.765625F,-0.750000F,-0.734375F,-0.718750F,-0.703125F,-0.687500F,-0.671875F,-0.656250F,-0.640625F,-0.625000F,-0.609375F,-0.593750F,-0.578125F,-0.562500F,-0.546875F,-0.531250F,-0.515625F,-0.500000F,-0.484375F,-0.468750F,-0.453125F,-0.437500F,-0.421875F,-0.406250F,-0.390625F,-0.375000F,-0.359375F,-0.343750F,-0.328125F,-0.312500F,-0.296875F,-0.281250F,-0.265625F,-0.250000F,-0.234375F,-0.218750F,-0.203125F,-0.187500F,-0.171875F,-0.156250F,-0.140625F,-0.125000F,-0.109375F,-0.093750F,-0.078125F,-0.062500F,-0.046875F,-0.031250F,-0.015625F,0.000000F,0.015625F,0.031250F,0.046875F,0.062500F,0.078125F,0.093750F,0.109375F,0.125000F,0.140625F,0.156250F,0.171875F,0.187500F,0.203125F,0.218750F,0.234375F,0.250000F,0.265625F,0.281250F,0.296875F,0.312500F,0.328125F,0.343750F,0.359375F,0.375000F,0.390625F,0.406250F,0.421875F,0.437500F,0.453125F,0.468750F,0.484375F,0.500000F,0.515625F,0.531250F,0.546875F,0.562500F,0.578125F,0.593750F,0.609375F,0.625000F,0.640625F,0.656250F,0.671875F,0.687500F,0.703125F,0.718750F,0.734375F,0.750000F,0.765625F,0.781250F,0.796875F,0.812500F,0.828125F,0.843750F,0.859375F,0.875000F,0.890625F,0.906250F,0.921875F,0.937500F,0.953125F,0.968750F,0.984375F,1.000000F,0.984375F,0.968750F,0.953125F,0.937500F,0.921875F,0.906250F,0.890625F,0.875000F,0.859375F,0.843750F,0.828125F,0.812500F,0.796875F,0.781250F,0.765625F,0.750000F,0.734375F,0.718750F,0.703125F,0.687500F,0.671875F,0.656250F,0.640625F,0.625000F,0.609375F,0.593750F,0.578125F,0.562500F,0.546875F,0.531250F,0.515625F,0.500000F,0.484375F,0.468750F,0.453125F,0.437500F,0.421875F,0.406250F,0.390625F,0.375000F,0.359375F,0.343750F,0.328125F,0.312500F,0.296875F,0.281250F,0.265625F,0.250000F,0.234375F,0.218750F,0.203125F,0.187500F,0.171875F,0.156250F,0.140625F,0.125000F,0.109375F,0.093750F,0.078125F,0.062500F,0.046875F,0.031250F,0.015625F,0.000000F,-0.015625F,-0.031250F,-0.046875F,-0.062500F,-0.078125F,-0.093750F,-0.109375F,-0.125000F,-0.140625F,-0.156250F,-0.171875F,-0.187500F,-0.203125F,-0.218750F,-0.234375F,-0.250000F,-0.265625F,-0.281250F,-0.296875F,-0.312500F,-0.328125F,-0.343750F,-0.359375F,-0.375000F,-0.390625F,-0.406250F,-0.421875F,-0.437500F,-0.453125F,-0.468750F,-0.484375F,-0.500000F,-0.515625F,-0.531250F,-0.546875F,-0.562500F,-0.578125F,-0.593750F,-0.609375F,-0.625000F,-0.640625F,-0.656250F,-0.671875F,-0.687500F,-0.703125F,-0.718750F,-0.734375F,-0.750000F,-0.765625F,-0.781250F,-0.796875F,-0.812500F,-0.828125F,-0.843750F,-0.859375F,-0.875000F,-0.890625F,-0.906250F,-0.921875F,-0.937500F,-0.953125F,-0.968750F,-0.984375F,-1.000000F};

const float *wave_list[AUDIO_WAVE_NUM] = {
    saw_wave,
    rsaw_wave,
    square_wave,
    sin_wave,
    tri_wave,
};

/* Private function prototypes -----------------------------------------------*/
/* Private function definition -----------------------------------------------*/
/* Public function prototypes -----------------------------------------------*/

audio_ret_t AUDIO_WAVE_init_voice(AudioWaveTableVoice_t *pVoice, uint32_t u32SampleRate, float fMaxAmplitude )
{
    ERR_ASSERT(pVoice != NULL);

    pVoice->bActive = false;
    pVoice->fCurrentSample = 0.0F;
    pVoice->fFreq = WAVE_DEFAULT_FREQ;
    pVoice->fMaxAmplitude = fMaxAmplitude;
    pVoice->fAmplitude = 1.0F;
    pVoice->u32SampleRate = u32SampleRate;
    pVoice->u32WaveTableSize = WAVE_TABLE_SIZE;
    pVoice->eWaveId = WAVE_DEFAULT_WAVE;
    pVoice->pu32WaveTable = wave_list[pVoice->eWaveId];

    return AUDIO_OK;
}

audio_ret_t AUDIO_WAVE_change_wave(AudioWaveTableVoice_t *pVoice, audio_wave_id_t eWave)
{
    ERR_ASSERT(pVoice != NULL);
    ERR_ASSERT(eWave < AUDIO_WAVE_NUM);

    pVoice->eWaveId = eWave;
    pVoice->pu32WaveTable = wave_list[eWave];

    return AUDIO_OK;
}

audio_ret_t AUDIO_WAVE_update_freq(AudioWaveTableVoice_t *pVoice, float fFreq)
{
    ERR_ASSERT(pVoice != NULL);

    pVoice->fFreq = fFreq;

    return AUDIO_OK;
}

audio_ret_t AUDIO_WAVE_update_amp(AudioWaveTableVoice_t *pVoice, float fAmp)
{
    ERR_ASSERT(pVoice != NULL);
    ERR_ASSERT(fAmp >= 0.0F);
    ERR_ASSERT(fAmp <= 1.0F);

    pVoice->fAmplitude = fAmp * pVoice->fMaxAmplitude;

    return AUDIO_OK;
}

audio_ret_t AUDIO_WAVE_set_active(AudioWaveTableVoice_t *pVoice, bool bState)
{
    ERR_ASSERT(pVoice != NULL);

    pVoice->bActive = bState;

    if ( !bState )
    {
        pVoice->fCurrentSample = 0.0F;
    }

    return AUDIO_OK;
}

float AUDIO_WAVE_get_next_sample(AudioWaveTableVoice_t *pVoice)
{
    ERR_ASSERT(pVoice != NULL);

    float fOutData = 0.0F;

    if ( pVoice->bActive )
    {
        // Compute linear interpolation
        uint32_t u32IndexBelow = (uint32_t)pVoice->fCurrentSample;
        uint32_t u32IndexAbove = (u32IndexBelow + 1U) % pVoice->u32WaveTableSize;

        // Compute weigh for each sample
        float fFractionAbove = pVoice->fCurrentSample - (float)u32IndexBelow;
        float fFractionBelow = 1.0F - fFractionAbove;

        fOutData = pVoice->fAmplitude * ( fFractionAbove * pVoice->pu32WaveTable[u32IndexAbove] + fFractionBelow * pVoice->pu32WaveTable[u32IndexBelow] );

        // Compute next sample index from wavetable
        pVoice->fCurrentSample += (pVoice->u32WaveTableSize * pVoice->fFreq) / pVoice->u32SampleRate;
        while (pVoice->fCurrentSample >= (float)pVoice->u32WaveTableSize)
        {
            pVoice->fCurrentSample -= (float)pVoice->u32WaveTableSize;
        }
    }

    return fOutData;
}

/* EOF */