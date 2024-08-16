{
    // geometry
    Float_t dimX = 100;
    Float_t dimY = 50;

    // Define vectors for entry and exit points
    std::vector<double> entryPoint = {25., 49., 0.5};
    std::vector<double> exitPoint = {25., 1., 0.5};

    // Create a TF1 for neff
    TF1 *neffFunc = new TF1("neff", "((x >= 48 && x <= 49) ? 20000 : 10)", 0, 50);

    // TCanvas nf;
    // neffFunc->Draw();
    // TF1 *neffFunc = new TF1("neffFunc", "[0]+x[0]*0", 0, 1000);
    // neffFunc->SetParameter(0, 0.1);
    // Initialize the KPad object
    KPad det(dimX, dimY); // width and thickness : parameters of the detector

    // Define the detector parameters
    det.Neff = neffFunc;
    det.Voltage = 1000;
    //det.Temperature = 293;
    det.MTresh = 1.0001; // has to be above 1 to work, default value = -1
    det.BDTresh = 7; //breakdown treshold
    det.SetUpVolume(0.1); // nx = cellx/value ny = celly/st1
    det.SetUpElectrodes();

    // Set up the entry and exit points using the vector values
    det.SetDriftHisto(15e-9);
    TCanvas c1;
    det.SetEntryPoint(entryPoint[0], entryPoint[1], entryPoint[2]);
    det.SetExitPoint(exitPoint[0], exitPoint[1], exitPoint[2]);
    det.diff = 1;       // Diffusion simulation: 1 = true, 0 = false
    det.ShowMipIR(100); // Show the field

    // Draw charge plot
    TCanvas charge;
    det.MipIR(200); // calculate
    det.sum->DrawCopy("HIST");
    cout << det.sum->Integral(0,10) << endl;
    det.neg->DrawCopy("HISTSAME");
    det.pos->DrawCopy("HISTSAME");

    // Display entry and exit point information
    TPaveText *entryInfo = new TPaveText(0.6, 0.4, 0.9, 0.6, "NDC");
    entryInfo->AddText(Form("Entry Point: (%.1f, %.1f, %.1f)", entryPoint[0], entryPoint[1], entryPoint[2]));
    entryInfo->AddText(Form("Exit Point: (%.1f, %.1f, %.1f)", exitPoint[0], exitPoint[1], exitPoint[2]));
    entryInfo->AddText(Form("Voltage: %.1f", det.Voltage));
    entryInfo->AddText(Form("MTresh: %.5f", det.MTresh));
    entryInfo->SetFillColor(0);
    entryInfo->Draw();

    TH2F *tmp;
    TCanvas Show;
    Show.Divide(2, 2);
    Show.cd(1);
    // Draw electric potential [V]
    tmp = det.Draw("EP");
    tmp->SetTitle("Electric Potential [V]");
    tmp->Draw("COLZ");
    Show.cd(2);
    // Draw weighting potential
    tmp = det.Draw("WP");
    tmp->SetTitle("Weighting Potential");
    tmp->Draw("COLZ");
    Show.cd(3);
    tmp = det.Draw("EF");
    tmp->SetTitle("Absolute value of the electric field [V/um]");
    tmp->Draw("COLZ");
}