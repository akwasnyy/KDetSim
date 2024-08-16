{
     // geometry
    det.SStep = 1;
    det.nx = 100;
    det.ny = 50;
    det.nz = 1;
    Float_t dimX = 100;
    Float_t dimY = 50;
    
    // Define vectors for entry and exit points
    std::vector<double> entryPoint = {25., 299.9, 0.5};
    std::vector<double> exitPoint = {25., 1., 0.5};
    auto neff = [](double *x, double *params) {
        double y = x[1];
        if (y >= 48 && y <= 49) {
            return 20000.0;
        } else {
            return 10.0;
        }
    };

    // Create a TF1 using the lambda function
    TF1 *neffFunc = new TF1("neffFunc", neff, 0, dimY, 0);
    //TF1 *neff = new TF1("neff", "[0]+x[0]*0", 0, 1000);
    //neff->SetParameter(0, 1); // 3D histogram + for loop modification

    // Initialize the KPad object
    KPad det(50, 100); // width and thickness

    // Define the detector parameters
    det.Neff = neffFunc;
    det.Voltage = -200; // go to around 6450
    // det.MTresh = 1.0; // has to be above 1 to work, default value = -1

   

    // init geometry
    // det.EG = new TH3I("EG", "EG", det.nx, 0, dimX, det.ny, 0, dimY, det.nz, 0, 1);
    // det.EG->GetXaxis()->SetTitle("x [#mum]");
    // det.EG->GetYaxis()->SetTitle("y [#mum]");
    // det.EG->GetZaxis()->SetTitle("z [#mum]");

    det.SetUpVolume(1.); // of the pad
    det.SetUpElectrodes();

    // init material
    // det.DM = new TH3I("DM", "DM", det.nx, 0, dimX, det.ny, 0, dimY, det.nz, 0, 1);
    // det.DM->GetXaxis()->SetTitle("x [#mum]");
    // det.DM->GetYaxis()->SetTitle("y [#mum]");
    // det.DM->GetZaxis()->SetTitle("z [#mum]");
    // set up material

    for (int j = 0; j <= det.ny; j++)
        for (int i = 0; i <= det.nx; i++)
        {
            det.DM->SetBinContent(i, j, 1, 0);
        }


    //init the space charge 
    det.NeffH = new TH3F("Neff", "Neff", det.nx, 0, dimX, det.ny, 0, dimY, det.nz, 0, 1);
    det.NeffH->GetXaxis()->SetTitle("x [#mum]");
    cout << det.ny << endl;
    det.NeffH->GetYaxis()->SetTitle("y [#mum]");
    det.NeffH->GetZaxis()->SetTitle("z [#mum]");
    //set up the space charge 
    cout << " nx = " << det.nx;
   for(int j = 1; j <= det.ny; j++) {  // Loop from 1 to ny (50)
    for(int i = 1; i <= det.nx; i++) {  // Loop from 1 to nx (100)
        if((j == 48) || (j == 49)) {
            det.NeffH->SetBinContent(i, j, 1, 20000);  // Set 20,000 for y = 48 or 49
        } else {
            det.NeffH->SetBinContent(i, j, 1, 10);  // Set 1,000 for other y values
        }
    }
}

    // Set up the entry and exit points using the vector values
    det.SetDriftHisto(10e-9);
    TCanvas c1;
    det.SetEntryPoint(entryPoint[0], entryPoint[1], entryPoint[2]);
    det.SetExitPoint(exitPoint[0], exitPoint[1], exitPoint[2]);
    det.Temperature = 253;
    det.diff = 1;       // Diffusion simulation: 1 = true, 0 = false
    det.ShowMipIR(100); // Show the field

    // Draw charge plot
    TCanvas charge;
    det.MipIR(200);
    det.sum->DrawCopy("HIST");
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


TCanvas *c10 = new TCanvas("c1", "3D Histogram", 800, 600);
det.NeffH->SetMinimum(10.);
    // Draw the histogram
    det.NeffH->Draw("BOX2Z");

    // Update the canvas to display the histogram
    c10->Update();
    // TCanvas Show_2;
    // Show_2.Divide(2, 2);
    // Show_2.cd(3);
    // TPaveText *e_2 = new TPaveText(0.6, 0.4, 0.9, 0.6, "NDC");
    // e_2->AddText(Form("Entry Point: (%.1f, %.1f, %.1f)", entryPoint[0], entryPoint[1], entryPoint[2]));
    // e_2->AddText(Form("Exit Point: (%.1f, %.1f, %.1f)", exitPoint[0], exitPoint[1], exitPoint[2]));
    // e_2->AddText(Form("Voltage: %.1f", det.Voltage));
    // e_2->AddText(Form("MTresh: %.5f", det.MTresh));
    // e_2->SetFillColor(0);
    // e_2->Draw();

    // Show_2.cd(1);
    // det.MipIR(200);
    // det.sum->DrawCopy("HIST");
    // det.neg->DrawCopy("HISTSAME");
    // det.pos->DrawCopy("HISTSAME");
    // Show_2.cd(2);
    // det.ShowMipIR(100);

    // // Create the file name with the necessary parameters
    // std::string filename = "results/run_V_" + std::to_string(static_cast<int>(det.Voltage)) + "_MT_" + std::to_string(det.MTresh) + "_" + std::to_string(time(0)) + ".png";

    // // Save the canvas as a PNG file
    // Show_2.SaveAs(filename.c_str());
}