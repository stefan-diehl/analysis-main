#ifndef nathan_archive_cxx
#define nathan_archive_cxx 

// c++ includes
#include <iostream>
#include <vector>
using namespace std;

// root includes 
#include "TROOT.h"

// this includes 
#include "NathanArchive.h"

// my includes
#include "h22Event.h"

NathanEIDWrapper::NathanEIDWrapper()
{
  // Nothing to do 
  runno = 0;
  vz_strict = 0;
  cc_fid_strict = 0;   cc_phi_strict = 0;  cc_theta_strict = 0;
  dc_r1_strict = 0;   dc_r3_strict = 0;
  ec_edep_strict = 0;   ec_uvw_strict = 0;  ec_sf_strict = 0;

}

NathanEIDWrapper::~NathanEIDWrapper()
{
  // Nothing to destroy
}

void NathanEIDWrapper::set_info(int r, bool mc)
{
  GSIM = mc; runno = r;
}

int NathanEIDWrapper::get_electron(h22Event event)
{
  // Please note opposite GSIM convention in this function. In Nathan's code bool GSIM = 0 means the file is GSIM 
  int e_index = eID(event.gpart, event.q, event.p, event.cc_sect, event.sc_sect, event.ec_sect, event.dc_sect, event.cx, event.cy,
		    event.cz, event.tl1_x, event.tl1_y, event.tl3_x, event.tl3_y, event.tl3_z, event.tl3_cx, event.tl3_cy, event.tl3_cz,
		    vz_strict, event.vz, event.vy, event.vx, ec_sf_strict, !GSIM, event.etot, ec_edep_strict, event.ec_ei, event.ech_x, event.ech_y, event.ech_z, cc_theta_strict,
		    event.cc_segm, ec_uvw_strict, dc_r1_strict, dc_r3_strict, cc_phi_strict, event.sc_pd, cc_fid_strict);
  return e_index;
}

Bool_t e_zvertex_pass(int strict, int ExpOrSim, Float_t corrvz)
{
if(strict == 9) return 1;

 if (ExpOrSim == 0) return 1;

int strictIndex = strict + 2;
const int Nstrictnesses = 5;
float leftcut[Nstrictnesses] = {-27.7302 - 0.4, -27.7302 - 0.2, -27.7302, -27.7302 + 0.2, -27.7302 + 0.4};
float rightcut[Nstrictnesses] = {-22.6864 + 0.4, -22.6864 + 0.2, -22.6864, -22.6864 - 0.2, -22.6864 - 0.4};

if(ExpOrSim == 1 && corrvz > leftcut[strictIndex] && corrvz < rightcut[strictIndex]) return 1;
if(ExpOrSim == 0 && corrvz > -29.0 && corrvz < -21.0) return 1; // bad agreement between MC and data, so not using the same cuts

return 0;
}

Bool_t e_ECsampling_pass(int strict, int ExpOrSim, int sector, Float_t etot, Float_t p)
{
 if(strict == 9) return 1;

int strictIndex = strict + 2;
const int Nstrictnesses = 5;

 // looser cut:  6 sigma on top, 4 sigma on bottom
// loose cut:   5.5 sigma on top, 3.5 sigma on bottom
// nominal cut: 5 sigma on top, 3 sigma on bottom
// tight cut:   4.5 sigma on top, 2.5 sigma on bottom
// tighter cut: 4 sigma on top, 2 sigma on bottom

// upper cut, y = a + b*x + c*x*x
// [exp/sim][strictness][sector]
float a[2][Nstrictnesses][6] = {{{0.420613,0.424858,0.424702,0.429074,0.427833,0.428191}, {0.404299,0.408212,0.407958,0.41203,0.41093,0.411107}, {0.387985,0.391566,0.391215,0.394985,0.394027,0.394024}, {0.371671,0.374919,0.374471,0.377941,0.377124,0.376941}, {0.355357,0.358273,0.357728,0.360896,0.360221,0.359858}}, {{0.482389,0.598243,0.503293,0.493301,0.488109,0.48344}, {0.464078,0.572589,0.483542,0.474487,0.468019,0.464093}, {0.445767,0.546936,0.463791,0.455673,0.447928,0.444746}, {0.427455,0.521282,0.44404,0.436859,0.427838,0.425399}, {0.409144,0.495628,0.424289,0.418045,0.407748,0.408474}}};
float b[2][Nstrictnesses][6] = {{{-0.0495682,-0.0568233,-0.0539308,-0.059438,-0.0569282,-0.0604589}, {-0.0446684,-0.0512794,-0.0484624,-0.0537194,-0.0513468,-0.0546141}, {-0.0397687,-0.0457355,-0.0429939,-0.0480007,-0.0457654,-0.0487694}, {-0.0348689,-0.0401916,-0.0375255,-0.042282,-0.0401839,-0.0429246}, {-0.0299691,-0.0346476,-0.0320571,-0.0365633,-0.0346025,-0.0370799}}, {{-0.023125,-0.0296968,-0.0240658,0.0155969,-0.0219532,-0.0119168}, {-0.0190486,-0.0237774,-0.0192748,0.0167098,-0.0174922,-0.00806353}, {-0.0149722,-0.017858,-0.0144838,0.0178227,-0.0130313,-0.00421029}, {-0.0108958,-0.0119386,-0.00969275,0.0189356,-0.00857032,-0.000357051}, {-0.00681948,-0.00601912,-0.00490176,0.0200485,-0.00410937,0.00121479}}};
float c[2][Nstrictnesses][6] = {{{0.00514962,0.00749162,0.00646194,0.0077146,0.00744141,0.00844627}, {0.0045421,0.00669672,0.00570553,0.00688928,0.00664832,0.00756436}, {0.00393459,0.00590182,0.00494909,0.00606393,0.00585522,0.00668245}, {0.00332707,0.00510691,0.00419267,0.00523861,0.00506211,0.00580053}, {0.00271954,0.004312,0.00343625,0.00441326,0.00426903,0.00491862}}, {{0.00404757,0.004256,0.00488331,-0.00314474,0.00393541,0.00139946}, {0.00354432,0.00352811,0.00424241,-0.00310631,0.00330154,0.000932067}, {0.00304107,0.00280023,0.00360153,-0.00306787,0.00266766,0.000464671}, {0.00253782,0.00207234,0.00296064,-0.00302943,0.00203378,-2.72665e-06}, {0.00203457,0.00134445,0.00231975,-0.00299099,0.00139991,-2.84297e-05}}};

// lower cut, y = d + e*x + f*x*x
// [exp/sim][strictness][sector]
float d[2][Nstrictnesses][6] = {{{0.0943313,0.0919313,0.0898338,0.0881846,0.0897731,0.0865283}, {0.110645,0.108578,0.106577,0.105229,0.106676,0.103611}, {0.12696,0.125224,0.123321,0.122274,0.123579,0.120695}, {0.143274,0.14187,0.140064,0.139318,0.140482,0.137778}, {0.159588,0.158517,0.156807,0.156363,0.157385,0.154861}}, {{0.116163,0.085166,0.108276,0.117021,0.0863021,0.0965012}, {0.134474,0.11082,0.128026,0.135835,0.106392,0.115848}, {0.152785,0.136474,0.147777,0.154649,0.126483,0.135195}, {0.171097,0.162128,0.167528,0.173463,0.146573,0.154542}, {0.189408,0.187781,0.187279,0.192277,0.166663,0.173889}}};
float e[2][Nstrictnesses][6] = {{{0.048427,0.0540548,0.0554377,0.054936,0.0547004,0.056436}, {0.0435273,0.0485109,0.0499693,0.0492173,0.049119,0.0505913}, {0.0386275,0.042967,0.0445008,0.0434986,0.0435376,0.0447465}, {0.0337277,0.0374231,0.0390324,0.0377799,0.0379561,0.0389018}, {0.028828,0.0318792,0.033564,0.0320612,0.0323747,0.033057}}, {{0.0584026,0.0886916,0.0717544,0.0378552,0.0672658,0.0651479}, {0.0543262,0.0827722,0.0669633,0.0367423,0.0628048,0.0612946}, {0.0502498,0.0768528,0.0621723,0.0356294,0.0583439,0.0574414}, {0.0461734,0.0709334,0.0573813,0.0345165,0.0538829,0.0535882}, {0.0420971,0.0650139,0.0525903,0.0334035,0.049422,0.0497349}}};
float f[2][Nstrictnesses][6] = {{{-0.0070008,-0.0084064,-0.00866651,-0.00879214,-0.00842053,-0.00919199}, {-0.00639328,-0.0076115,-0.00791009,-0.0079668,-0.00762743,-0.00831007}, {-0.00578576,-0.0068166,-0.00715367,-0.00714147,-0.00683433,-0.00742816}, {-0.00517824,-0.0060217,-0.00639724,-0.00631613,-0.00604123,-0.00654625}, {-0.00457072,-0.0052268,-0.00564082,-0.00549079,-0.00524814,-0.00566434}}, {{-0.00601745,-0.0103018,-0.00793447,-0.00237599,-0.00874211,-0.00794845}, {-0.0055142,-0.00957388,-0.00729358,-0.00241442,-0.00810823,-0.00748105}, {-0.00501095,-0.00884599,-0.00665269,-0.00245286,-0.00747436,-0.00701365}, {-0.00450769,-0.0081181,-0.0060118,-0.0024913,-0.00684048,-0.00654626}, {-0.00400444,-0.00739022,-0.00537091,-0.00252974,-0.00620661,-0.00607887}}};

if(etot/p > d[ExpOrSim][strictIndex][sector-1] + e[ExpOrSim][strictIndex][sector-1]*p + f[ExpOrSim][strictIndex][sector-1]*p*p && etot/p < a[ExpOrSim][strictIndex][sector-1] + b[ExpOrSim][strictIndex][sector-1]*p + c[ExpOrSim][strictIndex][sector-1]*p*p) return 1;

return 0;
}

// __________________________________________________________________________________________ //

Bool_t e_ECoutVin_pass(int strict, Float_t ec_ei)
{
if(strict == 9) return 1;

int strictIndex = strict + 2;
const int Nstrictnesses = 5;
float cutval[Nstrictnesses] = {0.045, 0.050, 0.055, 0.060, 0.065};

if(ec_ei > cutval[strictIndex]) return 1;

return 0;
}

// __________________________________________________________________________________________ //

Bool_t e_ECgeometric_pass(int strict, Float_t x, Float_t y, Float_t z)
{
if(strict == 9) return 1;

int strictIndex = strict + 2;
const int Nstrictnesses = 5;
float uMin[Nstrictnesses] = {58, 64, 70, 76, 82}; // loosest, loose, nominal, tight, tightest
float uMax[Nstrictnesses] = {412, 406, 400, 394, 388};
float vMax[Nstrictnesses] = {374, 368, 362, 356, 350};
float wMax[Nstrictnesses] = {407, 401, 395, 389, 383};

Float_t u, v, w, xi, yi, zi; 
Float_t EC_the = 0.4363323;
Float_t EC_phi;
Float_t ylow   = -182.974;
Float_t yhi    = 189.956;
Float_t tgrho  = 1.95325;
Float_t sinrho = 0.8901256;
Float_t cosrho = 0.455715;
Float_t rot[3][3];

Float_t phi = 57.2957795*atan3(y, x); 

phi=phi+30.;
if (phi>=360.) phi=phi-360.;

EC_phi = (int)(phi/60.) * 1.0471975;

rot[0][0] = cos(EC_the)*cos(EC_phi);
rot[0][1] = -sin(EC_phi);
rot[0][2] = sin(EC_the)*cos(EC_phi);
rot[1][0] = cos(EC_the)*sin(EC_phi);
rot[1][1] = cos(EC_phi);
rot[1][2] = sin(EC_the)*sin(EC_phi);
rot[2][0] = -sin(EC_the);
rot[2][1] = 0.; 
rot[2][2] = cos(EC_the);

yi=x*rot[0][0]+y*rot[1][0]+z*rot[2][0];
xi=x*rot[0][1]+y*rot[1][1]+z*rot[2][1];
zi=x*rot[0][2]+y*rot[1][2]+z*rot[2][2];

zi=zi-510.32;

u = (yi-ylow)/sinrho;
v = (yhi-ylow)/tgrho-xi+(yhi-yi)/tgrho;
w = ((yhi-ylow)/tgrho+xi+(yhi-yi)/tgrho)/2./cosrho;

if(u >= uMin[strictIndex] && u <= uMax[strictIndex] && v <= vMax[strictIndex] && w <= wMax[strictIndex]) return 1;

return 0;
}

// __________________________________________________________________________________________ //

Bool_t e_CCthetaMatching_pass(int strict, int ExpOrSim, int sector, Float_t thetaCC, Int_t ccseg)
{
if(strict == 9) return 1;
 if (!ExpOrSim) return 1;

int strictIndex = strict + 2;
const int Nstrictnesses = 5;
float Nsigma[Nstrictnesses] = {5.0, 4.5, 4.0, 3.5, 3.0};

double CCseg_mu_e[2][6][18] = {{{9.83512,10.8576,12.3534,14.0989,16.0519,17.9409,19.9201,21.9689,24.0949,26.1043,28.2876,30.4488,32.8855,35.4499,38.0482,40.7007,42.8706,44.3719},{9.8311,10.7617,12.2906,14.0424,16.0356,17.9779,19.9756,21.9923,24.0451,26.0393,28.2977,30.4821,32.9376,35.5373,38.2148,40.8042,42.967,44.3765},{9.85194,10.9203,12.4324,14.1801,16.1147,18.0023,19.9695,22.023,24.1619,26.193,28.417,30.5605,33.0145,35.5573,38.1174,40.7452,42.9429,44.434},{9.83662,10.8673,12.3797,14.101,16.0623,18.0108,20.0157,22.0435,24.1044,26.0546,28.2541,30.4766,32.9183,35.5939,38.2295,40.8019,42.9187,44.4056},{9.84106,10.8594,12.4104,14.138,16.0514,17.9305,19.8963,21.9696,24.0971,26.1388,28.3623,30.5144,32.9263,35.4355,38.0802,40.7846,42.9397,44.4507},{9.83076,10.6723,12.3371,14.1462,16.0434,17.9909,19.9782,21.9891,24.0858,26.1182,28.3126,30.4825,32.9697,35.5284,38.1669,40.8796,42.9852,44.3872}},{{9.34037,10.7594,12.7647,14.736,16.6515,18.7595,20.8056,22.8826,24.8338,26.773,28.798,31.1131,33.6118,36.6606,39.3775,41.8706,43.7937,43.5108},{9.39088,10.7841,12.5898,14.4987,16.4176,18.5062,20.6564,22.7226,24.7938,26.8372,28.8277,31.009,33.685,36.3016,39.3323,41.7001,43.5399,43.361},{9.28666,10.6877,12.6832,14.5967,16.5731,18.6448,20.6933,22.8032,24.8087,26.7244,28.7212,30.912,33.8709,36.6297,39.3059,41.7144,43.6106,44.0706},{9.03196,10.7856,12.8366,14.7598,16.6495,18.6836,20.7819,22.9004,24.8551,26.7158,28.6873,30.9702,33.4061,36.4594,39.1286,41.7975,43.8097,43.9377},{9.96753,10.6624,13.427,14.113,16.7449,18.7448,20.8307,23.0411,25.1834,27.1859,29.5993,31.5893,34.1584,36.6028,39.3136,41.8122,43.8056,43.3776},{9.70209,10.8024,12.7174,14.7,16.5544,18.589,20.5457,22.7765,24.6167,26.7398,28.7212,31.0336,33.3699,36.3344,39.5463,42.0496,43.7081,43.539}}};
double CCseg_sigma_e[2][6][18] = {{{0.580766,0.906869,0.991186,1.04888,0.97877,0.971421,0.999088,1.01544,1.06258,1.04797,1.10965,1.14557,1.17488,1.24825,1.28958,1.31761,1.1265,0.781513},{0.581483,0.903044,0.985741,1.04766,1.00184,0.986884,0.988112,0.998394,1.03887,1.04899,1.14625,1.15233,1.21354,1.28391,1.26709,1.3108,1.11322,0.723599},{0.590405,0.910468,0.99884,1.04504,0.988364,0.975463,1.00485,1.02654,1.08591,1.05942,1.12799,1.16096,1.17887,1.23828,1.29489,1.35788,1.18078,0.781653},{0.585818,0.907593,0.996763,1.03009,1.00414,1.00494,0.993469,1.01078,1.03484,1.02931,1.13705,1.15768,1.21619,1.28737,1.26566,1.30526,1.09945,0.774484},{0.586356,0.917402,1.0065,1.03095,0.975623,0.967723,0.992303,1.03544,1.06634,1.05213,1.12664,1.15316,1.16242,1.25045,1.3042,1.34331,1.12554,0.774596},{0.564845,0.867598,1.05251,1.00669,1.00833,0.992138,0.987713,1.0045,1.05828,1.05632,1.12619,1.16169,1.1983,1.26686,1.28113,1.30279,1.0695,0.704273}},{{0.943102,1.07199,1.18821,1.12258,1.10298,1.12796,1.24707,1.13558,1.10508,1.2582,1.40941,1.56726,1.76608,1.67508,1.60719,1.54769,1.18419,1.93474},{0.989609,1.16341,1.25064,1.02821,1.06759,1.24962,1.21702,1.30632,1.34487,1.29768,1.49358,1.57154,1.75414,1.97463,1.65982,1.62968,1.32612,1.98807},{0.910813,1.11557,1.20224,1.14018,1.1002,1.1347,1.17672,1.27507,1.20364,1.20651,1.39737,1.59964,1.7779,1.81327,1.72549,1.68432,1.2769,1.64381},{0.793913,1.1091,1.19104,1.0315,0.982395,1.06427,1.14631,1.10191,1.1023,1.1032,1.23496,1.49888,1.65003,1.53575,1.61459,1.54627,1.20327,1.71327},{1.09804,1.42595,1.36764,1.94174,1.09743,1.14992,1.23973,1.25774,1.39793,1.35578,1.56965,1.6683,1.60559,1.71278,1.6854,1.7311,1.29476,1.99692},{1.01493,1.07025,1.25704,1.15883,1.06627,1.09337,1.17806,1.13053,1.15602,1.24616,1.36269,1.45487,1.6465,1.9598,1.73498,1.66839,1.5,2.04473}}};

if(thetaCC > CCseg_mu_e[ExpOrSim][sector-1][ccseg-1] - Nsigma[strictIndex]*CCseg_sigma_e[ExpOrSim][sector-1][ccseg-1] && thetaCC < CCseg_mu_e[ExpOrSim][sector-1][ccseg-1] + Nsigma[strictIndex]*CCseg_sigma_e[ExpOrSim][sector-1][ccseg-1]) return 1;

return 0;
}

// __________________________________________________________________________________________ //

Bool_t e_CCfiducial_pass(int strict, Float_t thetaCC, Float_t relphi)
{
if(strict == 9) return 1;

if(strict == -2 && thetaCC > 45.0 - 35.0*sqrt(1.0 - pow(relphi,2.0)/375.0)) return 1;
if(strict == -1 && thetaCC > 45.5 - 35.0*sqrt(1.0 - pow(relphi,2.0)/362.5)) return 1;
if(strict == 0 && thetaCC > 46.0 - 35.0*sqrt(1.0 - pow(relphi,2.0)/350.0)) return 1; // nominal cut
if(strict == 1 && thetaCC > 46.5 - 35.0*sqrt(1.0 - pow(relphi,2.0)/337.5)) return 1;
if(strict == 2 && thetaCC > 47.0 - 35.0*sqrt(1.0 - pow(relphi,2.0)/325.0)) return 1;

return 0;
}

// __________________________________________________________________________________________ //

Bool_t e_R1fid_pass(int strict, Int_t sector, Float_t tl1_x, Float_t tl1_y)
{
if(strict == 9) return 1;

int strictIndex = strict + 2;

const int Nstrictnesses = 5;
float height[Nstrictnesses] = {20, 21, 22, 23, 24};
float angle[Nstrictnesses] = {62, 61, 60, 59, 58};

float slope[Nstrictnesses];

for(int st = 0; st < Nstrictnesses; st++)
{
slope[st] = 1.0/tan(0.5*(3.141592653/180.0)*angle[st]);
}

int sm1 = sector-1;
float rotx = tl1_y*sin(sm1*(3.14159/180.0)*60.0)+tl1_x*cos(sm1*(3.14159/180.0)*60.0);
float roty = tl1_y*cos(sm1*(3.14159/180.0)*60.0)-tl1_x*sin(sm1*(3.14159/180.0)*60.0);

if(rotx > height[strictIndex] - slope[strictIndex]*roty && rotx > height[strictIndex] + slope[strictIndex]*roty) return 1;

return 0;
}

// __________________________________________________________________________________________ //

Bool_t e_R3fid_pass(int strict, Int_t sector, Float_t tl3_x, Float_t tl3_y)
{
if(strict == 9) return 1;

int strictIndex = strict + 2;

const int Nstrictnesses = 5;
float height[Nstrictnesses] = {73, 78, 83, 88, 93};
float angle[Nstrictnesses] = {51, 50, 49, 48, 47};

float slope[Nstrictnesses];

for(int st = 0; st < Nstrictnesses; st++)
{
slope[st] = 1.0/tan(0.5*(3.141592653/180.0)*angle[st]);
}

if(tl3_x > height[strictIndex] - slope[strictIndex]*tl3_y && tl3_x > height[strictIndex] + slope[strictIndex]*tl3_y) return 1;

return 0;
}

// __________________________________________________________________________________________ //

Bool_t e_CCphiMatching_pass(int strict, Int_t CCphiM)
{
if(strict == 9) return 1;

if(CCphiM >= -1 && CCphiM < 2) return 1;

return 0;
}

	// dc_xsc etc. are renamed to tl3_x etc. in the h10 --> h22 conversion
Float_t get_thetaCC(Float_t dc_xsc, Float_t dc_ysc, Float_t dc_zsc, Float_t dc_cxsc, Float_t dc_cysc, Float_t dc_czsc)
{
Float_t cc_pln[3] = {-0.0007840784063, 0.0, -0.001681461571};
Float_t d = 1.0;

Float_t dir[3] = {dc_cxsc, dc_cysc, dc_czsc};

// Need the point in cm for this to work
Float_t cm_factor = 1.0; // this is 10 in the original CLAS_Event.cc script
Float_t P1[3] = {dc_xsc/cm_factor, dc_ysc/cm_factor, dc_zsc/cm_factor};
Float_t t = (cc_pln[0]*P1[0] + cc_pln[1]*P1[1] + cc_pln[2]*P1[2] + d)/(cc_pln[0]*dir[0] + cc_pln[1]*dir[1] + cc_pln[2]*dir[2]);

Float_t CCx = (P1[0] + dir[0]*t)*10;
Float_t CCy = (P1[1] + dir[1]*t)*10;
Float_t CCz = (P1[2] + dir[2]*t)*10;

Float_t thetaCC = atan2(sqrt(CCx*CCx + CCy*CCy), CCz);
return thetaCC;
}

Float_t shift180180to30330(Float_t phi)
{
if(phi < -30) phi = phi + 360;
return phi;
}

Float_t get_rel_phi(Float_t phi)
{
if(phi < -150) return phi + 180;
if(phi >= -150 && phi < -90) return phi + 120;
if(phi >= -90 && phi < -30) return phi + 60;
if(phi >= -30 && phi < 30) return phi;
if(phi >= 30 && phi < 90) return phi - 60;
if(phi >= 90 && phi < 150) return phi - 120;
if(phi >= 150 && phi < 210) return phi - 180;
if(phi >= 210 && phi < 270) return phi - 240;
if(phi >= 270 && phi < 330) return phi - 300;
if(phi >= 330) return phi - 360;
return 0;
}

Float_t get_rel_phi2(Float_t phi, Int_t sector) // phi should be in the -30 to 330 range convention
{
if(sector == 2) phi = phi - 60;
if(sector == 3) phi = phi - 120;
if(sector == 4) phi = phi - 180;
if(sector == 5) phi = phi - 240;
if(sector == 6) phi = phi - 300;
return phi;
}

Int_t ccphimatching(Int_t cc_segm, Float_t phi){
  Int_t ccpmt    = cc_segm/1000 - 1; // -1 left pmt; +1 right pmt; 0 both
  Float_t relphi = get_rel_phi(phi);
  
  if(relphi > 0 && ccpmt > 0) return 1;
  if(relphi > 0 && ccpmt < 0) return 2;
  if(relphi < 0 && ccpmt < 0) return -1;
  if(relphi < 0 && ccpmt > 0) return -2;
  if(ccpmt == 0 || relphi == 0) return 0;
  return 0;
}

Float_t atan3(Float_t y, Float_t x) // same as atan2 but returns a value between 0 and 2pi instead of between -pi and pi
{
	if(atan2(y, x) >= 0)
	{
	return atan2(y, x);
	}
	else if(atan2(y, x) < 0)
	{
	return atan2(y, x) + 2*3.14159265359;
	}
	else
	{
	return -123;
	}
}

int getBinN(float val, int arrayLength, float LimitsArray[])
{
if(val < LimitsArray[0] || val >= LimitsArray[arrayLength-1]) return -123;

int binN = 0;
while(val >= LimitsArray[binN+1]) binN++;

return binN;
}

int getBinN2(float val, int Nbins, float min, float max)
{
if(val < min || val >= max) return -123;

int binN = static_cast<int>((val - min)/((max - min)/Nbins));
return binN;
}

int getBinN3(float val, int arrayLength, float lowEdge[], float highEdge[]) // for bins with gaps between them (like for bin centering corrections)
{
for(int k = 0; k < arrayLength; k++)
{
if(val >= lowEdge[k] && val < highEdge[k]) return k;
}

return -123;
}

int eID(Int_t gpart, Int_t q[], Float_t p[], UChar_t cc_sect[], UChar_t sc_sect[], UChar_t ec_sect[], UChar_t dc_sect[], Float_t cx[], Float_t cy[], Float_t cz[], Float_t tl1_x[], Float_t tl1_y[], Float_t tl3_x[], Float_t tl3_y[], Float_t tl3_z[], Float_t tl3_cx[], Float_t tl3_cy[], Float_t tl3_cz[], int e_zvertex_strict, Float_t vz[], Float_t vy[], Float_t vx[], int e_ECsampling_strict, int ExpOrSim, Float_t etot[], int e_ECoutVin_strict, Float_t ec_ei[], Float_t ech_x[], Float_t ech_y[], Float_t ech_z[], int e_CCthetaMatching_strict, UShort_t cc_segm[], int e_ECgeometric_strict, int e_R1fid_strict, int e_R3fid_strict, int e_CCphiMatching_strict, UChar_t sc_pd[], int e_CCfiducial_strict)
{
vector<int> Veindex;
for(int k = 0; k < gpart; k++) // loop over particles
{
if(q[k] == -1 && cc_sect[k] != 0 && sc_sect[k] != 0 && ec_sect[k] != 0 && dc_sect[k] != 0 && goodORbadSCpaddle(dc_sect[k], sc_pd[k]) == 1)
{
Float_t phi = atan3(cy[k],cx[k])*57.2957795;
Float_t relphi = get_rel_phi2(shift180180to30330(atan2(cy[k],cx[k])*57.2957795), dc_sect[k]);
Float_t thetaCC = 57.2957795*get_thetaCC(tl3_x[k], tl3_y[k], tl3_z[k], tl3_cx[k], tl3_cy[k], tl3_cz[k]);

// check if particle passes eID cuts:
Bool_t zvertex_pass, ECsampling_pass, ECoutVin_pass, ECgeometric_pass, CCthetaMatching_pass, R1fid_pass, R3fid_pass, CCphiMatching_pass, CCfiducial_pass;
zvertex_pass = ECsampling_pass = ECoutVin_pass = ECgeometric_pass = CCthetaMatching_pass = R1fid_pass = R3fid_pass = CCphiMatching_pass = CCfiducial_pass = 0;

zvertex_pass = e_zvertex_pass(e_zvertex_strict, ExpOrSim, getCorrZ(ExpOrSim, vx[k], vy[k], vz[k], p[k]*cx[k], p[k]*cy[k], p[k]*cz[k], dc_sect[k]));
if(zvertex_pass) ECsampling_pass = e_ECsampling_pass(e_ECsampling_strict, ExpOrSim, dc_sect[k], etot[k], p[k]);
if(zvertex_pass && ECsampling_pass) ECoutVin_pass = e_ECoutVin_pass(e_ECoutVin_strict, ec_ei[k]);
if(zvertex_pass && ECsampling_pass && ECoutVin_pass) ECgeometric_pass = e_ECgeometric_pass(e_ECgeometric_strict, ech_x[k], ech_y[k], ech_z[k]);
if(zvertex_pass && ECsampling_pass && ECoutVin_pass && ECgeometric_pass) CCthetaMatching_pass = e_CCthetaMatching_pass(e_CCthetaMatching_strict, ExpOrSim, dc_sect[k], thetaCC, (cc_segm[k]%1000)/10);
if(zvertex_pass && ECsampling_pass && ECoutVin_pass && ECgeometric_pass && CCthetaMatching_pass) R1fid_pass = e_R1fid_pass(e_R1fid_strict, dc_sect[k], tl1_x[k], tl1_y[k]);
if(zvertex_pass && ECsampling_pass && ECoutVin_pass && ECgeometric_pass && CCthetaMatching_pass && R1fid_pass) R3fid_pass = e_R3fid_pass(e_R3fid_strict, dc_sect[k], tl3_x[k], tl3_y[k]);
if(zvertex_pass && ECsampling_pass && ECoutVin_pass && ECgeometric_pass && CCthetaMatching_pass && R1fid_pass && R3fid_pass) CCphiMatching_pass = e_CCphiMatching_pass(e_CCphiMatching_strict, ccphimatching(cc_segm[k], phi));
if(zvertex_pass && ECsampling_pass && ECoutVin_pass && ECgeometric_pass && CCthetaMatching_pass && R1fid_pass && R3fid_pass && CCphiMatching_pass) CCfiducial_pass = e_CCfiducial_pass(e_CCfiducial_strict, thetaCC, relphi);

if(zvertex_pass && ECsampling_pass && ECoutVin_pass && ECgeometric_pass && CCthetaMatching_pass && R1fid_pass && R3fid_pass && CCphiMatching_pass && CCfiducial_pass) Veindex.push_back(k);

}
} // end of loop over gpart

// %%%%% find highest momentum electron %%%%%
int electron_index = -123;
if(Veindex.size() > 0) electron_index = Veindex[0];
if(Veindex.size() > 1)
{
for(unsigned int v = 1; v < Veindex.size(); v++)
{
if(p[Veindex[v]] > p[electron_index]) electron_index = Veindex[v];
}
}

return electron_index;
}

float getCorrZ(int ExpOrSim, float vx, float vy, float vz, float px, float py, float pz, int s)
{
  s--;
  float s0, sp, sv;

  float n[3][6];

  for(int abc = 0; abc < 3; abc++) // initialize to zero
    {
      for(int def = 0; def < 6; def++)
	{
	  n[abc][def] = 0.0;
	}
    }

  n[0][0] = 1.0;
  n[1][0] = 0.0;

  n[0][1] = 0.5;
  n[1][1] = 0.866025388;

  n[0][2] = -0.5;
  n[1][2] = 0.866025388;

  n[0][3] = -1.0;
  n[1][3] = 0.0;

  n[0][4] = -0.5;
  n[1][4] = -0.866025388;

  n[0][5] = 0.5;
  n[1][5] = -0.866025388;

  float x0, y0, z0; // beam position (cm)
  if(ExpOrSim == 1)
    {
      x0 = 0.15;
      y0 = -0.25;
      z0 = 0.0;
    }
  if(ExpOrSim == 0)
    {
      x0 = 0.0;
      y0 = 0.0;
      z0 = 0.0;
    }

  float A;

  s0 = x0*n[0][s] + y0*n[1][s] + z0*n[2][s];
  sp = px*n[0][s] + py*n[1][s] + pz*n[2][s];
  sv = vx*n[0][s] + vy*n[1][s] + vz*n[2][s];

  float cvz;

  if(fabs(sp) > 0.0000000001)
    {
      A = (s0-sv)/sp;
      cvz = vz + A*pz;
    }
  else
    {
      cvz = vz;
    }

  return cvz;
}

float e_sctimeCorr(int ExpOrSim, float sctime, int sector, int paddle, int runno)
{
  if(ExpOrSim == 0) return sctime;

  if(sector == 2 && paddle == 16 && runno >= 37776 && runno <= 38548) return sctime + 0.5;
  if(sector == 2 && paddle == 16 && runno >= 38549) return sctime + 0.9;

  if(sector == 3 && paddle == 11 && runno >= 37777) return sctime - 2.3;

  if(sector == 4 && paddle == 5 && runno >= 38548) return sctime + 2.0;

  if(sector == 5 && paddle == 3 && runno >= 37673 && runno <= 37854) return sctime + 31.0;
  if(sector == 5 && paddle == 3 && runno >= 37855) return sctime - 0.25;

  if(sector == 5 && paddle == 18 && runno >= 38050 && runno <= 38548) return sctime + 1.1;

  if(sector == 5 && paddle == 20 && runno >= 37777) return sctime - 0.5;

  if(sector == 6 && paddle == 18 && runno >= 38050 && runno <= 38548) return sctime - 1.6;

  // note: electrons very rarely hit paddle 2, so the below values were copied from the hadron correction function further down (having these vs not having these makes very little difference)
  if(sector == 3 && paddle == 2 && runno >= 0) return sctime - 15.45;

  if(sector == 4 && paddle == 2 && runno <= 37853) return sctime - 1.9;

  if(sector == 5 && paddle == 2 && runno <= 38240) return sctime - 17.9;
  if(sector == 5 && paddle == 2 && runno >= 38241) return sctime - 19.15;

  return sctime;
}

// ____________________________________ //

float h_sctimeCorr(int ExpOrSim, float sctime, int sector, int paddle, int runno) // h for hadron
{
  if(ExpOrSim == 0) return sctime;

  // calibrated using negative tracks: (low paddle numbers)
  if(sector == 6 && paddle == 1 && runno >= 0) return sctime + 18.25;

  if(sector == 3 && paddle == 2 && runno >= 0) return sctime - 15.45;

  if(sector == 4 && paddle == 2 && runno <= 37853) return sctime - 1.9;

  if(sector == 5 && paddle == 2 && runno <= 38240) return sctime - 17.9;
  if(sector == 5 && paddle == 2 && runno >= 38241) return sctime - 19.15;

  if(sector == 5 && paddle == 3 && runno >= 37673 && runno <= 37854) return sctime + 31.0;
  if(sector == 5 && paddle == 3 && runno >= 37855) return sctime - 0.25;

  // calibrated using positive tracks:
  if(sector == 1 && paddle == 24 && runno >= 37749) return sctime + 1.13334;

  if(sector == 2 && paddle == 16 && runno >= 37776 && runno <= 38548) return sctime + 0.565033;
  if(sector == 2 && paddle == 16 && runno >= 38549) return sctime + 1.04168;

  if(sector == 2 && paddle == 38 && runno >= 38535) return sctime - 1.89592;

  if(sector == 3 && paddle == 11 && runno >= 37777) return sctime - 2.26126;

  if(sector == 3 && paddle == 24 && runno >= 37855 && runno <= 38546) return sctime - 1.78266;

  if(sector == 3 && paddle == 25 && runno >= 37743 && runno <= 38266) return sctime + 2.44804;

  if(sector == 3 && paddle == 27 && runno >= 37854 && runno <= 38546) return sctime - 1.85815;

  if(sector == 3 && paddle == 28 && runno >= 37854) return sctime + 1.21704;

  if(sector == 4 && paddle == 5 && runno >= 38549) return sctime + 1.91688;

  if(sector == 4 && paddle == 19 && runno >= 37854) return sctime - 0.365798;

  if(sector == 4 && paddle == 34 && runno >= 37854) return sctime - 2.33721;

  if(sector == 4 && paddle == 42 && runno >= 37750) return sctime - 1.4118;

  if(sector == 4 && paddle == 45 && runno >= 38551) return sctime - 3.36406;

  if(sector == 5 && paddle == 18 && runno >= 37854 && runno <= 38545) return sctime + 1.24884;

  if(sector == 5 && paddle == 20 && runno >= 37809) return sctime - 0.468722;

  if(sector == 5 && paddle == 34 && runno <= 37853) return sctime - 1.0;
  if(sector == 5 && paddle == 34 && runno >= 37854) return sctime + 6.0;

  if(sector == 5 && paddle == 36 && runno >= 37748) return sctime + 1.07962;

  if(sector == 6 && paddle == 18 && runno >= 37854 && runno <= 38545) return sctime - 1.69106;

  if(sector == 6 && paddle == 42 && runno >= 37854 && runno <= 38545) return sctime - 6.0;

  return sctime;
}

// ____________________________________ //

bool goodORbadSCpaddle(int sector, int paddle)
{
  if(sector == 3 && paddle == 2) return 0;
  if(sector == 5 && paddle == 3) return 0;
  if(sector == 2 && paddle == 16) return 0;
  if(sector == 2 && paddle == 40) return 0;
  if(sector == 3 && paddle == 40) return 0;
  if(sector == 5 && paddle == 40) return 0;
  if(sector == 6 && paddle == 40) return 0;
  if(sector == 1 && paddle == 41) return 0;
  if(sector == 2 && paddle == 41) return 0;
  if(sector == 3 && paddle == 41) return 0;
  if(sector == 5 && paddle == 41) return 0;
  if(sector == 1 && paddle == 42) return 0;
  if(sector == 2 && paddle == 42) return 0;
  if(sector == 3 && paddle == 42) return 0;
  if(sector == 5 && paddle == 42) return 0;
  if(sector == 6 && paddle == 42) return 0;
  if(sector == 2 && paddle == 43) return 0;
  if(sector == 3 && paddle == 43) return 0;
  if(sector == 4 && paddle == 43) return 0;
  if(sector == 5 && paddle == 43) return 0;
  if(sector == 1 && paddle == 44) return 0;
  if(sector == 3 && paddle == 44) return 0;
  if(sector == 5 && paddle == 44) return 0;
  if(sector == 6 && paddle == 44) return 0;
  if(sector == 1 && paddle == 45) return 0;
  if(sector == 2 && paddle == 45) return 0;
  if(sector == 3 && paddle == 45) return 0;
  if(sector == 6 && paddle == 45) return 0;
  if(sector == 1 && paddle == 46) return 0;
  if(sector == 2 && paddle == 46) return 0;
  if(sector == 3 && paddle == 46) return 0;
  if(sector == 4 && paddle == 46) return 0;
  if(sector == 5 && paddle == 46) return 0;
  if(sector == 1 && paddle == 47) return 0;
  if(sector == 5 && paddle == 47) return 0;

  return 1;
}

#endif
