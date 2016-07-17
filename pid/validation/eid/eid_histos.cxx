////////////////////////////////////////
/*
 David Riser, University of Connecticut
 
 July 17, 2016
 
 template.cxx -> Your info here.
 
 */
////////////////////////////////////////

// c++ includes
#include <iostream>
#include <map>
using namespace std;

// my includes from h22 library
#include "CommonTools.h"
#include "Corrections.h"
#include "h22Event.h"
#include "h22Option.h"
#include "h22Reader.h"
#include "pars.h"
#include "ParticleFilter.h"

// root includes
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TLine.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TStopwatch.h"

int main(int argc, char * argv[])
{
    
    // Setup Options
    h22Options opts;
    opts.set(argc,argv);
    int GSIM        = opts.args["MC"].arg;
    long int nev    = opts.args["N"].arg;
    string eparfile = opts.args["EPARS"].args;
    string hparfile = opts.args["HPARS"].args;
    string outfile  = opts.args["OUT"].args;
    
    // Setup Reader and Output
    h22Reader fReader(GSIM);
    for (auto it=opts.ifiles.begin(); it<opts.ifiles.end(); it++) { fReader.AddFile(*it); }
    fReader.Init();
    TFile out(outfile.c_str(),"recreate",outfile.c_str(),1);
    
    // If you ask for more the loop will analyze garbage
    nev = smallest(nev, fReader.GetEntries());
    
    // Setting important constants
    int runno = fReader.runno();
    
    // Setting up PID and Corrections
    ParticleFilter filter(eparfile);
    filter.set_info(GSIM, runno);
    Corrections corr;
    
    // Histograms
    // [3][7] = [0:Raw, 1:Passed This, 2:Passed All][0:All Sectors, 1-6:Sector Number]
    string cut[3]  = {"raw","this","all"};
    
    // 1-D
    TH1F * h1_nphe[3][7];
    TH1F * h1_vz[3][7];
    TH1F * h1_ec_in[3][7];
    
    // 2-D
    TH2F * h2_ec_sampling[3][7];
    TH2F * h2_ec_uvw[3][7];
    TH2F * h2_dcr1[3][7];
    TH2F * h2_dcr3[3][7];
    TH2F * h2_cc[3][7];
    
    //! Initialize
    for (int c=0; c<3; c++)
        for (int s=0; s<7; s++)
        {
            h1_nphe[c][s]  = new TH1F(Form("h1_nphe_%s_%d",cut[c].c_str(),s),Form(" Number of Photoelectrons %s Sector %d",cut[c].c_str(),s),200,0,100);
            h1_vz[c][s]    = new TH1F(Form("h1_vz_%s_%d",cut[c].c_str(),s),Form(" Z-Vertex %s Sector %d",cut[c].c_str(),s),                  200,-35.0,-15.0);
            h1_ec_in[c][s] = new TH1F(Form("h1_ec_in_%s_%d",cut[c].c_str(),s),Form(" EC Inner Energy Dep. %s Sector %d",cut[c].c_str(),s),   200,0,2.0);
            
            h2_ec_sampling[c][s] = new TH2F(Form("h2_ec_sampling_%s_%d",cut[c].c_str(),s),Form(" EC Sampling %s Sector %d ",cut[c].c_str(),s),   100,0.5,4.0,100,0.05,0.5);
            h2_ec_uvw[c][s]      = new TH2F(Form("h2_ec_uvw_%s_%d",cut[c].c_str(),s),Form(" EC UVW %s Sector %d ",cut[c].c_str(),s),             100,-450,450,100,-450,450);
            h2_dcr1[c][s]        = new TH2F(Form("h2_dcr1_%s_%d",cut[c].c_str(),s),Form(" DC Region 1 Fid. %s Sector %d ",cut[c].c_str(),s),     100,15,60,100,-50,50);
            h2_dcr3[c][s]        = new TH2F(Form("h2_dcr3_%s_%d",cut[c].c_str(),s),Form(" DC Region 3 Fid. %s Sector %d ",cut[c].c_str(),s),     100,20,500,100,-500,500);
            h2_cc[c][s]          = new TH2F(Form("h2_cc_%s_%d",cut[c].c_str(),s),Form(" Cherenkov Counter Fid. %s Sector %d ",cut[c].c_str(),s), 100,-30,30,100,10,60);
            
        }
    
    // Keep track of time.
    TStopwatch timer;
    timer.Reset();
    timer.Start();
    
    for (int iev=0; iev<nev; iev++)
    {
        fReader.GetEntry(iev);
        h22Event event = fReader.GetEvent();
        
        // keeping track of run number and making sure particle filter knows about it as well.
        if ( runno != fReader.runno() ){ runno = fReader.runno(); filter.set_info(GSIM, runno); }
        
        int sector = event.dc_sect[0];
        if (event.q[0] < 0 && sector > 0)
        {
            //! raw
            h1_nphe[0][0]       ->Fill(event.nphe[0]);
            h1_vz[0][0]         ->Fill(corr.vz(event,0,runno,GSIM));
            h1_ec_in[0][0]      ->Fill(event.ec_ei[0]);
            
            h1_nphe[0][sector]  ->Fill(event.nphe[0]);
            h1_vz[0][sector]    ->Fill(corr.vz(event,0,runno,GSIM));
            h1_ec_in[0][sector] ->Fill(event.ec_ei[0]);
            
            h2_ec_sampling[0][0] ->Fill(event.p[0],event.etot[0]/event.p[0]);
            h2_ec_uvw[0][0]      ->Fill(event.ech_x[0],event.ech_y[0]);
            h2_dcr1[0][0]        ->Fill(event.rot_dc1x(0),event.rot_dc1y(0));
            h2_dcr3[0][0]        ->Fill(event.tl3_x[0], event.tl3_y[0]);
            h2_cc[0][0]          ->Fill(event.rphi(0), event.theta_cc(0));
            
            h2_ec_sampling[0][sector] ->Fill(event.p[0],event.etot[0]/event.p[0]);
            h2_ec_uvw[0][sector]      ->Fill(event.ech_x[0],event.ech_y[0]);
            h2_dcr1[0][sector]        ->Fill(event.rot_dc1x(0),event.rot_dc1y(0));
            h2_dcr3[0][sector]        ->Fill(event.tl3_x[0], event.tl3_y[0]);
            h2_cc[0][sector]          ->Fill(event.rphi(0), event.theta_cc(0));
        }
        
        // Broken up for clarity not speed. Doing This cut histograms now.
        if (event.q[0] < 0 && sector > 0)
        {
            // Get the eid results to load histos for this cut
            map<string,bool> results = filter.eid_map(event);
            
            if (results["CC_NPHE"]) { h1_nphe[1][0]  ->Fill(event.nphe[0]);               h1_nphe[1][sector]  ->Fill(event.nphe[0]); }
            if (results["VZ"])      { h1_vz[1][0]    ->Fill(corr.vz(event,0,runno,GSIM)); h1_vz[1][sector]    ->Fill(corr.vz(event,0,runno,GSIM)); }
            if (results["EC_EDEP"]) { h1_ec_in[1][0] ->Fill(event.ec_ei[0]);              h1_ec_in[1][sector] ->Fill(event.ec_ei[0]); }
            
            if (results["EC_SAMPLING"]) { h2_ec_sampling[1][0] ->Fill(event.p[0],event.etot[0]/event.p[0]); h2_ec_sampling[1][sector] ->Fill(event.p[0],event.etot[0]/event.p[0]); }
            if (results["EC_GEO"])      { h2_ec_uvw[1][0]      ->Fill(event.ech_x[0],event.ech_y[0]);       h2_ec_uvw[1][sector]      ->Fill(event.ech_x[0],event.ech_y[0]); }
            if (results["DCR1_FID"])    { h2_dcr1[1][0]        ->Fill(event.rot_dc1x(0),event.rot_dc1y(0)); h2_dcr1[1][sector]        ->Fill(event.rot_dc1x(0),event.rot_dc1y(0)); }
            if (results["DCR3_FID"])    { h2_dcr3[1][0]        ->Fill(event.tl3_x[0], event.tl3_y[0]);      h2_dcr3[1][sector]        ->Fill(event.tl3_x[0], event.tl3_y[0]); }
            if (results["CC_FID"])      { h2_cc[1][0]          ->Fill(event.rphi(0), event.theta_cc(0));    h2_cc[1][sector]          ->Fill(event.rphi(0), event.theta_cc(0)); }
            
        }
        
        
        if (filter.has_electron(event))
        {
            // all cuts histos
            h1_nphe[2][0]       ->Fill(event.nphe[0]);
            h1_vz[2][0]         ->Fill(corr.vz(event,0,runno,GSIM));
            h1_ec_in[2][0]      ->Fill(event.ec_ei[0]);
            
            h1_nphe[2][sector]  ->Fill(event.nphe[0]);
            h1_vz[2][sector]    ->Fill(corr.vz(event,0,runno,GSIM));
            h1_ec_in[2][sector] ->Fill(event.ec_ei[0]);
            
            h2_ec_sampling[2][0] ->Fill(event.p[0],event.etot[0]/event.p[0]);
            h2_ec_uvw[2][0]      ->Fill(event.ech_x[0],event.ech_y[0]);
            h2_dcr1[2][0]        ->Fill(event.rot_dc1x(0),event.rot_dc1y(0));
            h2_dcr3[2][0]        ->Fill(event.tl3_x[0], event.tl3_y[0]);
            h2_cc[2][0]          ->Fill(event.rphi(0), event.theta_cc(0));
            
            h2_ec_sampling[2][sector] ->Fill(event.p[0],event.etot[0]/event.p[0]);
            h2_ec_uvw[2][sector]      ->Fill(event.ech_x[0],event.ech_y[0]);
            h2_dcr1[2][sector]        ->Fill(event.rot_dc1x(0),event.rot_dc1y(0));
            h2_dcr3[2][sector]        ->Fill(event.tl3_x[0], event.tl3_y[0]);
            h2_cc[2][sector]          ->Fill(event.rphi(0), event.theta_cc(0));
        }
        
        // Tell the user
        if ( (int)iev%(int)opts.args["PRINT"].arg == 0) cout << "\r done " << iev << " of " << nev << flush;
    }
    
    double loop_time  = timer.RealTime();
    double event_rate = (double)nev/loop_time;
    
    cout << " Event loop finished in " << loop_time << " seconds w/ rate " << event_rate << " events/sec " << endl;
    
    out.Write();
    out.Close();
}