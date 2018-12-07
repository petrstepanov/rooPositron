/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   TwoGaussProvider.cpp
 * Author: petrstepanov
 * 
 * Created on December 6, 2018, 5:10 AM
 */

#include "TwoGaussProvider.h"
#include "RooFormulaVar.h"
#include "../../model/RootConstants.h"
#include "../pdfs/TwoGaussian.h"

TwoGaussProvider::TwoGaussProvider(RooRealVar* observable) : AbstractProvider(observable) {}
    
TwoGaussProvider::~TwoGaussProvider() {
}

RooAbsPdf* TwoGaussProvider::initPdf() {
	RooConstVar* fwhm2disp = RootConstants::getInstance()->fwhm2disp;
	RooConstVar* channelWidth = RootConstants::getInstance()->channelWidth;
	
	// 1st Gauss FWHM
	RooRealVar* g1FWHM = new RooRealVar("gauss1FWHM", "1st gauss FWHM", 0.3, 0.1, 0.5, "ns");
	RooFormulaVar* g1Dispersion = new RooFormulaVar("gauss1Dispersion", "@0*@1/@2", RooArgList(*g1FWHM, *fwhm2disp, *channelWidth));

	// 2nd gauss FWHM
	RooRealVar* g2FWHM = new RooRealVar("gauss2FWHM", "2nd gauss FWHM", 0.7, 0.4, 2.0, "ns");
	RooFormulaVar* g2Dispersion = new RooFormulaVar("gauss2Dispersion", "@0*@1/@2", RooArgList(*g2FWHM, *fwhm2disp, *channelWidth));

       	// Fraction of the 2nd gauss
	RooRealVar* g2FractionPct = new RooRealVar("gauss2Frac", "2nd gauss fraction", 1, 0, 10, "%");
	RooFormulaVar* g2Fraction = new RooFormulaVar("gauss2Frac", "@0/100", *g2FractionPct);

	// 2nd Gauss shift
//	RooRealVar** g2_shift = new RooRealVar*[iNumberOfFiles];
//	RooFormulaVar** zero_ch_relative_2 = new RooFormulaVar*[iNumberOfFiles];
//	for (unsigned i = 0; i < iNumberOfFiles; i++) {
//		g2_shift[i] = storage->getOrMakeNew(TString::Format("g2_shift_%d", i + 1), TString::Format("2nd_gauss_shift_%d", i + 1), 0, -10, 10, "ch");
//		zero_ch_relative_2[i] = new RooFormulaVar(TString::Format("zero_ch_relative_2_%d", i + 1), "@0+@1", RooArgList(*zero_ch[i], *g2_shift[i]));
//	}

	// Zero channel start values are assigned later (relative to the MIN_CHANNEL value)
	RooRealVar* gMean = new RooRealVar("gaussMean", "Resolution function mean", 300, "ch");
        return new TwoGaussian("twoGaussPDF", "Two gauss resolution function", *observable, *gMean, *g1Dispersion, *gMean, *g2Dispersion, *g2Fraction);	
}
